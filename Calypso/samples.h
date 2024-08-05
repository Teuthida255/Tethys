// Ignore this
#include "sample_struct.h"
#ifndef __TETHYS_SAMPLE_DATA_H__
# define __TETHYS_SAMPLE_DATA_H__

// Write the number of samples you want to use here
// 32 is the maximum for this testing program
#define NumberOfSamples 32
// Write a 0 here instead of a 1 to disable the default multi-sample values
// Turn this off if you plan on using a different layout for your samples
#define UseDefaultMultiSamples 1

const sample_import sample_data[NumberOfSamples] = {
	//{filename, sample rate, data type, loop type, sample start, loop start, loop end, base note},
	{"SINE128.RAW", 0, PCM_16_Bit_With_FM_Padding, Forward_Loop, 0, 0, 0, -3},
	{"HSINE128.RAW", 0, PCM_16_Bit_With_FM_Padding, Forward_Loop, 0, 0, 0, -3},
	{"ASINE128.RAW", 0, PCM_16_Bit_With_FM_Padding, Forward_Loop, 0, 0, 0, -3},
	{"QSINE128.RAW", 0, PCM_16_Bit_With_FM_Padding, Forward_Loop, 0, 0, 0, -3},
	{"SSINE128.RAW", 0, PCM_16_Bit_With_FM_Padding, Forward_Loop, 0, 0, 0, -3},
	{"SASIN128.RAW", 0, PCM_16_Bit_With_FM_Padding, Forward_Loop, 0, 0, 0, -3},
	{"PULSE128.RAW", 0, PCM_16_Bit_With_FM_Padding, Forward_Loop, 0, 0, 0, -3},
	{"DSINE128.RAW", 0, PCM_16_Bit_With_FM_Padding, Forward_Loop, 0, 0, 0, -3},
	{"PULSE512.RAW", 0, PCM_8_Bit, Forward_Loop, 120, 0, 256, -3},
	{"2A03T512.RAW", 0, PCM_16_Bit_With_FM_Padding, Forward_Loop, 0, 0, 0, -3},
	{"2A03NOIS.RAW", 0, PCM_8_Bit, Forward_Loop, 128, 0, 65000, 0},
	{"2A03PNOI.RAW", 0, PCM_8_Bit_With_FM_Padding, Forward_Loop, 0, 0, 0, 0},
	{"PIANOLOW.RAW", 44100, PCM_8_Bit_With_Silence_At_Start, Forward_Loop, 0, 5892, 6902, 24},
	{"PIANOMID.RAW", 44100, PCM_8_Bit_With_Silence_At_Start, Alternating_Loop, 0, 14786, 16992, 36},
	{"PIANOHGH.RAW", 44100, PCM_8_Bit_With_Silence_At_Start, Alternating_Loop, 0, 5394, 9225, 60},
	{"LESORGAN.RAW", 44100, PCM_8_Bit_With_Silence_At_Start, Forward_Loop, 0, 5137, 12047, 24},
	{"ACOUSTIC.RAW", 44100, PCM_8_Bit_With_Silence_At_Start, Forward_Loop, 0, 14491, 14659, 24},
	{"DISTORTO.RAW", 44100, PCM_8_Bit_With_Silence_At_Start, Alternating_Loop, 0, 11660, 17088, 24},
	{"FINGBASS.RAW", 44100, PCM_8_Bit_With_Silence_At_Start, Forward_Loop, 0, 4639, 4839, 36 - 3},
	{"ORCHIT2.RAW", 44100, PCM_8_Bit_With_Silence_At_Start, No_Loop, 0, 0, 24576, 24 - 1},
	{"CELLO.RAW", 44100, PCM_8_Bit_With_Silence_At_Start, Forward_Loop, 0, 5242, 5641, 24 - 3},
	{"TRUMPET.RAW", 44100, PCM_8_Bit_With_Silence_At_Start, Forward_Loop, 0, 7219, 7261, 24},
	{"SAXYBOY.RAW", 44100, PCM_8_Bit_With_Silence_At_Start, Forward_Loop, 0, 3403, 4920, 24},
	{"CHOIR.RAW", 44100, PCM_8_Bit_With_Silence_At_Start, Forward_Loop, 0, 5474, 13245, 24},
	{"DEFKICK.RAW", 22050, PCM_8_Bit, No_Loop, 0, 0, 0, 0},
	{"DEFSNARE.RAW", 22050, PCM_8_Bit, No_Loop, 0, 0, 0, 0},
	{"DEFCHAT.RAW", 22050, PCM_8_Bit, No_Loop, 0, 0, 0, 7},
	{"DEFOHAT.RAW", 22050, PCM_8_Bit, No_Loop, 0, 0, 0, 10},
	{"DEFTOM.RAW", 22050, PCM_8_Bit, No_Loop, 0, 0, 0, 0},
	{"DEFCRASH.RAW", 22050, PCM_8_Bit, No_Loop, 0, 0, 0, 12},
	{"YES.RAW", 11025, PCM_8_Bit, No_Loop, 0, 0, 0, 36},
	{"BOOML.RAW", 8500, PCM_8_Bit, No_Loop, 0, 0, 0, 39},
};

#endif