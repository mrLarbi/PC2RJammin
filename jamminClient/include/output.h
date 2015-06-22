#ifndef __OUTPUT_H__
#define __OUTPUT_H__
#include <unistd.h>
#include <stdio.h>

void send_connect(int sock, char* user);
void send_set_options(int sock, char* style, char* tempo);
void send_audio_chunk(int sock, char* tick, char* buff);
void send_audio_ack(int sock);
void send_exit(int sock, char* user);
void send_text(int sock, char* text);
#endif /* __OUTPUT_H__ */

