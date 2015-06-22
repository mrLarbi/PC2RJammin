#ifndef __TOOLS_H__
#define __TOOLS_H__

#include "core.h"

void flush_buffer();
int read_string(char* string);
client_core_t parse_arguments(int argc, char** argv);
void copy_all(char* dest, char* src, int size);

#endif /* __TOOLS_H__ */

