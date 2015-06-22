#ifndef __INPUT_H__
#define __INPUT_H__

#include "core.h"
#include "sound.h"

void handle_welcome(client_core_t core, char* user);
void handle_audio_port(client_core_t core, char* port);
void handle_connected(client_core_t core, char* user);

void handle_exited(client_core_t core, char* user);

void handle_empty_session(client_core_t core);
void handle_current_session(client_core_t core, char* style, char* temp, char* nbMus);
void handle_ack_opts(client_core_t core);
void handle_full_session(client_core_t core);

void handle_audio_ok(client_core_t core);
void handle_audio_ko(client_core_t core);
void handle_audio_sync(client_core_t core, char* tick);


void handle_audio_mix(client_core_t core, char* buff);

void handle_chat_listen(client_core_t core, char* sender, char* text);

#endif /* __INPUT_H__ */

