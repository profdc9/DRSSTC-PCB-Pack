#ifndef __CONSTANTS_H
#define __CONSTANTS_H

#define OUTPUT 	DDB4
#define SWITCH DDB3
#define MIDI_NOTE_ON 0x09
#define MIDI_NOTE_OFF 0x08
#define MIDI_CTRL_CHANGE 0x0b
#define MIDI_BYTE_MSG 0x0f

static float base[] = {65.9, 69.8, 73.9, 78.2, 82.9, 87.8, 92.9, 98.5, 104.3, 110.5, 117.0, 124.0};
static unsigned char pow2[] = {1, 2, 4, 8, 16};
	
#endif