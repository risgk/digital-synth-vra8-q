

A4_PITCH = 440.0

MIDI_CH             = 0
SAMPLING_RATE       = 31250
FREQUENCY_MAX       = 15000
BIT_DEPTH           = 8
NOTE_NUMBER_MIN     = 24
NOTE_NUMBER_MAX     = 96
NOTE_NUMBER_INVALID = 255

PROGRAM_NUMBER_MAX           = 7

OSC_CONTROL_INTERVAL_BITS    = 1
OSC_CONTROL_INTERVAL         = 0x01 << OSC_CONTROL_INTERVAL_BITS
OSC_PHASE_RESOLUTION_BITS    = 16
OSC_TUNE_TABLE_STEPS_BITS    = 8
OSC_TUNE_DENOMINATOR_BITS    = 9
OSC_WAVE_TABLE_AMPLITUDE     = 109
OSC_WAVE_TABLE_SAMPLES_BITS  = 8
OSC_DETUNE_MUL_NUM_BITS      = 4
OSC_DETUNE_FREQ_MAX          = 0
FILTER_CONTROL_INTERVAL_BITS = 3
FILTER_CONTROL_INTERVAL      = 0x01 << FILTER_CONTROL_INTERVAL_BITS
FILTER_TABLE_FRACTION_BITS   = 14
ENV_GEN_CONTROL_INTERVAL     = 0x10
ENV_GEN_LEVEL_MAX            = 0x8000
ENV_GEN_LEVEL_MAX_X_1_5      = ENV_GEN_LEVEL_MAX + (ENV_GEN_LEVEL_MAX >> 1)
AMP_ENV_AMT_MAX              = 240
AMP_ENV_AMT_STEP             = 16

DATA_BYTE_MAX         = 0x7F
STATUS_BYTE_INVALID   = 0x7F
DATA_BYTE_INVALID     = 0x80
STATUS_BYTE_MIN       = 0x80
NOTE_OFF              = 0x80
NOTE_ON               = 0x90
CONTROL_CHANGE        = 0xB0
PROGRAM_CHANGE        = 0xC0
PITCH_BEND            = 0xE0
SYSTEM_MESSAGE_MIN    = 0xF0
SYSTEM_EXCLUSIVE      = 0xF0
TIME_CODE             = 0xF1
SONG_POSITION         = 0xF2
SONG_SELECT           = 0xF3
TUNE_REQUEST          = 0xF6
EOX                   = 0xF7
REAL_TIME_MESSAGE_MIN = 0xF8
ACTIVE_SENSING        = 0xFE

MODULATION      = 1
EXPRESSION      = 11
SUSTAIN_PEDAL   = 64

OSC_WAVE        = 24
OSC_LEVEL       = 108

CUTOFF          = 16
RESONANCE       = 17
EG_TO_CUTOFF    = 18
AMP_EG          = 31

ATTACK          = 23
DECAY           = 19
SUSTAIN         = 27

LFO_DEPTH       = 81
LFO_RATE        = 80
LFO_TO_PITCH    = 82
LFO_TO_CUTOFF   = 83

CHORUS_DEPTH    = 60
CHORUS_RATE     = 61
CHORUS_DELAY_T  = 62
CHORUS_MODE     = 63

P_BEND_RANGE    = 85
V_TO_CUTOFF     = 89
OSC_MODE        = 87

ALL_SOUND_OFF   = 120
RESET_ALL_CTRLS = 121
ALL_NOTES_OFF   = 123
OMNI_MODE_OFF   = 124
OMNI_MODE_ON    = 125
MONO_MODE_ON    = 126
POLY_MODE_ON    = 127

SP_PROG_CHG_0   = 112
SP_PROG_CHG_1   = 113
SP_PROG_CHG_2   = 114
SP_PROG_CHG_3   = 115
SP_PROG_CHG_4   = 116
SP_PROG_CHG_5   = 117
SP_PROG_CHG_6   = 118
SP_PROG_CHG_7   = 119

OSC_WAVE_SAW       = 0
OSC_WAVE_PUL       = 127

LFO_WAVE_TRI_ASYNC = 0
LFO_WAVE_TRI_SYNC  = 32
LFO_WAVE_SAW_DOWN  = 64
LFO_WAVE_RANDOM    = 96
LFO_WAVE_SQ        = 127

CHORUS_OFF         = 0
CHORUS_MONO        = 64
CHORUS_STEREO      = 96
CHORUS_STEREO_2    = 127
