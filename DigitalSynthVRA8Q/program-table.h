#pragma once

const uint8_t PROGRAM_NUMBER_DEFAULT = 0;

// PRESET                                         #0   #1   #2   #3   #4   #5   #6   #7   
const uint8_t g_preset_table_OSC_WAVE       [] = {0  , 0  , 0  , 127, 127, 0  , 0  , 0  };
const uint8_t g_preset_table_EG_TO_PITCH    [] = {64 , 62 , 64 , 64 , 64 , 80 , 120, 64 };

const uint8_t g_preset_table_CUTOFF         [] = {64 , 16 , 16 , 112, 0  , 100, 64 , 127};
const uint8_t g_preset_table_RESONANCE      [] = {96 , 80 , 127, 0  , 0  , 64 , 64 , 0  };
const uint8_t g_preset_table_EG_TO_CUTOFF   [] = {82 , 112, 94 , 64 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_AMP_EG         [] = {127, 0  , 127, 0  , 127, 127, 127, 127};

const uint8_t g_preset_table_ATTACK         [] = {0  , 0  , 0  , 0  , 32 , 16 , 0  , 0  };
const uint8_t g_preset_table_DECAY          [] = {64 , 96 , 96 , 96 , 32 , 32 , 64 , 0  };
const uint8_t g_preset_table_SUSTAIN        [] = {127, 0  , 0  , 0  , 127, 96 , 0  , 127};

const uint8_t g_preset_table_LFO_DEPTH      [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_LFO_RATE       [] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_LFO_TO_PITCH   [] = {66 , 66 , 66 , 66 , 66 , 66 , 66 , 66 };
const uint8_t g_preset_table_LFO_TO_CUTOFF  [] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };

const uint8_t g_preset_table_CHORUS_DEPTH   [] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_CHORUS_RATE    [] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_CHORUS_MODE    [] = {127, 127, 127, 127, 127, 127, 127, 127};

const uint8_t g_preset_table_P_BEND_RANGE   [] = {2  , 2  , 2  , 2  , 2  , 2  , 2  , 2  };
