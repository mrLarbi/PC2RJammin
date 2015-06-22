#ifndef __SOUND_H__
#define __SOUND_H__

#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>

typedef enum { SND_CAPTURE, SND_PLAYBACK } sound_type_t; 

typedef struct sound_t
{
	char* buffer;
	snd_pcm_t* handle;
	unsigned int freq;
	snd_pcm_uframes_t frames;
	sound_type_t type;
	int size;
}*sound_t;

sound_t create_sound(unsigned int freq, int frames, sound_type_t type);
int play_sound(sound_t sound);
int record_sound(sound_t sound);
void clean_buffer(sound_t sound);
void print_buffer(sound_t sound);
void destroy_sound(sound_t sound);

#endif /* __SOUND_H__ */

