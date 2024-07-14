#ifndef __TETHYS_SAMPLE_STRUCT_H__
# define __TETHYS_SAMPLE_STRUCT_H__

enum sample_import_types {
	PCM_8_Bit = 0,
	PCM_16_Bit = 1,
	PCM_8_Bit_With_FM_Padding = 2,
	PCM_16_Bit_With_FM_Padding = 3,
	PCM_8_Bit_With_Silence_At_Start = 4,
	PCM_16_Bit_With_Silence_At_Start = 5,
};

enum sample_loop_types {
	No_Loop = 0,
	Forward_Loop = 1,
	Reverse_Loop = 2,
	Alternating_Loop = 3,
};

typedef struct {
	const char filename[12];
	const int sample_rate;
	const short data_type;
	const short loop_type;
	const unsigned short playback_start;
	const unsigned short loop_start;
	const unsigned short loop_end;
	const char base_note;
} sample_import;

#endif