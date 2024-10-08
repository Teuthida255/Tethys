//pcm_sys.c
//this file is compiled separately
//hopefully somewhat portable
//
#include <sl_def.h> //Mostly to link us with SBL file system
#include "pcmsys.h"
#include <SEGA_GFS.H>

static const int logtbl[] = {
	/* 0 */		0,
	/* 1 */		1,
	/* 2 */		2, 2,
	/* 4 */		3, 3, 3, 3,
	/* 8 */		4, 4, 4, 4, 4, 4, 4, 4,
	/* 16 */	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	/* 32 */	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
				/* 64 */	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
							7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
							7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
							7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
							/* 128 */	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
										8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
										8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
										8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
										8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
										8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
										8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
										8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
};

#define PCM_MSK1(a)				((a)&0x0001)
#define PCM_MSK3(a)				((a)&0x0007)
#define PCM_MSK4(a)				((a)&0x000F)
#define PCM_MSK5(a)				((a)&0x001F)
#define PCM_MSK10(a)			((a)&0x03FF)

#define PCM_SCSP_FREQUENCY					(44100L)

#define PCM_CALC_OCT(smpling_rate) 											\
		((int)logtbl[PCM_SCSP_FREQUENCY / ((smpling_rate) + 1)])

#define PCM_CALC_SHIFT_FREQ(oct)											\
		(PCM_SCSP_FREQUENCY >> (oct))

#define PCM_CALC_FNS(smpling_rate, shift_freq)								\
		((((smpling_rate) - (shift_freq)) << 10) / (shift_freq))

#define PCM_SET_PITCH_WORD(oct, fns)										\
		((int)((PCM_MSK4(-(oct)) << 11) | PCM_MSK10(fns)))

#define MELODIC_KEYON 0x40
#define MELODIC_KEYOFF 0x80

sysComPara* m68k_com = (sysComPara*)((SNDPRG + DRV_SYS_END) | 0x20000000);
static unsigned int* scsp_loading_start = (unsigned int*)(0x408 + DRV_SYS_END + 0x40); //Local loading address for sound data, is DRV_SYS_END ahead of the SNDPRG, and ahead of the communication data
unsigned int* scsp_load;
unsigned short* master_volume = (unsigned short*)(SNDRAM + 0x100400);
unsigned short driver_master_volume = 0;
short numberChans = 0;
short numberInsts = 0;
short numberPCMs = 0;

_CHN_CTRL chnCtrl[CHN_CTRL_MAX];
_INS_CTRL insCtrl[INS_CTRL_MAX];
_PCM_CTRL pcmCtrl[PCM_CTRL_MAX];
_MLT_CTRL mltCtrl[MLT_CTRL_MAX];
_MCR_CTRL mcrCtrl[MCR_CTRL_MAX];
short coefficients[NUM_COEFFICIENTS];
unsigned short addresses[NUM_ADDRESSES];
short base_coefficients[NUM_COEFFICIENTS];
unsigned short base_addresses[NUM_ADDRESSES];
short coefficientBounds[NUM_COEFFICIENTS][2];
unsigned short addressBounds[NUM_ADDRESSES][2];

const unsigned short macroBounds[NUM_MACRO_TYPES][2] = {
	{MACRO_KEYON_LBOUND,MACRO_KEYON_UBOUND},
	{MACRO_SAMPLETRAITS_LBOUND,MACRO_SAMPLETRAITS_UBOUND},
	{MACRO_ENVELOPETRAITS_LBOUND,MACRO_ENVELOPETRAITS_UBOUND},
	{MACRO_LFORESET_LBOUND,MACRO_LFORESET_UBOUND},
	{MACRO_PITCHLFOWAVEFORM_LBOUND,MACRO_PITCHLFOWAVEFORM_UBOUND},
	{MACRO_AMPLFOWAVEFORM_LBOUND,MACRO_AMPLFOWAVEFORM_UBOUND},
	{MACRO_EFFECTSLOT_LBOUND,MACRO_EFFECTSLOT_UBOUND},
	{MACRO_SAMPLEOFFSET_LBOUND,MACRO_SAMPLEOFFSET_UBOUND},
	{MACRO_LOOPSTART_LBOUND,MACRO_LOOPSTART_UBOUND},
	{MACRO_LOOPEND_LBOUND,MACRO_LOOPEND_UBOUND},
	{MACRO_TOTALLEVEL_LBOUND,MACRO_TOTALLEVEL_UBOUND},
	{MACRO_VOLUME_LBOUND,MACRO_VOLUME_UBOUND},
	{MACRO_ATTACK_LBOUND,MACRO_ATTACK_UBOUND},
	{MACRO_DECAY1_LBOUND,MACRO_DECAY1_UBOUND},
	{MACRO_SUSTAIN_LBOUND,MACRO_SUSTAIN_UBOUND},
	{MACRO_DECAY2_LBOUND,MACRO_DECAY2_UBOUND},
	{MACRO_RELEASE_LBOUND,MACRO_RELEASE_UBOUND},
	{MACRO_KEYSCALING_LBOUND,MACRO_KEYSCALING_UBOUND},
	{MACRO_LFOFREQ_LBOUND,MACRO_LFOFREQ_UBOUND},
	{MACRO_PITCHLFOSTRENGTH_LBOUND,MACRO_PITCHLFOSTRENGTH_UBOUND},
	{MACRO_AMPLFOSTRENGTH_LBOUND,MACRO_AMPLFOSTRENGTH_UBOUND},
	{MACRO_MODLEVEL_LBOUND,MACRO_MODLEVEL_UBOUND},
	{MACRO_PITCHMULTIPLIER_LBOUND,MACRO_PITCHMULTIPLIER_UBOUND},
	{MACRO_PITCHDIVIDER_LBOUND,MACRO_PITCHDIVIDER_UBOUND},
	{MACRO_LEVELSCALING_LBOUND,MACRO_LEVELSCALING_UBOUND},
	{MACRO_EFFECTVOLUME_LBOUND,MACRO_EFFECTVOLUME_UBOUND},
	{MACRO_INPUTLEVEL_LBOUND,MACRO_INPUTLEVEL_UBOUND},
	{MACRO_PAN_LBOUND,MACRO_PAN_UBOUND},
	{MACRO_NOTEOFFSET_LBOUND,MACRO_NOTEOFFSET_UBOUND},
	{MACRO_CENTDETUNE_LBOUND,MACRO_CENTDETUNE_UBOUND},
	{MACRO_REGISTERDETUNE_LBOUND,MACRO_REGISTERDETUNE_UBOUND},
	{MACRO_MODINPUTX_LBOUND,MACRO_MODINPUTX_UBOUND},
	{MACRO_MODINPUTY_LBOUND,MACRO_MODINPUTY_UBOUND},
	{MACRO_EFFECTPAN_LBOUND,MACRO_EFFECTPAN_UBOUND},
	{MACRO_EFFECTCOEF_LBOUND, MACRO_EFFECTCOEF_UBOUND},
	{MACRO_EFFECTADRS_LBOUND, MACRO_EFFECTADRS_UBOUND},
};

static short adx_coef_tbl[8][2] =
{
	{ADX_768_COEF_1,   ADX_768_COEF_2},
	{ADX_1152_COEF_1, ADX_1152_COEF_2},
	{ADX_1536_COEF_1, ADX_1536_COEF_2},
	{ADX_2304_COEF_1, ADX_2304_COEF_2},
	{ADX_640_COEF_1,   ADX_640_COEF_2},
	{ADX_960_COEF_1,   ADX_960_COEF_2},
	{ADX_1280_COEF_1, ADX_1280_COEF_2},
	{ADX_1920_COEF_1, ADX_1920_COEF_2}
};

static const jo_fixed semitone_mults[13] = {
	0x10000,
	0x10F38,
	0x11F59,
	0x1306F,
	0x1428A,
	0x155B8,
	0x16A09,
	0x17F91,
	0x1965F,
	0x1AE89,
	0x1C823,
	0x1E343,
	0x20000,
};

static const jo_fixed cent_mults[101] = {
	0x10000,
	0x10025,
	0x1004B,
	0x10071,
	0x10097,
	0x100BD,
	0x100E3,
	0x10109,
	0x1012F,
	0x10155,
	0x1017B,
	0x101A1,
	0x101C7,
	0x101ED,
	0x10214,
	0x1023A,
	0x10260,
	0x10286,
	0x102AC,
	0x102D3,
	0x102F9,
	0x1031F,
	0x10346,
	0x1036C,
	0x10392,
	0x103B9,
	0x103DF,
	0x10406,
	0x1042C,
	0x10453,
	0x10479,
	0x104A0,
	0x104C6,
	0x104ED,
	0x10513,
	0x1053A,
	0x10561,
	0x10587,
	0x105AE,
	0x105D5,
	0x105FB,
	0x10622,
	0x10649,
	0x10670,
	0x10696,
	0x106BD,
	0x106E4,
	0x1070B,
	0x10732,
	0x10759,
	0x10780,
	0x107A7,
	0x107CE,
	0x107F5,
	0x1081C,
	0x10843,
	0x1086A,
	0x10891,
	0x108B8,
	0x108DF,
	0x10907,
	0x1092E,
	0x10955,
	0x1097C,
	0x109A4,
	0x109CB,
	0x109F2,
	0x10A1A,
	0x10A41,
	0x10A68,
	0x10A90,
	0x10AB8,
	0x10ADF,
	0x10B06,
	0x10B2E,
	0x10B55,
	0x10B7D,
	0x10BA4,
	0x10BCC,
	0x10BF3,
	0x10C1B,
	0x10C43,
	0x10C6A,
	0x10C92,
	0x10CBA,
	0x10CE1,
	0x10D09,
	0x10D31,
	0x10D59,
	0x10D81,
	0x10DA9,
	0x10DD0,
	0x10DF8,
	0x10E20,
	0x10E48,
	0x10E70,
	0x10E98,
	0x10EC0,
	0x10EE8,
	0x10F10,
	0x10F38,
};

// MVOL is a 4-bit number; values 0-15 are valid.
// If the value is higher than 15, this will just cap it at 15.
void	set_master_volume(unsigned short volume)
{
	volume = (volume >= 0xF) ? 0xF : volume;
	*master_volume = 0x200 | (volume & 0xF);
	driver_master_volume = volume;
}

void	chn_play(short chnNumber, char ctrlType, char volume)
{
	if (chnNumber < 0) return;
	chnCtrl[chnNumber].volume = volume;
	chnCtrl[chnNumber].ctrlType = ctrlType;
	chnCtrl[chnNumber].sh2_permit = 1;
	chn_soft_reset_macros(chnNumber);
}

void	chn_play_melodic(short chnNumber) {
	if (chnNumber < 0) return;
	if (chnCtrl[chnNumber].melodic_data == 0) return;
	chnCtrl[chnNumber].melodic_data = chnCtrl[chnNumber].melodic_data | MELODIC_KEYON;
	chnCtrl[chnNumber].sh2_permit = 1;
	chn_soft_reset_macros(chnNumber);
}

void	chn_instrument_change(short chnNumber, short instrument) {
	if (chnNumber < 0) return;
	if (chnCtrl[chnNumber].instrumentID == instrument) return;
	chnCtrl[chnNumber].instrumentID = instrument;
	chnCtrl[chnNumber].bytes_per_blank = pcmCtrl[insCtrl[instrument].sampleID].bytes_per_blank;
	chn_cease(chnNumber);
	chn_hard_reset_macros(chnNumber);
}

void	chn_note_change(short chnNumber, unsigned char note) {
	if (chnNumber < 0) return;
	chnCtrl[chnNumber].note = note;
}

void	chn_velocity_change(short chnNumber, unsigned char velocity) {
	if (chnNumber < 0) return;
	chnCtrl[chnNumber].velocity = velocity;
}

void	chn_channel_volume_change(short chnNumber, unsigned char volume) {
	if (chnNumber < 0) return;
	chnCtrl[chnNumber].channel_volume = volume;
}

void	chn_reset_lfo(short chnNumber) {
	if (chnNumber < 0) return;
	chnCtrl[chnNumber].reset_bits |= 1;
}

void	chn_set_lfo(short chnNumber) {
	if (chnNumber < 0) return;
	chnCtrl[chnNumber].reset_bits &= 0xFE;
}


void	chn_set_melodic_data(short chnNumber, char slot) {
	if (chnNumber < 0) return;
	chnCtrl[chnNumber].melodic_data = slot + 1;
	chnCtrl[chnNumber].note = NOTE_INACTIVE;
}


void	chn_remove_melodic_flags(void) {
	for (short i = 0; i < CHN_CTRL_MAX; i++) {
		chnCtrl[i].melodic_data &= 0x3F;
	}
}


void	pcm_loop_type_change(short pcmNumber, unsigned char loop) {
	if (pcmNumber < 0) return;
	pcmCtrl[pcmNumber].loopType = loop;
}

void	pcm_sample_start_change(short pcmNumber, unsigned short start) {
	if (pcmNumber < 0) return;
	pcmCtrl[pcmNumber].sample_start = start;
}

void	pcm_loop_start_change(short pcmNumber, unsigned short start) {
	if (pcmNumber < 0) return;
	pcmCtrl[pcmNumber].LSA = start;
}

void	pcm_loop_end_change(short pcmNumber, unsigned short end) {
	if (pcmNumber < 0) return;
	pcmCtrl[pcmNumber].playsize = end + 1;
}

void	pcm_pitch_change(short pcmNumber, short sampleRate) {
	if (pcmNumber < 0) return;
	pcmCtrl[pcmNumber].base_pitch = convert_bitrate_to_pitchword(sampleRate);
}

void	pcm_base_pitch_change(short pcmNumber, short pitch) {
	if (pcmNumber < 0) return;
	pcmCtrl[pcmNumber].base_pitch = pitch;
}

void	pcm_base_note_change(short pcmNumber, unsigned char note) {
	if (pcmNumber < 0) return;
	pcmCtrl[pcmNumber].base_note = note;
}


unsigned short	pcm_get_max_playsize(short pcmNumber) {
	if (pcmNumber < 0) return 0;
	return pcmCtrl[pcmNumber].max_playsize - 1;
}

unsigned short	pcm_get_sample_start(short pcmNumber) {
	if (pcmNumber < 0) return 0;
	return pcmCtrl[pcmNumber].sample_start;
}

unsigned short	pcm_get_loop_start(short pcmNumber) {
	if (pcmNumber < 0) return 0;
	return pcmCtrl[pcmNumber].LSA;
}

unsigned short	pcm_get_loop_end(short pcmNumber) {
	if (pcmNumber < 0) return 0;
	return pcmCtrl[pcmNumber].playsize;
}


void	ins_sample_change(short insNumber, unsigned char sample) {
	if (insNumber < 0) return;
	insCtrl[insNumber].sampleID = sample;
}

void	ins_sample_type_change(short insNumber, unsigned char type) {
	if (insNumber < 0) return;
	insCtrl[insNumber].use_multi = type;
}

void	ins_sample_offset_change(short insNumber, unsigned short offset) {
	if (insNumber < 0) return;
	insCtrl[insNumber].sample_offset = offset;
}

void	ins_loopstart_offset_change(short insNumber, unsigned short offset) {
	if (insNumber < 0) return;
	insCtrl[insNumber].loopstart_offset = offset;
}

void	ins_loopend_offset_change(short insNumber, unsigned short offset) {
	if (insNumber < 0) return;
	insCtrl[insNumber].loopend_offset = offset;
}

void	ins_parameter_change(short insNumber, char volume, char pan)
{
	if (insNumber < 0) return;
	insCtrl[insNumber].volume = volume;
	insCtrl[insNumber].pan = pan;
}

void	ins_totallevel_change(short insNumber, unsigned char totalLevel) {
	if (insNumber < 0) return;
	insCtrl[insNumber].totallevel = totalLevel;
}

void	ins_lfo_freq_change(short insNumber, char lfo_freq) {
	if (insNumber < 0) return;
	insCtrl[insNumber].lfo_freq = lfo_freq;
}

void	ins_lfo_pitch_change(short insNumber, char lfo_strength, char lfo_wave) {
	if (insNumber < 0) return;
	insCtrl[insNumber].lfo_pitch = lfo_strength | (lfo_wave << 3);
}

void	ins_lfo_amp_change(short insNumber, char lfo_strength, char lfo_wave) {
	if (insNumber < 0) return;
	insCtrl[insNumber].lfo_amp = lfo_strength | (lfo_wave << 3);
}

void	ins_lfo_delay_change(short insNumber, char lfo_delay) {
	if (insNumber < 0) return;
	insCtrl[insNumber].lfo_delay = lfo_delay;
}

void	ins_bit_reverse_change(short insNumber, char bit_reverse, char invert) {
	if (insNumber < 0) return;
	insCtrl[insNumber].bit_reverse = bit_reverse ^ ((invert) ? 0x3 : 0);
}

void	ins_noise_mode_change(short insNumber, char noise_mode) {
	if (insNumber < 0) return;
	insCtrl[insNumber].noise_mode = noise_mode;
}

void	ins_attack_hold_change(short insNumber, char attack_hold, char hold_mode) {
	if (insNumber < 0) return;
	insCtrl[insNumber].attack_hold = attack_hold | ((hold_mode != 0) ? 1 : 0) << 5;
}
void	ins_decay1_change(short insNumber, char decay1) {
	if (insNumber < 0) return;
	insCtrl[insNumber].decay1 = decay1;
}
void	ins_sustain_change(short insNumber, char sustain) {
	if (insNumber < 0) return;
	insCtrl[insNumber].sustain = sustain;
}
void	ins_decay2_change(short insNumber, char decay2) {
	if (insNumber < 0) return;
	insCtrl[insNumber].decay2 = decay2;
}
void	ins_release_change(short insNumber, char release) {
	if (insNumber < 0) return;
	insCtrl[insNumber].release = release;
}
void	ins_key_scaling_sync_change(short insNumber, char key_scaling, char loop_sync) {
	if (insNumber < 0) return;
	insCtrl[insNumber].key_scaling_sync = key_scaling | ((loop_sync != 0) ? 1 : 0) << 4;
}

void	ins_enable_envelope(short insNumber) {
	if (insNumber < 0) return;
	insCtrl[insNumber].use_envelope = 1;
}

void	ins_disable_envelope(short insNumber) {
	if (insNumber < 0) return;
	insCtrl[insNumber].use_envelope = 0;
}

void	ins_patchlength_change(short insNumber, unsigned char length) {
	if (insNumber < 0) return;
	insCtrl[insNumber].patchlength = length;
}

void	ins_mod_volume_change(short insNumber, char mod_volume) {
	if (insNumber < 0) return;
	insCtrl[insNumber].mod_volume = mod_volume;
}

void	ins_mod_input_x_change(short insNumber, char mod_input, char generation) {
	if (insNumber < 0) return;
	insCtrl[insNumber].mod_input_x = mod_input | generation << 5;
}

void	ins_mod_input_y_change(short insNumber, char mod_input, char generation) {
	if (insNumber < 0) return;
	insCtrl[insNumber].mod_input_y = mod_input | generation << 5;
}

void	ins_effect_parameter_change(short insNumber, char volume, char pan) {
	if (insNumber < 0) return;
	insCtrl[insNumber].effect_volume = volume;
	insCtrl[insNumber].effect_pan = pan;
}

void	ins_effect_input_change(short insNumber, char level, char slot) {
	if (insNumber < 0) return;
	insCtrl[insNumber].input_volume = level;
	insCtrl[insNumber].effects_slot = slot;
}

void	ins_freq_ratio_change(short insNumber, unsigned char multiplier, unsigned char divider) {
	if (insNumber < 0) return;
	insCtrl[insNumber].freq_ratio = multiplier | (divider << 8);
}

void	ins_level_scaling_change(short insNumber, unsigned char scaling) {
	if (insNumber < 0) return;
	insCtrl[insNumber].level_scaling = scaling;
}

void	ins_note_offset_change(short insNumber, unsigned char semitone, unsigned char cent) {
	if (insNumber < 0) return;
	insCtrl[insNumber].pitch_offset = semitone | (cent << 8);
}

void	ins_register_detune_change(short insNumber, short detune) {
	if (insNumber < 0) return;
	insCtrl[insNumber].register_detune = detune;
}

void	ins_ignore_velocity_change(short insNumber, unsigned char ignore_velocity) {
	if (insNumber < 0) return;
	insCtrl[insNumber].ignore_velocity = ignore_velocity;
}

void	ins_macro_change(short insNumber, short index, unsigned char macro) {
	if (insNumber < 0) return;
	if (index >= INS_MACROS_MAX) return;
	insCtrl[insNumber].macros[index] = macro;
}


short	ins_get_current_sample(short insNumber, unsigned short note) {
	if (insNumber < 0) return 0;
	if (insCtrl[insNumber].use_multi == 0) {
		return insCtrl[insNumber].sampleID;
	}
	else {
		unsigned char mltNumber = insCtrl[insNumber].sampleID;
		char note_value = (char)note;
		short length = mlt_get_length(mltNumber);
		if (mlt_is_keysplit(mltNumber)) {
			// All but the last sample in a keysplit consist
			// of a sample ID followed by a cutoff point,
			// so each sample actually takes up two bytes
			if (length == 0) {
				return mltCtrl[mltNumber].data[0];
			}
			if (note_value >= (char)mltCtrl[mltNumber].data[length * 2 - 1]) {
				return mltCtrl[mltNumber].data[length * 2];
			}
			for (short i = 0; i <= length; i++) {
				if (note_value < (char)mltCtrl[mltNumber].data[i * 2 + 1]) {
					return mltCtrl[mltNumber].data[i * 2];
				}
			}
			// Fallback value
			return mltCtrl[mltNumber].data[length * 2];
		}
		else {
			if (note_value < 0) {
				return mltCtrl[mltNumber].data[0];
			}
			else if (note_value > length) {
				return mltCtrl[mltNumber].data[length];
			}
			else {
				return mltCtrl[mltNumber].data[(short)note_value];
			}
		}
	}
}

unsigned char ins_get_patchlength(short insNumber) {
	if (insNumber < 0) return 0;
	return insCtrl[insNumber].patchlength;
}


void	mlt_traits_change(short mltNumber, unsigned char length, unsigned char base_note_override, unsigned char type) {
	if (mltNumber < 0) return;
	mltCtrl[mltNumber].traits = length | (base_note_override << 6) | (type << 7);
}

void	mlt_set_data(short mltNumber, short index, unsigned char data) {
	if (mltNumber < 0) return;
	mltCtrl[mltNumber].data[index] = data;
}


short	mlt_get_length(short mltNumber) {
	if (mltNumber < 0) return 0;
	return mltCtrl[mltNumber].traits & 0x3F;
}

Bool	mlt_is_keysplit(short mltNumber) {
	if (mltNumber < 0) return 0;
	return ((mltCtrl[mltNumber].traits & 0x80) != 0);
}

Bool	mlt_is_base_note_override(short mltNumber) {
	if (mltNumber < 0) return 0;
	return ((mltCtrl[mltNumber].traits & 0x40) != 0);
}


void	mcr_type_change(short mcrNumber, unsigned char type) {
	if (mcrNumber < 0) return;
	mcrCtrl[mcrNumber].type = type;
}

void	mcr_length_change(short mcrNumber, unsigned char length) {
	if (mcrNumber < 0) return;
	mcrCtrl[mcrNumber].length = length;
}

void	mcr_traits_change(short mcrNumber, Bool variable_length, Bool relative, Bool fixed, Bool jump_on_release, Bool overflow, Bool continuous, Bool progressive, Bool propagate) {
	if (mcrNumber < 0) return;
	mcrCtrl[mcrNumber].traits = ((variable_length) ? MCR_VARIABLE_LENGTH : 0) | ((relative) ? MCR_RELATIVE : 0) | ((fixed) ? MCR_FIXED : 0) | ((jump_on_release) ? MCR_JUMP_ON_RELEASE : 0) |
		((overflow) ? MCR_OVERFLOW : 0) | ((continuous) ? MCR_CONTINUOUS : 0) | ((progressive) ? MCR_PROGRESSIVE : 0) | ((propagate) ? MCR_PROPAGATE : 0);
}

void	mcr_loop1_change(short mcrNumber, unsigned char point) {
	if (mcrNumber < 0) return;
	mcrCtrl[mcrNumber].loop_1 = point;
}

void	mcr_loop2_change(short mcrNumber, unsigned char point) {
	if (mcrNumber < 0) return;
	mcrCtrl[mcrNumber].loop_2 = point;
}

void	mcr_release_change(short mcrNumber, unsigned char point) {
	if (mcrNumber < 0) return;
	mcrCtrl[mcrNumber].release = point;
}

void	mcr_set_data(short mcrNumber, short index, unsigned short data) {
	if (mcrNumber < 0) return;
	mcrCtrl[mcrNumber].data[index] = data;
}


short	chn_get_current_instrument(short chnNumber) {
	if (chnNumber < 0) return 0;
	if (chnCtrl[chnNumber].instrumentID < INS_CTRL_MAX) {
		return chnCtrl[chnNumber].instrumentID;
	}
	else if (chnCtrl[chnNumber].instrumentID >= CHN_INHERIT_MIN && chnCtrl[chnNumber].instrumentID <= CHN_INHERIT_MAX) {
		return -(chnCtrl[chnNumber].instrumentID - (CHN_INHERIT_MIN - 1));
	}
	return 0;
}

void	chn_hard_reset_macros(short chnNumber) {
	_CHN_CTRL* channel = (_CHN_CTRL*)&(chnCtrl[chnNumber]);
	for (short i = 0; i < INS_MACROS_MAX; i++) {
		channel->macro_timers[i] = MACRO_UNINITIALIZED;
		channel->macro_length_timers[i] = 0;
	}
	channel->macro_states = 0;
}

void	chn_soft_reset_macros(short chnNumber) {
	_CHN_CTRL* channel = (_CHN_CTRL*)&(chnCtrl[chnNumber]);
	short insNumber = chn_get_current_instrument(chnNumber);
	if (insNumber < 0) {
		insNumber = chnCtrl[chnNumber + insNumber].instrumentID - insNumber;
	}
	_INS_CTRL* instrument = &(insCtrl[insNumber]);
	for (short i = 0; i < INS_MACROS_MAX; i++) {
		short mcrNumber = instrument->macros[i];
		if (mcrNumber != 0) {
			mcrNumber--;
			if (!mcr_is_continuous(&(mcrCtrl[mcrNumber]))) {
				channel->macro_timers[i] = MACRO_OFF;
				channel->macro_length_timers[i] = 0;
				channel->macro_states &= ~(1 << (i));
			}
		}
	}
}

void		get_macro_bounds(unsigned char type, int* lbound, int* ubound) {
	if (type < MACRO_EFFECTCOEF) {
		if (type >= FIRST_SIGNED) {
			*lbound = (short)macroBounds[type][0];
			*ubound = (short)macroBounds[type][1];
		}
		
		else {
			*lbound = macroBounds[type][0];
			*ubound = macroBounds[type][1];
		}
	}
	else if (type >= MACRO_EFFECTADRS) {
		*lbound = addressBounds[type - MACRO_EFFECTADRS][0];
		*ubound = macroBounds[type - MACRO_EFFECTADRS][1];
	}
	else {
		*lbound = coefficientBounds[type - MACRO_EFFECTCOEF][0];
		*ubound = coefficientBounds[type - MACRO_EFFECTCOEF][1];
	}
}

int		clamp_macro_value(int value, unsigned char type) {
	int lbound = 0;
	int ubound = 0;
	get_macro_bounds(type, &lbound, &ubound);

	if (value < lbound) {
		return lbound;
	}
	if (value > ubound) {
		return ubound;
	}
	return value;
}

int		overflow_macro_value(int value, unsigned char type) {
	int lbound = 0;
	int ubound = 0;
	get_macro_bounds(type, &lbound, &ubound);

	value -= lbound;
	ubound -= lbound;
	ubound++;
	if (value < 0) {
		value += ubound << 16;
	}
	value %= ubound;
	return value + lbound;
}

void	dsp_load_base_variables(void) {
	for (short i = 0; i < NUM_COEFFICIENTS; i++) {
		coefficients[i] = base_coefficients[i];
	}
	for (short i = 0; i < NUM_ADDRESSES; i++) {
		addresses[i] = base_addresses[i];
	}
}

void	dsp_set_variables(void) {
	for (short i = 0; i < NUM_COEFFICIENTS; i++) {
		m68k_com->coefficients[i] = coefficients[i] * 8;
	}
	for (short i = 0; i < NUM_ADDRESSES; i++) {
		m68k_com->addresses[i] = addresses[i];
	}
}

unsigned short	get_initial_macro_value(unsigned char type, _INS_CTRL* instrument) {
	if (type >= MACRO_EFFECTADRS) {
		return base_addresses[type - MACRO_EFFECTADRS];
	}
	else if (type >= MACRO_EFFECTCOEF) {
		return base_coefficients[type - MACRO_EFFECTCOEF];
	}
	else {
		switch (type) {
		case MACRO_SAMPLEOFFSET:
			return instrument->sample_offset;
		case MACRO_LOOPSTART:
			return instrument->loopstart_offset;
		case MACRO_LOOPEND:
			return instrument->loopend_offset;
		case MACRO_TOTALLEVEL:
			return instrument->totallevel;
		case MACRO_VOLUME:
			return instrument->volume;
		case MACRO_ATTACK:
			return instrument->attack_hold & 0x1F;
		case MACRO_DECAY1:
			return instrument->decay1;
		case MACRO_SUSTAIN:
			return instrument->sustain;
		case MACRO_DECAY2:
			return instrument->decay2;
		case MACRO_RELEASE:
			return instrument->release;
		case MACRO_KEYSCALING:
			return instrument->key_scaling_sync & 0xF;
		case MACRO_LFOFREQ:
			return instrument->lfo_freq;
		case MACRO_PITCHLFOSTRENGTH:
			return instrument->lfo_pitch & 0x7;
		case MACRO_AMPLFOSTRENGTH:
			return instrument->lfo_amp & 0x7;
		case MACRO_MODLEVEL:
			return instrument->mod_volume;
		case MACRO_PITCHMULTIPLIER:
			return instrument->freq_ratio >> 8;
		case MACRO_PITCHDIVIDER:
			return instrument->loopstart_offset & 0xFF;
		case MACRO_LEVELSCALING:
			return instrument->loopstart_offset & 0xF;
		case MACRO_EFFECTVOLUME:
			return instrument->effect_volume;
		case MACRO_INPUTLEVEL:
			return instrument->input_volume;
		case MACRO_PAN:
			return (instrument->pan < 0x10) ? -instrument->pan : (instrument->pan - 0x10);
		case MACRO_NOTEOFFSET:
			return (char)(instrument->pitch_offset & 0xFF);
		case MACRO_CENTDETUNE:
			return instrument->pitch_offset >> 8;
		case MACRO_REGISTERDETUNE:
			return instrument->register_detune;
		case MACRO_MODINPUTX:
			return instrument->mod_input_x;
		case MACRO_MODINPUTY:
			return instrument->mod_input_y;
		case MACRO_EFFECTPAN:
			return (instrument->effect_pan < 0x10) ? -instrument->effect_pan : (instrument->effect_pan - 0x10);
		default:
			return 0;
		}
	}
}

#pragma GCC push_options
#pragma GCC optimize("O3")
void	chn_set_macro_values(short chnNumber) {
	//jo_set_default_background_color(JO_COLOR_DarkPurple);
	if (chnNumber < 0) return;
	short insNumber = chn_get_current_instrument(chnNumber);
	short patchLeaderChnNumber = chnNumber;
	short patchLeaderInsNumber = insNumber;
	if (insNumber < 0) {
		patchLeaderChnNumber += insNumber;
		patchLeaderInsNumber = chnCtrl[patchLeaderChnNumber].instrumentID;
		insNumber = patchLeaderInsNumber - insNumber;
	}

	_CHN_CTRL* channel = (_CHN_CTRL*)&(chnCtrl[chnNumber]);
	_INS_CTRL* instrument = &(insCtrl[insNumber]);
	_INS_CTRL* patch_leader_instrument = &(insCtrl[patchLeaderInsNumber]);

	for (short i = 0; i < INS_MACROS_MAX; i++) {
		short mcrNumber = instrument->macros[i];
		short patch_leader_mcrNumber = patch_leader_instrument->macros[i];
		unsigned char* macro_timer = &(channel->macro_timers[i]);
		if (patch_leader_mcrNumber != 0) {
			if (mcr_does_propagate(&(mcrCtrl[patch_leader_mcrNumber - 1]))) {
				mcrNumber = patch_leader_mcrNumber;
			}
		}
		if (mcrNumber == 0) {
			*macro_timer = MACRO_OFF;
		}
		else if ((*macro_timer == MACRO_OFF || *macro_timer == MACRO_UNINITIALIZED) && ((channel->melodic_data & MELODIC_KEYON) != 0)) {
			*macro_timer = 0;
		}
		if (*macro_timer != MACRO_OFF && *macro_timer != MACRO_UNINITIALIZED) {
			mcrNumber--;
			_MCR_CTRL* macro = &(mcrCtrl[mcrNumber]);
			Bool is_released = ((channel->macro_states & (1 << i)) != 0);
			Bool should_jump = mcr_does_jump_on_release(macro);
			if ((should_jump && is_released && (*macro_timer <= macro->release || *macro_timer == MACRO_AT_RELEASE))
				|| (!should_jump && is_released && *macro_timer == MACRO_AT_RELEASE)) {
				*macro_timer = macro->release + 1;
			}
			if (*macro_timer >= macro->length) {
				if (*macro_timer != MACRO_AT_RELEASE) {
					*macro_timer = MACRO_OFF;
				}
			}
			else {
				unsigned short macro_value = (mcr_is_variable_length(macro)) ? macro->data[*macro_timer * 2] : macro->data[*macro_timer];
				int macro_value_buffer;
				if (mcr_is_relative(macro)) {
					if ((channel->melodic_data & MELODIC_KEYON) != 0 && (!mcr_is_continuous(macro) && !mcr_is_progressive(macro))) {
						if (mcr_is_fixed(macro)) {
							channel->macro_values[i] = 0;
						}
						else {
							channel->macro_values[i] = get_initial_macro_value(macro->type, instrument);
						}
					}
					if (macro->type >= FIRST_SIGNED) {
						macro_value_buffer = (short)(channel->macro_values[i]);
					}
					else {
						macro_value_buffer = channel->macro_values[i];
					}
					macro_value_buffer += (short)macro_value;
				}
				else {
					if (macro->type >= FIRST_SIGNED) {
						if (macro->type == MACRO_NOTEOFFSET
							|| macro->type == MACRO_CENTDETUNE
							|| macro->type == MACRO_REGISTERDETUNE) {
							macro_value_buffer = (short)macro_value + (short)get_initial_macro_value(macro->type, instrument);
						}
						else {
							macro_value_buffer = (short)macro_value;
						}
					}
					else {
						macro_value_buffer = macro_value;
					}
				}
				if (mcr_does_overflow(macro)) {
					macro_value_buffer = overflow_macro_value(macro_value_buffer, macro->type);
				}
				else {
					macro_value_buffer = clamp_macro_value(macro_value_buffer, macro->type);
				}
				if (macro->type >= FIRST_SIGNED) {
					macro_value = (short)macro_value_buffer;
				}
				else {
					macro_value = (unsigned short)macro_value_buffer;
				}
				
				channel->macro_values[i] = macro_value;

				unsigned char nearest_loop = MACRO_OFF;
				if (*macro_timer >= macro->loop_1
					&& ((*macro_timer <= macro->release) == (macro->loop_1 <= macro->release))) {
					nearest_loop = macro->loop_1;
				}
				if (*macro_timer >= macro->loop_2
					&& ((*macro_timer <= macro->release) == (macro->loop_2 <= macro->release))) {
					nearest_loop = macro->loop_2;
				}
				if (nearest_loop == macro->loop_2 && ((macro->loop_2 <= macro->release) == (macro->loop_1 <= macro->release))) {
					nearest_loop = (macro->loop_2 > macro->loop_1) ? macro->loop_2 : macro->loop_1;
				}

				if (mcr_is_variable_length(macro)) {
					unsigned short current_length = macro->data[*macro_timer * 2 + 1];
					unsigned short* macro_length_timer = &(channel->macro_length_timers[i]);
					(*macro_length_timer)++;
					if (*macro_length_timer >= current_length) {
						*macro_length_timer = 0;
						(*macro_timer)++;
					}
				}
				else {
					(*macro_timer)++;
				}
				if (*macro_timer >= macro->length
					|| (!is_released && (*macro_timer > macro->release))) {
					
					if (nearest_loop != MACRO_OFF) {
						*macro_timer = nearest_loop;
					}
					else if (*macro_timer < macro->length) { // If we're before the release
						*macro_timer = MACRO_AT_RELEASE;
					}
					else {
						*macro_timer = MACRO_OFF;
					}
				}
			}
		}
	}
}

#define MAX_PITCH 0x3FF
#define MAX_PITCH_SIZE 0x400
#define MAX_OCTAVE 7
#define MIN_OCTAVE -8
#define OCTAVE_MIDPOINT 8
#define OCTAVE_SIZE 16
#define SEMITONES_PER_OCT 12
void	chn_set_final_pitch(_CHN_CTRL* channel, _INS_CTRL* instrument, _PCM_CTRL* sample, short note, short note_offset, short cent, short freq_mul, short freq_div, short reg_detune) {
	short semitone = note_offset + ((instrument->use_multi != 0 && mlt_is_base_note_override(instrument->sampleID) ? 0 : (char)note)) - ((char)(sample->base_note));
	short pitch_as_short;
	jo_fixed pitch = jo_int2fixed(sample->base_pitch & MAX_PITCH);
	signed char octave = (sample->base_pitch >> 11);

	if (octave >= OCTAVE_MIDPOINT)
		octave -= OCTAVE_SIZE;

	pitch += jo_int2fixed(MAX_PITCH_SIZE);

	if (cent > 0) {
		pitch = jo_fixed_mult(pitch, cent_mults[cent]);
	}
	else if (cent < 0) {
		semitone--;
		pitch = jo_fixed_mult(pitch, cent_mults[100 - (-cent)]);
	}

	if (semitone != 0) {
		octave += semitone / 12;
		if (semitone > 0) {
			pitch = jo_fixed_mult(pitch, semitone_mults[semitone % SEMITONES_PER_OCT]);
		}
		else if (semitone < 0) {
			octave--;
			pitch = jo_fixed_mult(pitch, semitone_mults[SEMITONES_PER_OCT - ((-semitone) % SEMITONES_PER_OCT)]);
		}
	}

	if (freq_mul != freq_div) {	
		jo_fixed freq_ratio = jo_fixed_div(jo_int2fixed(freq_mul), jo_int2fixed(freq_div));
		// jo_fixed can only hold 16 bits of integer, and the maximum size of pitch by here is slightly less than 0xFFF
		if (freq_ratio >= jo_int2fixed(16)) {
			octave += 4;
			freq_ratio = JO_DIV_BY_16(freq_ratio);
		}
		pitch = jo_fixed_mult(pitch, freq_ratio);
	}

	if (pitch == (jo_fixed)JO_FIXED_OVERFLOW) {
		octave = MAX_OCTAVE;
		pitch_as_short = MAX_PITCH;
	}
	else {
		const jo_fixed fixed_max_pitch = jo_int2fixed(MAX_PITCH_SIZE);
		pitch -= fixed_max_pitch;

		if (pitch < 0) {
			pitch = JO_MULT_BY_2(pitch);
			octave--;
			while (pitch < -fixed_max_pitch) {
				pitch += fixed_max_pitch;
				pitch = JO_MULT_BY_2(pitch);
				octave--;
			}
			pitch += fixed_max_pitch;
		}
		else {
			while (pitch >= fixed_max_pitch) {
				pitch -= fixed_max_pitch;
				pitch = JO_DIV_BY_2(pitch);
				octave++;
			}
		}

		pitch_as_short = jo_fixed2int(pitch);
		if (reg_detune != 0) {
			pitch_as_short += reg_detune;
			if (pitch_as_short >= 0) {
				octave += pitch_as_short / MAX_PITCH_SIZE;
				pitch_as_short %= MAX_PITCH_SIZE;
			}
			else {
				octave += pitch_as_short / MAX_PITCH_SIZE - 1;
				pitch_as_short = MAX_PITCH_SIZE - (-pitch_as_short % MAX_PITCH_SIZE);
			}
		}

		if (octave > MAX_OCTAVE) {
			octave = MAX_OCTAVE;
			pitch_as_short = MAX_PITCH;
		}
		else if (octave < MIN_OCTAVE) {
			octave = MIN_OCTAVE;
			pitch_as_short = 0;
		}
	}

	if (octave < 0) 
		octave += OCTAVE_SIZE;
	channel->final_pitch = (pitch_as_short & MAX_PITCH) | (octave << 11);

	if (sample->bitDepth != PCM_TYPE_ADX) {
		channel->bytes_per_blank = calculate_bytes_per_blank_with_pitchword(channel->final_pitch, sample->bitDepth);
	}
}

void	chn_set_final_level(_CHN_CTRL* channel, _CHN_CTRL* patch_leader_channel, _INS_CTRL* instrument, short level, unsigned char scaling) {
	if (scaling != 0) {
		short octave = channel->final_pitch >> 11;
		if (octave < 8) {
			octave += 8;
		}
		else {
			octave -= 8;
		}
		short fine = channel->final_pitch & MAX_PITCH;
		short pitch = fine | (octave << 10);
		short level_offset = (16 * scaling * pitch) / (1 << 14);
		level += level_offset;
	}
	if (instrument->ignore_velocity == 0) {
		level += 0xFF - patch_leader_channel->velocity;
		level += 0xFF - patch_leader_channel->channel_volume;
	}
	if (level > 0xFF) {
		channel->attenuation = 0xFF;
	}
	else {
		channel->attenuation = (unsigned char)level;
	}	
}



void	chn_set_values(short chnNumber) {
	//jo_set_default_background_color(JO_COLOR_DarkBlue);
	/*if ((short)m68k_com->start - 2 < chnNumber) {
		jo_set_default_background_color(JO_COLOR_DarkGreen);
	}
	else if ((short)m68k_com->start - 2 > chnNumber) {
		jo_set_default_background_color(JO_COLOR_DarkPurple);
	}
	else {
		jo_set_default_background_color(JO_COLOR_DarkYellow);
	}*/
	if (chnNumber < 0) return;
	chnCtrl[chnNumber].test_area = chnCtrl[chnNumber].melodic_data;
	short insNumber = chn_get_current_instrument(chnNumber);
	short patchLeaderChnNumber = chnNumber;
	short patchLeaderInsNumber = insNumber;
	if (insNumber < 0) {
		patchLeaderChnNumber += insNumber;
		patchLeaderInsNumber = chnCtrl[patchLeaderChnNumber].instrumentID;
		insNumber = patchLeaderInsNumber - insNumber;
	}

	short pcmNumber = ins_get_current_sample(insNumber, chnCtrl[chnNumber].note);
	short loop_type = (pcmCtrl[pcmNumber].loopType >= 0) ? pcmCtrl[pcmNumber].loopType : 1;

	_CHN_CTRL* channel = (_CHN_CTRL*)&(chnCtrl[chnNumber]);
	_CHN_CTRL* patch_leader_channel = (_CHN_CTRL*)&(chnCtrl[patchLeaderChnNumber]);
	_INS_CTRL* instrument = &(insCtrl[insNumber]);
	_PCM_CTRL* sample = &(pcmCtrl[pcmNumber]);

	channel->key_data = (loop_type << 5) | ((sample->bitDepth % 2) << 4) | (instrument->bit_reverse << 9) | (instrument->noise_mode << 7);

	// chnCtrl's volume is used here instead of the instrument for Reasons (supporting poneSound's direct-only volume control)
	Bool useInsVolume = (channel->volume == 0);
	if (useInsVolume) {
		channel->volume = instrument->volume;
	}
	channel->pan_send = channel->volume << 13 | instrument->pan << 8 | instrument->effect_volume << 5 | instrument->effect_pan;
	channel->lfo_data = (instrument->lfo_amp) | (instrument->lfo_pitch << 5) | (instrument->lfo_freq << 10);
	if (instrument->use_envelope != 0) {
		channel->decay_1_2_attack = instrument->attack_hold | instrument->decay1 << 6 | instrument->decay2 << 11;
		channel->key_decay_release = instrument->release | instrument->sustain << 5 | instrument->key_scaling_sync << 10;
	}
	else {
		// We could skip writing these and specify direct data playback, but as far as I understand, this removes sound
		// panning. Attenuation, bit 9 [SD].
		channel->decay_1_2_attack = 31;
		channel->key_decay_release = 31;
	}
	// Set the volume back for future reuse
	if (useInsVolume) {
		channel->volume = 0;
	}

	channel->FM_data = instrument->mod_input_y | instrument->mod_input_x << 6 | instrument->mod_volume << 12;

	channel->input_sel = instrument->input_volume | instrument->effects_slot << 3;

	channel->lfo_delay = instrument->lfo_delay;
	channel->reset_bits &= 1;

	channel->decompression_size = sample->decompression_size;

	//jo_set_default_background_color(JO_COLOR_DarkCyan);
	/*if ((short)m68k_com->start - 2 < chnNumber) {
		jo_set_default_background_color(JO_COLOR_DarkGreen);
	}
	else if ((short)m68k_com->start - 2 > chnNumber) {
		jo_set_default_background_color(JO_COLOR_DarkPurple);
	}
	else {
		jo_set_default_background_color(JO_COLOR_DarkYellow);
	}*/

	unsigned short sample_offset = instrument->sample_offset;
	unsigned short loopstart_offset = instrument->loopstart_offset;
	unsigned short loopend_offset = instrument->loopend_offset;

	short level = instrument->totallevel;
	unsigned char scaling = instrument->level_scaling;

	short note = chnCtrl[patchLeaderChnNumber].note;
	short note_offset = (char)(instrument->pitch_offset & 0xFF);
	short cent = (char)(instrument->pitch_offset >> 8);
	short reg_detune = instrument->register_detune;
	short freq_mul = (instrument->freq_ratio & 0xFF) + 1;
	short freq_div = (instrument->freq_ratio >> 8) + 1;

	for (short i = 0; i < INS_MACROS_MAX; i++) {
		short mcrNumber = instrument->macros[i];
		short patch_leader_macro = insCtrl[patchLeaderInsNumber].macros[i];
		if (patch_leader_macro != 0) {
			if (mcr_does_propagate(&(mcrCtrl[patch_leader_macro - 1]))) {
				mcrNumber = patch_leader_macro;
			}
		}		
		if (channel->macro_timers[i] != MACRO_UNINITIALIZED) {
			if (mcrNumber != 0) {
				mcrNumber--;
				_MCR_CTRL* macro = &(mcrCtrl[mcrNumber]);
				if (mcr_get_length(macro) > 0 && (!mcr_is_fixed(macro) || channel->macro_timers[i] != MACRO_OFF)) {
					if (macro->type >= MACRO_EFFECTADRS) {
						addresses[macro->type - MACRO_EFFECTADRS] = channel->macro_values[i];
					}
					else if (macro->type >= MACRO_EFFECTCOEF) {
						coefficients[macro->type - MACRO_EFFECTCOEF] = (short)channel->macro_values[i];
					}
					else {
						short value_buffer = 0;
						short macro_value = channel->macro_values[i];
						switch (macro->type) {
						case MACRO_KEYON:
							channel->reset_bits |= (1 << 4) | (macro_value << 5);
							break;
						case MACRO_SAMPLETRAITS:
							value_buffer = (macro_value & 1) ^ (((macro_value & 2) != 0) ? 0x3 : 0);
							value_buffer <<= 2;
							value_buffer |= (macro_value & 4) >> 2;
							channel->key_data &= ((0xFFFF >> (16 - 7)) | (0xFFFF << 11));
							channel->key_data |= value_buffer << 7;
							break;
						case MACRO_ENVELOPETRAITS:
							value_buffer = (macro_value & 1);
							channel->decay_1_2_attack &= ((0xFFFF >> (16 - 5)) | (0xFFFF << 6));
							channel->decay_1_2_attack |= value_buffer << 5;
							value_buffer = (macro_value & 3) >> 1;
							channel->key_decay_release &= ((0xFFFF >> (16 - 14)) | (0xFFFF << 15));
							channel->key_decay_release |= value_buffer << 14;
							break;
						case MACRO_LFORESET:
							channel->reset_bits |= (1 << 1) | (macro_value << 2);
							break;
						case MACRO_SAMPLEOFFSET:
							sample_offset = macro_value;
							break;
						case MACRO_LOOPSTART:
							loopstart_offset = macro_value;
							break;
						case MACRO_LOOPEND:
							loopend_offset = macro_value;
							break;
						case MACRO_PITCHLFOWAVEFORM:
							channel->lfo_data &= ((0xFFFF >> (16 - 8)) | (0xFFFF << 10));
							channel->lfo_data |= macro_value << 8;
							break;
						case MACRO_AMPLFOWAVEFORM:
							channel->lfo_data &= ((0xFFFF >> (16 - 3)) | (0xFFFF << 5));
							channel->lfo_data |= macro_value << 3;
							break;
						case MACRO_EFFECTSLOT:
							channel->input_sel &= ((0xFFFF >> (16 - 3)) | (0xFFFF << 7));
							channel->input_sel |= macro_value << 3;
							break;
						case MACRO_TOTALLEVEL:
							level = macro_value;
							break;
						case MACRO_LEVELSCALING:
							scaling = macro_value;
							break;
						case MACRO_VOLUME:
							channel->pan_send &= (0xFFFF >> (16 - 13));
							channel->pan_send |= macro_value << 13;
							break;
						case MACRO_ATTACK:
							channel->decay_1_2_attack &= (0xFFFF << 5);
							channel->decay_1_2_attack |= macro_value;
							break;
						case MACRO_DECAY1:
							channel->decay_1_2_attack &= ((0xFFFF >> (16 - 6)) | (0xFFFF << 11));
							channel->decay_1_2_attack |= macro_value << 6;
							break;
						case MACRO_SUSTAIN:
							channel->key_decay_release &= ((0xFFFF >> (16 - 5)) | (0xFFFF << 10));
							channel->key_decay_release |= macro_value << 5;
							break;
						case MACRO_DECAY2:
							channel->decay_1_2_attack &= (0xFFFF >> (16 - 11));
							channel->decay_1_2_attack |= macro_value << 11;
							break;
						case MACRO_RELEASE:
							channel->key_decay_release &= (0xFFFF << 5);
							channel->key_decay_release |= macro_value;
							break;
						case MACRO_KEYSCALING:
							channel->key_decay_release &= ((0xFFFF >> (16 - 10)) | (0xFFFF << 14));
							channel->key_decay_release |= macro_value << 10;
							break;
						case MACRO_LFOFREQ:
							channel->lfo_data &= ((0xFFFF >> (16 - 10)) | (0xFFFF << 15));
							channel->lfo_data |= macro_value << 10;
							break;
						case MACRO_PITCHLFOSTRENGTH:
							channel->lfo_data &= ((0xFFFF >> (16 - 5)) | (0xFFFF << 8));
							channel->lfo_data |= macro_value << 5;
							break;
						case MACRO_AMPLFOSTRENGTH:
							channel->lfo_data &= (0xFFFF << 3);
							channel->lfo_data |= macro_value;
							break;
						case MACRO_MODLEVEL:
							channel->FM_data &= (0xFFFF >> (16 - 12));
							channel->FM_data |= macro_value << 12;
							break;
						case MACRO_EFFECTVOLUME:
							channel->pan_send &= ((0xFFFF >> (16 - 5)) | (0xFFFF << 8));
							channel->pan_send |= macro_value << 5;
							break;
						case MACRO_INPUTLEVEL:
							channel->input_sel &= (0xFFFF << 3);
							channel->input_sel |= macro_value;
							break;
						case MACRO_PAN:
							value_buffer = (short)macro_value;
							if (value_buffer > 0)
								value_buffer += 0x10;
							else
								value_buffer *= -1;
							channel->pan_send &= ((0xFFFF >> (16 - 8)) | (0xFFFF << 13));
							channel->pan_send |= ((unsigned short)value_buffer) << 8;
							break;
						case MACRO_NOTEOFFSET:
							note_offset = (short)macro_value;
							if (mcr_is_fixed(macro)) {
								note = 0;
							}
							break;
						case MACRO_CENTDETUNE:
							cent = (short)macro_value;
							break;
						case MACRO_REGISTERDETUNE:
							reg_detune = (short)macro_value;
							break;
						case MACRO_PITCHMULTIPLIER:
							freq_mul = (short)macro_value + 1;
							break;
						case MACRO_PITCHDIVIDER:
							freq_div = (short)macro_value + 1;
							break;
						case MACRO_MODINPUTX:
							channel->FM_data &= ((0xFFFF >> (16 - 5)) | (0xFFFF << 12));
							channel->FM_data |= ((unsigned short)(macro_value + 0x10)) << 5;
							break;
						case MACRO_MODINPUTY:
							channel->FM_data &= (0xFFFF << 6);
							channel->FM_data |= ((unsigned short)(macro_value + 0x10));
							break;
						case MACRO_EFFECTPAN:
							value_buffer = (short)macro_value;
							if (value_buffer > 0)
								value_buffer += 0x10;
							else
								value_buffer *= -1;
							channel->pan_send &= (0xFFFF << 5);
							channel->pan_send |= ((unsigned short)value_buffer);
							break;
						}
					}
				}
			}
		}
	}

	//jo_set_default_background_color(JO_COLOR_DarkGreen);
	/*if ((short)m68k_com->start - 2 < chnNumber) {
		jo_set_default_background_color(JO_COLOR_DarkGreen);
	}
	else if ((short)m68k_com->start - 2 > chnNumber) {
		jo_set_default_background_color(JO_COLOR_DarkPurple);
	}
	else {
		jo_set_default_background_color(JO_COLOR_DarkYellow);
	}*/

	int address = (sample->hiAddrBits << 16) | sample->loAddrBits;
	unsigned short loop = sample->LSA;
	unsigned short playsize = sample->max_playsize;

	int start_offset = sample_offset;
	start_offset += sample->sample_start;
	playsize = sample->playsize;

	if (start_offset > (int)sample->max_playsize) {
		start_offset = sample->max_playsize;
	}
	address += start_offset;

	int new_loopstart = (int)loopstart_offset + (int)sample->LSA;
	int new_max_size = pcm_get_max_playsize(pcmNumber) - start_offset + 1;
	if (new_loopstart > new_max_size) {
		new_loopstart = new_max_size;
	}
	loop = (unsigned short)new_loopstart;

	int new_loopend = (int)playsize - loopend_offset;
	if (new_loopend < new_loopstart) {
		new_loopend = new_loopstart;
	}
	else if (new_loopend > new_max_size) {
		new_loopend = new_max_size;
	}
	playsize = (unsigned short)new_loopend;

	channel->key_data |= (address >> 16);
	channel->start_addr = (unsigned short)address;
	channel->LSA = loop;
	channel->playsize = playsize;
	channel->test_area = (unsigned short)address;

	//jo_set_default_background_color(JO_COLOR_DarkYellow);
	/*if ((short)m68k_com->start - 2 < chnNumber) {
		jo_set_default_background_color(JO_COLOR_DarkGreen);
	}
	else if ((short)m68k_com->start - 2 > chnNumber) {
		jo_set_default_background_color(JO_COLOR_DarkPurple);
	}
	else {
		jo_set_default_background_color(JO_COLOR_DarkYellow);
	}*/

	chn_set_final_pitch(channel, instrument, sample, note, note_offset, cent, freq_mul, freq_div, reg_detune);

	//jo_set_default_background_color(JO_COLOR_DarkRed);
	/*if ((short)m68k_com->start - 2 < chnNumber) {
		jo_set_default_background_color(JO_COLOR_DarkGreen);
	}
	else if ((short)m68k_com->start - 2 > chnNumber) {
		jo_set_default_background_color(JO_COLOR_DarkPurple);
	}
	else {
		jo_set_default_background_color(JO_COLOR_DarkYellow);
	}*/

	chn_set_final_level(channel, patch_leader_channel, instrument, level, scaling);

	//jo_set_default_background_color(JO_COLOR_Black);
}
#pragma GCC pop_options

void chn_release(short chnNumber) {
	if (chnNumber < 0) return;
	if (chnCtrl[chnNumber].melodic_data == 0) chn_cease(chnNumber);
	else {
		chnCtrl[chnNumber].melodic_data = chnCtrl[chnNumber].melodic_data | MELODIC_KEYOFF;
		chnCtrl[chnNumber].macro_states = 0xFF; // Flag each macro as being in the release state
	} 
}

void	chn_cease(short chnNumber)
{
	if (chnNumber < 0) return;
	if (chnCtrl[chnNumber].ctrlType <= 0 && chnCtrl[chnNumber].melodic_data == 0) // If it is a volatile or protected sound, the expected control method is to mute the sound and let it end itself.
	{												// Protected sounds have a permission state of "until they end".
		chnCtrl[chnNumber].volume = 0;
	}
	else {
		chn_release(chnNumber);
		chnCtrl[chnNumber].sh2_permit = 0; // If it is a melodic or looping sound, the control method is to command it to stop.	}
	}
}

void	chn_cease_all() {
	for (int i = 0; i < numberChans; i++) {
		chn_cease(i);
		chn_hard_reset_macros(i);
	}
}

void	chn_release_all() {
	for (int i = 0; i < numberChans; i++) {
		chn_release(i);
	}
}

int test_func(short chnNumber) {
	return chnCtrl[chnNumber].test_area;
}

int test_inst(short insNumber) {
	return insCtrl[insNumber].test_area;
}

//
// Usage:
// Intended as the "level reset" function.
// Does not soft or hard reset driver. To do that, re-load the driver binary (run load_drv again).
// This instead resets the loading pointer and number of PCMs to a specific PCM number.
// In use with proper sequence of asset loading, a certain number of sound assets can be retained in sound memory, with others discarded.
// 
// The argument "highest_pcm_number_to_keep" is the latest sequentially loaded PCM in sound RAM that signals the point at which:
// Any PCM number loaded earlier than this will be kept in memory and its number still valid to play the sound.
// Any PCM number loaded later than this will be ignored in memory when loading new sounds, but the number is still valid to play sound.
// Pass -1 to this function to clear all PCMs.
void	pcm_reset(short highest_pcm_number_to_keep)
{
	//For clearing all sounds, input is negative
	if (highest_pcm_number_to_keep < 0) {
		scsp_load = scsp_loading_start;
		numberInsts = 0;
		return;
	}

	numberPCMs = highest_pcm_number_to_keep + 1;
	scsp_load = (unsigned int*)((unsigned int)(pcmCtrl[highest_pcm_number_to_keep] .hiAddrBits << 16) | (int)(pcmCtrl[highest_pcm_number_to_keep].loAddrBits));
	if (pcmCtrl[highest_pcm_number_to_keep].bitDepth == 2)
	{ //If this is an ADX sound, offset the loading pointer by # of frames by 18. Address includes 18-byte header offset.
		scsp_load = (unsigned int*)((unsigned int)scsp_load + (pcmCtrl[highest_pcm_number_to_keep].playsize * 18));
	}
	else if (pcmCtrl[highest_pcm_number_to_keep].bitDepth == 1)
	{ //If this is an 8-bit PCM, offset the loading pointer by the playsize, exactly (one byte samples).
		scsp_load = (unsigned int*)((unsigned int)scsp_load + pcmCtrl[highest_pcm_number_to_keep].playsize);
	}
	else if (pcmCtrl[highest_pcm_number_to_keep].bitDepth == 0)
	{ //If this is a 16-bit PCM, offset the loading pointer by the playsize, shifted left once (two byte samples).
		scsp_load = (unsigned int*)((unsigned int)scsp_load + (pcmCtrl[highest_pcm_number_to_keep].playsize << 1));
	}
}

/**stolen from xl2**/
#define     OPEN_MAX    (Sint32)5
#define     DIR_MAX     (Sint32)1024
#define     RD_UNIT     (10)
#define     SECT_SIZE   (2048)
GfsDirTbl gfsDirTbl;
GfsDirName gfsDirName[DIR_MAX];
Uint32 gfsLibWork[GFS_WORK_SIZE(OPEN_MAX) / sizeof(Uint32)];
Sint32 gfsDirN;
void    cd_init(void)
{
	GFS_DIRTBL_TYPE(&gfsDirTbl) = GFS_DIR_NAME;
	GFS_DIRTBL_DIRNAME(&gfsDirTbl) = gfsDirName;
	GFS_DIRTBL_NDIR(&gfsDirTbl) = DIR_MAX;
	gfsDirN = GFS_Init(OPEN_MAX, gfsLibWork, &gfsDirTbl);
}

void smpc_wait_till_ready(void)
{
	// Wait until SF register is cleared
	while (SMPC_REG_SF & 0x1) {}
}

//////////////////////////////////////////////////////////////////////////////

void smpc_issue_command(unsigned char cmd)
{
	// Set SF register so that no other command can be issued.
	SMPC_REG_SF = 1;

	// Writing COMREG starts execution
	SMPC_REG_COMREG = cmd;
}

//////////////////////////////////////////////////////////////////////////////

void	load_driver_binary(Sint8* filename, void* buffer, int master_adx_frequency)
{
	cd_init();
	GfsHn s_gfs;
	Sint32 file_size;

	Sint32 local_name = GFS_NameToId(filename);

	// Open GFS
	s_gfs = GFS_Open((Sint32)local_name);
	// Get file information (mostly, the file size)
	GFS_GetFileInfo(s_gfs, NULL, NULL, &file_size, NULL);

	GFS_Close(s_gfs);

	GFS_Load(local_name, 0, (Uint32*)buffer, file_size);

	// The immediacy of these commands is important.
	// As per SEGA technical bulletin 51, the Sound CPU is not to be turned off for more than 0.5 seconds.
	// Turn off Sound CPU
	smpc_issue_command(SMPC_CMD_SNDOFF);
	smpc_wait_till_ready();
	//Set max master volume + 4mbit memory 
	set_master_volume(0xF);
	//Copy the driver binary (code) over to sound RAM. The binary includes the vector table information.
	slDMACopy(buffer, (void*)SNDRAM, file_size);
	slDMAWait();
	//Set the ADX coefficients for the driver to use, if one was selected.

	m68k_com->drv_adx_coef_1 = adx_coef_tbl[master_adx_frequency][0];
	m68k_com->drv_adx_coef_2 = adx_coef_tbl[master_adx_frequency][1];

	// Turn on Sound CPU again
	smpc_wait_till_ready();
	smpc_issue_command(SMPC_CMD_SNDON);
	//
}

void			load_drv(int master_adx_frequency)
{
	// Make sure SCSP is set to 512k mode
	*(unsigned char*)(0x25B00400) = 0x02;

	// Clear Sound Ram
	for (int i = 0; i < 0x80000; i += 4) {
		*(unsigned int*)(SNDRAM + i) = 0x00000000;
	}
	// Volatile loading buffer
	// This memory is not permanently used, its just a place to temporarily store the driver binary
	void* binary_buffer = (void*)2097152;

	// Copy driver over
	load_driver_binary((Sint8*)"SDRV.BIN", binary_buffer, master_adx_frequency);
	m68k_com->start = 0xFFFF;
	volatile int i = 0;
	scsp_load = scsp_loading_start; // Re-set loading pointer.
	for (i = 0; i < (int)scsp_load; i++)
	{
		// This is to pop the stack here. Because GCC.
	}
	// Additionally, reset the number of PCMs.
	numberChans = 0;
	numberInsts = 0;
	numberPCMs = 0;

	for (i = 0; i < NUM_COEFFICIENTS; i++) {
		base_coefficients[i] = 4000;
		coefficientBounds[i][0] = -4096;
		coefficientBounds[i][1] = 4095;
	}
	for (i = 0; i < NUM_ADDRESSES; i++) {
		base_addresses[i] = 0;
		addressBounds[i][0] = 0;
		addressBounds[i][1] = 0xFFFF;
	}

	GfsHn s_gfs;
	Sint32 file_size;

	Sint32 local_name = GFS_NameToId((Sint8*)"BASIC.EFF");

	// Open GFS
	s_gfs = GFS_Open((Sint32)local_name);
	// Get file information (mostly, the file size)
	GFS_GetFileInfo(s_gfs, NULL, NULL, &file_size, NULL);

	GFS_Close(s_gfs);

	GFS_Load(local_name, 0, (void*)(0x25B00700), file_size);

	// Write each set of DSP coefficients to the main struct
	for (i = 0; i < NUM_COEFFICIENTS; i++) {
		base_coefficients[i] = (*(short*)(0x25B00700 + (2 * (1 + i)))) / 8;
	}
	for (i = 0; i < NUM_ADDRESSES; i++) {
		base_addresses[i] = (*(short*)(0x25B00780 + (2 * (i))));
	}
}

unsigned short	calculate_bytes_per_blank(int sampleRate, Bool is8Bit, Bool isPAL)
{
	int frameCount = (isPAL == true) ? 50 : 60;
	int sampleSize = (is8Bit == true) ? 8 : 16;
	int bytes_per_blank = ((sampleRate * sampleSize) >> 3) / frameCount;
	if (bytes_per_blank >= 1 << 16) {
		bytes_per_blank = (1 << 16) - 1;
	}
	return bytes_per_blank;
}

unsigned short calculate_bytes_per_blank_with_pitchword(short pitchword, unsigned char bitDepth) {
	return calculate_bytes_per_blank(convert_pitchword_to_bitrate(pitchword), bitDepth, PCM_SYS_REGION);
}

int			convert_pitchword_to_bitrate(short pitchWord)
{

	short fns = (short)(pitchWord & 0x3FF); // isolate rightmost 10 bits
	short oct = (short)(pitchWord >> 11);
	float freq_shift = ((float)(1024 + fns)) / 1024; // based on how cents work
	float base_freq = PCM_SCSP_FREQUENCY;
	if (oct < 8) {
		base_freq *= (1 << oct);
	}
	else {
		oct -= 16;
		base_freq /= (1 << -oct);
	}
	return (int)(base_freq * freq_shift);
}

short			convert_bitrate_to_pitchword(short sampleRate)
{

	int octr;
	int shiftr;
	int fnsr;

	octr = PCM_CALC_OCT(sampleRate);
	shiftr = PCM_CALC_SHIFT_FREQ(octr);
	fnsr = PCM_CALC_FNS(sampleRate, shiftr);

	return PCM_SET_PITCH_WORD(octr, fnsr);
}

// Note: as I currently understand things, the actual maximum limit for FM sample offsets is incredibly high,
// which would cause wavetable samples to take up far more space in memory than the typical user will ever need
// This constant is a reasonable upper limit for most people
// Future iterations of the driver should allow custom padding based on the amount of FM used for the sample
#define MAX_FM_READ_OFFSET 256
short initialize_new_pcm(Bool is8Bit, int sampleRate, Sint32 file_size, Sint32 playsize, Bool addSilence) {
	if (addSilence) {
		scsp_load = (unsigned int*)((unsigned int)scsp_load + MAX_FM_READ_OFFSET);
	}
	if (is8Bit) {
		pcmCtrl[numberPCMs].hiAddrBits = (unsigned short)((unsigned int)scsp_load >> 16);
		pcmCtrl[numberPCMs].loAddrBits = (unsigned short)((unsigned int)scsp_load & 0xFFFF);
	}
	else {
		pcmCtrl[numberPCMs].hiAddrBits = (unsigned short)((unsigned int)scsp_load >> 16);
		pcmCtrl[numberPCMs].loAddrBits = (unsigned short)((unsigned int)scsp_load & 0xFFFF);
	}
	pcmCtrl[numberPCMs].base_pitch = convert_bitrate_to_pitchword(sampleRate);
	pcmCtrl[numberPCMs].max_playsize = ((is8Bit) ? playsize : (playsize >> 1));
	pcmCtrl[numberPCMs].bytes_per_blank = calculate_bytes_per_blank(sampleRate, is8Bit, PCM_SYS_REGION); 
	pcmCtrl[numberPCMs].bitDepth = (is8Bit) ? PCM_TYPE_8BIT : PCM_TYPE_16BIT;
	pcmCtrl[numberPCMs].loopType = 0; //Initialize as non-looping
	pcmCtrl[numberPCMs].playsize = pcmCtrl[numberPCMs].max_playsize;
	pcmCtrl[numberPCMs].sample_start = 0;

	numberPCMs++; // Increment PCM #
	scsp_load = (unsigned int*)((unsigned int)scsp_load + file_size);
	return (numberPCMs - 1); // Return the PCM # this sound received
}

short			copy_pcm(short pcmNumber)
{
	if (numberPCMs >= PCM_CTRL_MAX) return -1; // Maximum number of PCMs reached, exit
	if (pcmNumber < 0) return -1;
	pcmCtrl[numberPCMs].hiAddrBits = pcmCtrl[pcmNumber].hiAddrBits;
	pcmCtrl[numberPCMs].loAddrBits = pcmCtrl[pcmNumber].loAddrBits;
	pcmCtrl[numberPCMs].base_pitch = pcmCtrl[pcmNumber].base_pitch;
	pcmCtrl[numberPCMs].playsize = pcmCtrl[pcmNumber].playsize;
	pcmCtrl[numberPCMs].bytes_per_blank = pcmCtrl[pcmNumber].bytes_per_blank;
	pcmCtrl[numberPCMs].bitDepth = pcmCtrl[pcmNumber].bitDepth;
	pcmCtrl[numberPCMs].loopType = pcmCtrl[pcmNumber].loopType;
	pcmCtrl[numberPCMs].LSA = pcmCtrl[pcmNumber].LSA;
	pcmCtrl[numberPCMs].base_note = pcmCtrl[pcmNumber].base_note;
	pcmCtrl[numberPCMs].max_playsize = pcmCtrl[pcmNumber].max_playsize;
	pcmCtrl[numberPCMs].sample_start = pcmCtrl[pcmNumber].sample_start;
	numberPCMs++; // Increment pcm #
	return (numberPCMs - 1); // Return the PCM # this sound received
}

short			load_16bit_pcm(Sint8* filename, int sampleRate, Bool shouldLoop, Bool addSilenceAtStart)
{
	if ((int)scsp_load > 0x7F800) return -1; // Illegal PCM data address, exit
	if (numberPCMs >= PCM_CTRL_MAX) return -1; // Maximum number of PCMs reached, exit

	GfsHn s_gfs;
	Sint32 file_size;

	Sint32 local_name = GFS_NameToId(filename);

	// Open GFS
	s_gfs = GFS_Open((Sint32)local_name);
	// Get file information (mostly, the file size)
	GFS_GetFileInfo(s_gfs, NULL, NULL, &file_size, NULL);

	GFS_Close(s_gfs);

	if (file_size > (128 * 1024)) return -1; // PCM size too large for general-purpose playback [could still work with timed execution & offets]

	file_size += ((unsigned int)file_size & 1) ? 1 : 0;
	file_size += ((unsigned int)file_size & 3) ? 2 : 0;

	if ((int)scsp_load + file_size + ((addSilenceAtStart) ? MAX_FM_READ_OFFSET : 0) > 0x7F800) return -1; // Sample won't fit into RAM

	if (addSilenceAtStart)
		GFS_Load(local_name, 0, (Uint32*)((unsigned int)scsp_load + MAX_FM_READ_OFFSET + SNDRAM), file_size);
	else
		GFS_Load(local_name, 0, (Uint32*)((unsigned int)scsp_load + SNDRAM), file_size);

	return initialize_new_pcm(false, sampleRate, file_size, file_size + ((shouldLoop) ? 2 : 0), addSilenceAtStart);
}

short			load_8bit_pcm(Sint8* filename, int sampleRate, Bool shouldLoop, Bool addSilenceAtStart)
{
	if ((int)scsp_load > 0x7F800) return -1; // Illegal PCM data address, exit
	if (numberPCMs >= PCM_CTRL_MAX) return -1; // Maximum number of PCMs reached, exit

	GfsHn s_gfs;
	Sint32 file_size;

	Sint32 local_name = GFS_NameToId(filename);

	// Open GFS
	s_gfs = GFS_Open((Sint32)local_name);
	// Get file information (mostly, the file size)
	GFS_GetFileInfo(s_gfs, NULL, NULL, &file_size, NULL);

	GFS_Close(s_gfs);

	if (file_size > (64 * 1024)) return -1; // PCM size too large for general-purpose playback [could still work with timed execution & offets]

	file_size += ((unsigned int)file_size & 1) ? 1 : 0;
	file_size += ((unsigned int)file_size & 3) ? 2 : 0;

	if ((int)scsp_load + file_size + ((addSilenceAtStart) ? MAX_FM_READ_OFFSET : 0) > 0x7F800) return -1; // Sample won't fit into RAM

	if (addSilenceAtStart) 
		GFS_Load(local_name, 0, (Uint32*)((unsigned int)scsp_load + MAX_FM_READ_OFFSET + SNDRAM), file_size);
	else 
		GFS_Load(local_name, 0, (Uint32*)((unsigned int)scsp_load + SNDRAM), file_size);
	

	return initialize_new_pcm(true, sampleRate, file_size, file_size + ((shouldLoop) ? 1 : 0), addSilenceAtStart);
}

short			load_16bit_pcm_with_fm_padding(Sint8* filename, int sampleRate)
{
	if ((int)scsp_load > 0x7F800) return -1; // Illegal PCM data address, exit
	if (numberPCMs >= PCM_CTRL_MAX) return -1; // Maximum number of PCMs reached, exit

	GfsHn s_gfs;
	Sint32 file_size;

	Sint32 local_name = GFS_NameToId(filename);

	// Open GFS
	s_gfs = GFS_Open((Sint32)local_name);
	// Get file information (mostly, the file size)
	GFS_GetFileInfo(s_gfs, NULL, NULL, &file_size, NULL);

	GFS_Close(s_gfs);

	file_size += ((unsigned int)file_size & 1) ? 1 : 0;
	file_size += ((unsigned int)file_size & 3) ? 2 : 0;
	short chnNumber = -1;
	short pad_length = 1;
	if (file_size < MAX_FM_READ_OFFSET) {
		pad_length = MAX_FM_READ_OFFSET / file_size + 1;
	}
	short total_repeats = (pad_length * 2) + 1;

	if (file_size * total_repeats > (128 * 1024)) return -1; // PCM size too large for general-purpose playback [could still work with timed execution & offets]
	
	if ((int)scsp_load + (file_size * total_repeats) > 0x7F800) return -1; // Sample won't fit into RAM

	for (short i = 0; i < total_repeats; i++) {
		GFS_Load(local_name, 0, (Uint32*)((unsigned int)scsp_load + SNDRAM), file_size);
		if (i != pad_length) { // iterator is not loading the central sample
			scsp_load = (unsigned int*)((unsigned int)scsp_load + file_size);
		}
		else {
			chnNumber = initialize_new_pcm(false, sampleRate, file_size, file_size + 2, false);
		}
	}
	
	return chnNumber;
}

short			load_8bit_pcm_with_fm_padding(Sint8* filename, int sampleRate)
{
	if ((int)scsp_load > 0x7F800) return -1; // Illegal PCM data address, exit
	if (numberPCMs >= PCM_CTRL_MAX) return -1; // Maximum number of PCMs reached, exit

	GfsHn s_gfs;
	Sint32 file_size;

	Sint32 local_name = GFS_NameToId(filename);

	// Open GFS
	s_gfs = GFS_Open((Sint32)local_name);
	// Get file information (mostly, the file size)
	GFS_GetFileInfo(s_gfs, NULL, NULL, &file_size, NULL);

	GFS_Close(s_gfs);


	file_size += ((unsigned int)file_size & 1) ? 1 : 0;
	file_size += ((unsigned int)file_size & 3) ? 2 : 0;
	short chnNumber = -1;
	short pad_length = 1;
	if (file_size < MAX_FM_READ_OFFSET) {
		pad_length = MAX_FM_READ_OFFSET / file_size + 1;
	}
	short total_repeats = (pad_length * 2) + 1;

	if (file_size * total_repeats > (64 * 1024)) return -1; // PCM size too large for general-purpose playback [could still work with timed execution & offets]

	if ((int)scsp_load + (file_size * total_repeats) > 0x7F800) return -1; // Sample won't fit into RAM

	for (short i = 0; i < total_repeats; i++) {
		GFS_Load(local_name, 0, (Uint32*)((unsigned int)scsp_load + SNDRAM), file_size);
		if (i != pad_length) { // iterator is not loading the central sample
			scsp_load = (unsigned int*)((unsigned int)scsp_load + file_size);
		}
		else {
			chnNumber = initialize_new_pcm(true, sampleRate, file_size, file_size + 1, false);
		}
	}

	return chnNumber;
}

short initialize_new_instrument(short pcmNumber) {
	if (numberInsts >= INS_CTRL_MAX) return -1;
	if (pcmNumber < 0) return -1;

	// This is the 'default' instrument for the module
	// It will have a much greater purpose later on
	// - the default values will be editable later
	if (numberInsts == 0) {
		insCtrl[numberInsts].sampleID = pcmNumber;
		insCtrl[numberInsts].pan = 0;
		insCtrl[numberInsts].volume = 7;
		insCtrl[numberInsts].totallevel = 0;
		insCtrl[numberInsts].lfo_freq = 0;
		insCtrl[numberInsts].lfo_pitch = 0;
		insCtrl[numberInsts].lfo_amp = 0;
		insCtrl[numberInsts].lfo_delay = 0;
		insCtrl[numberInsts].bit_reverse = 0;
		insCtrl[numberInsts].noise_mode = 0;
		insCtrl[numberInsts].attack_hold = 0;
		insCtrl[numberInsts].decay1 = 0;
		insCtrl[numberInsts].sustain = 0;
		insCtrl[numberInsts].decay2 = 0;
		insCtrl[numberInsts].release = 0;
		insCtrl[numberInsts].key_scaling_sync = 0;
		insCtrl[numberInsts].mod_volume = 0;
		insCtrl[numberInsts].mod_input_x = 0;
		insCtrl[numberInsts].mod_input_y = 0;
		insCtrl[numberInsts].use_envelope = 0;
		insCtrl[numberInsts].effect_pan = 0;
		insCtrl[numberInsts].effect_volume = 0;
		insCtrl[numberInsts].input_volume = 0;
		insCtrl[numberInsts].effects_slot = 0;
		insCtrl[numberInsts].freq_ratio = 0;
		insCtrl[numberInsts].level_scaling = 0;
		insCtrl[numberInsts].pitch_offset = 0;
		insCtrl[numberInsts].register_detune = 0;
		insCtrl[numberInsts].ignore_velocity = 0;
		insCtrl[numberInsts].test_area = 0;
	}
	else {
		insCtrl[numberInsts].sampleID = pcmNumber;
		insCtrl[numberInsts].pan = insCtrl[0].pan;
		insCtrl[numberInsts].volume = insCtrl[0].volume;
		insCtrl[numberInsts].totallevel = insCtrl[0].totallevel;
		insCtrl[numberInsts].lfo_freq = insCtrl[0].lfo_freq;
		insCtrl[numberInsts].lfo_pitch = insCtrl[0].lfo_pitch;
		insCtrl[numberInsts].lfo_amp = insCtrl[0].lfo_amp;
		insCtrl[numberInsts].lfo_delay = insCtrl[0].lfo_delay;
		insCtrl[numberInsts].bit_reverse = insCtrl[0].bit_reverse;
		insCtrl[numberInsts].noise_mode = insCtrl[0].noise_mode;
		insCtrl[numberInsts].attack_hold = insCtrl[0].attack_hold;
		insCtrl[numberInsts].decay1 = insCtrl[0].decay1;
		insCtrl[numberInsts].sustain = insCtrl[0].sustain;
		insCtrl[numberInsts].decay2 = insCtrl[0].decay2;
		insCtrl[numberInsts].release = insCtrl[0].release;
		insCtrl[numberInsts].key_scaling_sync = insCtrl[0].key_scaling_sync;
		insCtrl[numberInsts].mod_volume = insCtrl[0].mod_volume;
		insCtrl[numberInsts].mod_input_x = insCtrl[0].mod_input_x;
		insCtrl[numberInsts].mod_input_y = insCtrl[0].mod_input_y;
		insCtrl[numberInsts].use_envelope = insCtrl[0].use_envelope;
		insCtrl[numberInsts].effect_pan = insCtrl[0].effect_pan;
		insCtrl[numberInsts].effect_volume = insCtrl[0].effect_volume;
		insCtrl[numberInsts].input_volume = insCtrl[0].input_volume;
		insCtrl[numberInsts].effects_slot = insCtrl[0].effects_slot;
		insCtrl[numberInsts].freq_ratio = insCtrl[0].freq_ratio;
		insCtrl[numberInsts].level_scaling = insCtrl[0].level_scaling;
		insCtrl[numberInsts].pitch_offset = insCtrl[0].pitch_offset;
		insCtrl[numberInsts].register_detune = insCtrl[0].register_detune;
		insCtrl[numberInsts].ignore_velocity = insCtrl[0].ignore_velocity;
		insCtrl[numberInsts].test_area = insCtrl[0].test_area;
	}
	numberInsts++;
	return (numberInsts - 1);
}

// Recursive function to return gcd of a and b 
short gcd(short a, short b)
{
	if (a == 0)
		return b;
	return gcd(b % a, a);
}

// Function to return LCM of two numbers 
// Used specifically to find the buffer size for ADX sound effects
short lcm(short a, short b)
{
	return (a / gcd(a, b)) * b;
}

short		load_adx(Sint8* filename)
{
	static adx_header adx;

	if ((int)scsp_load > 0x7F800) return -1; // Illegal PCM data address, exit
	if (numberInsts >= INS_CTRL_MAX) return -1; // Maximum number of PCMs reached, exit

	Sint32 local_name = GFS_NameToId(filename);

	//////////////////////////
	// Step 1: Load the size of the header from the file to the header's location
	//////////////////////////
	GFS_Load(local_name, 0, (Uint32*)&adx, sizeof(adx_header));
	//////////////////////////
	// Step 2: Check the data we just loaded and make sure it's an ADX file.
	// If the data does not match what the decompression routine expects, this function will return -1.
	//////////////////////////
		// jo_printf(13, 9, "ohalf(%i)", adx.one_half);
		// jo_printf(13, 10, "blocksz(%i)", adx.block_size);
		// jo_printf(13, 11, "bitdepth(%i)", adx.bit_depth);
		// jo_printf(13, 12, "srate(%i)", adx.sample_rate);
	if (adx.one_half != 32768 || adx.block_size != 18 || adx.bit_depth != 4) return -1;
	//////////////////////////
	// Step 3: Parse the data in the header to the sound driver PCM control struct.
	// Special things for ADX is the playsize is actually the # of ADX frames, not the number of samples.
	// We expect a block size of 16, so each frame has 16 bytes of sample data which is 32 samples.
	//////////////////////////
		// Because we ""dumbly"" load the header to sound RAM, let's give the used pointer the offset.
	unsigned int working_address = (unsigned int)(scsp_load)+adx.offset2data + 4;
	pcmCtrl[numberPCMs].hiAddrBits = (unsigned short)((unsigned int)working_address >> 16);
	pcmCtrl[numberPCMs].loAddrBits = (unsigned short)((unsigned int)working_address & 0xFFFF);
	pcmCtrl[numberPCMs].base_pitch = convert_bitrate_to_pitchword(adx.sample_rate);
	pcmCtrl[numberPCMs].max_playsize = (adx.sample_ct / 32);
	pcmCtrl[numberPCMs].playsize = pcmCtrl[numberPCMs].max_playsize;
	pcmCtrl[numberPCMs].sample_start = 0;
	unsigned short bpb = calculate_bytes_per_blank((int)adx.sample_rate, false, PCM_SYS_REGION); //Iniitalize as max volume
	if (bpb != 768 && bpb != 512 && bpb != 384 && bpb != 256 && bpb != 192 && bpb != 128)
	{
		slPrint("!(ADX INVALID BYTE-RATE)!", slLocate(0, 1));
		return -2;
	}
	pcmCtrl[numberPCMs].bytes_per_blank = bpb;
	unsigned short big_dct_sz = (bpb >= 256) ? lcm(bpb, bpb + 64) << 1 : 5376; // Dirty fix for ultra low bitrate
	pcmCtrl[numberPCMs].decompression_size = (big_dct_sz > (adx.sample_ct << 1)) ? adx.sample_ct << 1 : big_dct_sz;
	pcmCtrl[numberPCMs].bitDepth = PCM_TYPE_ADX; // Select ADX type
	pcmCtrl[numberPCMs].loopType = PCM_SEMI; // Initialize as semi-protected.
	numberPCMs++;
	/////////////////////////
	// Step 4: Load the compressed ADX data to sound RAM. Unfortunately, we must include the 20 byte header.
	// We want to load from an offset location from CD and then load (sample_ct / 32) * 18 bytes.
	// We divide the sample count by 32 to retrieve the ADX frame count. We multiply by 18, as that is the size of an ADX 'frame'.
	/////////////////////////
	unsigned int number_of_bytes_to_load = (adx.sample_ct / 32) * 18;
	number_of_bytes_to_load += ((unsigned int)number_of_bytes_to_load & 1) ? 1 : 0;
	number_of_bytes_to_load += ((unsigned int)number_of_bytes_to_load & 3) ? 2 : 0;
	GFS_Load(local_name, 0, (Uint32*)((unsigned int)scsp_load + SNDRAM), number_of_bytes_to_load);
	scsp_load = (unsigned int*)((unsigned int)scsp_load + number_of_bytes_to_load);
	return (numberPCMs - 1); //Return the PCM # this sound recieved
}

// Sets the number of slots to use for sequenced (i.e. non-dynamic) channels
// All dynamic channels will be loaded after these initialized channels,
// and any dynamic channels occupting these slots will be overriden
short	init_sequenced_channels(short numChannels) {
	if (numChannels > 32) {
		numChannels = 32;
	}
	for (int i = 0; i < numChannels; i++) {
		chn_set_melodic_data(i, i);
	}
	numberChans = numChannels;
	return numberChans;
}

short	initialize_new_channel(short insNumber) {
	if (numberChans >= CHN_CTRL_MAX) {
		return -1;
	}
	chn_instrument_change(numberChans, insNumber);
	chn_velocity_change(numberChans, 0xFF);
	chn_channel_volume_change(numberChans, 0xFF);
	chn_set_values(numberChans);
	// This fixes a bug related to the first time a pcm sample plays
	// Its value will be overwritten by chn_play anyway, so any non-zero value should work
	chnCtrl[numberChans].ctrlType = PCM_PROTECTED;
	numberChans++;
	return numberChans - 1;
}

void		sdrv_vblank_rq(void)
{
	//jo_printf(0, 0, "drv_stat(%i)", m68k_com->start);
	jo_dma_copy(chnCtrl, (void*)m68k_com->chnCtrl, sizeof(_CHN_CTRL) * CHN_CTRL_MAX);
	m68k_com->start = 1;
}

////////////////////////////////////////////////////////////////////////////////
// Redbook Support
// These are mostly CDC commands.
// credit to: ndiddy, ReyeMe, CyberWarriorX [iapetus]
////////////////////////////////////////////////////////////////////////////////


void CDDA_SetVolume(int vol)
{
	//Step 1: Remove the volume bits from the value (isolate the pan)
	unsigned char newvol = m68k_com->cdda_left_channel_vol_pan & 0x1F;
	//Step 2: Apply the volume to the correct bits
	newvol |= ((vol & 0x7) << 5);
	//Step 3: Apply value back to left channel
	m68k_com->cdda_left_channel_vol_pan = newvol;
	//Step 4: Repeat for right channel
	newvol = m68k_com->cdda_right_channel_vol_pan & 0x1F;
	//Step 5: Apply the volume to the correct bits
	newvol |= ((vol & 0x7) << 5);
	//Step 6: Apply value back to right channel
	m68k_com->cdda_right_channel_vol_pan = newvol;
}

//	To see what this does and how to use it, refer to the SCSP manual.
//	Warning: Use without reading the manual may break your CD audio.
void CDDA_SetChannelVolPan(unsigned char left_channel, unsigned char right_channel)
{
	m68k_com->cdda_left_channel_vol_pan = left_channel;
	m68k_com->cdda_right_channel_vol_pan = right_channel;
}

void CDDA_Play(int fromTrack, int toTrack, Bool loop)
{
	CdcPly ply;
	CDC_PLY_STYPE(&ply) = CDC_PTYPE_TNO; // track number
	CDC_PLY_STNO(&ply) = fromTrack;
	CDC_PLY_SIDX(&ply) = 1;
	CDC_PLY_ETYPE(&ply) = CDC_PTYPE_TNO;
	CDC_PLY_ETNO(&ply) = toTrack;
	CDC_PLY_EIDX(&ply) = 1;

	if (loop)
	{
		CDC_PLY_PMODE(&ply) = CDC_PM_DFL | 0xf; // 0xf = infinite repetitions
	}
	else
	{
		CDC_PLY_PMODE(&ply) = CDC_PM_DFL;
	}

	CDC_CdPlay(&ply);
}

void CDDA_PlaySingle(int track, Bool loop)
{
	CDDA_Play(track, track, loop);
}

void CDDA_Stop(void)
{
	CdcPos poswk;
	CDC_POS_PTYPE(&poswk) = CDC_PTYPE_DFL;
	CDC_CdSeek(&poswk);
}

