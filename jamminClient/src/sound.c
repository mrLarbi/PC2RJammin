#include "sound.h"
#include <stdlib.h>

sound_t create_sound(unsigned int freq, int frames, sound_type_t type)
{
	sound_t sound = malloc(sizeof(struct sound_t));

	sound->freq = freq;
	sound->frames = frames;
	sound->type = type;

	int rc;
	snd_pcm_hw_params_t *params;
	int dir;
	snd_pcm_stream_t temp_type = (type == SND_CAPTURE) ? SND_PCM_STREAM_CAPTURE : SND_PCM_STREAM_PLAYBACK;

	if ((rc = snd_pcm_open(&(sound->handle), "default", temp_type, 0)) < 0) 
	{
		fprintf(stderr, "unable to open pcm device: %s\n",snd_strerror(rc));
		exit(1);
	}

	snd_pcm_hw_params_alloca(&params);
	
	if((rc = snd_pcm_hw_params_any(sound->handle, params)) < 0)
	{
		fprintf(stderr, "unable to allocate fill default parameters: %s\n",snd_strerror(rc));
		exit(1);
	}

	if((rc = snd_pcm_hw_params_set_access(sound->handle, params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
	{
		fprintf(stderr, "unable to set access: %s\n",snd_strerror(rc));
		exit(1);
	}
	
	if((rc = snd_pcm_hw_params_set_format(sound->handle, params, SND_PCM_FORMAT_S16_LE)) < 0)
	{
		fprintf(stderr, "unable to set format: %s\n",snd_strerror(rc));
		exit(1);
	}
	
	if((rc = snd_pcm_hw_params_set_channels(sound->handle, params, 2)) < 0)
	{
		fprintf(stderr, "unable to set channels: %s\n",snd_strerror(rc));
		exit(1);
	}

	if((rc = snd_pcm_hw_params_set_rate_near(sound->handle, params, &freq, &dir)) < 0)
	{
		fprintf(stderr, "unable to set rate parameter: %s\n",snd_strerror(rc));
		exit(1);
	}
	
	
	if((rc = snd_pcm_hw_params_set_period_size_near(sound->handle, params, &sound->frames, &dir)) < 0)
	{
		fprintf(stderr, "unable to set period size parameter: %s\n",snd_strerror(rc));
		exit(1);
	} 

	if ((rc = snd_pcm_hw_params(sound->handle, params)) < 0) {
		fprintf(stderr, "unable to set hardware parameters: %s\n", snd_strerror(rc));
		exit(1);
	}

	snd_pcm_hw_params_get_period_size(params, &sound->frames, &dir);

	sound->size = sound->frames *4;
	sound->buffer = malloc(sound->size);

	return sound;
}

int play_sound(sound_t sound)
{
	int rc;
	if(sound->type != SND_PLAYBACK)
	{
		fprintf(stderr, "You want to play a sound in a recorder, are you mad ?\n");
		return -1;
	}
	rc = snd_pcm_writei(sound->handle, sound->buffer, sound->frames);
	if (rc == -EPIPE) {
		//fprintf(stderr, "underrun occurred\n");
		snd_pcm_prepare(sound->handle);
		snd_pcm_writei(sound->handle, sound->buffer, sound->frames);
		return -1;
	} 
	else if (rc < 0) 
	{
		fprintf(stderr, "error from writei: %s\n", snd_strerror(rc));
		return -1;
	}  
	else if (rc != (int)sound->frames) 
	{
		fprintf(stderr, "short write, write %d frames\n", rc);
		return -1;
	}
	return 0;
}

int record_sound(sound_t sound)
{
	int rc;
	if(sound->type != SND_CAPTURE)
	{
		fprintf(stderr, "You want to capture a sound in a player, are you mad ?\n");
		return -1;
	}
	rc = snd_pcm_readi(sound->handle, sound->buffer, sound->frames);
	if (rc == -EPIPE) 
	{
		fprintf(stderr, "overrun occurred\n");
		snd_pcm_prepare(sound->handle);
		return -1;
	} 
	else if (rc < 0) 
	{
		fprintf(stderr, "error from read: %s\n", snd_strerror(rc));
		return -1;
	} 
	else if (rc != (int)sound->frames) 
	{
		fprintf(stderr, "short read, read %d frames\n", rc);
		return -1;
	}
	return 0;
}

void destroy_sound(sound_t sound)
{
	snd_pcm_drain(sound->handle);
  	snd_pcm_close(sound->handle);
  	free(sound->buffer);
}

void clean_buffer(sound_t sound)
{
	int i;
	for (i = 0; i < sound->size; i += 1)
	{
		if(sound->buffer[i] == '\n' || sound->buffer[i] == 11 || sound->buffer[i] == 12 || sound->buffer[i] == 13)
		{
			sound->buffer[i] = 14;
		}
		if(sound->buffer[i] == '/')
		{
			sound->buffer[i] = sound->buffer[i] + 1;
		}
	}
}

void print_buffer(sound_t sound)
{
	int i;
	for (i = 0; i < sound->size; i += 1)
	{
		printf("%c", sound->buffer[i]);
	}
}
