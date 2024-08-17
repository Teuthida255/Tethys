
#include <jo/jo.h>
#include "pcmsys.h"
#include "keyboard_funcs.h"
#include "samples.h"

#define CALYPSO_DEBUG 0

#define PAN_LEFT_START 16
#define MAX_PAN_VALUE 15

int framerate;

#define NUM_SAMPLES (32)
#define NUM_MULTIS (NUM_SAMPLES)
#define NUM_MACROS (63)
#define NUM_INSTRUMENTS (64)
#define NUM_CHANNELS 32
#define NUM_EDITORS 5

#define MAX_SLOTS 0x40
#define MAX_OPTIONS 0x100
#define SAVE_SIZE MAX_SLOTS * MAX_OPTIONS

#define NUM_PCM_OPTIONS 6
enum pcm_options {
	PCM_LOOPTYPE = 0,
	PCM_SAMPLESTART = 1,
	PCM_LOOPSTART = 2,
	PCM_LOOPEND = 3,
	PCM_BASE_PITCH = 4,
	PCM_BASE_NOTE = 5,
};
#define NUM_MULT_OPTIONS 3 + (MLT_CTRL_LENGTH_MAX * 2)
enum mult_options {
	MULT_TYPE = 0,
	MULT_NOTE_OVERRIDE = 1,
	MULT_LENGTH = 2,
	MULT_DATA = 3,
};
#define NUM_MACR_OPTIONS 14 + (MCR_CTRL_LENGTH_MAX * 2)
enum macr_options {
	MACR_TYPE = 0,
	MACR_VARIABLE_LENGTH = 1,
	MACR_MODE = 2,
	MACR_FIXED = 3,
	MACR_RELEASETYPE = 4,
	MACR_CONTINUOUS = 5,
	MACR_PROGRESSIVE = 6,
	MACR_OVERFLOW = 7,
	MACR_PROPAGATE = 8,
	MACR_LENGTH = 9,
	MACR_LOOP1 = 10,
	MACR_LOOP2 = 11,
	MACR_RELEASE = 12,
	MACR_PLACEHOLDER = 13,
	MACR_DATA = 14,
};
#define NUM_INST_OPTIONS 42 + INS_MACROS_MAX
enum inst_options {
	INST_SAMPLE = 0,
	INST_USEMULTISAMPLE = 1,
	INST_SAMPLESTART = 2,
	INST_LOOPSTART = 3,
	INST_LOOPEND = 4,
	INST_BIT_REVERSE = 5,
	INST_INVERT = 6,
	INST_NOISE_ONLY = 7,
	INST_LEVEL = 8,
	INST_VOLUME = 9,
	INST_PAN = 10,
	INST_SEMITONE = 11,
	INST_CENT = 12,
	INST_REGISTER_DETUNE = 13,

	INST_HOLD_MODE = 14,
	INST_ATTACK = 15,
	INST_DECAY1 = 16,
	INST_SUSTAIN = 17,
	INST_DECAY2 = 18,
	INST_RELEASE = 19,
	INST_KEY_SCALING = 20,
	INST_LOOP_SYNC = 21,
	INST_LFO_FREQ = 22,
	INST_LFO_DELAY = 23,
	INST_PITCH_LFO_STRENGTH = 24,
	INST_PITCH_LFO_WAVEFORM = 25,
	INST_AMP_LFO_STRENGTH = 26,
	INST_AMP_LFO_WAVEFORM = 27,

	INST_PATCHLENGTH = 28,
	INST_IGNORE_VELOCITY = 29,
	INST_MOD_STRENGTH = 30,
	INST_MOD_INPUT_X = 31,
	INST_X_GENERATION = 32,
	INST_MOD_INPUT_Y = 33,
	INST_Y_GENERATION = 34,
	INST_FREQ_MULTIPLIER = 35,
	INST_FREQ_DIVIDER = 36,
	INST_LEVEL_SCALING = 37,
	INST_EFFECT_VOLUME = 38,
	INST_EFFECT_PAN = 39,
	INST_EFFECT_SLOT = 40,
	INST_INPUT_VOLUME = 41,
	INST_MACRO = 42,
};
#define NUM_CHAN_OPTIONS 3
enum chan_options {
	CHAN_NOTE = 0,
	CHAN_VELOCITY = 1,
	CHAN_VOLUME = 2,
};

#define OPTIONS_PER_PAGE 14

const char* LOOP_NAMES[] = {
	"NONE",
	"FWRD",
	"RVRS",
	"ALTR"
};

const char* LFO_NAMES[] = {
	"SAW",
	"SQUARE",
	"TRI",
	"NOISE"
};

const char* GENERATION_NAMES[] = {
	"PREV",
	"NEXT",
};

const char* SAMPLETRAIT_NAMES[] = {
	"NONE ",
	"BREV ",
	"INVT ",
	"B + I",
	"NOIS ",
	"B + N",
	"I + N",
	"B+I+N",
};

const char* ENVTRAIT_NAMES[] = {
	"NONE ",
	"HOLD ",
	"LSYN ",
	"H + S",
};

const char* KEYON_NAMES[] = {
	" NONE",
	"  OFF",
	"   ON",
	"RESET",
};

const char* MACRO_TYPE_NAMES[] = {
	"KEY ",
	"SOFF",
	"LSTR",
	"LEND",
	"TRTS",
	"TLVL",
	"DVOL",
	"DPAN",
	"NOFF",
	"CDET",
	"RDET",
	"ETRT",
	"ATTK",
	"DEC1",
	"SUST",
	"DEC2",
	"RLSE",
	"KSCL",
	"LFOF",
	"LFOR",
	"PLST",
	"PLWV",
	"ALST",
	"ALWV",
	"MLVL",
	"MODX",
	"MODY",
	"PMUL",
	"PDIV",
	"LSCL",
	"EVOL",
	"EPAN",
	"ESLT",
	"ILVL",
	"EC01",
	"EC02",
	"EC03",
	"EC04",
	"EC05",
	"EC06",
	"EC07",
	"EC08",
	"EC09",
	"EC10",
	"EC11",
	"EC12",
	"EC13",
	"EC14",
	"EC15",
	"EC16",
	"EC17",
	"EC18",
	"EC19",
	"EC20",
	"EC21",
	"EC22",
	"EC23",
	"EC24",
	"EC25",
	"EC26",
	"EC27",
	"EC28",
	"EC29",
	"EC30",
	"EC31",
	"EC32",
	"EC33",
	"EC34",
	"EC35",
	"EC36",
	"EC37",
	"EC38",
	"EC39",
	"EC40",
	"EC41",
	"EC42",
	"EC43",
	"EC44",
	"EC45",
	"EC46",
	"EC47",
	"EC48",
	"EC49",
	"EC50",
	"EC51",
	"EC52",
	"EC53",
	"EC54",
	"EC55",
	"EC56",
	"EC57",
	"EC58",
	"EC59",
	"EC60",
	"EC61",
	"EC62",
	"EC63",
	"EA00",
	"EA01",
	"EA02",
	"EA03",
	"EA04",
	"EA05",
	"EA06",
	"EA07",
	"EA08",
	"EA09",
	"EA10",
	"EA11",
	"EA12",
	"EA13",
	"EA14",
	"EA15",
	"EA16",
	"EA17",
	"EA18",
	"EA19",
	"EA20",
	"EA21",
	"EA22",
	"EA23",
	"EA24",
	"EA25",
	"EA26",
	"EA27",
	"EA28",
	"EA29",
	"EA30",
	"EA31",
};

const short MACRO_TYPE_TO_INTERNAL[] = {
	MACRO_KEYON,
	MACRO_SAMPLEOFFSET,
	MACRO_LOOPSTART,
	MACRO_LOOPEND,
	MACRO_SAMPLETRAITS,
	MACRO_TOTALLEVEL,
	MACRO_VOLUME,
	MACRO_PAN,
	MACRO_NOTEOFFSET,
	MACRO_CENTDETUNE,
	MACRO_REGISTERDETUNE,
	MACRO_ENVELOPETRAITS,
	MACRO_ATTACK,
	MACRO_DECAY1,
	MACRO_SUSTAIN,
	MACRO_DECAY2,
	MACRO_RELEASE,
	MACRO_KEYSCALING,
	MACRO_LFOFREQ,
	MACRO_LFORESET,
	MACRO_PITCHLFOSTRENGTH,
	MACRO_PITCHLFOWAVEFORM,
	MACRO_AMPLFOSTRENGTH,
	MACRO_AMPLFOWAVEFORM,
	MACRO_MODLEVEL,
	MACRO_MODINPUTX,
	MACRO_MODINPUTY,
	MACRO_PITCHMULTIPLIER,
	MACRO_PITCHDIVIDER,
	MACRO_LEVELSCALING,
	MACRO_EFFECTVOLUME,
	MACRO_EFFECTPAN,
	MACRO_EFFECTSLOT,
	MACRO_INPUTLEVEL,

	MACRO_EFFECTCOEF + 0,
	MACRO_EFFECTCOEF + 1,
	MACRO_EFFECTCOEF + 2,
	MACRO_EFFECTCOEF + 3,
	MACRO_EFFECTCOEF + 4,
	MACRO_EFFECTCOEF + 5,
	MACRO_EFFECTCOEF + 6,
	MACRO_EFFECTCOEF + 7,
	MACRO_EFFECTCOEF + 8,
	MACRO_EFFECTCOEF + 9,
	MACRO_EFFECTCOEF + 10,
	MACRO_EFFECTCOEF + 11,
	MACRO_EFFECTCOEF + 12,
	MACRO_EFFECTCOEF + 13,
	MACRO_EFFECTCOEF + 14,
	MACRO_EFFECTCOEF + 15,
	MACRO_EFFECTCOEF + 16,
	MACRO_EFFECTCOEF + 17,
	MACRO_EFFECTCOEF + 18,
	MACRO_EFFECTCOEF + 19,
	MACRO_EFFECTCOEF + 20,
	MACRO_EFFECTCOEF + 21,
	MACRO_EFFECTCOEF + 22,
	MACRO_EFFECTCOEF + 23,
	MACRO_EFFECTCOEF + 24,
	MACRO_EFFECTCOEF + 25,
	MACRO_EFFECTCOEF + 26,
	MACRO_EFFECTCOEF + 27,
	MACRO_EFFECTCOEF + 28,
	MACRO_EFFECTCOEF + 29,
	MACRO_EFFECTCOEF + 30,
	MACRO_EFFECTCOEF + 31,
	MACRO_EFFECTCOEF + 32,
	MACRO_EFFECTCOEF + 33,
	MACRO_EFFECTCOEF + 34,
	MACRO_EFFECTCOEF + 35,
	MACRO_EFFECTCOEF + 36,
	MACRO_EFFECTCOEF + 37,
	MACRO_EFFECTCOEF + 38,
	MACRO_EFFECTCOEF + 39,
	MACRO_EFFECTCOEF + 40,
	MACRO_EFFECTCOEF + 41,
	MACRO_EFFECTCOEF + 42,
	MACRO_EFFECTCOEF + 43,
	MACRO_EFFECTCOEF + 44,
	MACRO_EFFECTCOEF + 45,
	MACRO_EFFECTCOEF + 46,
	MACRO_EFFECTCOEF + 47,
	MACRO_EFFECTCOEF + 48,
	MACRO_EFFECTCOEF + 49,
	MACRO_EFFECTCOEF + 50,
	MACRO_EFFECTCOEF + 51,
	MACRO_EFFECTCOEF + 52,
	MACRO_EFFECTCOEF + 53,
	MACRO_EFFECTCOEF + 54,
	MACRO_EFFECTCOEF + 55,
	MACRO_EFFECTCOEF + 56,
	MACRO_EFFECTCOEF + 57,
	MACRO_EFFECTCOEF + 58,
	MACRO_EFFECTCOEF + 59,
	MACRO_EFFECTCOEF + 60,
	MACRO_EFFECTCOEF + 61,
	MACRO_EFFECTCOEF + 62,

	MACRO_EFFECTADRS + 0,
	MACRO_EFFECTADRS + 1,
	MACRO_EFFECTADRS + 2,
	MACRO_EFFECTADRS + 3,
	MACRO_EFFECTADRS + 4,
	MACRO_EFFECTADRS + 5,
	MACRO_EFFECTADRS + 6,
	MACRO_EFFECTADRS + 7,
	MACRO_EFFECTADRS + 8,
	MACRO_EFFECTADRS + 9,
	MACRO_EFFECTADRS + 10,
	MACRO_EFFECTADRS + 11,
	MACRO_EFFECTADRS + 12,
	MACRO_EFFECTADRS + 13,
	MACRO_EFFECTADRS + 14,
	MACRO_EFFECTADRS + 15,
	MACRO_EFFECTADRS + 16,
	MACRO_EFFECTADRS + 17,
	MACRO_EFFECTADRS + 18,
	MACRO_EFFECTADRS + 19,
	MACRO_EFFECTADRS + 20,
	MACRO_EFFECTADRS + 21,
	MACRO_EFFECTADRS + 22,
	MACRO_EFFECTADRS + 23,
	MACRO_EFFECTADRS + 24,
	MACRO_EFFECTADRS + 25,
	MACRO_EFFECTADRS + 26,
	MACRO_EFFECTADRS + 27,
	MACRO_EFFECTADRS + 28,
	MACRO_EFFECTADRS + 29,
	MACRO_EFFECTADRS + 30,
	MACRO_EFFECTADRS + 31,
};

const char* NOTE_NAMES[] = {
	"C-",
	"C#",
	"D-",
	"D#",
	"E-",
	"F-",
	"F#",
	"G-",
	"G#",
	"A-",
	"A#",
	"B-",
};

const char* NOTE_NAMES_NEGATIVE[] = {
	"c-",
	"c#",
	"d-",
	"d#",
	"e-",
	"f-",
	"f#",
	"g-",
	"g#",
	"a-",
	"a#",
	"b-",
};

const short SCANCODE_TO_NOTE[] = {
	-1, /*0x00*/
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,

	-1, /*0x10*/
	-1,
	-1,
	-1,
	-1,
	12, // Q
	-1, // 1
	-1,
	-1,
	-1,
	 0, // Z
	 1, // S
	-1,
	14, // W
	13, // 2
	-1,

	-1, /*0x20*/
	 4, // C
	 2, // X
	 3, // D
	16, // E
	-1,
	15, // 3
	-1,
	-1,
	-1, 
	 5, // V
	-1,
	19, // T
	17, // R
	18, // 5
	-1,

	-1, /*0x30*/
	 9, // N
	 7, // B
	 8, // H
	 6, // G
	21, // Y
	20, // 6
	-1,
	-1,
	-1,
	11, // M
	10, // J
	23, // U
	22, // 7
	-1, // 8
	-1,

	-1, /*0x40*/
	12, // ,
	-1,
	24, // I
	26, // O
	27, // 0
	25, // 9
	-1,
	-1,
	14, // .
	16, // /
	13, // L
	15, // ;
	28, // P
	-1,
	-1,

	-1, /*0x50*/
	-1,
	-1,
	-1,
	29, // [
	30, // =
	-1,
	-1,
	-1,
	-1,
	-1,
	31, // ]
	-1,
	-1, 
	-1,
	-1,

	-1, /*0x60*/
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,

	-1, /*0x70*/
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,

	-1, /*0x80*/
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
};

typedef struct {
	short play_state;
	short note;
	short scancode;
	short patchlength;
} jam_channel;

jam_channel jam_channels[NUM_CHANNELS] = {
	{0, -1, -1, 0},
	{0, -1, -1, 0},
	{0, -1, -1, 0},
	{0, -1, -1, 0},
	{0, -1, -1, 0},
	{0, -1, -1, 0},
	{0, -1, -1, 0},
	{0, -1, -1, 0},
	{0, -1, -1, 0},
	{0, -1, -1, 0},
	{0, -1, -1, 0},
	{0, -1, -1, 0},
	{0, -1, -1, 0},
	{0, -1, -1, 0},
	{0, -1, -1, 0},
	{0, -1, -1, 0},
};

void set_jam_channel(jam_channel* channels, short index, short note, short scancode, short patchlength) {
	channels[index].note = note;
	channels[index].scancode = scancode;
	channels[index].patchlength = patchlength;
	channels[index].play_state = 1;
}

enum editor_types {
	EDITOR_MULT = 0,
	EDITOR_PCM = 1,
	EDITOR_INST = 2,
	EDITOR_MACR = 3,
	EDITOR_CHAN = 4,
};

short option_values[NUM_EDITORS][0x40][0x100];
short pcm_option_defaults[NUM_PCM_OPTIONS] = {
	1, // PCM_LOOPTYPE
	0, // PCM_SAMPLESTART
	0, // PCM_LOOPSTART
	129, // PCM_LOOPEND
	0x11b, // PCM_BASE_PITCH
	-3, // PCM_BASE_NOTE
};

short inst_option_defaults[NUM_INST_OPTIONS] = {
	0,			// INST_SAMPLE
	0,			// INST_USEMULTISAMPLE
	0,			// INST_SAMPLESTART
	0,			// INST_LOOPSTART
	0,			// INST_LOOPEND
	0,			// INST_BIT_REVERSE
	0,			// INST_INVERT
	0,			// INST_NOISE_ONLY
	16,			// INST_LEVEL
	4,			// INST_VOLUME
	0,			// INST_PAN
	0,	        // INST_SEMITONE
	0,			// INST_CENT
	0,			// INST_REGISTER_DETUNE
	0,			// INST_HOLD_MODE
	0x1F,		// INST_ATTACK
	0x0,		// INST_DECAY1
	0x0,		// INST_SUSTAIN
	0x0,		// INST_DECAY2
	0x1F,		// INST_RELEASE
	0xF,		// INST_KEY_SCALING
	0,			// INST_LOOP_SYNC
	16,			// INST_LFO_FREQ
	0,			// INST_LFO_DELAY
	0,			// INST_PITCH_LFO_STRENGTH
	LFO_TRI,	// INST_PITCH_LFO_WAVEFORM
	0,			// INST_AMP_LFO_STRENGTH
	LFO_TRI,	// INST_AMP_LFO_WAVEFORM
	0,			// INST_PATCHLENGTH
	0,			// INST_IGNORE_VELOCITY
	0,			// INST_MOD_STRENGTH
	0,			// INST_MOD_INPUT_X
	MOD_OLD,	// INST_X_GENERATION
	0,	        // INST_MOD_INPUT_Y
	MOD_OLD,	// INST_Y_GENERATION
	0,	        // INST_FREQ_MULTIPLIER
	0,			// INST_FREQ_DIVIDER
	0,			// INST_LEVEL_SCALING
	0,			// INST_EFFECT_VOLUME
	0,			// INST_EFFECT_PAN
	0,			// INST_EFFECT_SLOT
	0,			// INST_INPUT_VOLUME
	0,			// INST_MACRO
	0,
	0,
	0,
	//0, in the cringe corner until I'm a better programmer
	//0,
	//0,
	//0,
};

short chan_option_defaults[NUM_INST_OPTIONS] = {
	0,			// CHAN_NOTE
	0xFF,		// CHAN_VELOCITY
	0xFF,		// CHAN_VOLUME
};

short instruments[NUM_INSTRUMENTS];
short samples[NUM_SAMPLES];

void init_option_values() {
	for (short i = 0; i < NUM_INSTRUMENTS; i++) {
		for (short j = 0; j < NUM_INST_OPTIONS; j++) {
			option_values[EDITOR_INST][i][j] = inst_option_defaults[j];
		}
	}

	for (short i = 0; i < NUM_CHANNELS; i++) {
		for (short j = 0; j < NUM_CHAN_OPTIONS; j++) {
			option_values[EDITOR_CHAN][i][j] = chan_option_defaults[j];
		}
	}

	if (UseDefaultMultiSamples) {
		option_values[EDITOR_MULT][0][MULT_TYPE] = 0;
		option_values[EDITOR_MULT][0][MULT_NOTE_OVERRIDE] = 0;
		option_values[EDITOR_MULT][0][MULT_LENGTH] = 14;

		option_values[EDITOR_MULT][0][MULT_DATA + 0] = 24;  // C-0
		option_values[EDITOR_MULT][0][MULT_DATA + 1] = 24;  // C#0
		option_values[EDITOR_MULT][0][MULT_DATA + 2] = 25;  // D-0
		option_values[EDITOR_MULT][0][MULT_DATA + 3] = 25;  // D#0
		option_values[EDITOR_MULT][0][MULT_DATA + 4] = 25;  // E-0
		option_values[EDITOR_MULT][0][MULT_DATA + 5] = 28;  // F-0
		option_values[EDITOR_MULT][0][MULT_DATA + 6] = 26;  // F#0
		option_values[EDITOR_MULT][0][MULT_DATA + 7] = 28;  // G-0
		option_values[EDITOR_MULT][0][MULT_DATA + 8] = 26;  // G#0
		option_values[EDITOR_MULT][0][MULT_DATA + 9] = 28;  // A-0
		option_values[EDITOR_MULT][0][MULT_DATA + 10] = 27;  // A#0
		option_values[EDITOR_MULT][0][MULT_DATA + 11] = 28;  // B-0
		option_values[EDITOR_MULT][0][MULT_DATA + 12] = 28;  // C-1
		option_values[EDITOR_MULT][0][MULT_DATA + 13] = 29;  // C#1
		option_values[EDITOR_MULT][0][MULT_DATA + 14] = 28;  // D-1


		option_values[EDITOR_MULT][1][MULT_TYPE] = 1;
		option_values[EDITOR_MULT][1][MULT_NOTE_OVERRIDE] = 0;
		option_values[EDITOR_MULT][1][MULT_LENGTH] = 2;

		option_values[EDITOR_MULT][1][MULT_DATA + 0] = 12;  // C-0
		option_values[EDITOR_MULT][1][MULT_DATA + 1] = 12;  // C#0
		option_values[EDITOR_MULT][1][MULT_DATA + 2] = 13;  // D-0
		option_values[EDITOR_MULT][1][MULT_DATA + 3] = 36;  // D#0
		option_values[EDITOR_MULT][1][MULT_DATA + 4] = 14;  // E-0
	}

	if (CALYPSO_DEBUG) {
		option_values[EDITOR_INST][0][INST_VOLUME] = 0;
		option_values[EDITOR_INST][0][INST_EFFECT_VOLUME] = 7;
		option_values[EDITOR_INST][0][INST_INPUT_VOLUME] = 4;
	}
}

bool pan_is_right(char pan) {
	return (pan < PAN_LEFT_START);
}

short getRealPanValue(char pan) {
	if (pan_is_right(pan))
		return -pan;
	else
		return pan - PAN_LEFT_START;
}

char getRegPanValue(short real_pan) {
	if (real_pan <= 0)
		return (char)(-real_pan);
	else
		return (char)(real_pan + PAN_LEFT_START);
}

char offsetPan(char pan, char offset) {
	short real_pan = getRealPanValue(pan);
	real_pan += offset;
	if (real_pan < -MAX_PAN_VALUE)
		real_pan = -MAX_PAN_VALUE;
	if (real_pan > MAX_PAN_VALUE)
		real_pan = MAX_PAN_VALUE;
	return getRegPanValue(real_pan);
}

short getOctave(short pitchword) {
	unsigned short octave = (pitchword >> 11) % (1 << 4);
	return (octave < 8) ? octave : (((short)octave) - (1 << 4));
}

short getFine(short pitchword) {
	return pitchword & ((1 << 10) - 1);
}

short offsetPitchWord(short pitchword, short offset) {
	short fine = getFine(pitchword);
	short octave = getOctave(pitchword);
	fine += offset;
	short carry = fine / 0x400;
	if (fine < 0) {
		carry--;
		fine += (-carry * 0x400);
	}
	octave += carry;
	fine %= 0x400;
	if (octave >= 8) {
		octave = 7;
		fine = 0x3ff;
	}
	else if (octave < -8) {
		octave = -8;
		fine = 0;
	}
	if (octave < 0) {
		octave += (1 << 4);
	}
	return fine + (octave << 11);
}

char getRegisterSlotInput(short input) {
	if (input < 0)
		return input + 32;
	else
		return input;
}

short offsetWithClamp(short base_value, short offset, short lower_bound, short upper_bound) {
	int value = base_value;
	value += offset;
	if (value > upper_bound)
		value = upper_bound;
	else if (value < lower_bound)
		value = lower_bound;
	return (short)value;
}

unsigned short offsetUnsignedWithClamp(unsigned short unsigned_value, short offset, unsigned short lower_bound, unsigned short upper_bound) {
	int value = unsigned_value;
	value += offset;
	if (value > upper_bound)
		value = upper_bound;
	else if (value < lower_bound)
		value = lower_bound;
	return (unsigned short)value;
}

int offsetIntWithClamp(int value, short offset, int lower_bound, int upper_bound) {
	value += offset;
	if (value > upper_bound)
		value = upper_bound;
	else if (value < lower_bound)
		value = lower_bound;
	return value;
}

typedef struct {
	const char* label;
	const short coarse_change_value;
	const short lower_bound;
	const short upper_bound;
	const char display_type;
} edit_option;

enum display_types {
	DISP_DEFAULT,
	DISP_UNSIGNEDHEX,
	DISP_SIGNEDHEX,
	DISP_ADDONE,
	DISP_ONOFF,
	DISP_ZERODISABLE,
	DISP_ZERODISABLEMINUSONE,
	DISP_KEYSCALE,
	DISP_NOTE,
	DISP_PITCHWORD,
	DISP_PAN,
	DISP_LFOWAVE,
	DISP_GENERATION,
	DISP_LOOPTYPE,
	DISP_SAMPLESTART,
	DISP_LOOPSTART,
	DISP_LOOPEND,
	DISP_VARIABLELENGTH,
	DISP_VARIABLELENGTHADDONE,
	DISP_MACRO,
	DISP_MULTITYPE,
	DISP_MULTIDATA,
	DISP_MACROTYPE,
	DISP_MACROMODE,
	DISP_MACROFORMAT,
	DISP_MACRORELEASETYPE,
	DISP_MACROVALUEUNSIGNED,
	DISP_MACROVALUESIGNED,
	DISP_MACROVALUEUNSIGNEDHEX,
	DISP_MACROVALUESIGNEDHEX,
	DISP_MACROVALUEADDONE,
	DISP_MACROVALUEKEYON,
	DISP_MACROVALUELFOWAVE,
	DISP_MACROVALUESAMPLETRAITS,
	DISP_MACROVALUEENVTRAITS,
	DISP_MACROVALUEMODINPUT,
	DISP_SELECTOR,
	DISP_NONE
};

short offset_option(const edit_option* option, short option_value, bool subtract, bool coarse, short current_sample, bool halve_length) {
	short offset = ((coarse) ? option->coarse_change_value : 1) * ((subtract) ? -1 : 1);
	switch (option->display_type) {
	case DISP_SAMPLESTART:
		return offsetUnsignedWithClamp(option_value, offset, option->lower_bound, pcm_get_max_playsize(current_sample));
	case DISP_LOOPSTART:
		return offsetUnsignedWithClamp(option_value, offset, option->lower_bound, pcm_get_max_playsize(current_sample) - pcm_get_sample_start(current_sample));
	case DISP_LOOPEND:
		return offsetUnsignedWithClamp(option_value, offset, pcm_get_loop_start(current_sample), pcm_get_max_playsize(current_sample) - pcm_get_sample_start(current_sample));
	case DISP_PITCHWORD:
		return offsetPitchWord(option_value, offset);
	case DISP_PAN:
		return offsetPan(option_value, -offset);
	case DISP_UNSIGNEDHEX:
	case DISP_MACROVALUEUNSIGNED:
	case DISP_MACROVALUEUNSIGNEDHEX:
		return (short)offsetUnsignedWithClamp((unsigned short)option_value, offset, (unsigned short)option->lower_bound, (unsigned short)option->upper_bound);
	case DISP_VARIABLELENGTH:
	case DISP_VARIABLELENGTHADDONE:
		return offsetWithClamp(option_value, offset, option->lower_bound, ((halve_length) ? (option->upper_bound / 2) : option->upper_bound));
	case DISP_SELECTOR:
		return offsetWithClamp(option_value, offset, option->lower_bound, option->upper_bound - 1);
	default:
		return offsetWithClamp(option_value, offset, option->lower_bound, option->upper_bound);
	}
}

void	clamp_pcm_loop_points(short* options, short sample) {
	unsigned short new_max_size = pcm_get_max_playsize(samples[sample]) - options[PCM_SAMPLESTART];
	if ((unsigned short)options[PCM_LOOPSTART] > new_max_size) {
		options[PCM_LOOPSTART] = (short)new_max_size;
	}
	if ((unsigned short)options[PCM_LOOPEND] > new_max_size) {
		options[PCM_LOOPEND] = (short)new_max_size;
	}
	if ((unsigned short)options[PCM_LOOPSTART] > (unsigned short)options[PCM_LOOPEND]) {
		options[PCM_LOOPSTART] = options[PCM_LOOPEND];
	}
}

const char left_arrow[2] = "<-";
const char right_arrow[2] = "->";
void print_option(short x, short y, short highlight_color, bool is_selected, const edit_option* option, short option_value, short current_sample, short option_index) {
	// Only used by DISP_NOTE
	short octave;
	short note;
	char value_string[7];
	switch (option->display_type) {
	case DISP_LOOPTYPE:
		sprintf(value_string, "%6s", LOOP_NAMES[option_value]);
		break;
	case DISP_PITCHWORD:
		sprintf(value_string, "%2d:%3x", getOctave(option_value), getFine(option_value));
		break;
	case DISP_PAN:
		sprintf(value_string, "%6d", -getRealPanValue(option_value));
		break;
	case DISP_LFOWAVE:
	case DISP_MACROVALUELFOWAVE:
		sprintf(value_string, "%6s", LFO_NAMES[option_value]);
		break;
	case DISP_ZERODISABLE:
		if (option_value == 0)
			sprintf(value_string, "%6s", "OFF");
		else
			sprintf(value_string, "%6d", option_value);
		break;
	case DISP_ZERODISABLEMINUSONE:
	case DISP_MACRO:
		if (option_value == 0)
			sprintf(value_string, "%6s", "OFF");
		else
			sprintf(value_string, "%6d", option_value - 1);
		break;
	case DISP_KEYSCALE:
		if (option_value == 0x0F)
			sprintf(value_string, "%6s", "OFF");
		else
			sprintf(value_string, "%6d", option_value);
		break;
	case DISP_ONOFF:
		sprintf(value_string, "%6s", (option_value != 0) ? "ON" : "OFF");
		break;
	case DISP_GENERATION:
		sprintf(value_string, "%6s", (option_value != 0) ? "NEW" : "OLD");
		break;
	case DISP_MACROMODE:
		sprintf(value_string, "%6s", (option_value != 0) ? "RELA" : "ABSO");
		break;
	case DISP_MACROFORMAT:
		sprintf(value_string, "%6s", (option_value != 0) ? "VRBL" : "SNGL");
		break;
	case DISP_MACRORELEASETYPE:
		sprintf(value_string, "%6s", (option_value != 0) ? "JUMP" : "PLAY");
		break;
	case DISP_MULTITYPE:
		sprintf(value_string, "%6s", (option_value != 0) ? "SPLT" : "MAP");
		break;
	case DISP_ADDONE:
	case DISP_VARIABLELENGTHADDONE:
	case DISP_MACROVALUEADDONE:
		sprintf(value_string, "%6d", option_value + 1);
		break;
	case DISP_MACROTYPE:
		sprintf(value_string, "%6s", MACRO_TYPE_NAMES[option_value]);
		break;
	case DISP_SIGNEDHEX:
	case DISP_MACROVALUESIGNEDHEX:
	case DISP_MULTIDATA:
		if (option_value >= 0)
			sprintf(value_string, "%6x", option_value);
		else
			sprintf(value_string, "%2s%4x", "-", -option_value);
		break;
	case DISP_UNSIGNEDHEX:
	case DISP_SAMPLESTART:
	case DISP_LOOPSTART:
	case DISP_LOOPEND:
	case DISP_MACROVALUEUNSIGNEDHEX:
		sprintf(value_string, "%6x", (unsigned short)option_value);
		break;
	case DISP_MACROVALUEKEYON:
		sprintf(value_string, "%6s", KEYON_NAMES[option_value]);
		break;
	case DISP_MACROVALUESAMPLETRAITS:
		sprintf(value_string, "%6s", SAMPLETRAIT_NAMES[option_value]);
		break;
	case DISP_MACROVALUEENVTRAITS:
		sprintf(value_string, "%6s", ENVTRAIT_NAMES[option_value]);
		break;
	case DISP_MACROVALUEMODINPUT:
		if (option_value < 16)
			sprintf(value_string, "%5d%c", option_value, 'O');
		else
			sprintf(value_string, "%5d%c", option_value - 32, 'N');
		break;
	case DISP_NOTE:
		octave = ((option_value >= 0) ? (option_value / 12) : (((option_value + 1) / 12) - 1));
		note = ((option_value >= 0) ? (option_value % 12) : ((12 - (-option_value % 12)) % 12));
		if (octave >= 0)
			sprintf(value_string, "%5s%1d", NOTE_NAMES[note], octave);
		else
			sprintf(value_string, "%5s%1d", NOTE_NAMES_NEGATIVE[note], -octave);
		break;
	case DISP_SELECTOR:
		sprintf(value_string, "%3x", option_value);
		break;
	case DISP_NONE:
		sprintf(value_string, "      ");
		break;
	default:
		sprintf(value_string, "%6d", option_value);
	}

	jo_clear_screen_line(y);

	char macro_value_string[19];
	if (option->display_type >= DISP_MACROVALUEUNSIGNED && option->display_type <= DISP_MACROVALUEMODINPUT) {
		sprintf(macro_value_string, "%s %d", option->label, option_index + 1);
	}
	if (is_selected) {
		switch (option->display_type) {
		case DISP_SAMPLESTART:
			jo_printf_with_color(x, y, highlight_color, "%s: %s %s %s", option->label, (option_value != option->lower_bound) ? left_arrow : "  ", value_string, (option_value != pcm_get_max_playsize(current_sample)) ? right_arrow : "  ");
			break;
		case DISP_LOOPSTART:
			jo_printf_with_color(x, y, highlight_color, "%s: %s %s %s", option->label, (option_value != option->lower_bound) ? left_arrow : "  ", value_string, (option_value != pcm_get_max_playsize(current_sample) - pcm_get_sample_start(current_sample)) ? right_arrow : "  ");
			break;
		case DISP_LOOPEND:
			jo_printf_with_color(x, y, highlight_color, "%s: %s %s %s", option->label, (option_value != pcm_get_loop_start(current_sample)) ? left_arrow : "  ", value_string, (option_value != pcm_get_max_playsize(current_sample) - pcm_get_sample_start(current_sample)) ? right_arrow : "  ");
			break;
		case DISP_MACRO:
			jo_printf_with_color(x, y, highlight_color, "%s %1d: %s %s %s", option->label, option_index, (option_value != option->lower_bound) ? left_arrow : "  ", value_string, (option_value != option->upper_bound) ? right_arrow : "  ");
			break;
		case DISP_MULTIDATA:
			jo_printf_with_color(x, y, highlight_color, "%s (%s%1d): %s %s %s", option->label, NOTE_NAMES[(option_index) % 12], (option_index) / 12, (option_value != option->lower_bound) ? left_arrow : "  ", value_string, (option_value != option->upper_bound) ? right_arrow : "  ");
			break;
		case DISP_MACROVALUESIGNED:
		case DISP_MACROVALUEUNSIGNED:
		case DISP_MACROVALUESIGNEDHEX:
		case DISP_MACROVALUEUNSIGNEDHEX:
		case DISP_MACROVALUEADDONE:
		case DISP_MACROVALUEKEYON:
		case DISP_MACROVALUELFOWAVE:
		case DISP_MACROVALUEMODINPUT:
			jo_printf_with_color(x, y, highlight_color, "%18s: %s %s %s", macro_value_string, (option_value != option->lower_bound) ? left_arrow : "  ", value_string, (option_value != option->upper_bound) ? right_arrow : "  ");
			break;
		case DISP_NONE:
			jo_printf_with_color(x, y, highlight_color, "%s:    %s", option->label, value_string);
			break;
		default:
			jo_printf_with_color(x, y, highlight_color, "%s: %s %s %s", option->label, (option_value != option->lower_bound) ? left_arrow : "  ", value_string, (option_value != option->upper_bound) ? right_arrow : "  ");
		}	
	}
	else {
		switch (option->display_type) {
		case DISP_MACRO:
			jo_printf_with_color(x, y, JO_COLOR_INDEX_White, "%s %1d:    %s", option->label, option_index, value_string);
			break;
		case DISP_MULTIDATA:
			jo_printf_with_color(x, y, JO_COLOR_INDEX_White, "%s (%s%1d):    %s", option->label, NOTE_NAMES[(option_index) % 12], (option_index) / 12, value_string);
			break;
		case DISP_MACROVALUESIGNED:
		case DISP_MACROVALUEUNSIGNED:
		case DISP_MACROVALUESIGNEDHEX:
		case DISP_MACROVALUEUNSIGNEDHEX:
		case DISP_MACROVALUEADDONE:
		case DISP_MACROVALUEKEYON:
		case DISP_MACROVALUELFOWAVE:
		case DISP_MACROVALUEMODINPUT:
			jo_printf_with_color(x, y, JO_COLOR_INDEX_White, "%18s:    %s", macro_value_string, value_string);
			break;
		default:
			jo_printf_with_color(x, y, JO_COLOR_INDEX_White, "%s:    %s", option->label, value_string);
		}
		
	}
}

void convert_interleaved_data(short* data, short* buffer, bool to_interleaved, short data_start, short data_size, short num_options) {
	// Unlike the actual driver struct, the multisample editor data stores
	// both the samples used in the mapping and their note values, even though
	// the actual struct will never use all of them
	// (this is to ensure that data is maintained while switching formats)
	// In keymap mode, the samples are stored in the first half of the data,
	// followed by the (unused) note values,
	// while keysplit mode interleaves the data types
	// As such, writing to the struct is the same process regardless of the
	// format - just write the first 63 bytes
	// This means, however, that the data must be moved into the proper format
	// any time the user changes it
	if (to_interleaved) {
		for (short i = 0; i < data_size; i++) {
			buffer[data_start + (i * 2)] = data[data_start + i];
			buffer[data_start + (i * 2) + 1] = data[data_start + data_size + i];
		}
	}
	else {
		for (short i = 0; i < data_size * 2; i += 2) {
			buffer[data_start + (i / 2)] = data[data_start + i];
			buffer[data_start + data_size + (i / 2)] = data[data_start + i + 1];
		}
	}
	for (short i = data_start; i < num_options; i++) {
		data[i] = buffer[i];
	}
}

short get_type_id(short type) {
	if (type < NUM_MACRO_TYPES - 2) {
		return type;
	}
	else if (type < MACRO_EFFECTADRS) {
		return NUM_MACRO_TYPES - 2;
	}
	else {
		return NUM_MACRO_TYPES - 1;
	}
}

typedef struct {
	const edit_option* options;
	short num_options;
	short current_option;
	const edit_option* selector;
	short num_selectors;
	short current_selector;
} option_page;

short getNumPages(option_page* option) {
	return (option->num_options % OPTIONS_PER_PAGE == 0) ? ((option->num_options / OPTIONS_PER_PAGE) - 1) : (option->num_options / OPTIONS_PER_PAGE);
}

static char* const FILENAMES[5][16] = {
	{
		"CL_ML_0.SAV",
		"CL_ML_1.SAV",
		"CL_ML_2.SAV",
		"CL_ML_3.SAV",
		"CL_ML_4.SAV",
		"CL_ML_5.SAV",
		"CL_ML_6.SAV",
		"CL_ML_7.SAV",
		"CL_ML_8.SAV",
		"CL_ML_9.SAV",
		"CL_ML_A.SAV",
		"CL_ML_B.SAV",
		"CL_ML_C.SAV",
		"CL_ML_D.SAV",
		"CL_ML_E.SAV",
		"CL_ML_F.SAV",
	},
	{
		"CL_SM_0.SAV",
		"CL_SM_1.SAV",
		"CL_SM_2.SAV",
		"CL_SM_3.SAV",
		"CL_SM_4.SAV",
		"CL_SM_5.SAV",
		"CL_SM_6.SAV",
		"CL_SM_7.SAV",
		"CL_SM_8.SAV",
		"CL_SM_9.SAV",
		"CL_SM_A.SAV",
		"CL_SM_B.SAV",
		"CL_SM_C.SAV",
		"CL_SM_D.SAV",
		"CL_SM_E.SAV",
		"CL_SM_F.SAV",
	},
	{
		"CL_IN_0.SAV",
		"CL_IN_1.SAV",
		"CL_IN_2.SAV",
		"CL_IN_3.SAV",
		"CL_IN_4.SAV",
		"CL_IN_5.SAV",
		"CL_IN_6.SAV",
		"CL_IN_7.SAV",
		"CL_IN_8.SAV",
		"CL_IN_9.SAV",
		"CL_IN_A.SAV",
		"CL_IN_B.SAV",
		"CL_IN_C.SAV",
		"CL_IN_D.SAV",
		"CL_IN_E.SAV",
		"CL_IN_F.SAV",
	},
	{
		"CL_MC_0.SAV",
		"CL_MC_1.SAV",
		"CL_MC_2.SAV",
		"CL_MC_3.SAV",
		"CL_MC_4.SAV",
		"CL_MC_5.SAV",
		"CL_MC_6.SAV",
		"CL_MC_7.SAV",
		"CL_MC_8.SAV",
		"CL_MC_9.SAV",
		"CL_MC_A.SAV",
		"CL_MC_B.SAV",
		"CL_MC_C.SAV",
		"CL_MC_D.SAV",
		"CL_MC_E.SAV",
		"CL_MC_F.SAV",
	},
	{
		"CL_CH_0.SAV",
		"CL_CH_1.SAV",
		"CL_CH_2.SAV",
		"CL_CH_3.SAV",
		"CL_CH_4.SAV",
		"CL_CH_5.SAV",
		"CL_CH_6.SAV",
		"CL_CH_7.SAV",
		"CL_CH_8.SAV",
		"CL_CH_9.SAV",
		"CL_CH_A.SAV",
		"CL_CH_B.SAV",
		"CL_CH_C.SAV",
		"CL_CH_D.SAV",
		"CL_CH_E.SAV",
		"CL_CH_F.SAV",
	},
};

void update_engine_state(void) {
	dsp_load_base_variables();
	for (short i = 0; i < NUM_CHANNELS; i++) {
		short* current_options = option_values[EDITOR_CHAN][i];
		short* velocity_options = option_values[EDITOR_CHAN][0];
		chn_note_change(i, (unsigned char)current_options[CHAN_NOTE]);
		chn_velocity_change(i, (unsigned char)velocity_options[CHAN_VELOCITY]);
		chn_channel_volume_change(i, (unsigned char)velocity_options[CHAN_VOLUME]);
		chn_set_macro_values(i);
		chn_set_values(i);
	}
	dsp_set_variables();
}

bool should_write = true;
bool did_offset = true;
bool should_set_pcm = true;
bool play_single = false;
short current_editor = EDITOR_INST;
short patch_offset = 0;

short test = 0;

const edit_option sample_options[NUM_PCM_OPTIONS] = {
		{"         Loop Type", 1, 0, 3, DISP_LOOPTYPE},
		{"      Sample Start", 0x100, 0, 0, DISP_SAMPLESTART},
		{" Loop Start Offset", 0x100, 0, 0, DISP_LOOPSTART},
		{"   Loop End Offset", 0x100, 0, 0, DISP_LOOPEND},
		{"        Base Pitch", 16, 0x4000, 0x3BFF, DISP_PITCHWORD},
		{"         Base Note", 12, -96, 95, DISP_NOTE},
};

const edit_option sample_option = { "Sample", 4, 0, NUM_SAMPLES, DISP_SELECTOR };

const edit_option multi_samp = { "   Sample ID", 4, 0, NUM_SAMPLES - 1, DISP_MULTIDATA };
const edit_option multi_splt = { "         Sample ID", 4, 0, NUM_SAMPLES - 1, DISP_UNSIGNEDHEX };
const edit_option multi_note = { "       Cutoff Note", 12, 0, 95, DISP_NOTE };
const edit_option ph = { "", 0, 0, 0, 0 };

// multi_option's options are decided dynamically, so their values are (p)lace(h)olders
const edit_option multi_options[NUM_MULT_OPTIONS] = {
	{"              Type", 1, 0, 1, DISP_MULTITYPE},
	{"Base Note Override", 1, 0, 1, DISP_ONOFF},
	{"            Length", 4, 0, MLT_CTRL_LENGTH_MAX - 1, DISP_VARIABLELENGTHADDONE},
	ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,
	ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,
};
short multi_conversion_buffer[NUM_MULT_OPTIONS];

const edit_option multi_option = { "Multi-Sample", 4, 0, NUM_MULTIS, DISP_SELECTOR };

const edit_option options[NUM_INST_OPTIONS - INS_MACROS_MAX + 1] = {
	{"   Sample/Multi ID", 4, 0, NUM_SAMPLES - 1, DISP_UNSIGNEDHEX},
	{"  Use Multi-Sample", 1, 0, 1, DISP_ONOFF},
	{"     Sample Offset", 0x100, 0, 0xFFFF, DISP_UNSIGNEDHEX},
	{" Loop Start Offset", 0x100, 0, 0xFFFF, DISP_UNSIGNEDHEX},
	{"   Loop End Offset", 0x100, 0, 0xFFFF, DISP_UNSIGNEDHEX},
	{"Bit Reverse Sample", 1, 0, 1, DISP_ONOFF},
	{"     Invert Sample", 1, 0, 1, DISP_ONOFF},
	{"        Noise Only", 1, 0, 3, DISP_DEFAULT},
	{"       Total Level", 0x10, 0, 0xFF, DISP_DEFAULT},
	{"     Direct Volume", 2, 0, 7, DISP_DEFAULT},
	{"        Direct Pan", 3, -15, 15, DISP_PAN},
	{"       Note Offset", 12, -96, 95, DISP_DEFAULT},
	{"       Cent Detune", 10, -100, 99, DISP_DEFAULT},
	{"   Register Detune", 165, -16384, 16384, DISP_SIGNEDHEX},
	{"         Hold Mode", 1, 0, 1, DISP_ONOFF},
	{"       Attack/Hold", 4, 0, 0x1F, DISP_ZERODISABLE},
	{"           Decay 1", 4, 0, 0x1F, DISP_ZERODISABLE},
	{"           Sustain", 4, 0, 0x1F, DISP_ZERODISABLE},
	{"           Decay 2", 4, 0, 0x1F, DISP_ZERODISABLE},
	{"           Release", 4, 0, 0x1F, DISP_ZERODISABLE},
	{"       Key Scaling", 4, 0, 0x0F, DISP_KEYSCALE},
	{"         Loop Sync", 1, 0, 1, DISP_ONOFF},
	{"     LFO Frequency", 4, 0, 0x1F, DISP_DEFAULT},
	{"         LFO Delay", 1, 0, 0xFF, DISP_ZERODISABLE},
	{"Pitch LFO Strength", 2, 0, 7, DISP_DEFAULT},
	{"Pitch LFO Waveform", 1, 0, 3, DISP_LFOWAVE},
	{"  Amp LFO Strength", 2, 0, 7, DISP_DEFAULT},
	{"  Amp LFO Waveform", 1, 0, 3, DISP_LFOWAVE},
	{"      Patch Length", 2, 0, 7, DISP_ZERODISABLE},
	{"   Ignore Velocity", 1, 0, 1, DISP_ONOFF},
	{"  Modulation Level", 2, 0, 15, DISP_DEFAULT},
	{"Modulation Input X", 4, -16, 15, DISP_DEFAULT},
	{" X Data Generation", 1, 0, 1, DISP_GENERATION},
	{"Modulation Input Y", 4, -16, 15, DISP_DEFAULT},
	{" Y Data Generation", 1, 0, 1, DISP_GENERATION},
	{"  Pitch Multiplier", 5, 0, 255, DISP_ADDONE},
	{"     Pitch Divider", 5, 0, 255, DISP_ADDONE},
	{"     Level Scaling", 4, 0, 0x0F, DISP_ZERODISABLE},
	{"     Effect Volume", 2, 0, 7, DISP_DEFAULT},
	{"        Effect Pan", 3, -15, 15, DISP_PAN},
	{"       Effect Slot", 3, 0, 15, DISP_DEFAULT},
	{"       Input Level", 2, 0, 7, DISP_DEFAULT},
	{"        Macro ID", 8, 0, NUM_MACROS + 1, DISP_MACRO},
};
const edit_option instrument_option = { "Instrument", 4, 0, NUM_INSTRUMENTS, DISP_SELECTOR };

const char macro_str_abs[] = "    Data Value";
const char  macro_str_rel[] = "   Data Offset";
const edit_option macro_data_abs[NUM_MACRO_TYPES] = {
	{macro_str_abs, 1, MACRO_KEYON_LBOUND, MACRO_KEYON_UBOUND, DISP_MACROVALUEKEYON},
	{macro_str_abs, 0x100, MACRO_SAMPLEOFFSET_LBOUND, MACRO_SAMPLEOFFSET_UBOUND, DISP_MACROVALUEUNSIGNEDHEX},
	{macro_str_abs, 0x100, MACRO_LOOPSTART_LBOUND, MACRO_LOOPSTART_UBOUND, DISP_MACROVALUEUNSIGNEDHEX},
	{macro_str_abs, 0x100, MACRO_LOOPEND_LBOUND, MACRO_LOOPEND_UBOUND, DISP_MACROVALUEUNSIGNEDHEX},
	{macro_str_abs, 1, MACRO_SAMPLETRAITS_LBOUND, MACRO_SAMPLETRAITS_UBOUND, DISP_MACROVALUESAMPLETRAITS},
	{macro_str_abs, 0x10, MACRO_TOTALLEVEL_LBOUND, MACRO_TOTALLEVEL_UBOUND, DISP_MACROVALUESIGNED},
	{macro_str_abs, 2, MACRO_VOLUME_LBOUND, MACRO_VOLUME_UBOUND, DISP_MACROVALUESIGNED},
	{macro_str_abs, 3, MACRO_PAN_LBOUND, MACRO_PAN_UBOUND, DISP_MACROVALUESIGNED},
	{macro_str_abs, 12, MACRO_NOTEOFFSET_LBOUND, MACRO_NOTEOFFSET_UBOUND, DISP_MACROVALUESIGNED},
	{macro_str_abs, 10, MACRO_CENTDETUNE_LBOUND, MACRO_CENTDETUNE_UBOUND, DISP_MACROVALUESIGNED},
	{macro_str_abs, 165, MACRO_REGISTERDETUNE_LBOUND, MACRO_REGISTERDETUNE_UBOUND, DISP_MACROVALUESIGNEDHEX},
	{macro_str_abs, 2, MACRO_ENVELOPETRAITS_LBOUND, MACRO_ENVELOPETRAITS_UBOUND, DISP_MACROVALUEENVTRAITS},
	{macro_str_abs, 4, MACRO_ATTACK_LBOUND, MACRO_ATTACK_UBOUND, DISP_MACROVALUESIGNED},
	{macro_str_abs, 4, MACRO_DECAY1_LBOUND, MACRO_DECAY1_UBOUND, DISP_MACROVALUESIGNED},
	{macro_str_abs, 4, MACRO_SUSTAIN_LBOUND, MACRO_SUSTAIN_UBOUND, DISP_MACROVALUESIGNED},
	{macro_str_abs, 4, MACRO_DECAY2_LBOUND, MACRO_DECAY2_UBOUND, DISP_MACROVALUESIGNED},
	{macro_str_abs, 4, MACRO_RELEASE_LBOUND, MACRO_RELEASE_UBOUND, DISP_MACROVALUESIGNED},
	{macro_str_abs, 4, MACRO_KEYSCALING_LBOUND, MACRO_KEYSCALING_UBOUND, DISP_MACROVALUESIGNED},
	{macro_str_abs, 4, MACRO_LFOFREQ_LBOUND, MACRO_LFOFREQ_UBOUND, DISP_MACROVALUESIGNED},
	{macro_str_abs, 1, MACRO_LFORESET_LBOUND, MACRO_LFORESET_UBOUND, DISP_MACROVALUESIGNED},
	{macro_str_abs, 2, MACRO_PITCHLFOSTRENGTH_LBOUND, MACRO_PITCHLFOSTRENGTH_UBOUND, DISP_MACROVALUESIGNED},
	{macro_str_abs, 1, MACRO_PITCHLFOWAVEFORM_LBOUND, MACRO_PITCHLFOWAVEFORM_UBOUND, DISP_MACROVALUELFOWAVE},
	{macro_str_abs, 2, MACRO_AMPLFOSTRENGTH_LBOUND, MACRO_AMPLFOSTRENGTH_UBOUND, DISP_MACROVALUESIGNED},
	{macro_str_abs, 1, MACRO_AMPLFOWAVEFORM_LBOUND, MACRO_AMPLFOWAVEFORM_UBOUND, DISP_MACROVALUELFOWAVE},
	{macro_str_abs, 2, MACRO_MODLEVEL_LBOUND, MACRO_MODLEVEL_UBOUND, DISP_MACROVALUESIGNED},
	{macro_str_abs, 4, MACRO_MODINPUTX_LBOUND, MACRO_MODINPUTX_UBOUND, DISP_MACROVALUEMODINPUT},
	{macro_str_abs, 4, MACRO_MODINPUTY_LBOUND, MACRO_MODINPUTY_UBOUND, DISP_MACROVALUEMODINPUT},
	{macro_str_abs, 5, MACRO_PITCHMULTIPLIER_LBOUND, MACRO_PITCHMULTIPLIER_UBOUND, DISP_MACROVALUEADDONE},
	{macro_str_abs, 5, MACRO_PITCHDIVIDER_LBOUND, MACRO_PITCHDIVIDER_UBOUND, DISP_MACROVALUEADDONE},
	{macro_str_abs, 4, MACRO_LEVELSCALING_LBOUND, MACRO_LEVELSCALING_UBOUND, DISP_MACROVALUESIGNED},
	{macro_str_abs, 2, MACRO_EFFECTVOLUME_LBOUND, MACRO_EFFECTVOLUME_UBOUND, DISP_MACROVALUESIGNED},
	{macro_str_abs, 3, MACRO_EFFECTPAN_LBOUND, MACRO_EFFECTPAN_UBOUND, DISP_MACROVALUESIGNED},
	{macro_str_abs, 3, MACRO_EFFECTSLOT_LBOUND, MACRO_EFFECTSLOT_UBOUND, DISP_MACROVALUESIGNED},
	{macro_str_abs, 2, MACRO_INPUTLEVEL_LBOUND, MACRO_INPUTLEVEL_UBOUND, DISP_MACROVALUESIGNED},
	{macro_str_abs, 100, MACRO_EFFECTCOEF_LBOUND, MACRO_EFFECTCOEF_UBOUND, DISP_MACROVALUESIGNED},
	{macro_str_abs, 0x100, MACRO_EFFECTADRS_LBOUND, MACRO_EFFECTADRS_UBOUND, DISP_MACROVALUEUNSIGNEDHEX},
};
const edit_option macro_data_rel = { macro_str_rel, 0x100, -(1 << 15), (1 << 15) - 1, DISP_MACROVALUESIGNEDHEX };
const edit_option macro_length = { "       Data Length", 0x100, 0, 0xFFFF, DISP_UNSIGNEDHEX };

const edit_option macro_mode_permit_rel = { "              Mode", 1, 0, 1, DISP_MACROMODE };
const edit_option macro_mode_forbid_rel = { "              Mode", 1, 0, 0, DISP_MACROMODE };

// see multi_option
const edit_option macro_options[NUM_MACR_OPTIONS] = {
	{"              Type", 1, 0, NUM_MACRO_TYPES_NONUNIQUE, DISP_MACROTYPE},
	{"      Value Length", 1, 0, 1, DISP_MACROFORMAT},
	ph, // Mode option; differs depending on Type
	{"      Fixed Values", 1, 0, 1, DISP_ONOFF},
	{"  Release Behavior", 1, 0, 1, DISP_MACRORELEASETYPE},
	{"        Continuous", 1, 0, 1, DISP_ONOFF},
	{"       Progressive", 1, 0, 1, DISP_ONOFF},
	{"    Allow Overflow", 1, 0, 1, DISP_ONOFF},
	{" Propagate Patches", 1, 0, 1, DISP_ONOFF},
	{"      Macro Length", 16, 0, MCR_CTRL_LENGTH_MAX, DISP_VARIABLELENGTH},
	{"      Loop 1 Point", 16, 0, MCR_CTRL_LENGTH_MAX, DISP_ZERODISABLE},
	{"      Loop 2 Point", 16, 0, MCR_CTRL_LENGTH_MAX, DISP_ZERODISABLE},
	{"     Release Point", 16, 0, MCR_CTRL_LENGTH_MAX, DISP_ZERODISABLE},
	{" Data on Next Page", 1, 0, 1, DISP_NONE},
	ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,
	ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,
	ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,
	ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,
	ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,
	ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,
	ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,
	ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,
	ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,
	ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,ph,
};
short macro_conversion_buffer[NUM_MACR_OPTIONS];

const edit_option macro_option = { "Macro", 4, 0, NUM_MACROS, DISP_SELECTOR };

const edit_option channel_options[NUM_CHAN_OPTIONS] = {
	{"              Note", 12, -96, 95, DISP_NOTE},
	{"          Velocity", 16, 0, 255, DISP_DEFAULT},
	{"    Channel Volume", 16, 0, 255, DISP_DEFAULT},
};
const edit_option channel_option = { "Channel", 4, 0, 0, DISP_SELECTOR };

const edit_option octave_option = { "Octave", 2, -8, 7, DISP_DEFAULT };
short octave = 0;

option_page sample_editor = { sample_options, NUM_PCM_OPTIONS, 0, &sample_option, NUM_SAMPLES, 0 };
option_page multi_editor = { multi_options, NUM_MULT_OPTIONS, 0, &multi_option, NUM_MULTIS, 0 };
option_page instrument_editor = { options, NUM_INST_OPTIONS, 0, &instrument_option, NUM_INSTRUMENTS, 0 };
option_page macro_editor = { macro_options, NUM_MACR_OPTIONS, 0, &macro_option, NUM_MACROS, 0 };
option_page channel_editor = { channel_options, NUM_CHAN_OPTIONS, 0, &channel_option, 1, 0 };
option_page* editors[NUM_EDITORS] = { &multi_editor, &sample_editor, &instrument_editor, &macro_editor, &channel_editor };

void write_option_values_and_update(void) {
	static bool write_all_values = true;
	short* current_options;
	if (should_write) {
		if (write_all_values || current_editor == EDITOR_PCM) {
			for (short i = (write_all_values) ? 0 : sample_editor.current_selector; i < ((write_all_values) ? (NUM_SAMPLES) : (sample_editor.current_selector + 1)); i++) {
				short current_sample = samples[i];
				current_options = option_values[EDITOR_PCM][i];
				// These functions halt sample playback and must be handled separately
				if ((should_set_pcm && did_offset) || write_all_values) {
					clamp_pcm_loop_points(option_values[current_editor][i], i);
					pcm_loop_type_change(current_sample, (char)current_options[PCM_LOOPTYPE]);
					pcm_sample_start_change(current_sample, (unsigned short)current_options[PCM_SAMPLESTART]);
					pcm_loop_start_change(current_sample, (unsigned short)current_options[PCM_LOOPSTART]);
					pcm_loop_end_change(current_sample, (unsigned short)current_options[PCM_LOOPEND]);
				}
				pcm_base_pitch_change(current_sample, (short)current_options[PCM_BASE_PITCH]);
				pcm_base_note_change(current_sample, (unsigned char)current_options[PCM_BASE_NOTE]);
			}
		}


		if (write_all_values || current_editor == EDITOR_MULT) {
			for (short i = (write_all_values) ? 0 : multi_editor.current_selector; i < ((write_all_values) ? (NUM_MULTIS) : (multi_editor.current_selector + 1)); i++) {
				current_options = option_values[EDITOR_MULT][i];
				mlt_traits_change(i, current_options[MULT_LENGTH], current_options[MULT_NOTE_OVERRIDE], current_options[MULT_TYPE]);
				for (short j = 0; j < MLT_CTRL_LENGTH_MAX; j++) {
					mlt_set_data(i, j, current_options[MULT_DATA + j]);
				}
			}
		}


		if (write_all_values || current_editor == EDITOR_MACR) {
			for (short i = (write_all_values) ? 0 : macro_editor.current_selector; i < ((write_all_values) ? (NUM_MACROS) : (macro_editor.current_selector + 1)); i++) {
				current_options = option_values[EDITOR_MACR][i];
				mcr_type_change(i, MACRO_TYPE_TO_INTERNAL[current_options[MACR_TYPE]]);
				mcr_length_change(i, current_options[MACR_LENGTH]);
				mcr_traits_change(i, current_options[MACR_VARIABLE_LENGTH], (current_options[MACR_MODE] != 0), (current_options[MACR_FIXED] != 0), (current_options[MACR_RELEASETYPE] != 0), current_options[MACR_OVERFLOW], current_options[MACR_CONTINUOUS], current_options[MACR_PROGRESSIVE], current_options[MACR_PROPAGATE]);
				mcr_loop1_change(i, (current_options[MACR_LOOP1] != 0) ? (current_options[MACR_LOOP1] - 1) : 0xFF);
				mcr_loop2_change(i, (current_options[MACR_LOOP2] != 0) ? (current_options[MACR_LOOP2] - 1) : 0xFF);
				mcr_release_change(i, (current_options[MACR_RELEASE] != 0) ? (current_options[MACR_RELEASE] - 1) : 0xFF);
				for (short j = 0; j < MCR_CTRL_LENGTH_MAX; j++) {
					mcr_set_data(i, j, current_options[MACR_DATA + j]);
				}
			}
		}


		if (write_all_values || current_editor == EDITOR_INST) {
			for (short i = (write_all_values) ? 0 : instrument_editor.current_selector; i < ((write_all_values) ? (NUM_INSTRUMENTS) : (instrument_editor.current_selector + 1)); i++) {
				short current_instrument = instruments[i];
				current_options = option_values[EDITOR_INST][i];
				if (current_editor != EDITOR_PCM && current_editor != EDITOR_MULT) {
					ins_sample_type_change(current_instrument, (unsigned char)current_options[INST_USEMULTISAMPLE]);
					ins_sample_change(current_instrument, samples[(unsigned char)current_options[INST_SAMPLE]]);
				}
				ins_sample_offset_change(current_instrument, (unsigned short)current_options[INST_SAMPLESTART]);
				ins_loopstart_offset_change(current_instrument, (unsigned short)current_options[INST_LOOPSTART]);
				ins_loopend_offset_change(current_instrument, (unsigned short)current_options[INST_LOOPEND]);
				ins_totallevel_change(current_instrument, (unsigned char)current_options[INST_LEVEL]);
				ins_ignore_velocity_change(current_instrument, (unsigned char)current_options[INST_IGNORE_VELOCITY]);
				ins_parameter_change(current_instrument, (char)current_options[INST_VOLUME], (char)current_options[INST_PAN]);
				ins_lfo_freq_change(current_instrument, (char)current_options[INST_LFO_FREQ]);
				ins_lfo_pitch_change(current_instrument, (char)current_options[INST_PITCH_LFO_STRENGTH], (char)current_options[INST_PITCH_LFO_WAVEFORM]);
				ins_lfo_amp_change(current_instrument, (char)current_options[INST_AMP_LFO_STRENGTH], (char)current_options[INST_AMP_LFO_WAVEFORM]);
				ins_lfo_delay_change(current_instrument, (unsigned char)current_options[INST_LFO_DELAY]);
				ins_bit_reverse_change(current_instrument, (char)current_options[INST_BIT_REVERSE], (char)current_options[INST_INVERT]);
				ins_noise_mode_change(current_instrument, (char)current_options[INST_NOISE_ONLY]);
				ins_attack_hold_change(current_instrument, (char)current_options[INST_ATTACK], (char)current_options[INST_HOLD_MODE]);
				ins_decay1_change(current_instrument, (char)current_options[INST_DECAY1]);
				ins_sustain_change(current_instrument, (char)current_options[INST_SUSTAIN]);
				ins_decay2_change(current_instrument, (char)current_options[INST_DECAY2]);
				ins_release_change(current_instrument, (char)current_options[INST_RELEASE]);
				ins_key_scaling_sync_change(current_instrument, (char)current_options[INST_KEY_SCALING], (char)current_options[INST_LOOP_SYNC]);
				ins_patchlength_change(current_instrument, (unsigned char)current_options[INST_PATCHLENGTH]);
				ins_mod_volume_change(current_instrument, (char)current_options[INST_MOD_STRENGTH]);
				ins_mod_input_x_change(current_instrument, getRegisterSlotInput((char)current_options[INST_MOD_INPUT_X]), (char)current_options[INST_X_GENERATION]);
				ins_mod_input_y_change(current_instrument, getRegisterSlotInput((char)current_options[INST_MOD_INPUT_Y]), (char)current_options[INST_Y_GENERATION]);
				ins_effect_parameter_change(current_instrument, (char)current_options[INST_EFFECT_VOLUME], (char)current_options[INST_EFFECT_PAN]);
				ins_effect_input_change(current_instrument, (char)current_options[INST_INPUT_VOLUME], (char)current_options[INST_EFFECT_SLOT]);
				ins_mod_input_y_change(current_instrument, getRegisterSlotInput((char)current_options[INST_MOD_INPUT_Y]), (char)current_options[INST_Y_GENERATION]);
				ins_freq_ratio_change(current_instrument, (unsigned char)current_options[INST_FREQ_MULTIPLIER], (unsigned char)current_options[INST_FREQ_DIVIDER]);
				ins_level_scaling_change(current_instrument, (unsigned char)current_options[INST_LEVEL_SCALING]);
				ins_note_offset_change(current_instrument, (unsigned char)current_options[INST_SEMITONE], (unsigned char)current_options[INST_CENT]);
				ins_register_detune_change(current_instrument, current_options[INST_REGISTER_DETUNE]);
				for (short j = 0; j < INS_MACROS_MAX; j++) {
					ins_macro_change(current_instrument, j, (unsigned char)(current_options[INST_MACRO + j]));
				}
			}
		}
		
		// Testing samples uses the default instrument (instrument 0)
		short insNumber = (current_editor >= EDITOR_INST) ? instruments[instrument_editor.current_selector - patch_offset] : 0;
		short patchlength = (play_single) ? 0 : ins_get_patchlength(insNumber);
		for (short i = 0; i < NUM_CHANNELS; i++) {
			switch (jam_channels[i].play_state) {
			case 0:
				if (jam_channels[i].note > -1) {
					for (short j = 0; j <= patchlength; j++) {
						if ((i + j) >= NUM_CHANNELS || (insNumber + j) >= NUM_INSTRUMENTS) {
							break;
						}
						chn_release(i + j);
					}
					jam_channels[i].note = -1;
				}
				break;
			case 1:
				if (current_editor < EDITOR_INST) {
					if (current_editor == EDITOR_PCM) {
						ins_sample_change(0, samples[sample_editor.current_selector]);
						ins_sample_type_change(0, 0);
					}
					else if (current_editor == EDITOR_MULT) {
						ins_sample_change(0, multi_editor.current_selector);
						ins_sample_type_change(0, 1);
					}
					ins_patchlength_change(0, 0);
				}
				chn_instrument_change(i, insNumber);
				chn_play_melodic(i);
				if (!(current_editor < EDITOR_INST)) {
					for (short j = 1; j <= patchlength; j++) {
						if ((i + j) >= NUM_CHANNELS || (insNumber + j) >= NUM_INSTRUMENTS) {
							break;
						}
						chn_instrument_change(i + j, 0x400 + j);
						chn_play_melodic(i + j);
					}
				}
				jam_channels[i].play_state++;
				break;
			}
		}
	}
	update_engine_state();
	write_all_values = false;
}


// This function (and save_data()) both assume the proper device had already been mounted
short load_save_data(jo_backup_device dev, bool at_start) {
	short* file_buffer = JO_NULL;
	for (short i = 0; i < NUM_EDITORS; i++) {
		for (short j = 0; j < MAX_SLOTS / 4; j++) {
			if (!jo_backup_file_exists(dev, FILENAMES[i][j])) return 1;

			if (at_start)
				jo_printf(1, 9, "Loading Editor %d, Section %d...  ", i + 1, j + 1);
			else
				jo_printf_with_color(1, 17, JO_COLOR_INDEX_Green, "   Loading Editor %d, Section %d...  ", i + 1, j + 1);

			file_buffer = jo_backup_load_file_contents(dev, FILENAMES[i][j], JO_NULL);
			if (file_buffer == JO_NULL) return 2;

			for (short k = 0; k < 4; k++) {
				for (short m = 0; m < MAX_OPTIONS; m++) {
					option_values[i][j * 4 + k][m] = file_buffer[k * MAX_OPTIONS + m];
				}
			}
			jo_free(file_buffer);
		}
	}
	if (at_start) {
		jo_clear_screen_line(9);
		jo_printf(1, 9, "Loading complete!");
	}

	return 0;
}

short save_data(jo_backup_device dev) {
	for (short i = 0; i < NUM_EDITORS; i++) {
		for (short j = 0; j < MAX_SLOTS / 4; j++) {
			jo_printf_with_color(1, 17, JO_COLOR_INDEX_Green, "   Saving Editor %d, Section %d...  ", i + 1, j + 1);
			if (!jo_backup_save_file_contents(dev, FILENAMES[i][j], "", option_values[i][j * 4], 0x100 * 0x4 * sizeof(short))) return i;
			if (!jo_backup_file_exists(dev, FILENAMES[i][j])) return i + NUM_EDITORS;
		}
	}

	return 0;
}


void			my_draw(void)
{
	bool using_keyboard = (jo_get_input_type(0) == JoRegularKeyboard);
	static bool disable_inputs = false;
	if (using_keyboard) {
		update_keyboard_state();
	}

	static bool show_info_page = false;

	static const short NUM_TITLE_LINES = 2;
	static const char* const TITLE_TEXT[] = {
		"- CALYPSO -",
		"Patch Testing Ground for TETHYS",
	};
	static const short TITLE_LINES_START = 1;

	static const short NUM_INFO_LINES = 11;
	static const char* const INFO_TEXT[] = {
		"Controls (Gamepad)",
		"  Up/Down = Move Cursor",
		"  Left/Right = Lower/Raise Value",
		"  L/R = Change Slot",
		"  Y/Z = Change Editor.",
		"  Press A to play the current Patch.",
		"  Hold X to only play the.",
		"    current Instrument.",
		"  Hold B for coarser value changes.",
		"  Hold C for less precise changes.",
		" Consult the readme for more info.",
	};
	static const char* const INFO_TEXT_KEYS[] = {
		"Controls (Keyboard)",
		"  Up/Down = Move Cursor",
		"  Left/Right = Lower/Raise Value",
		"  Tab + Left/Right = Change Slot",
		"  Tab + Up/Down = Change Editor",
		"  Press keys to play the Patch.",
		"  Hold Shift to only play the",
		"    current Instrument.",
		"  Hold Ctrl for less precise changes.",
		"  Hold Alt for coarser value changes.",
		"Consult the readme for more info.",
	};
	static const short INFO_LINES_START = 4;

	static const short NUM_CREDITS_LINES = 10;
	static const char* const CREDITS_TEXT[] = {
		"Credits",
		"  Modified from the PoneSound Driver",
		"    developed by Ponut.",
		"  GUI provided through the Jo Engine",
		"    developed by Johannes Fetz.",
		"  DSP code adapted from the",
		"    Audio Overload SDK",
		"    developed by R. Belmont",
		"    & Richard Bannister.",
		"  Developed by Teuthida.",
	};
	static const short CREDITS_LINES_START = 17;

	static const short NUM_SAVELOAD_LINES = 12;
	static const char* SAVELOAD_TEXT[] = {
		"    Press %c or %c to load or save.   ",
		"     What would you like to do?     ",
		"     Erase all unsaved settings?    ",
		"        Loading save data...        ",
		"       File loaded! Have fun!       ",
		"    Overwrite any previous data?    ",
		"           Saving data...           ",
		"     Save file created! Enjoy!      ",
		"  Couldn't find a backup device...  ",
		"           Saving failed!           ",
		"  Couldn't locate the save file...  ",
		"          Loading failed!           ",
	};
	static const char* SAVELOAD_PROMPTS[] = {
		"",
		"        (%c = Load, %c = Save)        ",
		"         (%c = Yes, %c = No)          ",
		"",
		"",
		"         (%c = Yes, %c = No)          ",
		"",
		"",
		" Please connect one and try again.  ",
		"",
		"",
		"",
	};

	enum saveload_states {
		SAVELOAD_INITIAL,
		SAVELOAD_CHOICE,
		SAVELOAD_LOAD_PROMPT,
		SAVELOAD_LOAD_WAIT,
		SAVELOAD_LOAD_DONE,
		SAVELOAD_SAVE_PROMPT,
		SAVELOAD_SAVE_WAIT,
		SAVELOAD_SAVE_DONE,
		SAVELOAD_ERROR_NODEVICE,
		SAVELOAD_ERROR_SAVEFAILURE,
		SAVELOAD_ERROR_SAVEMISSING,
		SAVELOAD_ERROR_LOADFAILURE,
	};

	static short saveload_status = 0;
	if (jo_is_input_key_down(0, JO_KEY_START) && !disable_inputs) {
		show_info_page = !show_info_page;
		saveload_status = 0;
	}

	chn_remove_melodic_flags();

	if (show_info_page) {
		chn_cease_all();

		bool left_pressed = jo_is_input_key_down(0, JO_KEY_L);
		bool right_pressed = jo_is_input_key_down(0, JO_KEY_R);

		static jo_backup_device dev = JoInternalMemoryBackup;

		if ((left_pressed || right_pressed) && !disable_inputs) {
			switch (saveload_status) {
			case SAVELOAD_INITIAL:
				saveload_status = SAVELOAD_CHOICE;
				break;
			case SAVELOAD_CHOICE:
				saveload_status = (right_pressed) ? SAVELOAD_SAVE_PROMPT : SAVELOAD_LOAD_PROMPT;
				break;
			case SAVELOAD_LOAD_PROMPT:
				if (right_pressed) {
					saveload_status = SAVELOAD_INITIAL;
				}
				else {
					saveload_status = SAVELOAD_LOAD_WAIT;
					disable_inputs = true;
				}
				break;
			case SAVELOAD_SAVE_PROMPT:
				if (right_pressed) {
					saveload_status = SAVELOAD_INITIAL;
				}
				else {
					saveload_status = SAVELOAD_SAVE_WAIT;
					disable_inputs = true;
				}
				break;
			}
		}

		char left_button = (using_keyboard) ? 'Q' : 'L';
		char right_button = (using_keyboard) ? 'E' : 'R';

		for (short i = 0; i < 29; i++) {
			jo_clear_screen_line(i + TITLE_LINES_START);
			if (i >= TITLE_LINES_START && i < TITLE_LINES_START + NUM_TITLE_LINES)
				jo_printf_with_color(1, i + TITLE_LINES_START, JO_COLOR_INDEX_Yellow, TITLE_TEXT[i - TITLE_LINES_START]);
			else if (i >= INFO_LINES_START && i < INFO_LINES_START + NUM_INFO_LINES)
				jo_printf_with_color(1, i + TITLE_LINES_START, JO_COLOR_INDEX_Purple, ((!using_keyboard) ? INFO_TEXT[i - INFO_LINES_START] : INFO_TEXT_KEYS[i - INFO_LINES_START]));
			else if (i >= CREDITS_LINES_START && i < CREDITS_LINES_START + NUM_CREDITS_LINES)
				jo_printf_with_color(1, i + TITLE_LINES_START, JO_COLOR_INDEX_Red, CREDITS_TEXT[i - CREDITS_LINES_START]);
			else if (i == INFO_LINES_START + NUM_INFO_LINES && saveload_status < NUM_SAVELOAD_LINES)
				jo_printf_with_color(1, i + TITLE_LINES_START, JO_COLOR_INDEX_Green, SAVELOAD_TEXT[saveload_status], left_button, right_button);
			else if (i == INFO_LINES_START + NUM_INFO_LINES + 1 && saveload_status < NUM_SAVELOAD_LINES) {
				if (saveload_status <= SAVELOAD_ERROR_NODEVICE)
					jo_printf_with_color(1, i + TITLE_LINES_START, JO_COLOR_INDEX_Green, SAVELOAD_PROMPTS[saveload_status], left_button, right_button);
				else
					jo_printf_with_color(1, i + TITLE_LINES_START, JO_COLOR_INDEX_Green, "Error Code: %d", jo_backup_get_last_status(dev));
			}
		}

		if (disable_inputs) {
			if (saveload_status == SAVELOAD_LOAD_WAIT) {
				if (jo_backup_mount(JoCartridgeMemoryBackup)) {
					dev = JoCartridgeMemoryBackup;
				}
				else if (jo_backup_mount(JoExternalDeviceBackup)) {
					dev = JoExternalDeviceBackup;
				}
				if (dev == JoInternalMemoryBackup) {
					saveload_status = SAVELOAD_ERROR_NODEVICE;
				}
				else {
					short load_result = load_save_data(dev, false);
					switch (load_result) {
					case 1:
						saveload_status = SAVELOAD_ERROR_SAVEMISSING;
						break;
					case 2:
						saveload_status = SAVELOAD_ERROR_LOADFAILURE;
						break;
					default:
						saveload_status = SAVELOAD_LOAD_DONE;
					}
				}
			}
			else if (saveload_status == SAVELOAD_SAVE_WAIT) {
				
				if (jo_backup_mount(JoCartridgeMemoryBackup)) {
					dev = JoCartridgeMemoryBackup;
				}
				else if (jo_backup_mount(JoExternalDeviceBackup)) {
					dev = JoExternalDeviceBackup;
				}
				if (dev == JoInternalMemoryBackup) {
					saveload_status = SAVELOAD_ERROR_NODEVICE;
				}
				else {
					short save_result = save_data(dev);
					switch (save_result) {
					case 1:
					case 2:
					case 3:
					case 4:
					case 5:
						saveload_status = SAVELOAD_ERROR_SAVEFAILURE;
						break;
					case 6:
					case 7:
					case 8:
					case 9:
					case 10:
						saveload_status = SAVELOAD_ERROR_SAVEMISSING;
						break;
					default:
						saveload_status = SAVELOAD_SAVE_DONE;
					}
				}	
			}
			else { // This should never be
				saveload_status = SAVELOAD_INITIAL;
			}
			disable_inputs = false;
		}
	}

	else {
		static short current_note = 0;

		bool cut_sound, coarse, while_pressed;
		if (!using_keyboard) {
			cut_sound = false;
			bool old_play_single = play_single;
			play_single = jo_is_input_key_pressed(0, JO_KEY_X);
			if (play_single != old_play_single) {
				cut_sound = true;
			}
			coarse = (jo_is_input_key_pressed(0, JO_KEY_B));
			while_pressed = (jo_is_input_key_pressed(0, JO_KEY_C));
		}
		else {
			cut_sound = is_char_key_pressed('|');
			coarse = is_modifier_held(MOD_ALT);
			while_pressed = is_modifier_held(MOD_CTRL);
			bool old_play_single = play_single;
			play_single = is_modifier_held(MOD_SHIFT);
			if (play_single != old_play_single) {
				cut_sound = true;
			}
		}

		bool move_left = (while_pressed) ? jo_is_input_key_pressed(0, JO_KEY_LEFT) : jo_is_input_key_down(0, JO_KEY_LEFT);
		bool move_right = (while_pressed) ? jo_is_input_key_pressed(0, JO_KEY_RIGHT) : jo_is_input_key_down(0, JO_KEY_RIGHT);
		bool move_up = (while_pressed) ? jo_is_input_key_pressed(0, JO_KEY_UP) : jo_is_input_key_down(0, JO_KEY_UP);
		bool move_down = (while_pressed) ? jo_is_input_key_pressed(0, JO_KEY_DOWN) : jo_is_input_key_down(0, JO_KEY_DOWN);
		
		bool instrument_left = false;
		bool instrument_right = false;

		bool octave_up = false;
		bool octave_down = false;

		// look for a patch instrument that contains the current one
		patch_offset = 0;
		if (!play_single && option_values[EDITOR_INST][instrument_editor.current_selector][INST_PATCHLENGTH] == 0) {
			for (short i = 1; i <= 7; i++) {
				if (instrument_editor.current_selector - i < 0) {
					break;
				}
				if (option_values[EDITOR_INST][instrument_editor.current_selector - i][INST_PATCHLENGTH] >= i) {
					patch_offset = i;
					break;
				}
			}
		}
		if (play_single) {
			patch_offset = 0;
		}

		short new_editor = current_editor;
		if (!using_keyboard) {
			instrument_left = (while_pressed) ? jo_is_input_key_pressed(0, JO_KEY_L) : jo_is_input_key_down(0, JO_KEY_L);
			instrument_right = (while_pressed) ? jo_is_input_key_pressed(0, JO_KEY_R) : jo_is_input_key_down(0, JO_KEY_R);

			if (jo_is_input_key_down(0, JO_KEY_Z)) {
				new_editor = offsetWithClamp(current_editor, 1, 0, NUM_EDITORS - 1);
			}
			if (jo_is_input_key_down(0, JO_KEY_Y)) {
				new_editor = offsetWithClamp(current_editor, -1, 0, NUM_EDITORS - 1);
			}
			if (new_editor != current_editor) {
				current_editor = new_editor;
				move_left = false;
				move_right = false;
				move_up = false;
				move_down = false;
				instrument_left = false;
				instrument_right = false;
				should_write = true;
			}

			if (jo_is_input_key_down(0, JO_KEY_A)) {
				jam_channels[channel_editor.current_selector].play_state = 1;
				jam_channels[channel_editor.current_selector].note = 1;
				should_write = true;
			}
			else if (!jo_is_input_key_pressed(0, JO_KEY_A)) {
				should_write = true;
				for (short i = 0; i < NUM_CHANNELS; i++) {
					jam_channels[i].play_state = 0;
				}
			}
		}
		else {
			octave_up = (while_pressed) ? is_special_key_pressed(KEY_F4) : is_special_key_down(KEY_F4);
			octave_up = (while_pressed) ? is_special_key_pressed(KEY_F4) : is_special_key_down(KEY_F4);
			octave_down = (while_pressed) ? is_special_key_pressed(KEY_F3) : is_special_key_down(KEY_F3);

			if (octave_up || octave_down) {
				octave = offset_option(&octave_option, octave, octave_down, coarse, 0, false);
			}
			if (is_special_key_pressed(KEY_TAB)) {
				
				instrument_left = (while_pressed) ? is_special_key_pressed(KEY_LEFT) : is_special_key_down(KEY_LEFT);
				instrument_right = (while_pressed) ? is_special_key_pressed(KEY_RIGHT) : is_special_key_down(KEY_RIGHT);

				if (jo_is_input_key_down(0, JO_KEY_DOWN)) {
					new_editor = offsetWithClamp(current_editor, 1, 0, NUM_EDITORS - 1);
				}
				if (jo_is_input_key_down(0, JO_KEY_UP)) {
					new_editor = offsetWithClamp(current_editor, -1, 0, NUM_EDITORS - 1);
				}
				if (new_editor != current_editor) {
					current_editor = new_editor;
					should_write = true;
				}

				move_left = false;
				move_right = false;
				move_up = false;
				move_down = false;
			}
			else if (new_signal_found()) {
				short last_scancode = get_last_scancode();
				static short last_used_channel = -1;
				if (is_last_key_pressed() && SCANCODE_TO_NOTE[last_scancode] != -1) {
					should_write = true;

					current_note = SCANCODE_TO_NOTE[last_scancode];
					short base_instrument = instrument_editor.current_selector - patch_offset;
					short patchlength = option_values[EDITOR_INST][(current_editor >= EDITOR_INST) ? base_instrument : 0][INST_PATCHLENGTH];
					if (base_instrument + patchlength >= NUM_INSTRUMENTS) {
						patchlength = NUM_INSTRUMENTS - instrument_editor.current_selector - 1;
					}
					// Find a channel the note can occupy (or has already occupied)
					for (short i = 0; i < NUM_CHANNELS - patchlength; i += patchlength + 1) {
						if (jam_channels[i].note == current_note) {
							if (jam_channels[i].scancode != last_scancode) {
									
								set_jam_channel(jam_channels, i, current_note, last_scancode, patchlength);
								option_values[EDITOR_CHAN][i][CHAN_NOTE] = current_note + (octave * 12);
								break;
							}
						}
						else if (jam_channels[i].note == -1) {
							set_jam_channel(jam_channels, i, current_note, last_scancode, patchlength);
							option_values[EDITOR_CHAN][i][CHAN_NOTE] = current_note + (octave * 12);
							last_used_channel = i;
							break;
						}
						else if (i + patchlength + 1 >= NUM_CHANNELS - patchlength) {
							last_used_channel += patchlength + 1;
							if (last_used_channel >= NUM_CHANNELS - patchlength) {
								last_used_channel = 0;
							}
							set_jam_channel(jam_channels, last_used_channel, current_note, last_scancode, patchlength);
							option_values[EDITOR_CHAN][last_used_channel][CHAN_NOTE] = current_note + (octave * 12);
							break;
						}
					}
				}
				else if (!is_last_key_pressed() && SCANCODE_TO_NOTE[last_scancode] != -1) {
					should_write = true;					
					for (short i = 0; i < NUM_CHANNELS; i++) {
						if (jam_channels[i].scancode == last_scancode) {
							jam_channels[i].play_state = 0;
						}
					}
				}
			}
		}
		bool should_offset = (move_left || move_right);

		

		option_page* editor = editors[current_editor];
		const edit_option* selector = editor->selector;

		short cursor_move_amount = (coarse) ? OPTIONS_PER_PAGE : 1;

		bool changed_instrument = false;
		bool changed_patchlength = false;

		if (instrument_left || instrument_right) {
			short new_instrument = offset_option(selector, editor->current_selector, instrument_left, coarse, 0, false);
			if (new_instrument != editor->current_selector) {
				editor->current_selector = new_instrument;
				changed_instrument = true;
				should_write = true;
			}
		}
		
		short highlight_color = JO_COLOR_INDEX_Yellow;
		if (coarse && while_pressed)
			highlight_color = JO_COLOR_INDEX_Green;
		else if (coarse)
			highlight_color = JO_COLOR_INDEX_Blue;
		else if (while_pressed)
			highlight_color = JO_COLOR_INDEX_Red;

		for (short i = 0; i <= 1; i++) {
			jo_clear_screen_line(i);
		}
		jo_clear_screen_line(2);
		if (play_single) {
			jo_printf_with_color(1, 2, JO_COLOR_INDEX_Yellow, "Play/Edit Mode: Single Slot");
		}
		else {
			jo_printf_with_color(1, 2, JO_COLOR_INDEX_Yellow, "Play/Edit Mode: Whole Patch");
		}
		jo_clear_screen_line(3);
		
		bool is_playing = false;
		static const char* editor_names[] = {
				"Multi-Sample",
				"Sample",
				"Instrument",
		};
		for (short i = 0; i < NUM_CHANNELS; i++) {
			if (jam_channels[i].play_state != 0) {
				is_playing = true;
			}
		}
		if (!is_playing) {
			jo_printf_with_color(1, 3, JO_COLOR_INDEX_White, "State: Stopped");
		}
		else if (current_editor < EDITOR_INST) {
			jo_printf_with_color(1, 3, JO_COLOR_INDEX_White, "State: Playing %s %d", editor_names[current_editor], editor->current_selector);
		}
		else {
			if (chnCtrl[0].instrumentID == instrument_editor.current_selector && option_values[EDITOR_INST][instrument_editor.current_selector][INST_PATCHLENGTH] == 0) {
				jo_printf_with_color(1, 3, JO_COLOR_INDEX_White, "State: Playing %s %d", editor_names[EDITOR_INST], instrument_editor.current_selector);
			}
			else {
				jo_printf_with_color(1, 3, JO_COLOR_INDEX_White, "State: Playing Patch %d", chnCtrl[0].instrumentID);
			}
		}
		
		for (short i = 4; i <= 6; i++) {
			jo_clear_screen_line(i);
		}

		if (using_keyboard) print_option(2, 5, highlight_color, true, &octave_option, octave, 0, 0);
		if (current_editor != EDITOR_CHAN) print_option(2, 6, highlight_color, true, selector, editor->current_selector, 0, 0);
			
		for (short i = 7; i <= 9; i++) {
			jo_clear_screen_line(i);
		}
		short current_page = (editor->current_option / OPTIONS_PER_PAGE);
		if (current_page > 0) {
			jo_printf_with_color(19, 8, JO_COLOR_INDEX_White, "^ Previous...");
		}
		short* current_options = option_values[current_editor][(short)editor->current_selector];
		
		if (should_offset) {
			short prev_value = current_options[editor->current_option];
			short current_value;
			if (current_editor == EDITOR_MULT) {
				bool is_keysplit = (option_values[current_editor][editor->current_selector][MULT_TYPE] != 0);
				if (editor->current_option >= MULT_DATA) {
					const edit_option* data_option = (is_keysplit) ? ((editor->current_option % 2 == 1) ? &multi_splt : &multi_note) : &multi_samp;
					current_value = offset_option(data_option, current_options[editor->current_option], move_left, coarse, samples[sample_editor.current_selector], is_keysplit);
				}
				else {
					current_value = offset_option(&(editor->options[editor->current_option]), current_options[editor->current_option], move_left, coarse, samples[sample_editor.current_selector], is_keysplit);
				}
			}
			else if (current_editor == EDITOR_MACR) {
				if (editor->current_option >= MACR_DATA) {
					bool is_variablelength = (option_values[current_editor][editor->current_selector][MACR_VARIABLE_LENGTH] != 0);
					bool is_relative = (option_values[current_editor][editor->current_selector][MACR_MODE] != 0);
					short type_id = get_type_id(option_values[current_editor][editor->current_selector][MACR_TYPE]);
					const edit_option* nonlength_option = (is_relative) ? &macro_data_rel : &(macro_data_abs[type_id]);
					const edit_option* data_option = is_variablelength ? ((editor->current_option % 2 == 1) ? &macro_length : nonlength_option) : nonlength_option;
					current_value = offset_option(data_option, current_options[editor->current_option], move_left, coarse, samples[sample_editor.current_selector], is_variablelength);
				}
				else if (editor->current_option == MACR_MODE) {
					bool allow_relative = (MACRO_TYPE_TO_INTERNAL[option_values[current_editor][editor->current_selector][MACR_TYPE]] >= FIRST_RELATIVE);
					const edit_option* mode_option = allow_relative ? &macro_mode_permit_rel : &macro_mode_forbid_rel;
					current_value = offset_option(mode_option, current_options[editor->current_option], move_left, coarse, samples[sample_editor.current_selector], false);
				}
				else {
					current_value = offset_option(&(editor->options[editor->current_option]), current_options[editor->current_option], move_left, coarse, samples[sample_editor.current_selector], false);
				}
			}
			else if (current_editor == EDITOR_INST && editor->current_option >= INST_MACRO) {
				current_value = offset_option(&(editor->options[INST_MACRO]), current_options[editor->current_option], move_left, coarse, samples[sample_editor.current_selector], false);
			}
			else {
				current_value = offset_option(&(editor->options[editor->current_option]), current_options[editor->current_option], move_left, coarse, samples[sample_editor.current_selector], false);
			}

			did_offset = (prev_value != current_value);
			if (did_offset) {
				if (current_editor == EDITOR_MULT && editor->current_option == MULT_TYPE) {
					bool to_interleaved = (current_value == 1);
					if (to_interleaved) {
						if (option_values[current_editor][editor->current_selector][MULT_LENGTH] > (MLT_CTRL_LENGTH_MAX / 2)) {
							option_values[current_editor][editor->current_selector][MULT_LENGTH] = MLT_CTRL_LENGTH_MAX / 2;
						}
					}
					convert_interleaved_data(option_values[current_editor][editor->current_selector], multi_conversion_buffer, to_interleaved, MULT_DATA, MLT_CTRL_LENGTH_MAX, NUM_MULT_OPTIONS);
				}
				else if (current_editor == EDITOR_MACR) {
					bool was_relative = (option_values[current_editor][editor->current_selector][MACR_MODE] != 0);
					short type_id = get_type_id(option_values[current_editor][editor->current_selector][MACR_TYPE]);
					const edit_option* old_data_option = (was_relative) ? &macro_data_rel : &(macro_data_abs[type_id]);
					if (editor->current_option == MACR_VARIABLE_LENGTH) {
						bool to_interleaved = (current_value == 1);
						if (to_interleaved) {
							if (option_values[current_editor][editor->current_selector][MACR_LENGTH] > (MCR_CTRL_LENGTH_MAX / 2)) {
								option_values[current_editor][editor->current_selector][MACR_LENGTH] = MCR_CTRL_LENGTH_MAX / 2;
							}
						}
						convert_interleaved_data(option_values[current_editor][editor->current_selector], macro_conversion_buffer, to_interleaved, MACR_DATA, MCR_CTRL_LENGTH_MAX, NUM_MACR_OPTIONS);
					}
					else if (editor->current_option == MACR_TYPE) {
						if (MACRO_TYPE_TO_INTERNAL[current_value] < FIRST_RELATIVE) {
							option_values[current_editor][editor->current_selector][MACR_MODE] = 0;
						}
					}
					current_options[editor->current_option] = current_value;
					for (short i = 0; i < NUM_CHANNELS; i++) {
						chn_hard_reset_macros(i);
					}

					bool is_relative = (option_values[current_editor][editor->current_selector][MACR_MODE] != 0);
					type_id = get_type_id(option_values[current_editor][editor->current_selector][MACR_TYPE]);
					const edit_option* new_data_option = (is_relative) ? &macro_data_rel : &(macro_data_abs[type_id]);
					if (new_data_option != old_data_option) {
						short increment_amount = (current_options[MACR_VARIABLE_LENGTH] != 0) ? 2 : 1;
						for (short i = MACR_DATA; i < MACR_DATA + (((current_options[MACR_VARIABLE_LENGTH] != 0)) ? (MCR_CTRL_LENGTH_MAX * 2) : MCR_CTRL_LENGTH_MAX); i += increment_amount) {
							int clamped_value;
							if (old_data_option->display_type == DISP_MACROVALUEUNSIGNED || old_data_option->display_type == DISP_MACROVALUEUNSIGNEDHEX) {
								clamped_value = (unsigned short)(current_options[i]);
							}
							else if (old_data_option->display_type == DISP_MACROVALUEADDONE) {
								clamped_value = (short)current_options[i] + 1;
							}
							else {
								clamped_value = (short)current_options[i];
							}
							

							if (new_data_option->display_type == DISP_MACROVALUEADDONE) {
								clamped_value--;
							} 
							
							// Fixes an inexplicable bug where the compiler interprets an int assignment of 0xFFFF as 0xFFFFFFFF
							if (new_data_option->display_type == DISP_MACROVALUEUNSIGNED || new_data_option->display_type == DISP_MACROVALUEUNSIGNEDHEX) {
								clamped_value = offsetIntWithClamp(clamped_value, 0, (unsigned short)new_data_option->lower_bound, (unsigned short)new_data_option->upper_bound);
								current_options[i] = (unsigned short)clamped_value;
							}
							else {
								clamped_value = offsetIntWithClamp(clamped_value, 0, new_data_option->lower_bound, new_data_option->upper_bound);
								current_options[i] = (short)clamped_value;
							}
						}
					}
				}
				else if (current_editor == EDITOR_INST) {
					if (editor->current_option == INST_PATCHLENGTH && !play_single) {
						changed_patchlength = true;
					}
				}
			}

			current_options[editor->current_option] = current_value;

			should_write = true;
		}

		if (current_editor == EDITOR_MULT || current_editor == EDITOR_MACR) {
			if (current_editor == EDITOR_MULT) {
				if (option_values[current_editor][editor->current_selector][MULT_TYPE] != 0) {
					editor->num_options = MULT_DATA + option_values[current_editor][editor->current_selector][MULT_LENGTH] * 2 + 1;
				}
				else {
					editor->num_options = MULT_DATA + option_values[current_editor][editor->current_selector][MULT_LENGTH] + 1;
				}
			}
			else if (current_editor == EDITOR_MACR) {
				if (option_values[current_editor][editor->current_selector][MACR_VARIABLE_LENGTH] != 0) {
					editor->num_options = MACR_DATA + option_values[current_editor][editor->current_selector][MACR_LENGTH] * 2;
				}
				else {
					editor->num_options = MACR_DATA + option_values[current_editor][editor->current_selector][MACR_LENGTH];
				}
			}
			
			if (editor->current_option >= editor->num_options) {
				editor->current_option = editor->num_options - 1;
			}
		}

		if (move_up) {
			editor->current_option -= cursor_move_amount;
			if (editor->current_option < 0)
				editor->current_option = 0;
		}

		if (move_down) {
			editor->current_option += cursor_move_amount;
			if (editor->current_option >= editor->num_options)
				editor->current_option = editor->num_options - 1;
		}


		if (changed_instrument || cut_sound || changed_patchlength) {
			chn_cease_all();
			for (short i = 0; i < NUM_CHANNELS; i++) {
				jam_channels[i].play_state = 0;
			}
		}

		if (current_editor == EDITOR_PCM) {
			should_set_pcm = true;
		}

		short page_start = OPTIONS_PER_PAGE * current_page;
		short page_end = OPTIONS_PER_PAGE * (current_page + 1);

		// Only used when the current editor is EDITOR_MULT or EDITOR_MACR
		bool is_keysplit = (option_values[current_editor][editor->current_selector][MULT_TYPE] != 0);
		bool is_variablelength = (option_values[current_editor][editor->current_selector][MACR_VARIABLE_LENGTH] != 0);
		bool is_relative = (option_values[current_editor][editor->current_selector][MACR_MODE] != 0);
		short type_id = get_type_id(option_values[current_editor][editor->current_selector][MACR_TYPE]);
		const edit_option* nonlength_option = (is_relative) ? &macro_data_rel : &(macro_data_abs[type_id]);
		for (short i = page_start; i < page_end; i++) {
			short text_offset = 10 + (i % OPTIONS_PER_PAGE);
			if (i < editor->num_options) {
				if (current_editor == EDITOR_MULT && i >= MULT_DATA) {
					if (is_keysplit) {
						short note_index = (i - MULT_DATA) / 2;
						if (i % 2 == 1) {
							print_option(1, text_offset, highlight_color, (i == editor->current_option), &multi_splt, current_options[i], samples[sample_editor.current_selector], note_index);
						}
						else {
							print_option(1, text_offset, highlight_color, (i == editor->current_option), &multi_note, current_options[i], samples[sample_editor.current_selector], note_index);
						}
					}
					else {
						short note_index = (i - MULT_DATA);
						print_option(1, text_offset, highlight_color, (i == editor->current_option), &multi_samp, current_options[i], samples[sample_editor.current_selector], note_index);
					}
				}
				else if (current_editor == EDITOR_MACR) {
					if (i >= MACR_DATA) {
						if (is_variablelength) {
							short macro_index = (i - MACR_DATA) / 2;
							if (i % 2 == 1){
								print_option(1, text_offset, highlight_color, (i == editor->current_option), &macro_length, current_options[i], samples[sample_editor.current_selector], macro_index);
							}
							else {
								print_option(1, text_offset, highlight_color, (i == editor->current_option), nonlength_option, current_options[i], samples[sample_editor.current_selector], macro_index);
							}
						}
						else {
							short macro_index = i - MACR_DATA;
							print_option(1, text_offset, highlight_color, (i == editor->current_option), nonlength_option, current_options[i], samples[sample_editor.current_selector], macro_index);
						}
					}
					else if (i == MACR_MODE) {
						bool allow_relative = (MACRO_TYPE_TO_INTERNAL[option_values[current_editor][editor->current_selector][MACR_TYPE]] >= FIRST_RELATIVE);
						const edit_option* mode_option = allow_relative ? &macro_mode_permit_rel : &macro_mode_forbid_rel;
						print_option(1, text_offset, highlight_color, (i == editor->current_option), mode_option, current_options[i], samples[sample_editor.current_selector], 0);
					}
					else {
						print_option(1, text_offset, highlight_color, (i == editor->current_option), &(editor->options[i]), current_options[i], samples[sample_editor.current_selector], 0);
					}
				}
				else if (current_editor == EDITOR_INST && i >= INST_MACRO) {
					print_option(1, text_offset, highlight_color, (i == editor->current_option), &(editor->options[INST_MACRO]), current_options[i], samples[sample_editor.current_selector], i - INST_MACRO + 1);
				}
				else {
					print_option(1, text_offset, highlight_color, (i == editor->current_option), &(editor->options[i]), current_options[i], samples[sample_editor.current_selector], 0);
				}
			}
			else {
				jo_clear_screen_line(text_offset);
			}
		}
		jo_clear_screen_line(24);
		jo_clear_screen_line(25);
		jo_clear_screen_line(26);
		if (current_page < getNumPages(editor)) {
			jo_printf_with_color(19, 25, JO_COLOR_INDEX_White, "v Next...");
		}
		if (!CALYPSO_DEBUG) {
			jo_clear_screen_line(27);
			jo_printf_with_color(1, 27, JO_COLOR_INDEX_White, "Sample bytes remaining: %d", 0x7F800 - (int)scsp_load);

			jo_clear_screen_line(28);
			if (using_keyboard)
				jo_printf_with_color(10, 28, JO_COLOR_INDEX_Purple, " Press ESC for help ");
			else
				jo_printf_with_color(10, 28, JO_COLOR_INDEX_Purple, "Press START for help");
			jo_clear_screen_line(29);
			jo_clear_screen_line(30);
		}
		else {
			/* Simple debug info*/
			jo_clear_screen_line(26);
			jo_printf_with_color(0, 26, JO_COLOR_INDEX_White, "00: %4x  01: %4x  02: %4x  03: %4x", test_func(0), test_func(1), test_func(2), test_func(3));
			jo_clear_screen_line(27);
			jo_printf_with_color(0, 27, JO_COLOR_INDEX_White, "08: %4x  09: %4x  0A: %4x  0B: %4x", test_func(8), test_func(9), test_func(10), test_func(11));
			jo_clear_screen_line(28);
			//jo_printf_with_color(0, 28, JO_COLOR_INDEX_White, "08: %4x  09: %4x  0A: %4x  0B: %4x", test_func(8), test_func(9), test_func(10), test_func(11));
			jo_clear_screen_line(29);
			//jo_printf_with_color(0, 29, JO_COLOR_INDEX_White, "0C: %4x  0D: %4x  0E: %4x  0F: %4x", test_func(12), test_func(13), test_func(14), test_func(15));
		}

		write_option_values_and_update();
		should_write = false;
		should_set_pcm = false;
		did_offset = false;
		jo_set_default_background_color(JO_COLOR_Black);

		
	}
	//slSynch();
}

static int input_id;
void			wait_for_input(void) {
	if (jo_is_input_key_down(0, JO_KEY_START)) {
		jo_core_remove_callback(input_id);
		jo_core_add_vblank_callback(sdrv_vblank_rq);
		jo_core_add_callback(my_draw);
	}
}

void			jo_main(void)
{
	jo_core_init(JO_COLOR_Black);

	jo_printf_with_color(1, 1, JO_COLOR_INDEX_Purple, "Welcome to Calypso!");

	if (!(jo_get_input_type(0) == JoRegularKeyboard || jo_get_input_type(0) == JoRegularGamepad || jo_get_input_type(0) == JoUnsupportedGamepad)) {
		jo_printf_with_color(1, 3, JO_COLOR_INDEX_Yellow, "Calypso can only be used with");
		jo_printf_with_color(1, 4, JO_COLOR_INDEX_Yellow, " a gamepad or keyboard in the");
		jo_printf_with_color(1, 5, JO_COLOR_INDEX_Yellow, " first controller slot.");

		jo_printf_with_color(1, 6, JO_COLOR_INDEX_Red, "Please connect a controller");
		jo_printf_with_color(1, 7, JO_COLOR_INDEX_Red, " and try again. %d", jo_get_input_type(0));

		while (true) {

		}
	}

	jo_printf_with_color(1, 3, JO_COLOR_INDEX_White, "Loading Driver...");

	load_drv(ADX_MASTER_2304);
	jo_printf(1, 3, "Loading Driver...Done!");
	
	/*
	To convert a sound to 16-bit
	ffmpeg -i %this%.wav -f s16be -ac 1 -ar (bitrate) %this%.PCM
	To convert a sound to 8-bit
	ffmpeg -i %this%.wav -f s8 -ac 1 -ar (bitrate) %this%.PCM
	*/

	short error_state = 0;
	for (short i = 0; i < NUM_SAMPLES; i++) {
		if (i < NumberOfSamples) { // Read from samples.h
			jo_printf(1, 5, "Loading Sample %d of %d...", i + 1, NumberOfSamples);
			jo_clear_screen_line(6);
			jo_printf(1, 6, "%d bytes of sample data remain...", 0x7F800 - (int)scsp_load);
			const sample_import* samp = &sample_data[i];
			switch (samp->data_type) {
			case PCM_8_Bit:
				samples[i] = load_8bit_pcm((Sint8*)(&samp->filename), samp->sample_rate, (samp->loop_type != No_Loop), false);
				break;
			case PCM_16_Bit:
				samples[i] = load_16bit_pcm((Sint8*)(&samp->filename), samp->sample_rate, (samp->loop_type != No_Loop), false);
				break;
			case PCM_8_Bit_With_Silence_At_Start:
				samples[i] = load_8bit_pcm((Sint8*)(&samp->filename), samp->sample_rate, (samp->loop_type != No_Loop), true);
				break;
			case PCM_16_Bit_With_Silence_At_Start:
				samples[i] = load_16bit_pcm((Sint8*)(&samp->filename), samp->sample_rate, (samp->loop_type != No_Loop), true);
				break;
			case PCM_8_Bit_With_FM_Padding:
				samples[i] = load_8bit_pcm_with_fm_padding((Sint8*)(&samp->filename), samp->sample_rate);
				break;
			case PCM_16_Bit_With_FM_Padding:
				samples[i] = load_16bit_pcm_with_fm_padding((Sint8*)(&samp->filename), samp->sample_rate);
				break;
			default:
				error_state = 1;
			}
			if (samples[i] == -1) {
				error_state = 2;
			}

			option_values[EDITOR_PCM][i][PCM_LOOPTYPE] = samp->loop_type;
			option_values[EDITOR_PCM][i][PCM_SAMPLESTART] = samp->playback_start;
			option_values[EDITOR_PCM][i][PCM_LOOPSTART] = samp->loop_start;
			option_values[EDITOR_PCM][i][PCM_LOOPEND] = (samp->loop_end == 0) ? pcmCtrl[samples[i]].max_playsize - 1 : samp->loop_end;
			option_values[EDITOR_PCM][i][PCM_BASE_NOTE] = samp->base_note;
			option_values[EDITOR_PCM][i][PCM_BASE_PITCH] = (samp->sample_rate == 0) ? 0x11b : pcmCtrl[samples[i]].base_note;

			if (samp->loop_type < No_Loop || samp->loop_type > Alternating_Loop) {
				error_state = 3;
			}

			else if (offsetUnsignedWithClamp(option_values[EDITOR_PCM][i][PCM_SAMPLESTART], 0, sample_options[PCM_SAMPLESTART].lower_bound, pcm_get_max_playsize(samples[i])) != samp->playback_start) {
				error_state = 4;
			}
			else if (offsetUnsignedWithClamp(option_values[EDITOR_PCM][i][PCM_LOOPSTART], 0, sample_options[PCM_LOOPSTART].lower_bound, pcm_get_max_playsize(samples[i]) - pcm_get_sample_start(samples[i])) != samp->loop_start) {
				error_state = 5;
			}
			else if (offsetUnsignedWithClamp(option_values[EDITOR_PCM][i][PCM_LOOPEND], 0, pcm_get_loop_start(samples[i]), pcm_get_max_playsize(samples[i]) - pcm_get_sample_start(samples[i]))
				!= ((samp->loop_end == 0) ? pcmCtrl[samples[i]].max_playsize - 1 : samp->loop_end)) {
				error_state = 6;
			}
			else if (offsetWithClamp(option_values[EDITOR_PCM][i][PCM_BASE_NOTE], 0, sample_options[PCM_BASE_NOTE].lower_bound, sample_options[PCM_BASE_NOTE].upper_bound) != option_values[EDITOR_PCM][i][PCM_BASE_NOTE]) {
				error_state = 7;
			}

			if (error_state != 0) {
				jo_printf_with_color(1, 8, JO_COLOR_INDEX_Yellow, "An error occured while loading!");
				switch (error_state) {
				case 1:
					jo_printf_with_color(1, 9, JO_COLOR_INDEX_Yellow, "'%s' did not have", samp->filename);
					jo_printf_with_color(1, 10, JO_COLOR_INDEX_Yellow, " a valid data type!");
					break;
				case 2:
					jo_printf_with_color(1, 9, JO_COLOR_INDEX_Yellow, "'%s' was too large or didn't", samp->filename);
					jo_printf_with_color(1, 10, JO_COLOR_INDEX_Yellow, " fit into RAM (or both)!");
					break;
				case 3:
					jo_printf_with_color(1, 9, JO_COLOR_INDEX_Yellow, "'%s' did not have", samp->filename);
					jo_printf_with_color(1, 10, JO_COLOR_INDEX_Yellow, " a valid sample loop type!");
					break;
				case 4:
					jo_printf_with_color(1, 9, JO_COLOR_INDEX_Yellow, "'%s' had a start point", samp->filename);
					jo_printf_with_color(1, 10, JO_COLOR_INDEX_Yellow, " larger than the sample!");
					jo_printf_with_color(1, 11, JO_COLOR_INDEX_Yellow, "(%u)", option_values[EDITOR_PCM][i][PCM_SAMPLESTART]);
					break;
				case 5:
					jo_printf_with_color(1, 9, JO_COLOR_INDEX_Yellow, "'%s' had a loop start point", samp->filename);
					jo_printf_with_color(1, 10, JO_COLOR_INDEX_Yellow, " larger than the sample!");
					jo_printf_with_color(1, 11, JO_COLOR_INDEX_Yellow, "(%u)", option_values[EDITOR_PCM][i][PCM_LOOPSTART]);
					break;
				case 6:
					jo_printf_with_color(1, 9, JO_COLOR_INDEX_Yellow, "'%s' had a loop end point", samp->filename);
					jo_printf_with_color(1, 10, JO_COLOR_INDEX_Yellow, " larger than the sample!");
					jo_printf_with_color(1, 11, JO_COLOR_INDEX_Yellow, "(%u)", option_values[EDITOR_PCM][i][PCM_LOOPEND]);
					break;
				case 7:
					jo_printf_with_color(1, 9, JO_COLOR_INDEX_Yellow, "'%s' had a invalid base note!", samp->filename, option_values[EDITOR_PCM][i][PCM_BASE_NOTE]);
					jo_printf_with_color(1, 9, JO_COLOR_INDEX_Yellow, "(%d)", option_values[EDITOR_PCM][i][PCM_BASE_NOTE]);
					break;
				default:
					jo_printf_with_color(1, 9, JO_COLOR_INDEX_Yellow, "An entirely undocumented");
					jo_printf_with_color(1, 10, JO_COLOR_INDEX_Yellow, " error occured! Tell the dev now!");
				}
				jo_printf_with_color(1, 13, JO_COLOR_INDEX_Red, "Please recompile the program");
				jo_printf_with_color(1, 14, JO_COLOR_INDEX_Red, " and try again.");
				while (true) {

				}
			}
		}
		else {
			samples[i] = copy_pcm(samples[0]);
			option_values[EDITOR_PCM][i][PCM_LOOPTYPE] = option_values[EDITOR_PCM][0][PCM_LOOPTYPE];
			option_values[EDITOR_PCM][i][PCM_SAMPLESTART] = option_values[EDITOR_PCM][0][PCM_SAMPLESTART];
			option_values[EDITOR_PCM][i][PCM_LOOPSTART] = option_values[EDITOR_PCM][0][PCM_LOOPSTART];
			option_values[EDITOR_PCM][i][PCM_LOOPEND] = option_values[EDITOR_PCM][0][PCM_LOOPEND];
			option_values[EDITOR_PCM][i][PCM_BASE_NOTE] = option_values[EDITOR_PCM][0][PCM_BASE_NOTE];
			option_values[EDITOR_PCM][i][PCM_BASE_PITCH] = option_values[EDITOR_PCM][0][PCM_BASE_PITCH];
		}
	}
	jo_printf(1, 5, "Loading Sample %d of %d...Done!", NumberOfSamples, NumberOfSamples);
	jo_clear_screen_line(6);
	jo_printf(1, 6, "%d bytes left of space...", 0x7F800 - (int)scsp_load);

	for (short i = 0; i < NUM_INSTRUMENTS; i++) {
		instruments[i] = initialize_new_instrument(i);
	}
	 
	init_sequenced_channels(NUM_CHANNELS);

	for (short i = 0; i < NUM_INSTRUMENTS; i++) {
		ins_enable_envelope(instruments[i]);
	}
	for (short i = 0; i < NUM_CHANNELS; i++) {
		chn_instrument_change(i, instruments[0]);
	}

	jo_printf(1, 7, "Checking backup memory...");

	bool load_defaults = false;
	bool check_external = false;

	if (!jo_backup_mount(JoCartridgeMemoryBackup)) {
		jo_printf(1, 7, "Checking backup memory...Not found!");
		check_external = true;
	}
	else if (load_save_data(JoCartridgeMemoryBackup, true) != 0) {
		jo_printf(1, 7, "Checking backup memory...No save found!");
		check_external = true;
	}
	if (check_external) {
		jo_printf(1, 8, "Checking external memory...");
		if (!jo_backup_mount(JoExternalDeviceBackup)) {
			jo_printf(1, 8, "Checking external memory...Not found!");
			load_defaults = true;
		}
		else if (load_save_data(JoExternalDeviceBackup, true) != 0) {
			jo_printf(1, 8, "Checking external memory...No save found!");
			load_defaults = true;
		}
	}
	if (load_defaults) {
		jo_printf(1, 9, "Loading default parameters...");
		init_option_values();
		jo_printf(1, 9, "Loading default parameters...Done!");
	}
	
	jo_printf(1, 11, "Setting up...");
	write_option_values_and_update();
	jo_printf(1, 11, "Setting up...Done!");

	if (jo_get_input_type(0) == JoRegularKeyboard) {
		jo_printf_with_color(1, 27, JO_COLOR_INDEX_Purple, "Press ESC to begin!");
	}
	else {
		jo_printf_with_color(1, 27, JO_COLOR_INDEX_Purple, "Press START to begin!");
	}

	input_id = jo_core_add_callback(wait_for_input);
	jo_core_run();

	
}

/*
** END OF FILE
*/

