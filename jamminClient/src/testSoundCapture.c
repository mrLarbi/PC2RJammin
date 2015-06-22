#include "sound.h"


int main (int argc, char **argv)
{
	sound_t test_sound = create_sound(44100, 32, SND_CAPTURE);
	int loop = 5000;
	while (loop)
	{
		record_sound(test_sound);
		write(1, test_sound->buffer, test_sound->size);
		loop--;
	}
	destroy_sound(test_sound);
	return 0;
}

