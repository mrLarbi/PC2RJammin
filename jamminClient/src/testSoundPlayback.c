#include "sound.h"

int main (int argc, char **argv)
{
	sound_t test_sound = create_sound(44100, 32, SND_PLAYBACK);
	int loop = 5000;
	while (loop)
	{
		read(0, test_sound->buffer, test_sound->size);
		play_sound(test_sound);
		loop--;
	}
	destroy_sound(test_sound);
	return 0;
}
