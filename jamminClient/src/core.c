#include <sys/select.h>
#include <sys/time.h>
#include "command_handler.h"
#include "connection.h"
#include "core.h"
#include <pthread.h>
#include "tools.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "input.h"
#include "output.h"

client_core_t create_client_core(char* user, int port, char* address) {
	client_core_t core = malloc(sizeof(struct client_core_t));
	core->main_socket = -1;
	core->second_socket = -1;
	strcpy(core->address, address);
	strcpy(core->user, user);
	core->main_port = port;
	core->cont = 1;
	core->session = NULL;
	core->capture = create_sound(44100, 32, SND_CAPTURE);
	core->playback = create_sound(44100, 32, SND_PLAYBACK);
	return core;
}

void destroy_client_core(client_core_t core) {
	if (core->session)
		destroy_session(core->session);
	disconnect_from_server(core->main_socket);
	disconnect_from_server(core->second_socket);
	destroy_sound(core->capture);
	destroy_sound(core->playback);
	free(core);
}

int true_connection(client_core_t core) {
	if ((core->main_socket = connect_to_server(core->address, core->main_port))
			== -1) {
		return -1;
	}
	send_connect(core->main_socket, core->user);
	wait_for_specific_answer(core, core->main_socket, ANS_WELCOME);
	if ((core->second_socket = connect_to_server(core->address, core->second_port))
			== -1) {
		return -1;
	}
	wait_for_specific_answer(core, core->main_socket, ANS_AUDIO_OK);
	wait_for_specific_answer(core, core->main_socket, ANS_CONNECTED);
	wait_for_specific_answer(core, core->main_socket, ANS_AUDIO_SYNC);
	return 0;
}

int read_audio_buff(int socket, char *buff, int size) {
	char msg;
	short slash_counter = 0;
	int i;

	while (!slash_counter) {
		if (read(socket, &msg, 1) == -1) {
			printf("Error\n");
			return -1;
		} else {
			if (msg == '/') {
				slash_counter++;
			} else if (msg != '\n') {
				//printf("%c ", msg);
			} else {
				// \n
			}
		}
	}

	for (i = 0; i < size; i += 1) {
		if (read(socket, &msg, 1) == -1) {
			return -1;
		} else {
			buff[i] = msg;
		}
	}

	//READ LAST TWO LETTERS ==> /\n
	if (read(socket, &msg, 1) == -1) {
		return -1;
	}
	if (read(socket, &msg, 1) == -1) {
		return -1;
	}
	return 1;
}

void* play_sounds(void* arg) {
	client_core_t core = (client_core_t) arg;
	char received[256];
	int buffer_size = core->playback->size;
	command_t comm;
	comm.nb_args = 0;
	comm.req = ANS_AUDIO_MIX;
	while (1) {
		if(read_audio_buff(core->second_socket, received, buffer_size)==-1) {
			break;
		}
		copy_all(comm.arguments[0], received, buffer_size);
		handle_answer(core, &comm);
	}
	return NULL;
}

void* recorder(void* arg) {
	client_core_t core = (client_core_t) arg;
	char string_tick[STRING_SZ];
	while (core->cont) {
		if (record_sound(core->capture) == 0) {
			sprintf(string_tick, "%f", core->tick);
			core->tick += (1.0 * core->session->tempo / 60);
			clean_buffer(core->capture);
			send_audio_chunk(core->second_socket, string_tick,
					core->capture->buffer);
		}
	}
	return NULL;
}

void run_client(client_core_t core) {
	if (true_connection(core) == -1) {
		fprintf(stderr, "Could not connect to the server\n");
		return;
	}
	pthread_t listener,player;

	pthread_create(&listener, NULL, play_sounds, core);
	pthread_create(&player, NULL, recorder, core);

	fd_set is_ready;
	printf("Chat section\n");
	char welc_msg[STRING_SZ];
	sprintf(welc_msg, "%s : ", core->user);
	write(1, welc_msg, strlen(welc_msg));
	while (core->cont) {
		FD_ZERO(&is_ready);
		FD_SET(STDIN_FILENO, &is_ready);
		FD_SET(core->main_socket, &is_ready);

		struct timeval tv = { 0, 30000 };
		command_t comm;
		comm.req = REQ_INVALID;
		if (select(core->main_socket + 1, &is_ready, NULL, NULL, &tv)
				== -1) {
			perror("select");
			continue;
		}
		if (FD_ISSET(STDIN_FILENO, &is_ready)) {
			char input_string[STRING_SZ];
			read_string(input_string);
			if (!strcmp(input_string, "\\EXIT")) {
				send_exit(core->main_socket, core->user);
				//core->cont = 0;
			} else {
				char msg[STRING_SZ];
				sprintf(msg, "%s : ", core->user);
				write(1, msg, strlen(msg));
				send_text(core->main_socket, input_string);
			}
		}

		if (FD_ISSET(core->main_socket, &is_ready)) {
			comm = wait_for_answer(core->main_socket);
			handle_answer(core, &comm);
		}
	}
	printf("Good bye :)\n");
	//pthread_join(listener, NULL);
	//pthread_join(player, NULL);
	destroy_client_core(core);
}

void handle_answer(client_core_t core, command_t *command) {
	switch (command->req) {
	case ANS_ACK_OPTS:
		handle_ack_opts(core);
		break;
	case ANS_AUDIO_KO:
		handle_audio_ko(core);
		break;
	case ANS_AUDIO_MIX:
		handle_audio_mix(core, command->arguments[0]);
		break;
	case ANS_AUDIO_OK:
		handle_audio_ok(core);
		break;
	case ANS_AUDIO_PORT:
		handle_audio_port(core, command->arguments[0]);
		break;
	case ANS_CONNECTED:
		handle_connected(core, command->arguments[0]);
		break;
	case ANS_CURRENT_SESSION:
		handle_current_session(core, command->arguments[0],
				command->arguments[1], command->arguments[2]);
		break;
	case ANS_EMPTY_SESSION:
		handle_empty_session(core);
		break;
	case ANS_EXITED:
		handle_exited(core, command->arguments[0]);
		break;
	case ANS_FULL_SESSION:
		handle_full_session(core);
		break;
	case ANS_WELCOME:
		handle_welcome(core, command->arguments[0]);
		break;
	case ANS_LISTEN:
		handle_chat_listen(core, command->arguments[0], command->arguments[1]);
		break;
	case ANS_AUDIO_SYNC:
		handle_audio_sync(core, command->arguments[0]);
		break;
	default:
		fprintf(stderr, "Unknown answer\n");
		break;
	}
}

command_t wait_for_answer(int sock) {
	char answer[STRING_SZ];
	read_msg(sock, answer, STRING_SZ);
	return parse_command(answer);
}

void wait_for_specific_answer(client_core_t core, int sock, request_t ans) {
	command_t comm;
	do {
		comm = wait_for_answer(sock);
		handle_answer(core, &comm);
	} while (comm.req != ans);
}
