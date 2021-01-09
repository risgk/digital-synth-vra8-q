#pragma once

const uint8_t PROGRAM_NUMBER_DEFAULT = 0;

// PRESET                                         #0   #1   #2   #3   #4   #5   #6   #7   
const uint8_t g_preset_table_OSC_WAVE       [] = {0  , 0  , 0  , 127, 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_OSC_LEVEL      [] = {90 , 90 , 90 , 90 , 90 , 90 , 90 , 90 };

const uint8_t g_preset_table_CUTOFF         [] = {64 , 40 , 112, 64 , 127, 127, 127, 127};
const uint8_t g_preset_table_RESONANCE      [] = {80 , 64 , 48 , 80 , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_EG_TO_CUTOFF   [] = {82 , 100, 16 , 88 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_AMP_EG         [] = {127, 0  , 0  , 127, 127, 127, 127, 127};

const uint8_t g_preset_table_ATTACK         [] = {0  , 16 , 96 , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_DECAY          [] = {127, 64 , 96 , 96 , 127, 127, 127, 127};
const uint8_t g_preset_table_SUSTAIN        [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };

const uint8_t g_preset_table_LFO_DEPTH      [] = {8  , 8  , 8  , 8  , 8  , 8  , 8  , 0  };
const uint8_t g_preset_table_LFO_RATE       [] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_LFO_TO_PITCH   [] = {65 , 65 , 65 , 65 , 65 , 65 , 65 , 64 };
const uint8_t g_preset_table_LFO_TO_CUTOFF  [] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };

const uint8_t g_preset_table_CHORUS_DEPTH   [] = {32 , 32 , 32 , 32 , 32 , 32 , 24 , 32 };
const uint8_t g_preset_table_CHORUS_RATE    [] = {32 , 32 , 32 , 32 , 16 , 48 , 32 , 32 };
const uint8_t g_preset_table_CHORUS_DELAY_T [] = {64 , 64 , 64 , 64 , 80 , 20 , 60 , 64 };
const uint8_t g_preset_table_CHORUS_MODE    [] = {127, 127, 127, 127, 127, 127, 127, 0  };

const uint8_t g_preset_table_P_BEND_RANGE   [] = {2  , 2  , 2  , 2  , 2  , 2  , 2  , 2  };
