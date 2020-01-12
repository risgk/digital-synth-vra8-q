#pragma once

const double    A4_PITCH = 440.0;

const uint8_t   MIDI_CH             = 0;
const uint16_t  SAMPLING_RATE       = 31250;
const uint16_t  FREQUENCY_MAX       = 15624;
const uint8_t   BIT_DEPTH           = 8;
const uint8_t   NOTE_NUMBER_MIN     = 0;
const uint8_t   NOTE_NUMBER_MAX     = 120;
const uint8_t   NOTE_NUMBER_INVALID = 255;

const uint8_t   PROGRAM_NUMBER_MAX            = 7;
const uint8_t   PROGRAM_NUMBER_RANDOM_CONTROL = 127;

const uint8_t   OSC_CONTROL_INTERVAL_BITS    = 2;
const uint8_t   OSC_CONTROL_INTERVAL         = 0x01 << OSC_CONTROL_INTERVAL_BITS;
const uint8_t   OSC_PHASE_RESOLUTION_BITS    = 24;
const uint8_t   OSC_TUNE_TABLE_STEPS_BITS    = 8;
const uint8_t   OSC_TUNE_DENOMINATOR_BITS    = 9;
const uint8_t   OSC_WAVE_TABLE_AMPLITUDE     = 96;
const uint8_t   OSC_WAVE_TABLE_SAMPLES_BITS  = 8;
const uint8_t   OSC_DETUNE_MUL_NUM_BITS      = 4;
const uint16_t  OSC_DETUNE_FREQ_MIN          = 64;
const uint16_t  OSC_DETUNE_FREQ_MAX          = 0;
const uint8_t   FILTER_CONTROL_INTERVAL_BITS = 3;
const uint8_t   FILTER_CONTROL_INTERVAL      = 0x01 << FILTER_CONTROL_INTERVAL_BITS;
const uint8_t   FILTER_CUTOFF_THROUGH_RATE   = 24;
const uint8_t   FILTER_TABLE_FRACTION_BITS   = 14;
const uint8_t   ENV_GEN_CONTROL_INTERVAL     = 0x10;
const uint16_t  ENV_GEN_LEVEL_MAX            = 0x8000;
const uint16_t  ENV_GEN_LEVEL_MAX_X_1_5      = ENV_GEN_LEVEL_MAX + (ENV_GEN_LEVEL_MAX >> 1);
const uint16_t  ENV_GEN_LEVEL_RELEASE_STEP   = 0x1600;
const uint8_t   AMP_ENV_AMT_MAX              = 240;
const uint8_t   AMP_ENV_AMT_STEP             = 16;

const uint8_t   DATA_BYTE_MAX         = 0x7F;
const uint8_t   STATUS_BYTE_INVALID   = 0x7F;
const uint8_t   DATA_BYTE_INVALID     = 0x80;
const uint8_t   STATUS_BYTE_MIN       = 0x80;
const uint8_t   NOTE_OFF              = 0x80;
const uint8_t   NOTE_ON               = 0x90;
const uint8_t   CONTROL_CHANGE        = 0xB0;
const uint8_t   PROGRAM_CHANGE        = 0xC0;
const uint8_t   PITCH_BEND            = 0xE0;
const uint8_t   SYSTEM_MESSAGE_MIN    = 0xF0;
const uint8_t   SYSTEM_EXCLUSIVE      = 0xF0;
const uint8_t   TIME_CODE             = 0xF1;
const uint8_t   SONG_POSITION         = 0xF2;
const uint8_t   SONG_SELECT           = 0xF3;
const uint8_t   TUNE_REQUEST          = 0xF6;
const uint8_t   EOX                   = 0xF7;
const uint8_t   REAL_TIME_MESSAGE_MIN = 0xF8;
const uint8_t   ACTIVE_SENSING        = 0xFE;

const uint8_t   MODULATION      = 1;
const uint8_t   EXPRESSION      = 11;

const uint8_t   OSC1_2_WAVE     = 24;
const uint8_t   OSC_MIX         = 25;
const uint8_t   OSC2_COARSE     = 20;
const uint8_t   OSC2_FINE       = 21;

const uint8_t   SUB_WAVE        = 29;
const uint8_t   SUB_LEVEL       = 26;
const uint8_t   EG_TO_PITCH     = 104;
const uint8_t   EG_TO_P_TGT     = 105;

const uint8_t   CUTOFF          = 16;
const uint8_t   RESONANCE       = 17;
const uint8_t   EG_TO_CUTOFF    = 18;
const uint8_t   AMP_EG          = 31;

const uint8_t   ATTACK          = 23;
const uint8_t   DECAY           = 19;
const uint8_t   SUSTAIN         = 27;
const uint8_t   RELEASE         = 28;

const uint8_t   LFO_WAVE        = 14;
const uint8_t   LFO_RATE        = 80;
const uint8_t   LFO_TO_PITCH    = 82;
const uint8_t   LFO_TO_CUTOFF   = 83;

const uint8_t   LFO_DEPTH       = 81;
const uint8_t   EG_TO_LFO_RATE  = 3;
const uint8_t   LFO_TO_P_TGT    = 9;
const uint8_t   LFO_FADE_TIME   = 15;

const uint8_t   P_BEND_RANGE    = 85;
const uint8_t   PITCH_TO_CUTOFF = 86;
const uint8_t   EXP_TO_CUTOFF   = 106;
const uint8_t   EXP_TO_AMP_LEV  = 107;

const uint8_t   PORTAMENTO      = 22;
const uint8_t   LEGATO          = 30;
const uint8_t   KEY_ASSIGN      = 87;
const uint8_t   EXP_BY_VEL      = 89;

const uint8_t   OSC_LEVEL       = 108;
const uint8_t   RESONANCE_LIMIT = 109;
const uint8_t   AMP_LEVEL       = 110;
const uint8_t   DAMP_AND_ATTACK = 111;

const uint8_t   ALL_NOTES_OFF   = 123;
const uint8_t   OMNI_MODE_OFF   = 124;
const uint8_t   OMNI_MODE_ON    = 125;
const uint8_t   MONO_MODE_ON    = 126;
const uint8_t   POLY_MODE_ON    = 127;

const uint8_t   SP_PROG_CHG_0   = 112;
const uint8_t   SP_PROG_CHG_1   = 113;
const uint8_t   SP_PROG_CHG_2   = 114;
const uint8_t   SP_PROG_CHG_3   = 115;
const uint8_t   SP_PROG_CHG_4   = 116;
const uint8_t   SP_PROG_CHG_5   = 117;
const uint8_t   SP_PROG_CHG_6   = 118;
const uint8_t   SP_PROG_CHG_7   = 119;

const uint8_t   SP_RAND_CTRL    = 90;

const uint8_t   OSC_WAVEFORM_SAW   = 0;
const uint8_t   OSC_WAVEFORM_NOISE = 64;
const uint8_t   OSC_WAVEFORM_SQ    = 127;

const uint8_t   SUB_WAVEFORM_SIN   = 0;
const uint8_t   SUB_WAVEFORM_NOISE = 64;
const uint8_t   SUB_WAVEFORM_SQ    = 127;

const uint8_t   LFO_WAVEFORM_TRI_ASYNC = 0;
const uint8_t   LFO_WAVEFORM_TRI_SYNC  = 32;
const uint8_t   LFO_WAVEFORM_SAW_DOWN  = 64;
const uint8_t   LFO_WAVEFORM_RANDOM    = 96;
const uint8_t   LFO_WAVEFORM_SQ        = 127;

const uint8_t   KEY_ASSIGN_LOW  = 0;
const uint8_t   KEY_ASSIGN_DUO  = 64;
const uint8_t   KEY_ASSIGN_HIGH = 96;
const uint8_t   KEY_ASSIGN_LAST = 127;
