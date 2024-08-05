//
//pcmsys.h
//
#ifndef __PCMSYS_H__
# define __PCMSYS_H__

#include <jo/jo.h>


///////////////
// Likely duplicates from other libraries (in this case, taken from iapetus)
//////////////////////////////////////////////////////////////////////////////

#define SMPC_REG_IREG(i)        *((volatile unsigned char *)0x20100001+((i) * 2))
#define SMPC_REG_COMREG         *((volatile unsigned char *)0x2010001F)
#define SMPC_REG_OREG(o)        *((volatile unsigned char *)0x20100021+((o) * 2))
#define SMPC_REG_SR             *((volatile unsigned char *)0x20100061)
#define SMPC_REG_SF             *((volatile unsigned char *)0x20100063)
#define SMPC_REG_PDR1           *((volatile unsigned char *)0x20100075)
#define SMPC_REG_PDR2           *((volatile unsigned char *)0x20100077)
#define SMPC_REG_DDR1           *((volatile unsigned char *)0x20100079)
#define SMPC_REG_DDR2           *((volatile unsigned char *)0x2010007B)
#define SMPC_REG_IOSEL          *((volatile unsigned char *)0x2010007D)
#define SMPC_REG_EXLE           *((volatile unsigned char *)0x2010007F)

//////////////////////////////////////////////////////////////////////////////

#define SMPC_CMD_MSHON				0x00
#define SMPC_CMD_SSHON				0x02
#define SMPC_CMD_SSHOFF				0x03
#define SMPC_CMD_SNDON				0x06
#define SMPC_CMD_SNDOFF				0x07
#define SMPC_CMD_CDON				0x08
#define SMPC_CMD_CDOFF				0x09
#define SMPC_CMD_CARTON				0x0A
#define SMPC_CMD_CARTOFF			0x0B
#define SMPC_CMD_SYSRES				0x0D
#define SMPC_CMD_CKCHG352			0x0E
#define SMPC_CMD_CKCHG320			0x0F
#define SMPC_CMD_INTBACK			0x10
#define SMPC_CMD_SETTIM				0x16
#define SMPC_CMD_SETSM				0x17
#define SMPC_CMD_NMIREQ				0x18
#define SMPC_CMD_RESENA				0x19
#define SMPC_CMD_RESDIS				0x1A

//////////////////////////////////////////////////////////////////////////////
//Address mapping shorthand
#define MAP_TO_SCSP(sh2map_snd_adr) ((sh2map_snd_adr - SNDRAM))
//////////////////////////////////////////////////////////////////////////////
//No Touchy Sound Ram Start!
#define SNDRAM  (631242752)
// 1KB here is reserved for interrupts
#define SNDPRG (SNDRAM + 0x408)
//Also the end of sound RAM
#define PCMEND	(SNDRAM + 0x7F000)
//////////////////////////////////////////////////////////////////////////////
#define DRV_SYS_END (53 * 1024) //System defined safe end of driver's address space
#define CHN_CTRL_MAX (64)
#define INS_CTRL_MAX (0x400)
#define PCM_CTRL_MAX (0x100)
#define MLT_CTRL_MAX (PCM_CTRL_MAX)
#define MLT_CTRL_LENGTH_MAX (63)
#define MCR_CTRL_MAX (0x7F)
#define MCR_CTRL_LENGTH_MAX (250)
#define INS_MACROS_MAX (4)
//////////////////////////////////////////////////////////////////////////////
#define CHN_INHERIT_MIN (0x401) // Bounds for the 'inherit' instrument
#define CHN_INHERIT_MAX (0x407) // that borrows values from adjacent channels for FM patches
//////////////////////////////////////////////////////////////////////////////
#define NOTE_INACTIVE -100 // more to come
//////////////////////////////////////////////////////////////////////////////
#define MCR_VARIABLE_LENGTH (1 << 0)
#define MCR_RELATIVE (1 << 1)
#define MCR_FIXED (1 << 2)
#define MCR_JUMP_ON_RELEASE (1 << 3)
#define MCR_OVERFLOW (1 << 4)
#define MCR_CONTINUOUS (1 << 5)
#define MCR_PROGRESSIVE (1 << 6)
#define MCR_PROPAGATE (1 << 7)
#define MACRO_OFF 0xFF
#define MACRO_AT_RELEASE 0xFE
#define MACRO_UNINITIALIZED 0xFD
//////////////////////////////////////////////////////////////////////////////
#define NUM_COEFFICIENTS 63
#define NUM_ADDRESSES 32
//////////////////////////////////////////////////////////////////////////////
#define	PCM_ALT_LOOP	(3)
#define PCM_RVS_LOOP	(2)
#define PCM_FWD_LOOP	(1)
#define PCM_VOLATILE	(0)
#define PCM_PROTECTED	(-1)
#define PCM_SEMI		(-2)
#define ADX_STREAM		(-3)
//////////////////////////////////////////////////////////////////////////////
#define PCM_TYPE_ADX (2) // 4-bit (compressed audio)
#define PCM_TYPE_8BIT (1) // 8-bit
#define PCM_TYPE_16BIT (0) // 16-bit
//////////////////////////////////////////////////////////////////////////////
#define PCM_SYS_REGION	(0) //0 for NTSC, 1 for PAL
//////////////////////////////////////////////////////////////////////////////
#define PCM_PAN_LEFT	(1<<4)
#define PCM_PAN_RIGHT	(0)
//////////////////////////////////////////////////////////////////////////////
#define ADX_MASTER_768 (0)
#define ADX_MASTER_1152 (1)
#define ADX_MASTER_1536 (2)
#define ADX_MASTER_2304 (3)
/* 7.68 Data */
#define ADX_768_COEF_1 (4401)
#define ADX_768_COEF_2 (-1183)
/* 11.52 data */
#define ADX_1152_COEF_1 (5386)
#define ADX_1152_COEF_2 (-1771)
/* 15.36 data */
#define ADX_1536_COEF_1 (5972)
#define ADX_1536_COEF_2 (-2187)
/* 23.04 data */
#define ADX_2304_COEF_1 (6631)
#define ADX_2304_COEF_2 (-2685)
//////////////////////////////////////////////////////////////////////////////
// 6400 Hz for PAL
#define ADX_PAL_640 (4)
#define ADX_640_COEF_1 (3915)
#define ADX_640_COEF_2 (-936)
// 9600 Hz for PAL
#define ADX_PAL_960 (5)
#define ADX_960_COEF_1 (4963)
#define ADX_960_COEF_2 (-1504)
// 12800 Hz for PAL
#define ADX_PAL_1280 (6)
#define ADX_1280_COEF_1 (5612)
#define ADX_1280_COEF_2 (-1923)
// 19200 Hz for PAL
#define ADX_PAL_1920 (7)
#define ADX_1920_COEF_1 (6359)
#define ADX_1920_COEF_2 (-2469)

#define NUM_MACRO_TYPES 36
#define NUM_MACRO_TYPES_NONUNIQUE (NUM_MACRO_TYPES - 2 + 63 + 32 - 1)
enum macro_types {
	MACRO_KEYON = 0,
	MACRO_SAMPLETRAITS = 1,
	MACRO_ENVELOPETRAITS = 2,
	MACRO_LFORESET = 3,
	MACRO_PITCHLFOWAVEFORM = 4,
	MACRO_AMPLFOWAVEFORM = 5,
	MACRO_EFFECTSLOT = 6,
	MACRO_SAMPLEOFFSET = 7,
	MACRO_LOOPSTART = 8,
	MACRO_LOOPEND = 9,
	MACRO_TOTALLEVEL = 10,
	MACRO_VOLUME = 11,
	MACRO_ATTACK = 12,
	MACRO_DECAY1 = 13,
	MACRO_SUSTAIN = 14,
	MACRO_DECAY2 = 15,
	MACRO_RELEASE = 16,
	MACRO_KEYSCALING = 17,
	MACRO_LFOFREQ = 18,
	MACRO_PITCHLFOSTRENGTH = 19,
	MACRO_AMPLFOSTRENGTH = 20,
	MACRO_MODLEVEL = 21,
	MACRO_PITCHMULTIPLIER = 22,
	MACRO_PITCHDIVIDER = 23,
	MACRO_LEVELSCALING = 24,
	MACRO_EFFECTVOLUME = 25,
	MACRO_INPUTLEVEL = 26,
	MACRO_PAN = 27,
	MACRO_NOTEOFFSET = 28,
	MACRO_CENTDETUNE = 29,
	MACRO_REGISTERDETUNE = 30,
	MACRO_MODINPUTX = 31,
	MACRO_MODINPUTY = 32,
	MACRO_EFFECTPAN = 33,
	MACRO_EFFECTCOEF = 34,
	MACRO_EFFECTADRS = 34 + 63,
};
#define FIRST_RELATIVE MACRO_SAMPLEOFFSET
#define FIRST_SIGNED MACRO_PAN

#define MACRO_KEYON_LBOUND 0
#define MACRO_KEYON_UBOUND 3
#define MACRO_SAMPLETRAITS_LBOUND 0
#define MACRO_SAMPLETRAITS_UBOUND 7
#define MACRO_ENVELOPETRAITS_LBOUND 0
#define MACRO_ENVELOPETRAITS_UBOUND 3
#define MACRO_LFORESET_LBOUND 0
#define MACRO_LFORESET_UBOUND 1
#define MACRO_PITCHLFOWAVEFORM_LBOUND 0
#define MACRO_PITCHLFOWAVEFORM_UBOUND 3
#define MACRO_AMPLFOWAVEFORM_LBOUND 0
#define MACRO_AMPLFOWAVEFORM_UBOUND 3
#define MACRO_EFFECTSLOT_LBOUND 0
#define MACRO_EFFECTSLOT_UBOUND 15
#define MACRO_SAMPLEOFFSET_LBOUND 0
#define MACRO_SAMPLEOFFSET_UBOUND 0xFFFF
#define MACRO_LOOPSTART_LBOUND 0
#define MACRO_LOOPSTART_UBOUND 0xFFFF
#define MACRO_LOOPEND_LBOUND 0
#define MACRO_LOOPEND_UBOUND 0xFFFF
#define MACRO_TOTALLEVEL_LBOUND 0
#define MACRO_TOTALLEVEL_UBOUND 0xFF
#define MACRO_VOLUME_LBOUND 0
#define MACRO_VOLUME_UBOUND 7
#define MACRO_ATTACK_LBOUND 0
#define MACRO_ATTACK_UBOUND 0x1F
#define MACRO_DECAY1_LBOUND 0
#define MACRO_DECAY1_UBOUND 0x1F
#define MACRO_SUSTAIN_LBOUND 0
#define MACRO_SUSTAIN_UBOUND 0x1F
#define MACRO_DECAY2_LBOUND 0
#define MACRO_DECAY2_UBOUND 0x1F
#define MACRO_RELEASE_LBOUND 0
#define MACRO_RELEASE_UBOUND 0x1F
#define MACRO_KEYSCALING_LBOUND 0
#define MACRO_KEYSCALING_UBOUND 0xF
#define MACRO_LFOFREQ_LBOUND 0
#define MACRO_LFOFREQ_UBOUND 0x1F
#define MACRO_PITCHLFOSTRENGTH_LBOUND 0
#define MACRO_PITCHLFOSTRENGTH_UBOUND 7
#define MACRO_AMPLFOSTRENGTH_LBOUND 0
#define MACRO_AMPLFOSTRENGTH_UBOUND 7
#define MACRO_MODLEVEL_LBOUND 0
#define MACRO_MODLEVEL_UBOUND 7
#define MACRO_PITCHMULTIPLIER_LBOUND 0
#define MACRO_PITCHMULTIPLIER_UBOUND 0xFF
#define MACRO_PITCHDIVIDER_LBOUND 0
#define MACRO_PITCHDIVIDER_UBOUND 0xFF
#define MACRO_LEVELSCALING_LBOUND 0
#define MACRO_LEVELSCALING_UBOUND 0xF
#define MACRO_EFFECTVOLUME_LBOUND 0
#define MACRO_EFFECTVOLUME_UBOUND 7
#define MACRO_INPUTLEVEL_LBOUND 0
#define MACRO_INPUTLEVEL_UBOUND 7
#define MACRO_PAN_LBOUND -15
#define MACRO_PAN_UBOUND 15
#define MACRO_NOTEOFFSET_LBOUND -96
#define MACRO_NOTEOFFSET_UBOUND 95
#define MACRO_CENTDETUNE_LBOUND -100
#define MACRO_CENTDETUNE_UBOUND 100
#define MACRO_REGISTERDETUNE_LBOUND -16384
#define MACRO_REGISTERDETUNE_UBOUND 16384
#define MACRO_MODINPUTX_LBOUND -15
#define MACRO_MODINPUTX_UBOUND 47
#define MACRO_MODINPUTY_LBOUND -15
#define MACRO_MODINPUTY_UBOUND 47
#define MACRO_EFFECTPAN_LBOUND -15
#define MACRO_EFFECTPAN_UBOUND 15
#define MACRO_EFFECTCOEF_LBOUND -4096
#define MACRO_EFFECTCOEF_UBOUND 4096
#define MACRO_EFFECTADRS_LBOUND 0
#define MACRO_EFFECTADRS_UBOUND 0xFFFF

typedef struct {
	short instrumentID;					// Instrument the channel should reference
	unsigned short note;			    // Low byte = semitone offset, high byte = (eventually) cent offset
	unsigned char velocity;				// Individual note volume
	unsigned char channel_volume;		// Individual channel volume
	char ctrlType;						// Playback and/or loop type
	unsigned char volume;				// Direct volume setting (Legacy support for chn_play)
	unsigned char reset_bits;           // Bit 0 = Disable LFO, Bit 1 = Use LFO reset macro; Bit 2 = LFO macro state
									    // Bit 4 = Use Keyon macro; Bit 5 = Keyoff, Bit 6 = Keyon
	unsigned short key_data;			// These correspond directly to SCSP registers
	unsigned short start_addr;
	unsigned short LSA;
	unsigned short playsize;

	unsigned short decay_1_2_attack;
	unsigned short key_decay_release;
	unsigned short attenuation;
	unsigned short FM_data;
	unsigned short lfo_data;
	unsigned short input_sel;
	unsigned short pan_send;
	unsigned char lfo_delay;           
	unsigned short decompression_size; // Size of the buffer used for an ADX sound effect.

	unsigned short macro_values[INS_MACROS_MAX];			// Holds the value that should be assigned from the macro on each frame
	unsigned char macro_timers[INS_MACROS_MAX];				// Keeps track of the channel's macro positions
	unsigned short macro_length_timers[INS_MACROS_MAX];		// Keeps track of the channel's macro value lengths (when variable length mode is enabled)
	unsigned char macro_states;								// 8 bits (one per macro) that set if the given macro should move to its release phase

	unsigned short final_pitch;        // Pitch value calculated from instrument and note data
	unsigned char melodic_data;        // Lowest 6 bits = static playback slot - 1, bit 6 = note on flag, bit 7 = note off flag
	unsigned short test_area;
	unsigned short bytes_per_blank;	   // Bytes the PCM will play every time the driver is run (vblank)
	unsigned char sh2_permit;		   // Does the SH2 permit this command? If TRUE, run the command. If FALSE, key its ICSR OFF.
	char icsr_target;				   // Which explicit ICSR is this to land in? Can be controlled by SH2 or by driver.
} _CHN_CTRL; // Driver Local Command Struct

typedef struct {
	unsigned char sampleID;			   // Sample data used by the instrument
	unsigned char use_multi;		   // Decides if sampleID refers to a sample or a sample map
	unsigned short sample_offset;	   // Number of samples to increase the start point by
	unsigned short loopstart_offset;   // Number of samples to increase the loop start point by
	unsigned short loopend_offset;	   // Number of samples to decrease the loop end point by
	unsigned char pan;				   // Direct pan setting
	unsigned char volume;			   // Direct volume setting
	unsigned char totallevel;          // Volume attenuation
	unsigned char lfo_freq;            // LFO reset bit and frequency
	unsigned char lfo_pitch;           // Pitch LFO strength and waveform
	unsigned char lfo_amp;             // Amplitude LFO strength and waveform
	unsigned char lfo_delay;           // Frames to wait before enabling LFO
	unsigned char bit_reverse;         // Waveform bit reversal (SBCTL)
	unsigned char noise_mode;          // Enables noise generation (SSCTL)
	unsigned char attack_hold;         // Envelope attack/hold rate and hold setting
	unsigned char decay1;              // Envelope decay 1 rate
	unsigned char sustain;             // Envelope 'decay level' (sustain)
	unsigned char decay2;              // Envelope decay 2 rate
	unsigned char release;             // Envelope release rate
	unsigned char key_scaling_sync;    // Envelope key scaling and loop sync mode
	unsigned short patchlength;	       // Number of additional instruments to play during keyon
	unsigned char mod_volume;          // Coarse modulation volume (good for feedback)
	unsigned char mod_input_x;         // First modulation input slot
	unsigned char mod_input_y;         // Second modulation input slot
	unsigned char use_envelope;        // Use envelope settings (in lieu of the defaults)
	unsigned char effect_pan;          // Effect pan setting
	unsigned char effect_volume;       // Effect volume setting
	unsigned char input_volume;        // Effect input setting
	unsigned char effects_slot;        // Effect input slot
	unsigned short freq_ratio;         // FM frequency multiplier/divider ( - 1)
	unsigned char level_scaling;	   // Lowers total level as pitch increases
	unsigned short pitch_offset;	   // Low byte = semitone offset, high byte = cent offset
	unsigned short register_detune;    // Directly added to the pitch register during pitch calculation
	unsigned char ignore_velocity;	   // Sets if the instrument's volume is affected by channel volume settings
	unsigned char macros[INS_MACROS_MAX]; // If 0, no macro will be used; if greater than 0, use macro (value - 1)
	unsigned short test_area;
	unsigned short bytes_per_blank;    //Bytes the PCM will play every time the driver is run (vblank)
	unsigned short decompression_size; //Size of the buffer used for an ADX sound effect. Specifically sized by Master SH2.
} _INS_CTRL; //Driver Instrument Struct

typedef struct {
	char loopType;					   // [0,1,2,3] No loop, normal loop, reverse loop, alternating loop
	unsigned char bitDepth;			   // 0, 1, or 2; 0 is 16-bit, 1 is 8-bit, 2 is ADX
	unsigned short hiAddrBits;		   // bits 19-16 of...
	unsigned short loAddrBits;		   // Two 16-bit chunks that when combined, form the start address of the sound.
	unsigned short sample_start;	   // Determines the start point of the sample, offset from the sample data's address
	unsigned short max_playsize;	   // The original size of the sample, without any modifiders
	unsigned short LSA;				   // The # of samples forward from the start address to return to after loop.
	unsigned short playsize;		   // The # of samples to play before the sound shall loop. **Otherwise used as the length of the sound.** Do not leave at 0!
									   // 8 bit PCM is 1 byte per sample. 16 bit PCM is 2 bytes per sample. Therefore an 8bit PCM is a maximum of 64KB, and 16bit is 128KB.
	unsigned short base_pitch;		   // the OCT & FNS, verbatim, to use when playing the sample at base_note.
	unsigned char base_note;		   // Note at which the sample plays at base_pitche
	unsigned short test_area;
	unsigned short bytes_per_blank;	   // Bytes the PCM will play every time the driver is run (vblank)
	unsigned short decompression_size; //Size of the buffer used for an ADX sound effect. Specifically sized by Master SH2.
} _PCM_CTRL; //Driver Sample Data Struct

typedef struct {
	unsigned char traits; // Bits 0-5 - Number of samples to use (-1); Bit 6 - Overrides channel note values; 
						  // Bit 7 - Determines if the data is a key map (one sample = one note) or a key split (each sample covers a range of notes)
	unsigned char data[MLT_CTRL_LENGTH_MAX]; // In a key map, each byte contains a sample ID (e.g. playing note 2 will play the sample in data[2])
											 // In a key split, each pair of bytes contains a sample ID and the lowest note that will play that sample 
} _MLT_CTRL; // Driver Multi-Sample Data Struct

typedef struct {
	unsigned char type;	   // ID to value the macro will operate on
	unsigned char traits;  // Bit 0: Enables variable-length macro values (as opposed to single-length ones)
						   // Bit 1: Enables relative macros (as opposed to absolute ones)
						   // Bit 2: Enables fixed value mode (for note offset macros)
						   // Bit 3: Enables jumping straight to the release portion of the macro after a note off
						   // Bit 4: Allows relative macros to overflow their value's bounds
						   // Bit 5: Does not reset the macro upon the start of a new note
						   // Bit 6: Does not reset the macro's internal value when resetting it
						   // Bit 7: If the instrument using the macro is the head of a patch, the patch's other instruments will inherit the macro's changes
	unsigned char length;  // Number of steps in the macro (bounded at 250)
	unsigned char loop_1;  // Start of the macro's first loop point
	unsigned char loop_2;  // Start of the macro's second loop point
	unsigned char release; // Start of the macro's release point
	unsigned short data[MCR_CTRL_LENGTH_MAX]; // Data in the macro (will be interpreted differently depending on the type and traits)
} _MCR_CTRL; // Driver macro Data Struct

typedef struct {
	volatile unsigned int adx_stream_length; // Length of the ADX stream (in ADX frames)
	volatile unsigned short start; // System Start Boolean
	volatile char	adx_buffer_pass[2]; // Booleans
	volatile short drv_adx_coef_1; // The (signed!) coefficient 1 the driver will use to build ADX multiplication tables.
	volatile short drv_adx_coef_2; // The (signed!) coefficient 2 the driver will use to build ADX multiplication tables.
	volatile _CHN_CTRL* chnCtrl;
	volatile unsigned char cdda_left_channel_vol_pan; // Redbook left channel volume & pan.
	volatile unsigned char cdda_right_channel_vol_pan; // Redbook right channel volume & pan.
	volatile short coefficients[NUM_COEFFICIENTS]; // DSP Coefficients
	volatile short addresses[NUM_ADDRESSES];  // DSP Addresses
} sysComPara;

// Unused by this driver
typedef struct {
	unsigned short one_half; // [this is 32768 or 0x8000]
	short offset2data;
	unsigned char format; // [this is 3]
	unsigned char block_size; // [this is 18]
	unsigned char bit_depth; // [this is 4]
	unsigned char channels; // [this should be 1]
	unsigned int sample_rate;
	unsigned int sample_ct;
	unsigned short hp_cutoff; // [this should be 500]
	unsigned char loop;
	unsigned char illegal; // [Boolean, 0 for false, 1 for true]
} adx_header;

//

//
extern	sysComPara* m68k_com;
extern _CHN_CTRL chnCtrl[CHN_CTRL_MAX];
extern _INS_CTRL insCtrl[INS_CTRL_MAX];
extern _PCM_CTRL pcmCtrl[PCM_CTRL_MAX];
extern _MLT_CTRL mltCtrl[MLT_CTRL_MAX];
extern _MCR_CTRL mcrCtrl[MCR_CTRL_MAX];
extern short coefficients[NUM_COEFFICIENTS];
extern unsigned short addresses[NUM_ADDRESSES];
extern short base_coefficients[NUM_COEFFICIENTS];
extern unsigned short base_addresses[NUM_ADDRESSES];
extern short coefficientBounds[NUM_COEFFICIENTS][2];
extern unsigned short addressBounds[NUM_ADDRESSES][2];
extern	unsigned int* scsp_load;
extern unsigned short* master_volume;
extern unsigned short driver_master_volume;
extern short numberChans;
extern short numberInsts;
extern short numberMults;
extern const unsigned short macroBounds[NUM_MACRO_TYPES][2];

//
// System functions shared for pcmstm.c/h
//
short			convert_bitrate_to_pitchword(short sampleRate);
int				convert_pitchword_to_bitrate(short pitchWord);
unsigned short	calculate_bytes_per_blank(int sampleRate, Bool is8Bit, Bool isPAL);
unsigned short	calculate_bytes_per_blank_with_pitchword(short pitchword, unsigned char bitDepth);
short 			lcm(short a, short b);
void			cd_init(void);

//
// These are likely to be duplicate commands from other libraries.
//
void	smpc_wait_till_ready(void);
void	smpc_issue_command(unsigned char cmd);
//
//
//

enum lfo_waves {
	LFO_SAW, LFO_SQUARE, LFO_TRI, LFO_NOISE
};

enum mod_generations {
	MOD_OLD, MOD_NEW
};

short	initialize_new_pcm(Bool is8Bit, int sampleRate, Sint32 file_size, Sint32 playsize, Bool addSilence);
short	copy_pcm(short pcmNumber);
short	load_16bit_pcm(Sint8* filename, int sampleRate, Bool shouldLoop, Bool addSilenceAtStart);
short	load_8bit_pcm(Sint8* filename, int sampleRate, Bool shouldLoop, Bool addSilenceAtStart);
short	load_16bit_pcm_with_fm_padding(Sint8* filename, int sampleRate);
short	load_8bit_pcm_with_fm_padding(Sint8* filename, int sampleRate);
short	load_adx(Sint8* filename);
void	load_drv(int master_adx_frequency);

short	initialize_new_instrument(short pcmNumber);

short	init_sequenced_channels(short numChannels);
short	initialize_new_channel(short insNumber);

void	set_master_volume(unsigned short volume);

void	pcm_loop_type_change(short pcmNumber, unsigned char loop);
void	pcm_sample_start_change(short pcmNumber, unsigned short start);
void	pcm_loop_start_change(short pcmNumber, unsigned short start);
void	pcm_loop_end_change(short pcmNumber, unsigned short end);
void	pcm_loop_end_change(short pcmNumber, unsigned short end);
void	pcm_pitch_change(short pcmNumber, short sampleRate);
void	pcm_base_pitch_change(short pcmNumber, short pitch);
void	pcm_base_note_change(short pcmNumber, unsigned char note);

unsigned short	pcm_get_max_playsize(short pcmNumber);
unsigned short	pcm_get_sample_start(short pcmNumber);
unsigned short	pcm_get_loop_start(short pcmNumber);
unsigned short	pcm_get_loop_end(short pcmNumber);

void	ins_sample_change(short insNumber, unsigned char sample);
void	ins_sample_type_change(short insNumber, unsigned char type);
void	ins_sample_offset_change(short insNumber, unsigned short offset);
void	ins_loopstart_offset_change(short insNumber, unsigned short offset);
void	ins_loopend_offset_change(short insNumber, unsigned short offset);
void	ins_parameter_change(short insNumber, char volume, char pan);
void	ins_totallevel_change(short insNumber, unsigned char totalLevel);
void	ins_lfo_freq_change(short insNumber, char lfo_freq);
void	ins_lfo_pitch_change(short insNumber, char lfo_strength, char lfo_wave);
void	ins_lfo_amp_change(short insNumber, char lfo_strength, char lfo_wave);
void	ins_lfo_delay_change(short insNumber, char lfo_delay);
void	ins_bit_reverse_change(short insNumber, char bit_reverse, char invert);
void	ins_noise_mode_change(short insNumber, char noise_mode);
void	ins_attack_hold_change(short insNumber, char attack_hold, char hold_mode);
void	ins_decay1_change(short insNumber, char decay1);
void	ins_sustain_change(short insNumber, char sustain);
void	ins_decay2_change(short insNumber, char decay2);
void	ins_release_change(short insNumber, char release);
void	ins_key_scaling_sync_change(short insNumber, char key_scaling, char loop_sync);
void	ins_enable_envelope(short insNumber);
void	ins_disable_envelope(short insNumber);
void	ins_patchlength_change(short insNumber, unsigned char length);
void	ins_mod_volume_change(short insNumber, char mod_volume);
void	ins_mod_input_x_change(short insNumber, char mod_input, char generation);
void	ins_mod_input_y_change(short insNumber, char mod_input, char generation);
void	ins_effect_parameter_change(short insNumber, char volume, char pan);
void	ins_effect_input_change(short insNumber, char level, char slot);
void	ins_freq_ratio_change(short insNumber, unsigned char multiplier, unsigned char divider);
void	ins_level_scaling_change(short insNumber, unsigned char scaling);
void	ins_note_offset_change(short insNumber, unsigned char semitone, unsigned char cent);
void	ins_register_detune_change(short insNumber, short detune);
void	ins_ignore_velocity_change(short insNumber, unsigned char ignore_velocity);
void	ins_macro_change(short insNumber, short index, unsigned char macro);

short	ins_get_current_sample(short insNumber, unsigned short note);
unsigned char ins_get_patchlength(short insNumber);

void	mlt_traits_change(short mltNumber, unsigned char length, unsigned char base_note_override, unsigned char type);
void	mlt_set_data(short mltNumber, short index, unsigned char data);

short	mlt_get_length(short mltNumber);
Bool	mlt_is_base_note_override(short mltNumber);
Bool	mlt_is_keysplit(short mltNumber);

void	mcr_type_change(short mcrNumber, unsigned char type);
void	mcr_length_change(short mcrNumber, unsigned char length);
void	mcr_traits_change(short mcrNumber, Bool variable_length, Bool relative, Bool fixed, Bool jump_on_release, Bool overflow, Bool continuous, Bool progressive, Bool propagate);
void	mcr_loop1_change(short mcrNumber, unsigned char point);
void	mcr_loop2_change(short mcrNumber, unsigned char point);
void	mcr_release_change(short mcrNumber, unsigned char point);
void	mcr_set_data(short mcrNumber, short index, unsigned short data);

static __jo_force_inline unsigned char	mcr_get_type(_MCR_CTRL* macro) {
	return macro->type;
}

static __jo_force_inline unsigned char	mcr_get_length(_MCR_CTRL* macro) {
	return macro->length;
}

static __jo_force_inline Bool	mcr_is_variable_length(_MCR_CTRL* macro) {
	return ((macro->traits & MCR_VARIABLE_LENGTH) != 0);
}

static __jo_force_inline Bool	mcr_is_relative(_MCR_CTRL* macro) {
	return ((macro->traits & MCR_RELATIVE) != 0);
}

static __jo_force_inline Bool	mcr_is_continuous(_MCR_CTRL* macro) {
	return ((macro->traits & MCR_CONTINUOUS) != 0);
}

static __jo_force_inline Bool	mcr_is_fixed(_MCR_CTRL* macro) {
	return ((macro->traits & MCR_FIXED) != 0);
}

static __jo_force_inline Bool	mcr_does_jump_on_release(_MCR_CTRL* macro) {
	return ((macro->traits & MCR_JUMP_ON_RELEASE) != 0);
}

static __jo_force_inline Bool	mcr_does_overflow(_MCR_CTRL* macro) {
	return ((macro->traits & MCR_OVERFLOW) != 0);
}

static __jo_force_inline Bool	mcr_is_progressive(_MCR_CTRL* macro) {
	return ((macro->traits & MCR_PROGRESSIVE) != 0);
}

static __jo_force_inline Bool	mcr_does_propagate(_MCR_CTRL* macro) {
	return ((macro->traits & MCR_PROPAGATE) != 0);
}

void	get_macro_bounds(unsigned char type, int* lbound, int* ubound);
int		clamp_macro_value(int value, unsigned char type);
int		overflow_macro_value(int value, unsigned char type);
unsigned short get_initial_macro_value(unsigned char type, _INS_CTRL* instrument);

void	dsp_load_base_variables(void);
void	dsp_set_variables(void);

void	chn_play(short chnNumber, char ctrlType, char volume);
void	chn_play_melodic(short chnNumber);
void	chn_instrument_change(short chnNumber, short instrument);
void	chn_reset_lfo(short chnNumber);
void	chn_set_lfo(short chnNumber);
void	chn_set_melodic_data(short chnNumber, char slot);
void	chn_remove_melodic_flags(void);
void	chn_note_change(short chnNumber, unsigned char note);
void	chn_velocity_change(short chnNumber, unsigned char velocity);
void	chn_channel_volume_change(short chnNumber, unsigned char volume);

short	chn_get_current_instrument(short chnNumber);

void	chn_hard_reset_macros(short chnNumber);
void	chn_soft_reset_macros(short chnNumber);

void	chn_set_macro_values(short chnNumber);
void	chn_set_final_pitch(_CHN_CTRL* channel, _INS_CTRL* instrument, _PCM_CTRL* sample, short note, short note_offset, short cent, short freq_mul, short freq_div, short reg_detune);
void	chn_set_final_level(_CHN_CTRL* channel, _CHN_CTRL* patch_leader_channel, _INS_CTRL* instrument, short level, unsigned char scaling);
void	chn_set_values(short chnNumber);

void	chn_release(short chnNumber);
void	chn_cease(short chnNumber);
void	chn_release_all();
void	chn_cease_all();

int		test_func(short chnNumber);
int		test_inst(short insNumber);

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
void	pcm_reset(short default_pcm_count);

void	sdrv_vblank_rq(void);

//
// Redbook support
// Credit: ndiddy, ReyeMe, CyberWarriorX [Iapetus]
//

void CDDA_SetVolume(int vol);
void CDDA_SetChannelVolPan(unsigned char left_channel, unsigned char right_channel);
void CDDA_Play(int fromTrack, int toTrack, Bool loop);
void CDDA_PlaySingle(int track, Bool loop);
void CDDA_Stop(void);


#endif

