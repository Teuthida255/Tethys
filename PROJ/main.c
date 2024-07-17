#define ADDR_PRG   (0x408)
#define ADDR_MAX   (0x7F000)
#define ADDR_STACK (0x80000 - 0x400)
#define ADDR_END   (0x80000)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Big credit to cWx :: cyberWarriorX for a sound driver prototype and most
// importatly, the linker script!
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int *vecTable[256]
  __attribute__((section("VECTORS"))) = {(int *) ADDR_STACK, (int *) ADDR_PRG, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END,
    (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END, (int *) ADDR_END};

// uArch warning:
// Some operations that handle 4-bytes (void, boolean, int, unsigned int,
// pointers) will not work as expected.

void lead_function(void) // Link start to main
{
  // note that _start is the ASM label that equates to
  // the lead function in this compiler. In a normal
  // compiler, it would be "main".
  __asm__("jmp _start");
}

// Place all includes past this line
#define CHN_CTRL_MAX (64)
#define ADX_CTRL_MAX (3)
#define DRV_SYS_END  (53 * 1024) // System defined safe end of driver's address space

#define PCM_ALT_LOOP                 (3)
#define PCM_RVS_LOOP                 (2)
#define PCM_FWD_LOOP                 (1)
#define PCM_VOLATILE                 (0)
#define PCM_PROTECTED                (-1)
#define PCM_SEMI                     (-2)
#define ADX_STREAM                   (-3)
#define ADX_STREAM_BUFFERED_FRAME_CT (1024)

#define PCM_TYPE_ADX   (2) // 4-bit (compressed audio)
#define PCM_TYPE_8BIT  (1) // 8-bit
#define PCM_TYPE_16BIT (0) // 16-bit

#define INS_MACROS_MAX 4

#define NUM_COEFFICIENTS 63
#define NUM_ADDRESSES 32

// clang-format off
//   GLOSSARY OF TERMS:
//    KX : EXECUTE KEY [select] [Start or stop the sound] [Reset to 0 by system]
//    KB : Key ON or Key OFF select [0 is OFF, 1 is ON]
//    SBCTL : Source bit control - Boolean for whether data is signed or unsigned
//    SSCTL : Source type control [0: PCM, 1: Noise Generator, 2: Waveform generator, 3: Illegal] - Pertains to LFO registers in 1&2
//    LPCTL : loop control [0: Sound ends when LEA is reached, 1: normal loop, 2: reverse loop, 3: alternating loop]
//            It is important to note that looping commands still need continuous KEY_ON setting?
//    8B : PCM 8-bit wave data boolean [1: 8 bit 0: 16 bit]
//    SA : Start address of waveform data [PCM]. If the PCM data is 16-bit, the data must be aligned on a 2 byte boundary.
//    LSA : Sample count, in distance from SA, to start the loop [at 8b, sample is 1 byte]
//    LEA : Sample count, in distance from SA, to end the loop [at 16b, sample is 2 bytes]
//    HO : EG Hold Mode Boolean [Changes volume rise according to attack rate setting]
//    LS : LPSLINK : Loop start link [Links or unlinks attack rates with looping playback]
//    SI : STWINH : Stack write inhibit [FM generator write inhibit, normally false is fine]
//    SD : SDIR : Sound direct [0: Attenuated by TL; 1: Output wave data directly]
//    TL : Total level [Bitwise data struct which may attenuate or divide the waveform of the input]
//    ...
//    ...
//    RE :    LFO Reset - Low frequency oscillator reset switch
//    PLF.. : LFO frequency modulation wave form select [OWS] or select [S] [PITCH] [???]
//    ALF.. : LFO amplitude modulation wave form select [OWS] or select [A] [AMP] [???]
//    ...
//    DISDL: Direct data send level [???]
//    DIPAN: Direct data pan level [MSB: Left/right boolean, bits 0-4: distance from center]
//    EFSDL: Effect send level [???]
//    EFPAN: Effect pan level [MSB: Left/right boolean, bits 0-4: distance from center]
//    ..
//    For FM sound source, there are a lot more things to set outside of the individual command.
//   BIG ENDIAN                                                      Byte+1
// INTEGER |--MSB--------------------------------------------------------------------------------------------------------------------LSB--|
//     BIT |  15  |   14  |   13  |   12  |   11  |   10  |   9   |   8   |   7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
// BYTE    |------------------------------------------------------------------------------------------------------------------------------|
// 0       |  -       -       -   |   KX  |   KB  |   SBCTL-2BIT  |   SSCTL-2BIT  |   LPCTL-2BIT  |   8B  |   START ADDRESS   19-16       | keys
// 2       |                                                  START ADDRESS [SA] 15-0                                                     | start_addr
// 4       |                                                  LOOP START ADDRESS [LSA] 15-0                                               | loop_start
// 6       |                                                  LOOP END ADDRESS    [LEA] 15-0                                              | playsize
// 8       |  D2R-DECAY 2 RATE    5 BIT           |   D1R-DECAY 1 RATE    5 BIT           |   HO  |   AR-ATTACK RATE  5 BIT               | decay_1_2_attack
// 10      |  -   |   LS  |   KRS-KEY RATE SCALE 4 BIT    |   DL-DECAY LEVEL      5 BIT           |   RR-RELEASE RATE 5 BIT               | key_decay_release
// 12      |  -       -       -       -       -       -   |   SD  |   SI  |   TL-TOTAL LEVEL  8 BIT   [VOLUME]                            | attenuation
// 14      |  MDL-MODULATION LEVEL 4BIT   |   MDXSL-X MODULATION SELECT   6 BIT           |   MDYSL-Y MODULATION SELECT   6 BIT           | FM_data
// 16      |  -   |   OCT-OCTAVE 4BIT             |   -   | FNS-FREQUENCY NUMBER SWITCH [PITCH] 10 BIT                                    | oct_fns
// 18      |  RE  |   LFOF-LFO FREQUENCY 5 BIT            |   PLFOWS-2BIT |   PLFOS-3BIT [PITCH]  |   AFLOWS-2BIT |   AFLOS-3BIT  [AMP]   | lfo_data
// 20      |  -       -       -       -       -       -       -       -       -   |   ISEL-INPUT SELECT 4 BIT     |   IMXL-INPUT MIX LVL  | input_sel
// 22      |  DISDL-3BIT          |   DIPAN-5 BIT                         |   EFSDL-3BIT          |   EFPAN - EFFECT PAN  5 BIT           | pan_send
//     BIT |  15  |   14  |   13  |   12  |   11  |   10  |   9   |   8   |   7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
// 24      |                                                              EMPTY                                                           | blanks
// 26      |                                                              EMPTY                                                           |
// 28      |                                                              EMPTY                                                           |
// 30      |                                              EMPTY           |   Byte 31 End of individual command slot register             |
// INTEGER |--MSB--------------------------------------------------------------------------------------------------------------------LSB--|
// clang-format on

typedef struct {
  unsigned short keys;
  unsigned short start_addr;
  unsigned short loop_start;
  // IMPORTANT NOTICE: The SCSP actually plays 1 more sample than this number,
  // even in a loop. This is probably due to pipelining: The SCSP processes 1
  // sample while reading the next sample. It won't get the correct condition
  // for the final sample played because the condition for the previous sample
  // is processed on the next.
  unsigned short playsize;
  unsigned short decay_1_2_attack;
  unsigned short key_decay_release;
  unsigned short attenuation;
  unsigned short FM_data; // Should leave 0 for PCM
  unsigned short oct_fns;
  unsigned short lfo_data;  // Should leave 0 for PCM
  unsigned short input_sel; // Should leave 0 for PCM
  unsigned short pan_send;
  unsigned short blanks[4];
} _ICSR; // SCSP Individual Command Slot Register

typedef struct {
    signed short coef[64];
    unsigned short madrs[64];
    unsigned short program[512]; // 128 4-word instructions
} _DSPR; // SCSP DSP Register

typedef struct {
  short instrumentID;                // Instrument the channel should reference
  unsigned short note;			     // Low byte = semitone offset, high byte = cent offset
  unsigned char velocity;			 // Individual note volume
  unsigned char channel_volume;		 // Individual channel volume
  char ctrlType;                     // Playback and/or loop type
                                     // alternating loop.
  unsigned char volume;              // Direct volume setting
  unsigned char reset_bits;          // Bit 0 = Disable LFO, Bit 1 = Use LFO reset macro; Bit 2 = LFO macro state
                                     // Bit 4 = Use Keyon macro; Bit 5 = Keyoff, Bit 6 = Keyon

  unsigned short key_data;
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

  unsigned short macro_values[INS_MACROS_MAX]; // Holds the value that should be assigned from the macro on each frame
  unsigned char macro_timers[INS_MACROS_MAX]; // Keeps track of the channel's macro positions
  unsigned short macro_length_timers[INS_MACROS_MAX]; // Keeps track of the channel's macro value lengths (when variable length mode is enabled)
  unsigned char macro_states; // 8 bits (one per macro) that set if the given macro should move to its release phase

  unsigned short final_pitch;        // Pitch value calculated from instrument and note data
  unsigned char melodic_data;        // Lowest 6 bits = static playback slot - 1, bit 6 = note on flag, bit 7 = note off flag
  unsigned short test_area;
  unsigned short bytes_per_blank;    // Bytes the PCM will play every time the
                                     // driver is run (vblank)
  unsigned char sh2_permit;          // Does the SH2 permit this command? If TRUE, run
                                     // the command. If FALSE, key its ICSR OFF.
  volatile char icsr_target;         // Which explicit ICSR is this to land in? Can be
                                     // controlled by SH2 or by driver.
} _CHN_CTRL;                         // Driver Local Command Struct

typedef struct {
  short decomp_demand;
  short decomp_space;
  short recharge_point;
  short work_play_pt;
  short work_decomp_pt;
  volatile short status;
  short pcm_number;
  unsigned int current_frame;
  unsigned short whippet_frame;
  unsigned short whippet_point;
  short last_sample;
  short last_last_sample;
  unsigned short passed_buffers;
  char buf_string[2];
  unsigned short test_area;
  volatile short *dst;
  volatile short *original_dst;
  volatile unsigned short *src;
  volatile unsigned short *original_src;
} _ADX_CTRL; // Driver Local ADX Struct

typedef struct {
  volatile unsigned int adx_stream_length; // Length of the ADX stream (in ADX frames)
  volatile unsigned short start;           // System Start Boolean
  volatile char adx_buffer_pass[2];        // Booleans
  volatile short drv_adx_coef_1;           // The (signed!) coefficient 1 the driver will
                                           // use to build ADX multiplication tables.
  volatile short drv_adx_coef_2;           // The (signed!) coefficient 2 the driver will
                                           // use to build ADX multiplication tables.
  volatile _CHN_CTRL* chnCtrl;
  volatile unsigned char cdda_left_channel_vol_pan;  // Redbook left channel volume & pan.
  volatile unsigned char cdda_right_channel_vol_pan; // Redbook right channel volume & pan.
  volatile short coefficients[NUM_COEFFICIENTS];     // DSP Coefficients
  volatile unsigned short addresses[NUM_ADDRESSES];  // DSP Addresses
} sysComPara;

// Warning: Do not alter the master volume register from within the 68k program.
volatile sysComPara *sh2Com = (volatile sysComPara *) (ADDR_PRG + DRV_SYS_END);
volatile _ICSR *csr = (volatile _ICSR *) 0x100000; // There are 32 of these.
volatile _DSPR *dsp = (volatile _DSPR*)0x100700;


_CHN_CTRL chnCtrlData[CHN_CTRL_MAX + ADX_CTRL_MAX];
short ICSR_Busy[32];
short icsr_index = 0;
short loopingPCMs[CHN_CTRL_MAX];
short volatilePCMs[CHN_CTRL_MAX];
short adxPCMs[CHN_CTRL_MAX];
int dataTimers[32];
short lfoTimers[32];

#define ADX_STATUS_NONE   (0)
#define ADX_STATUS_ACTIVE (1)
#define ADX_STATUS_DECOMP (1 << 1)
#define ADX_STATUS_EMPTY  (1 << 2)
#define ADX_STATUS_FULL   (1 << 3)
#define ADX_STATUS_PLAY   (1 << 4)
#define ADX_STATUS_START  (1 << 5)
#define ADX_STATUS_END    (1 << 6)
#define ADX_STATUS_LAST   (1 << 7)

///////////////////////////
// ADX Control Data Struct
///////////////////////////
_ADX_CTRL adx[ADX_CTRL_MAX];

///////////////////////////
// ADX Decompression Tables
// Filled at driver start-up
///////////////////////////
short coef_tbl_1[4096];
short *center_coef_1;
short coef_tbl_2[4096];
short *center_coef_2;

// ADX decompression work buffer
short adx_work_buf[(10 * 1024)];

// ADX decompression nibble table
const short nibble_to_int[16] = {0, 1, 2, 3, 4, 5, 6, 7, -8, -7, -6, -5, -4, -3, -2, -1};

// ADX PCM_CTRL Dummy. Sound slot that is configured by the ADX system.
short adx_dummy[ADX_CTRL_MAX];

// ADX Buffer Control Data
// "6" buffer slots

// clang-format off
// -----------------------NTSC VERSION----------------------------
// DRIVER REFRESH CYCLE : 60 HZ / 16.66ms
// TOTAL SLOTS: 6 (If we did IMA, slot count?)
// Bitrate | Buffer Size | Demand  | Slots | IMA Slots?
//  3840      768           192       1.2     1
//  5760      1536          256       1.6     1
//  7680      2560          320       2       1
//  11520     5376          448       3       2
//  15360     9216          576       4       3
//  23040     19968         832       6       6
//  (Total: 20KB)   (Total Budget: 960)
//
// ----------------------PAL VERSION------------------------------
// DRIVER REFRESH CYCLE : 50 HZ / 20ms
//                     (6 total)   (7 total)
// Bitrate | Buffer Size | Demand  | Slots | Adjusted slots?
//  6400      2560          320       2       2
//  9600      5376          448       3       3
//  12800     9216          576       4       3
//  19200     19968         832       6       5
// It really should be possible for PAL to have more ADX sounds.
// The driver has more time, and the bit-rates are lower.
// Though, the fixed buffer size of 20KB means that the slots cannot change without a PAL-specific driver being made.
// (Total: 20KB)   (Total Budget: 1152)
// clang-format on

short *adx_buf_addr[3] = {&adx_work_buf[0], &adx_work_buf[4608], &adx_work_buf[7924]};
short adx_buffer_used[6];

// This function is here, you know, just in case.
// int multiply(short data1, short data2) {
//   register volatile int out;
//   asm("mulsw %[data1],%[data2];"
//       "mov.l %[data2],%[out];"
//       : [out] "=X"(out), [data2] "=d"(data2) // OUT
//       : [data1] "d"(data1)                   // IN
//       :                                      // CLOBBERS
//   );
//   return out;
// }

// I am aware this clamps a little short on signed 16-bit integer.
inline int clamp_to_16bit(register int input) { return ((input > 32767) ? 32767 : (input < -32767) ? -32767 : input); }

void driver_data_init(void) {
  // I'm so bad at C it took me an hour to realize I had to typecast this
  // Assignment is for the SH2, so it adds the SNDRAM base uncached address.
  sh2Com->chnCtrl = (_CHN_CTRL *) ((unsigned int) &chnCtrlData[0] + 0x25A00000);

  // Set "start" to a specific number during start up to communicate the driver
  // is initializing.
  sh2Com->start = 0xFFFF;
  for (char i = 0; i < 32; i++) {
    ICSR_Busy[i] = -1;
    dataTimers[i] = 0;
    lfoTimers[i] = 0;

    // Soft-reset safety clear of sound slot information; contributed by fafling
    csr[i].keys = 0;
    csr[i].start_addr = 0;
    csr[i].loop_start = 0;
    csr[i].playsize = 0;
    csr[i].decay_1_2_attack = 0;
    csr[i].key_decay_release = 0;
    csr[i].attenuation = 0;
    csr[i].FM_data = 0;
    csr[i].oct_fns = 0;
    csr[i].lfo_data = 0;
    csr[i].input_sel = 0;
    csr[i].pan_send = 0;
  }

  // CD-DA Initialization
  // Slot 16 and 17 are **hard-wired** as the slots which play redbook audio.
  // They play redbook audio as "effect data".
  // Lock slot 16 & 17. These are for CDDA only.
  ICSR_Busy[16] = 1;
  ICSR_Busy[17] = 1;

  // Set max volume for CDDA, and the default pan information.
  sh2Com->cdda_left_channel_vol_pan = 0x1F | 224;
  sh2Com->cdda_right_channel_vol_pan = 0x0F | 224;

  for (short k = 0; k < CHN_CTRL_MAX; k++) {
    chnCtrlData[k].sh2_permit = 0;
    chnCtrlData[k].icsr_target = -1;
    loopingPCMs[k] = -1;
    volatilePCMs[k] = -1;
    adxPCMs[k] = -1;
  }

  /////////////////////////////////////////////////////
  // ADX Control Struct init
  /////////////////////////////////////////////////////
  for (short a = 0; a < ADX_CTRL_MAX; a++) {
    adx_dummy[a] = CHN_CTRL_MAX + a;
    adx[a].pcm_number = -1;
    adx[a].status = ADX_STATUS_NONE;
    chnCtrlData[adx_dummy[a]].ctrlType = 1; // fwd loop
    chnCtrlData[adx_dummy[a]].key_data = (unsigned short) ((unsigned int) &adx_work_buf[0] >> 16);
    chnCtrlData[adx_dummy[a]].start_addr = (unsigned short) ((unsigned int) &adx_work_buf[0] & 0xFFFF);
    chnCtrlData[adx_dummy[a]].volume = 7;
  }

  /////////////////////////////////////////////////////
  // ADX Decompression Table Initialization
  /////////////////////////////////////////////////////
  center_coef_1 = &coef_tbl_1[2048];
  center_coef_2 = &coef_tbl_2[2048];
  int last_shift = 0;

  // Filling coef table 1
  int coef_1 = sh2Com->drv_adx_coef_1;
  int coef_2 = sh2Com->drv_adx_coef_2;
  for (int a = 32767; a > -32768; a--) {
    int temp_value = (a * coef_1) >> 12;
    temp_value = clamp_to_16bit(temp_value);
    if (a >> 4 == last_shift)
      continue;
    center_coef_1[a >> 4] = (short) temp_value;
    last_shift = a >> 4;
  }
  // Filling coef table 2
  for (int a = 32767; a > -32768; a--) {
    int temp_value = (a * coef_2) >> 12;
    temp_value = clamp_to_16bit(temp_value);
    if (a >> 4 == last_shift)
      continue;
    center_coef_2[a >> 4] = (short) temp_value;
    last_shift = a >> 4;
  }

  // Load test effects data
  // SendLvl
  // MPY INREG00 SendLvl
  dsp->program[0] = 0x0000; 
  dsp->program[1] = 0xA800;
  dsp->program[2] = 0x0002;
  dsp->program[3] = 0x0200;
  // STR OUTREG
  dsp->program[4] = 0x0000;
  dsp->program[5] = 0x0000;
  dsp->program[6] = 0x1000;
  dsp->program[7] = 0x0000;

  // Set start to a specific number to communicate the driver has initialized
  sh2Com->start = 0x7777;
}

// NOTICE: To play the same sound struct multiple times per frame you have to
// copy its PCM_CTRL struct into another array member, and issue it to play.

#pragma GCC push_options
#pragma GCC optimize("O3")
void decompress_adx_frame(_ADX_CTRL *adx) {
  static short scale_table[16];
  static short last_scale = 0;

  // Retrieve the scale which is a big-endian value less than 0x2000
  short scale = *adx->src++;
  register short hist1 = adx->last_sample;
  register short hist2 = adx->last_last_sample;
  register int sample_raw;
  register short delta1;
  register short delta2;
  register short delta3;
  register short delta4;

  // Optimization: This reduces the most multiplications per 32 samples to 16,
  // halving the # of muls. Loop unrolling also means only one logical branch is
  // added. The effective cost saving per sample is at least 32 cycles, however
  // it is likely more, as the scale between frames is not guaranteed to vary.
  if (scale != last_scale) {
    scale_table[0] = nibble_to_int[0] * scale;
    scale_table[8] = nibble_to_int[8] * scale;
    scale_table[1] = nibble_to_int[1] * scale;
    scale_table[9] = nibble_to_int[9] * scale;
    scale_table[2] = nibble_to_int[2] * scale;
    scale_table[10] = nibble_to_int[10] * scale;
    scale_table[3] = nibble_to_int[3] * scale;
    scale_table[11] = nibble_to_int[11] * scale;
    scale_table[4] = nibble_to_int[4] * scale;
    scale_table[12] = nibble_to_int[12] * scale;
    scale_table[5] = nibble_to_int[5] * scale;
    scale_table[13] = nibble_to_int[13] * scale;
    scale_table[6] = nibble_to_int[6] * scale;
    scale_table[14] = nibble_to_int[14] * scale;
    scale_table[7] = nibble_to_int[7] * scale;
    scale_table[15] = nibble_to_int[15] * scale;
  }
  last_scale = scale;

  for (short i = 0; i < 8; i++) {
    // this contains four nibbles, four samples
    register short sample_short = *adx->src++;
    // Even samples use the high nibble.
    // Each nibble is a 4 bit two's complement signed value.
    // Due to big-endian data, the first nibbles come from the more significant
    // byte
    delta1 = scale_table[(sample_short >> 12) & 0xF];
    delta2 = scale_table[(sample_short >> 8) & 0xF];
    delta3 = scale_table[(sample_short >> 4) & 0xF];
    delta4 = scale_table[sample_short & 0xF];
    // We do not need to check the histories for their sign, as the tables
    // themselves are signed.

    // A special note for anyone who tries to pick up on this:
    // I am well aware that playing back ADX via this driver has a huge playback
    // issue; When the sound effect has samples which reach the peak or floor of
    // volume. In these cases, the output becomes crackly or garbled, and is
    // generally "blown out". I do know that even if you do not use the
    // coefficient multiplication tables, the issue persists! But if you play
    // the sounds back with WinAmp or back as an uncompressed sound, the issue
    // is not present. I suspect this is because either Windows or WinAmp has a
    // high-pass filter or limiter to prevent this from happening. Because, near
    // as I can tell, if you follow the algorithm it is present in the data
    // anyway. It could also be a simple issue relating to how the SCSP
    // interprets extreme samples (or sample deltas).

    ////////////////////////////////////////////////////////////////////////////////////////////
    sample_raw = clamp_to_16bit(center_coef_1[hist1] + center_coef_2[hist2] + delta1);
    hist2 = hist1;
    hist1 = sample_raw >> 4;
    *adx->dst++ = sample_raw;

    ////////////////////////////////////////////////////////////////////////////////////////////
    sample_raw = clamp_to_16bit(center_coef_1[hist1] + center_coef_2[hist2] + delta2);
    hist2 = hist1;
    hist1 = sample_raw >> 4;
    *adx->dst++ = sample_raw;

    ////////////////////////////////////////////////////////////////////////////////////////////
    sample_raw = clamp_to_16bit(center_coef_1[hist1] + center_coef_2[hist2] + delta3);
    hist2 = hist1;
    hist1 = sample_raw >> 4;
    *adx->dst++ = sample_raw;

    ////////////////////////////////////////////////////////////////////////////////////////////
    sample_raw = clamp_to_16bit(center_coef_1[hist1] + center_coef_2[hist2] + delta4);
    hist2 = hist1;
    hist1 = sample_raw >> 4;
    *adx->dst++ = sample_raw;
  }

  adx->last_sample = hist1;
  adx->last_last_sample = hist2;
  adx->test_area += 1;
}
#pragma GCC pop_options

void driver_end_sound(volatile char *cst, _CHN_CTRL *lctrl) {
  if (lctrl->melodic_data != 0) {
    if (csr[*cst].keys != 0){
      csr[*cst].decay_1_2_attack = 0;     // Melodic sounds need to be cut entirely
      csr[*cst].key_decay_release = 0x1F; // so release is set to its maximum before turning the note off
      csr[*cst].keys = 0;                 // Key select OFF 
      lctrl->sh2_permit = 0;
    }
  } else{
    csr[*cst].keys &= 0xF7FF;  // Key select OFF (while still preserving other parameters)
    //csr[*cst].keys |= 1 << 12; // Key EXECUTE OFF
    ICSR_Busy[*cst] = -1;      // Flag this ICSR as no longer busy
    dataTimers[*cst] = 0;      // Clear playback timer
    lctrl->sh2_permit = 0;
    lctrl->icsr_target = -1;
  }
}

void driver_stop_slot(volatile char *cst) {
  csr[*cst].keys &= 0xF7FF;        // Key select OFF
  //csr[*cst].keys |= 1 << 12; // Key EXECUTE OFF
  dataTimers[*cst] = 0;
  lfoTimers[*cst] = 0;
}

// This updates everything save for the keys and address registers (which have unique behavior for each control type)
// Note: key_bit should be left_shifted into its proper place before being passed into this function
void update_slot(volatile char cst, _CHN_CTRL* lctrl, short key_bit) {
    unsigned char lfo_bit;
    csr[cst].keys = key_bit | lctrl->key_data;
    csr[cst].start_addr = lctrl->start_addr;
    csr[cst].loop_start = lctrl->LSA;

    // "-1" to correct for the SCSP's pipeline. Helps that the linked library won't have to worry.
    csr[cst].playsize = lctrl->playsize - 1;

    csr[cst].oct_fns = lctrl->final_pitch;

    csr[cst].pan_send = lctrl->pan_send;
    csr[cst].attenuation = lctrl->attenuation;
    if ((lctrl->reset_bits & (1 << 1)) != 0) {
        lfo_bit = (lctrl->reset_bits >> 2) & 1;
    }
    else if ((lctrl->reset_bits & 1) != 0) {
        lfo_bit = 0;
    }
    else {
        lfo_bit = ((lfoTimers[cst] < (short)(lctrl->lfo_delay)) ? 1 : 0);
    }

    csr[cst].lfo_data = lctrl->lfo_data | (lfo_bit << 15);
    csr[cst].decay_1_2_attack = lctrl->decay_1_2_attack;
    csr[cst].key_decay_release = lctrl->key_decay_release;

    csr[cst].FM_data = lctrl->FM_data;
    csr[cst].input_sel = lctrl->input_sel;

    if (lfoTimers[cst] < (short)(lctrl->lfo_delay)) {
        lfoTimers[cst]++;
    }
    //lctrl->test_area = sh2Com->coefficients[1];
}

short find_free_slot(_CHN_CTRL *lctrl) {
  // Find an inactive channel for the sound
  while (ICSR_Busy[icsr_index] != -1) {
    // Set forward the icsr_index until it has reached an inactive ICSR
    icsr_index++;
  }

  // If there is no free ICSR, leave the function -- leaving the ICSR set to -1.
  if (icsr_index >= 32)
    return -1;

  // Give sound an ICSR index
  lctrl->icsr_target = icsr_index;
  return 1;
}

void driver_start_slot_with_sound(volatile char cst, _CHN_CTRL *lctrl) {
  lfoTimers[cst] = 0;
  // Key select ON | Bit depth | high bits of address | bit reversal settings
  update_slot(cst, lctrl, 1 << 11);
}

void prepare_volatile_sound(_CHN_CTRL* lctrl) {
    driver_stop_slot(&lctrl->icsr_target);
}

// This function is for non-looping sounds that will restart every time the
// driver detects sh2 permit. In other words, it is "volatile". This is a
// volatile playback function, meaning the sound is stopped and played back
// again from the start when this is triggered. The trigger is "sh2_permit", as
// written by the SH2, expectedly. To use this wisely, either limit sound
// trigger by a timer, or simply ensure the routine that has sound playback
// attached to it has some statute of limitations. Bonus: You could attach
// volatile playback to an sh2 function to *know* if its happening more than it
// should :)
void play_volatile_sound(_CHN_CTRL *lctrl) {
  driver_start_slot_with_sound(lctrl->icsr_target, lctrl);

  lctrl->sh2_permit = 0;    // Disallow sound
  lctrl->icsr_target = -1;  // Disassociate this command struct with any ICSR
}

void prepare_protected_sound(short index) {
    _CHN_CTRL* lctrl = &chnCtrlData[index];
    short cst = lctrl->icsr_target;
    // This ICSR is not presently associated with the protected sound, so we set
    // it up.
    if (ICSR_Busy[cst] != index) {
        driver_stop_slot(&lctrl->icsr_target);
    }
}

// This function is for playing non-looping sounds that will only restart once
// their entire length has played. In other words, they are "protected".
void play_protected_sound(short index) {
  _CHN_CTRL *lctrl = &chnCtrlData[index];
  short cst = lctrl->icsr_target;
  // This ICSR is not presently associated with the protected sound, so we set
  // it up.

  if (ICSR_Busy[cst] != index) {
    driver_start_slot_with_sound(lctrl->icsr_target, lctrl);

  } else {
      // Allow live volume and pitch adjustment of semi protected sounds
      update_slot(cst, lctrl, csr[cst].keys & (1 << 11));
  }

  if (dataTimers[cst] >= (lctrl->playsize << (1 - ((csr[cst].keys >> 4) & 1)))) // 1 - (bitDepth) is an expression that will or will
                                                                                // not multiply the playsize by 2,
  { // depending on if it is 8 bits per sample or 16 bits per sample. Remember,
    driver_end_sound(&lctrl->icsr_target,
      lctrl); // if the bits per sample is 16, the playsize >>=1 its data size.
  } else {
    ICSR_Busy[cst] = index; // Associate this ICSR with this protected sound.
    dataTimers[cst] += lctrl->bytes_per_blank;
  }
}

void prepare_semi_protected_sound(short index) {
    _CHN_CTRL* lctrl = &chnCtrlData[index];
    if (lctrl->sh2_permit == 1) {
        driver_stop_slot(&lctrl->icsr_target);
    }
}

// This function is for playing non-looping sounds that will restart whenever
// the SH2 commands them to, however, the duration of playback locks an ICSR
// from other sounds.
void play_semi_protected_sound(short index) {
  _CHN_CTRL *lctrl = &chnCtrlData[index];
  short cst = lctrl->icsr_target;
  if (lctrl->sh2_permit == 1) {
    driver_start_slot_with_sound(lctrl->icsr_target, lctrl);

    // With semi-protected sound, the SH2 can command the sound to re-start.
    // To facilitate this, the permission boolean is set to 0 in antipication of
    // the SH2 potentially permitting another start.
    lctrl->sh2_permit = 0;
  } else {
    // Allow live volume and pitch adjustment of semi protected sounds
    update_slot(cst, lctrl, csr[cst].keys & (1 << 11));
  }

  if (dataTimers[cst] >= (lctrl->playsize << (1 - ((csr[cst].keys >> 4) & 1)))) // 1 - (bitDepth) is an expression that will or will
                                                                                // not multiply the playsize by 2,
  { // depending on if it is 8 bits per sample or 16 bits per sample. Remember,
    // if the bits per sample is 16, the playsize >>=1 its data size.
    driver_end_sound(&lctrl->icsr_target, lctrl); 
  } else {
    ICSR_Busy[cst] = index; // Associate this ICSR with this protected sound.
    dataTimers[cst] += lctrl->bytes_per_blank;
  }
}

void prepare_looping_sound(short index) {
    _CHN_CTRL* lctrl = &chnCtrlData[index];
    short cst = lctrl->icsr_target;

    // This ICSR is not presently associated with the looping sound, so we
    // decimate its current playback.
    if (ICSR_Busy[cst] != index) {
        // Key select OFF
        csr[cst].keys = 0;

        dataTimers[cst] = 0;
        lfoTimers[cst] = 0;
    }
}

// This function is for setting up or updating a sound with looping playback
void set_looping_sound(short index) {
  _CHN_CTRL *lctrl = &chnCtrlData[index];
  short cst = lctrl->icsr_target;

  // Then, set up the sound.
  // Key select ON | loop type | Bit depth | high bits of address | bit reversal settings
  update_slot(cst, lctrl, 1 << 11);

  // Associate this ICSR with the looping sound
  ICSR_Busy[cst] = index;

  dataTimers[cst] = 1;
}

// Activates key off when a new note is detected 
void prepare_melodic_sound(short index) {
    _CHN_CTRL* lctrl = &chnCtrlData[index];
    short cst = lctrl->icsr_target;
    short should_key_on = lctrl->melodic_data & 0x40;

    // Prepare the channel for a new note on
    if (should_key_on != 0) {
        // Key select OFF (while still preserving other parameters)
        csr[cst].keys &= 0xF7FF;

        dataTimers[cst] = 0;
        lfoTimers[cst] = 0;
    }

    
    if ((lctrl->reset_bits & (1 << 4)) != 0) {
        if (((lctrl->reset_bits & (1 << 5)) != 0)) {
            // Key select OFF (while still preserving other parameters)
            csr[cst].keys &= 0xF7FF;
        }
    }
}

// This function is for setting up or updating a melodic sound
// Melodic sounds can have any looping method
// and can have their parameters changed even after keyoff has been written
void set_melodic_sound(short index) {
    _CHN_CTRL* lctrl = &chnCtrlData[index];
    short cst = lctrl->icsr_target;
    short should_key_on = lctrl->melodic_data & 0x40;
    short should_key_off = lctrl->melodic_data & 0x80;
    short key_bit;
    if ((lctrl->reset_bits & (1 << 4)) != 0) {
        if (((lctrl->reset_bits & (1 << 5)) != 0) || ((lctrl->reset_bits & (1 << 6)) != 0)) {
            key_bit = (((lctrl->reset_bits & (1 << 6)) != 0)) ? (1 << 11) : 0;
        }
        else {
            key_bit = csr[cst].keys & (1 << 11);
        }
    }
    else {
        key_bit = ((should_key_on != 0) ? (1 << 11) : ((should_key_off != 0) ? 0 : ((csr[cst].keys & (1 << 11)))));
    }
    // Set up the sound.
    // Key select ON | loop type | Bit depth | high bits of address | bit reversal settings
    update_slot(cst, lctrl, key_bit);

    // Associate this ICSR with the sound
    ICSR_Busy[cst] = index;

    dataTimers[cst] = 1;
}

void play_adx(short pcm_control_index, short loop_type) {
  _CHN_CTRL *snd = &chnCtrlData[pcm_control_index];
  unsigned short hiAddrBits = snd->key_data & 0xF;
  unsigned short loAddrBits = snd->start_addr;
  short target_adx = -1;
  short buffers_empty = 0;
  // sh2Com->adx_stream_comm = adx[0].pcm_number;
  // sh2Com->drv_adx_coef_1 = adx[1].pcm_number;
  // sh2Com->drv_adx_coef_2 = adx[2].pcm_number;
  ///////////////////////////////////////
  // ADX Condition Manager Selection

  for (short f = 0; f < ADX_CTRL_MAX; f++) {
    snd->test_area = 0;
    if (adx[f].pcm_number == -1 && adx[f].status == ADX_STATUS_NONE && snd->icsr_target == -1 && snd->sh2_permit == 1) {
      // Try to find a free slot to play the sound in.
      // If none is found, exit this function.
      if (find_free_slot(snd) < 0)
        return;
       
      target_adx = f;
      adx[f].pcm_number = pcm_control_index;
      adx[f].status = ADX_STATUS_ACTIVE;
      if (snd->bytes_per_blank == 256 || snd->bytes_per_blank == 192 || snd->bytes_per_blank == 128) {
        // Find two free slots
        for (short l = 0; l < 6; l++) {
          if (adx_buffer_used[l] == 0) {
            buffers_empty++;
          }
          if (buffers_empty >= 2)
            break;
        }
        if (!adx_buffer_used[5] && !adx_buffer_used[4]) {
          adx_buffer_used[4] = 1;
          adx_buffer_used[5] = 1;
          adx[f].original_dst = adx_buf_addr[2];
          adx[f].buf_string[0] = 4;
          adx[f].buf_string[1] = 5;
        } else if (!adx_buffer_used[3] && !adx_buffer_used[2]) {
          adx_buffer_used[2] = 1;
          adx_buffer_used[3] = 1;
          adx[f].original_dst = adx_buf_addr[1];
          adx[f].buf_string[0] = 2;
          adx[f].buf_string[1] = 3;
        } else if (!adx_buffer_used[0] && !adx_buffer_used[1]) {
          adx_buffer_used[0] = 1;
          adx_buffer_used[1] = 1;
          adx[f].original_dst = adx_buf_addr[0];
          adx[f].buf_string[0] = 0;
          adx[f].buf_string[1] = 1;
        } else {
          adx[f].pcm_number = -1;
          adx[f].status = ADX_STATUS_NONE;
          driver_end_sound(&snd->icsr_target, snd);
          return;
        }
        adx[f].whippet_point = (snd->bytes_per_blank == 256) ? 350 : (snd->bytes_per_blank == 192) ? 450 : 900;
      } else if (snd->bytes_per_blank == 384) {
        // Find three free slots
        for (short l = 0; l < 6; l++) {
          if (adx_buffer_used[l] == 0) {
            buffers_empty++;
          }
          if (buffers_empty >= 3)
            break;
        }
        if (!adx_buffer_used[5] && !adx_buffer_used[4] && !adx_buffer_used[3]) {
          adx_buffer_used[3] = 1;
          adx_buffer_used[4] = 1;
          adx_buffer_used[5] = 1;
          adx[f].original_dst = adx_buf_addr[1];
          adx[f].buf_string[0] = 3;
          adx[f].buf_string[1] = 5;
        } else if (!adx_buffer_used[2] && !adx_buffer_used[1] && !adx_buffer_used[0]) {
          adx_buffer_used[0] = 1;
          adx_buffer_used[1] = 1;
          adx_buffer_used[2] = 1;
          adx[f].original_dst = adx_buf_addr[0];
          adx[f].buf_string[0] = 0;
          adx[f].buf_string[1] = 2;
        } else {
          adx[f].pcm_number = -1;
          adx[f].status = ADX_STATUS_NONE;
          driver_end_sound(&snd->icsr_target, snd);
          return;
        }
        adx[f].whippet_point = snd->bytes_per_blank;
      } else if (snd->bytes_per_blank == 512) {
        // Find four free slots
        for (short l = 0; l < 6; l++) {
          if (adx_buffer_used[l] == 0) {
            buffers_empty++;
          }
          if (buffers_empty >= 4)
            break;
        }
        if (!adx_buffer_used[5] && !adx_buffer_used[4] && !adx_buffer_used[3] && !adx_buffer_used[2]) {
          adx_buffer_used[2] = 1;
          adx_buffer_used[3] = 1;
          adx_buffer_used[4] = 1;
          adx_buffer_used[5] = 1;
          adx[f].original_dst = adx_buf_addr[1];
          adx[f].buf_string[0] = 2;
          adx[f].buf_string[1] = 5;
        } else if (!adx_buffer_used[3] && !adx_buffer_used[2] && !adx_buffer_used[1] && !adx_buffer_used[0]) {
          adx_buffer_used[0] = 1;
          adx_buffer_used[1] = 1;
          adx_buffer_used[2] = 1;
          adx_buffer_used[3] = 1;
          adx[f].original_dst = adx_buf_addr[0];
          adx[f].buf_string[0] = 0;
          adx[f].buf_string[1] = 3;
        } else {
          adx[f].pcm_number = -1;
          adx[f].status = ADX_STATUS_NONE;
          driver_end_sound(&snd->icsr_target, snd);
          return;
        }
        adx[f].whippet_point = snd->bytes_per_blank;
      } else if (snd->bytes_per_blank == 768) {
        // Only work if no slots are used

        for (short l = 0; l < 6; l++) {
          if (adx_buffer_used[l] == 0) {
            buffers_empty++;
          }
          if (buffers_empty >= 6)
            break;
        }
        if (buffers_empty != 6) {
          adx[f].pcm_number = -1;
          adx[f].status = ADX_STATUS_NONE;
          driver_end_sound(&snd->icsr_target, snd);
          return;
        } else {
          adx_buffer_used[0] = 1;
          adx_buffer_used[1] = 1;
          adx_buffer_used[2] = 1;
          adx_buffer_used[3] = 1;
          adx_buffer_used[4] = 1;
          adx_buffer_used[5] = 1;
          adx[f].original_dst = adx_buf_addr[0];
          adx[f].buf_string[0] = 0;
          adx[f].buf_string[1] = 5;
        }
        adx[f].whippet_point = snd->bytes_per_blank;
      } else {
        // Invalid byte-rate
        adx[f].pcm_number = -1;
        adx[f].status = ADX_STATUS_NONE;
        driver_end_sound(&snd->icsr_target, snd);
        return;
      }
      break;
    } else if (adx[f].pcm_number == pcm_control_index && (adx[f].status & ADX_STATUS_ACTIVE)) {
      target_adx = f;
      break;
    }
  }
  // If the above loops did not find an available ADX slot, this value will be
  // -1. We should not continue in this case.
  if (target_adx == -1)
    return;
  ///////////////////////////////////////
  // ADX Play-type Condition Management
  if (snd->sh2_permit == 1) {
    snd->test_area += 1;
    if (loop_type == PCM_SEMI || (loop_type == PCM_PROTECTED && adx[target_adx].status == ADX_STATUS_ACTIVE)) {
      // sh2Com->adx_stream_comm = ('A' | ('S'<<8));
      if (loop_type == PCM_SEMI)
        snd->sh2_permit = 0;
      adx[target_adx].status = (ADX_STATUS_START | ADX_STATUS_ACTIVE);
      // Decompression demand is the # of bytes to decompress every 1/60 seconds
      // (60hz).
      //  + 64 must be added for margin ahead of the SCSP's playback. It is the
      //  size of 1 ADX frame.
      adx[target_adx].decomp_demand = snd->bytes_per_blank + 64;
      // Decompression space is the size of the playback buffer, in bytes.
      adx[target_adx].decomp_space = snd->decompression_size;
      // Recharge point is the point at which decompression will start.
      // It is compared against a value to which the bytes per blank is added
      // every 1/60 seconds.
      adx[target_adx].recharge_point = snd->decompression_size >> 1;
      adx[target_adx].current_frame = 0;
      adx[target_adx].original_src = (unsigned short *) ((int) ((hiAddrBits << 16) | (loAddrBits)));
      adx[target_adx].src = (unsigned short *) ((int) ((hiAddrBits << 16) | (loAddrBits)));
      adx[target_adx].dst = adx[target_adx].original_dst;
      adx[target_adx].work_decomp_pt = 0;
      adx[target_adx].work_play_pt = 0;
      adx[target_adx].last_sample = 0;
      adx[target_adx].last_last_sample = 0;
      adx[target_adx].passed_buffers = 0;
      sh2Com->adx_buffer_pass[0] = 0;
      sh2Com->adx_buffer_pass[1] = 0;
    } else if ((loop_type == PCM_FWD_LOOP || loop_type == ADX_STREAM) && adx[target_adx].status == ADX_STATUS_ACTIVE) {
      adx[target_adx].status = (ADX_STATUS_START | ADX_STATUS_ACTIVE);
      // Decompression demand is the # of bytes to decompress every 1/60 seconds
      // (60hz).
      //  + 64 must be added for margin ahead of the SCSP's playback. It is the
      //  size of 1 ADX frame.
      adx[target_adx].decomp_demand = snd->bytes_per_blank + 64;
      // Decompression space is the size of the playback buffer, in bytes.
      adx[target_adx].decomp_space = snd->decompression_size;
      // Recharge point is the point at which decompression will start.
      // It is compared against a value to which the bytes per blank is added
      // every 1/60 seconds.
      adx[target_adx].recharge_point = snd->decompression_size >> 1;
      adx[target_adx].current_frame = (loop_type == ADX_STREAM) ? 2 : 0;
      adx[target_adx].original_src = (unsigned short *) ((int) ((hiAddrBits << 16) | (loAddrBits)));
      adx[target_adx].src = (unsigned short *) ((int) ((hiAddrBits << 16) | (loAddrBits)));
      adx[target_adx].dst = adx[target_adx].original_dst;
      adx[target_adx].work_decomp_pt = 0;
      adx[target_adx].work_play_pt = 0;
      adx[target_adx].last_sample = 0;
      adx[target_adx].last_last_sample = 0;
      adx[target_adx].passed_buffers = 0;
      // For an ADX STREAM, flag the buffers as having been passed. This is
      // essentially marking them as empty. If the linked library sees the
      // buffers empty, it should copy in new data.
      if (loop_type == ADX_STREAM) {
        sh2Com->adx_buffer_pass[0] = 1;
        sh2Com->adx_buffer_pass[1] = 1;
      }
    }
  }
  // If this function attempted to run past this point to manage an ADX sound
  // without a valid ICSR, exit this function. There is no point in running it
  // then, as it won't play sound.
  if (snd->icsr_target == -1)
    return;
  // If this function attempted to run past this point with the ADX controller
  // status at "NONE", the attached PCM control data is in an error state.
  // Correct this error, and exit the function.
  if (adx[target_adx].status == ADX_STATUS_NONE) {
    adx[target_adx].pcm_number = -1;
    driver_end_sound(&snd->icsr_target, &chnCtrlData[adx_dummy[target_adx]]);
    return;
  }
  ///////////////////////////////////////
  // ADX Starting
  // When an ADX sound is started, first our decompression demand for this
  // refresh cycle is completed. The adx dummy recieves playback parameters from
  // the ADX PCM slot, including the ICSR to be used. This is used because we
  // need more data than we have in a single PCM slot to control a separate PCM
  // loop. The ADX dummy is set to point to the decompression buffer, and the
  // SCSP is set to play back 16-bit signed PCM from that region. On the next
  // refresh cycle, the driver will undergo decompression of more data, as the
  // buffer is not full.
  if (adx[target_adx].status & ADX_STATUS_START) {
    // sh2Com->adx_stream_comm = ('S' | ('T'<<8));
    // Decompress (decomp_demand) # of bytes to (dst)
    // Then start playing the sound
    for (short i = 0; i < adx[target_adx].decomp_demand;) {
      decompress_adx_frame(&adx[target_adx]);
      i += 64;
      adx[target_adx].current_frame += 1;
    }

    adx[target_adx].work_decomp_pt += adx[target_adx].decomp_demand;

    // ADX dummy 1 being a looping PCM control set to point at the adx work buf.
    chnCtrlData[adx_dummy[target_adx]].playsize = ((adx[target_adx].decomp_space) >> 1);
    chnCtrlData[adx_dummy[target_adx]].bytes_per_blank = snd->bytes_per_blank;
    chnCtrlData[adx_dummy[target_adx]].key_data = chnCtrlData[adx_dummy[target_adx]].key_data | (snd->key_data & 0xFFF0) | (1 << 5); // Mask out address bits and turn on forward looping
    chnCtrlData[adx_dummy[target_adx]].pan_send = snd->pan_send;
    chnCtrlData[adx_dummy[target_adx]].decay_1_2_attack = snd->decay_1_2_attack;
    chnCtrlData[adx_dummy[target_adx]].key_decay_release = snd->key_decay_release;
    chnCtrlData[adx_dummy[target_adx]].final_pitch = snd->final_pitch;
    chnCtrlData[adx_dummy[target_adx]].icsr_target = snd->icsr_target;
    chnCtrlData[adx_dummy[target_adx]].start_addr = (unsigned short)((unsigned int)adx[target_adx].original_dst);
    adx[target_adx].whippet_frame = 0;

    set_looping_sound(adx_dummy[target_adx]);
    adx[target_adx].status ^= ADX_STATUS_START;
    adx[target_adx].status |= (ADX_STATUS_PLAY | ADX_STATUS_DECOMP);
  } else if (adx[target_adx].status & ADX_STATUS_DECOMP && !(adx[target_adx].status & ADX_STATUS_FULL)) {
      snd->test_area += 0x10;
    ///////////////////////////////////////
    // ADX Real-time Decompression
    // Very simple - an amount of ADX frames is decompressed every refresh
    // cycle. An ADX "frame" is 18 bytes which contains 64 4-bit nibbles and 1
    // 16-bit scale value. It should be noted, if it wasn't clear already, that
    // your sound's byterate must be divisible by 64. The amount of frames
    // decompressed is the 60hz byterate of the raw PCM frequency, divided by
    // 64, plus 1.
    for (unsigned short i = 0; i < adx[target_adx].decomp_demand;) {
      ///////////////////////////////////////
      // ADX Decompression Completion Condition
      // The playsize of the ADX PCM control slot contains the # of ADX frames
      // of the sound. If the current frame being decompressed is greater than
      // this, the sound has been fully decompressed. The status is updated as
      // such, and no more ADX frames are to be decompressed (there are no
      // more!).
      if (adx[target_adx].current_frame >= snd->playsize && loop_type != ADX_STREAM) {
        adx[target_adx].status |= ADX_STATUS_FULL;
        break;
      } else if (loop_type == ADX_STREAM) {
        int frames_from_segment = (ADX_STREAM_BUFFERED_FRAME_CT * (adx[target_adx].passed_buffers + 1));
        if (adx[target_adx].current_frame == frames_from_segment) {
          sh2Com->adx_buffer_pass[1] = 1; // Communicate to stream manager software on SH2 that segment 1
                                          // (loop point) has passed.
          adx[target_adx].passed_buffers++;
          adx[target_adx].src = adx[target_adx].original_src;
        } else if (adx[target_adx].current_frame == (frames_from_segment - (ADX_STREAM_BUFFERED_FRAME_CT >> 1))) {
          sh2Com->adx_buffer_pass[0] = 1; // Communicate to stream manager software on SH2 that segment 0
                                          // (half-way point) is passed.
        }

        if (adx[target_adx].current_frame >= sh2Com->adx_stream_length) {
          adx[target_adx].status |= ADX_STATUS_FULL;
          break;
        }
      }

      decompress_adx_frame(&adx[target_adx]);
      i += 64;
      adx[target_adx].current_frame += 1;
    }
    adx[target_adx].work_decomp_pt += adx[target_adx].decomp_demand;
  }

  if (adx[target_adx].status & ADX_STATUS_PLAY) {
      snd->test_area += 2;
    ////////////////////////////////////////
    // The Whippet
    // Honestly, this is an imprecise piece of the equation.
    // This is needed because the SCSP consumes data at a rate faster than
    // bytes_per_blank. It does that because the SCSP's clock rate is
    // effectively tuned to 44.1KHz audio. The rates we use to attempt to get
    // perfect numbers of bytes per vertical blank don't equate perfectly to the
    // SCSP's clock rate. However, I could not manage to get a correct
    // calculation for the actual rate at which the SCSP will consume data. We
    // still need to compensate for this flaw that our code assumes perfect
    // bytes consumed per blank. I genuinely can't get sensible whippet
    // calculations.

    // 3840 Hz      ~ 895
    // 5760 Hz      ~ 450
    // 7680 Hz      ~ 350
    // 11520 Hz     ~ Bytes per blank
    // 15360 Hz     ~ Bytes per blank
    // 23040 Hz     ~ Bytes per blank
    //  Why at 11520, 15360, and 23040 is bytes per blank the perfect offset,
    // yet below that it is wildly off?
    adx[target_adx].whippet_frame += 1;
    if (adx[target_adx].whippet_frame >= adx[target_adx].whippet_point) {
      adx[target_adx].work_play_pt += snd->bytes_per_blank;
      adx[target_adx].whippet_frame = 0;
    }

    // sh2Com->drv_adx_coef_1 = adx[target_adx].whippet_frame;
    adx[target_adx].work_play_pt += snd->bytes_per_blank;

    // ADX Decompression Starting
    // If the play pointer has met or exceeded the recharge point,
    // this is the time we have have calculated as the appropriate time to begin
    // decompression into the buffer. We do start this now to maintain
    // synchronization: It will finish before the SCSP gets back here after it
    // loops, and it will be done after the SCSP has looped.
    if (adx[target_adx].work_play_pt >= adx[target_adx].recharge_point && !(adx[target_adx].status & ADX_STATUS_FULL)) {
        snd->test_area += 8;
      adx[target_adx].status |= ADX_STATUS_DECOMP;
    }

    // ADX Decompression Ending
    // If we have decompressed enough data to fill up the decompression buffer,
    // cease decompression and re-set the decompression target tracker and
    // pointer.
    if (adx[target_adx].work_decomp_pt >= adx[target_adx].decomp_space) {
        snd->test_area += 16;
      adx[target_adx].status ^= ADX_STATUS_DECOMP;
      adx[target_adx].work_decomp_pt -= snd->decompression_size;
      adx[target_adx].dst = adx[target_adx].original_dst;
    }

    // ADX End Condition
    // When we have decompressed all the data from the ADX sound,
    // we have flagged the ADX status as FULL. The sound has been fully
    // decompressed. At this point, we shouldn't immediately end the sound. The
    // decompression point is ahead of the playback point. Thus, we want to end
    // the sound only when the play point meets or passes the decompression
    // point.
    if (adx[target_adx].work_play_pt >= adx[target_adx].work_decomp_pt && adx[target_adx].status & ADX_STATUS_FULL) {
      if (loop_type == PCM_FWD_LOOP) {
        // To set an ADX sound to loop, just.. restart it!
        // But you *do* have to end the sound slot, and there is a blanking
        // period involved.
        adx[target_adx].status = (ADX_STATUS_START | ADX_STATUS_ACTIVE);
        adx[target_adx].current_frame = 0;
        adx[target_adx].src = (unsigned short *) ((int) ((hiAddrBits << 16) | (loAddrBits)));
        adx[target_adx].dst = adx[target_adx].original_dst;
        adx[target_adx].work_decomp_pt = 0;
        adx[target_adx].work_play_pt = 0;
        driver_end_sound(&snd->icsr_target, &chnCtrlData[adx_dummy[target_adx]]);
      } else {
        adx[target_adx].status = ADX_STATUS_END;
      }
    }

    // ADX Play Tracking
    // If the maximal position of the SCSP in the playback buffer will have met or exceeded the size of it, we **know**
    // the SCSP is going to go back to the beginning of the buffer. We know this because the SCSP has been set to play
    // back this region of sound as a loop. This also must be after we check the end condition for a sound, because if
    // it is not, the sound will loop into old data.
    if (adx[target_adx].work_play_pt >= adx[target_adx].decomp_space) {
        snd->test_area += 64;
      adx[target_adx].work_play_pt -= snd->decompression_size;
    }

    // ADX Parameter / Volume Update
    // Do this here for playing sounds. We should not need to update the ADX Dummy with this information here, since the
    // next time it is used, it will update.
    csr[snd->icsr_target].pan_send = snd->pan_send;
  }

  // ADX Stop Condition
  // This is controlled by conditions above, and exactly what gets us here changes. Control data from the ADX dummy, ADX
  // PCM control, and ADX control struct is cleared. In all cases, it is acceptable to indicate that the SH2 no longer
  // permits the sound as looping conditions happen earlier. However, it's not always needed. This is the control area
  // which stops sounds which must be prematurely stopped (in case of being commanded to stop).
  if (adx[target_adx].status & ADX_STATUS_END || loop_type == ADX_STATUS_NONE ||
    ((loop_type != PCM_SEMI) && snd->sh2_permit == 0)) {
    snd->sh2_permit = 0;
    for (short s = adx[target_adx].buf_string[0]; s <= adx[target_adx].buf_string[1]; s++) {
      adx_buffer_used[s] = 0;
    }

    // When an ADX stream ends, the appropriate logic is to flag both buffers as
    // having been passed. This is just to cover logical flaws in the linked
    // library.
    if (loop_type == ADX_STREAM) {
      sh2Com->adx_buffer_pass[0] = 1;
      sh2Com->adx_buffer_pass[1] = 1;
    }

    adx[target_adx].status = ADX_STATUS_NONE;
    adx[target_adx].pcm_number = -1;
    adx[target_adx].passed_buffers = 0;
    driver_end_sound(&snd->icsr_target, &chnCtrlData[adx_dummy[target_adx]]);
    driver_end_sound(&snd->icsr_target, snd);
  }
  chnCtrlData[0].test_area = (snd->key_data >> 5) & 0x3;
}

void pcm_control_loop(void) {
  icsr_index = 0;
  _CHN_CTRL *lctrl;
  
  short loopingIndex = 0;
  short adxIndex = 0;
  short volatileIndex = 0;

  // Write each set of effects data to the DSP
  for (short i = 0; i < NUM_COEFFICIENTS; i++) {
      dsp->coef[i + 1] = sh2Com->coefficients[i];
  }
  for (short i = 0; i < NUM_ADDRESSES; i++) {
      dsp->madrs[i] = sh2Com->addresses[i];
  }

  // TODO: A new control loop for multiple ADX sounds is required.

  // Build List of each type of basic control method
  for (short k = 0; k < CHN_CTRL_MAX; k++) {
    lctrl = &chnCtrlData[k];
    if (lctrl->decompression_size != 0) {
      adxPCMs[adxIndex] = k;
      adxIndex++;
    }
    else if (lctrl->ctrlType != PCM_VOLATILE || lctrl->melodic_data != 0) {
      loopingPCMs[loopingIndex] = k;
      loopingIndex++;
    }
    else if (lctrl->ctrlType == PCM_VOLATILE) {
      volatilePCMs[volatileIndex] = k;
      volatileIndex++;
    }
  }
  // ADX Control Loop
  // Moving this before the samples fixes a timing bug
  for (short a = 0; a < adxIndex; a++) {
      lctrl = &chnCtrlData[adxPCMs[a]];
      play_adx(adxPCMs[a], lctrl->ctrlType);
  }
  csr[0].keys |= 1 << 12; // Keyon everything
  // Control loop for new note-ons
  for (short l = 0; l < loopingIndex; l++) {
      lctrl = &chnCtrlData[loopingPCMs[l]];

      if (lctrl->melodic_data != 0) {
          ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
          // MELODIC CONTROL SEGMENT
          // Melodic samples are permanently linked to a particular sound slot (for now), even when they're not playing,
          // allowing them to be updated at all times
          ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
          lctrl->icsr_target = (lctrl->melodic_data & 0x3F) - 1;
          
          ICSR_Busy[lctrl->icsr_target] = 1;
          if (lctrl->sh2_permit == 1) {
              prepare_melodic_sound(loopingPCMs[l]);
          } 
      }
      else if (lctrl->ctrlType > 0) {
          ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
          //  PCM LOOP CONTROL SEGMENT
          //  PCM loop start and end conditions are based on SH2. Write 1 to start the loop and 0 to end it.
          ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
          // If the loop is allowed by SH2, either start it, or update its parameters.
          if (lctrl->sh2_permit == 1) {
              // The loop is not associated with an ICSR which _should_ mean it is not playing.
              if (lctrl->icsr_target == -1) {
                  if (find_free_slot(lctrl) < 0) {
                      break;
                  }
                  prepare_looping_sound(loopingPCMs[l]);
              }
              else {
                  // Update the sound
                  prepare_looping_sound(loopingPCMs[l]);
              }
          }
      }
      else if (lctrl->ctrlType == PCM_PROTECTED && lctrl->sh2_permit == 1) {
          /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
          // PROTECTED SOUND CONTROL SEGMENT
          // Protected sounds are expected to manage permission end by themselves. In this case, the SH2 is never to write
          // 0 to SH2 Permit to stop a protected sound. To prematurely stop a protected sound, set its volume to zero and
          // let the timer conclude.
          /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

          // If first run (no current ICSR)
          if (lctrl->icsr_target == -1) {
              // Attempt to grab a free ICSR. If there is none, function will return
              // -1, and thus we will stop.
              if (find_free_slot(lctrl) < 0)
                  break;
              prepare_protected_sound(loopingPCMs[l]);
          }
          else {
              // Update the sound
              prepare_protected_sound(loopingPCMs[l]);
          }
      }
      else if (lctrl->ctrlType == PCM_SEMI) {
          /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
          // SEMI-PROTECTED SOUND CONTROL SEGMENT
          // Semi-protected sounds manage the SH2 permit parameter but will still run the timer to the release of the ICSR
          // if they are not permitted. Permission 0 only disallows updates once the sound has started. Because of this,
          // writing SH2 Permit to 0 on a semi-protected sound results in undefined behaviour. To stop a semi-protected
          // sound, change the volume to 0 and let the timer conclude.
          /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

          // Sound does not currently occupy an ICSR
          if (lctrl->icsr_target == -1 && lctrl->sh2_permit == 1) {
              // Attempt to grab a free ICSR. If there is none, function will return -1, and thus we will stop.
              if (find_free_slot(lctrl) < 0)
                  break;
              prepare_semi_protected_sound(loopingPCMs[l]);
          }
          else if (lctrl->icsr_target != -1) {
              // Sound has an ICSR, update the sound
              prepare_semi_protected_sound(loopingPCMs[l]);
          }
      }
  } // PCM Control Loop End

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // VOLATILE SOUND CONTROL SEGMENT
  // Volatile sounds are essentially uncontrolled. They immediately write the SH2 no longer permits the sound after the
  // first blank of playback. But they will still play to their conclusion, provided no other sound decides to occupy
  // their ICSR.
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  for (short v = 0; v < volatileIndex; v++) {
      lctrl = &chnCtrlData[volatilePCMs[v]];
      if (lctrl->sh2_permit == 1) {
          // Attempt to grab a free ICSR. If there is none, function will return -1,
          // and thus we will stop.
          if (find_free_slot(lctrl) < 0)
              break;
          prepare_volatile_sound(lctrl);
      }
  }

  csr[0].keys |= 1 << 12; // Keyon everything

  // Control loop for looping, protected or melodic sounds
  for (short l = 0; l < loopingIndex; l++) {
    lctrl = &chnCtrlData[loopingPCMs[l]];
    
    if (lctrl->melodic_data != 0) {
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // MELODIC CONTROL SEGMENT
      // Melodic samples are permanently linked to a particular sound slot (for now), even when they're not playing,
      // allowing them to be updated at all times
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        lctrl->icsr_target = (lctrl->melodic_data & 0x3F) - 1;
        ICSR_Busy[lctrl->icsr_target] = 1;
        if (lctrl->sh2_permit == 1) {
            set_melodic_sound(loopingPCMs[l]);
        }
        else {
            // If the sound is not allowed by the SH2, turn it off.
            driver_end_sound(&lctrl->icsr_target, lctrl);
        }
        lctrl->melodic_data &= 0x3F; // Remove note on/off flags
    } else if (lctrl->ctrlType > 0) {
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      //  PCM LOOP CONTROL SEGMENT
      //  PCM loop start and end conditions are based on SH2. Write 1 to start the loop and 0 to end it.
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // If the loop is allowed by SH2, either start it, or update its parameters.
      if (lctrl->sh2_permit == 1) {
        // The loop is not associated with an ICSR which _should_ mean it is not playing.
        if (lctrl->icsr_target == -1) {
          if (find_free_slot(lctrl) < 0) {
            break;
          }
          set_looping_sound(loopingPCMs[l]);
        } else {
          // Update the sound
          set_looping_sound(loopingPCMs[l]);
        }
      } else if (lctrl->icsr_target != -1) {
        // If the loop is not allowed by the SH2, and it is associated with an ICSR, it is active. Turn it off.
        driver_end_sound(&lctrl->icsr_target, lctrl);
      }
    } else if (lctrl->ctrlType == PCM_PROTECTED && lctrl->sh2_permit == 1) {
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // PROTECTED SOUND CONTROL SEGMENT
      // Protected sounds are expected to manage permission end by themselves. In this case, the SH2 is never to write
      // 0 to SH2 Permit to stop a protected sound. To prematurely stop a protected sound, set its volume to zero and
      // let the timer conclude.
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

      // If first run (no current ICSR)
      if (lctrl->icsr_target == -1) {
        // Attempt to grab a free ICSR. If there is none, function will return
        // -1, and thus we will stop.
        if (find_free_slot(lctrl) < 0)
          break;
        play_protected_sound(loopingPCMs[l]);
      } else {
        // Update the sound
        play_protected_sound(loopingPCMs[l]);
      }
    } else if (lctrl->ctrlType == PCM_SEMI) {
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      // SEMI-PROTECTED SOUND CONTROL SEGMENT
      // Semi-protected sounds manage the SH2 permit parameter but will still run the timer to the release of the ICSR
      // if they are not permitted. Permission 0 only disallows updates once the sound has started. Because of this,
      // writing SH2 Permit to 0 on a semi-protected sound results in undefined behaviour. To stop a semi-protected
      // sound, change the volume to 0 and let the timer conclude.
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      
      // Sound does not currently occupy an ICSR
      if (lctrl->icsr_target == -1 && lctrl->sh2_permit == 1) {
        // Attempt to grab a free ICSR. If there is none, function will return -1, and thus we will stop.
        if (find_free_slot(lctrl) < 0)
          break;
        play_semi_protected_sound(loopingPCMs[l]);
      } else if (lctrl->icsr_target != -1) {
        // Sound has an ICSR, update the sound
        play_semi_protected_sound(loopingPCMs[l]);
      }
    }
  } // PCM Control Loop End

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // VOLATILE SOUND CONTROL SEGMENT
  // Volatile sounds are essentially uncontrolled. They immediately write the SH2 no longer permits the sound after the
  // first blank of playback. But they will still play to their conclusion, provided no other sound decides to occupy
  // their ICSR.
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  for (short v = 0; v < volatileIndex; v++) {
      lctrl = &chnCtrlData[volatilePCMs[v]];
      if (lctrl->sh2_permit == 1) {
          // Attempt to grab a free ICSR. If there is none, function will return -1,
          // and thus we will stop.
          if (find_free_slot(lctrl) < 0)
              break;
          play_volatile_sound(lctrl);
      }
  }

  csr[0].keys |= 1 << 12; // Keyon everything
}

void _start(void) {
  static short new_volume = 0;
  static short old_volume = 0;
  driver_data_init();

  while (1) {
    while (sh2Com->start != 1) {
      __asm__ volatile("nop");
    }

    sh2Com->start = 0;

    // Region will run the program once for every time the SH2 commands the
    // driver to start.
    pcm_control_loop();

    // Update CDDA volume
    // Note that SCSP Slot 16 & 17 are hard-wired as the redbook / CDDA playback
    // slots. They play the sound back as effect data. The pan_send bits set the
    // volume, and the pan, for each channel. The process here **must** be
    // followed for changing CD-DA volume as well as for initializing it.
    // ------------- ----------First: Check if we need to update the information
    // at all. We find that out by doing a process to see if the volume
    // information has changed. If the old volume information is not the same as
    // the current volume information, update the sound slots.
    new_volume = sh2Com->cdda_left_channel_vol_pan + sh2Com->cdda_right_channel_vol_pan;
    if (old_volume != new_volume) {
      csr[16].keys = 0x1000;
      csr[16].attenuation = 0xFF;
      csr[16].pan_send = sh2Com->cdda_left_channel_vol_pan;
      csr[17].keys = 0x1000;
      csr[17].attenuation = 0xFF;
      csr[17].pan_send = sh2Com->cdda_right_channel_vol_pan;
    }

    old_volume = new_volume;
  }
}
