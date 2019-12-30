#pragma once

const uint8_t PROGRAM_NUMBER_DEFAULT = 0;

#if !defined(ENABLE_VOLTAGE_CONTROL) || !defined(USE_PITCH_CV_IN)

// PRESET                                         #0   #1   #2   #3   #4   #5   #6   #7   
const uint8_t g_preset_table_OSC1_2_WAVE    [] = {0  , 0  , 0  , 127, 127, 0  , 48 , 0  };
const uint8_t g_preset_table_OSC_MIX        [] = {64 , 64 , 64 , 64 , 0  , 32 , 127, 0  };
const uint8_t g_preset_table_OSC2_COARSE    [] = {71 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_OSC2_FINE      [] = {68 , 72 , 66 , 72 , 72 , 48 , 68 , 64 };

const uint8_t g_preset_table_SUB_WAVE       [] = {127, 0  , 0  , 0  , 64 , 127, 0  , 0  };
const uint8_t g_preset_table_SUB_LEVEL      [] = {127, 127, 127, 0  , 8  , 0  , 0  , 0  };
const uint8_t g_preset_table_EG_TO_PITCH    [] = {64 , 62 , 64 , 64 , 64 , 80 , 120, 64 };
const uint8_t g_preset_table_EG_TO_P_TGT    [] = {0  , 127, 0  , 0  , 0  , 127, 0  , 0  };

const uint8_t g_preset_table_CUTOFF         [] = {64 , 16 , 16 , 112, 0  , 100, 64 , 127};
const uint8_t g_preset_table_RESONANCE      [] = {96 , 80 , 127, 0  , 0  , 64 , 64 , 0  };
const uint8_t g_preset_table_EG_TO_CUTOFF   [] = {82 , 112, 94 , 64 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_AMP_EG         [] = {127, 0  , 127, 0  , 127, 127, 127, 127};

const uint8_t g_preset_table_ATTACK         [] = {0  , 0  , 0  , 0  , 32 , 16 , 0  , 0  };
const uint8_t g_preset_table_DECAY          [] = {64 , 96 , 96 , 96 , 32 , 32 , 64 , 0  };
const uint8_t g_preset_table_SUSTAIN        [] = {127, 0  , 0  , 0  , 127, 96 , 0  , 127};
const uint8_t g_preset_table_RELEASE        [] = {127, 127, 127, 127, 127, 0  , 127, 127};

const uint8_t g_preset_table_LFO_WAVE       [] = {0  , 0  , 32 , 127, 32 , 32 , 0  , 0  };
const uint8_t g_preset_table_LFO_RATE       [] = {64 , 64 , 64 , 80 , 64 , 64 , 0  , 64 };
const uint8_t g_preset_table_LFO_TO_PITCH   [] = {66 , 66 , 64 , 88 , 66 , 66 , 64 , 64 };
const uint8_t g_preset_table_LFO_TO_CUTOFF  [] = {64 , 64 , 40 , 64 , 64 , 64 , 112, 64 };

const uint8_t g_preset_table_LFO_DEPTH      [] = {8  , 8  , 0  , 0  , 8  , 8  , 0  , 0  };
const uint8_t g_preset_table_EG_TO_LFO_RATE [] = {64 , 64 , 64 , 72 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_LFO_TO_P_TGT   [] = {127, 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_LFO_FADE_TIME  [] = {64 , 64 , 0  , 0  , 64 , 64 , 0  , 0  };

const uint8_t g_preset_table_P_BEND_RANGE   [] = {2  , 2  , 2  , 2  , 2  , 2  , 12 , 2  };
const uint8_t g_preset_table_PITCH_TO_CUTOFF[] = {0  , 0  , 0  , 0  , 127, 127, 127, 0  };
const uint8_t g_preset_table_EXP_TO_CUTOFF  [] = {64 , 64 , 64 , 64 , 88 , 112, 64 , 64 };
const uint8_t g_preset_table_EXP_TO_AMP_LEV [] = {0  , 0  , 0  , 0  , 127, 127, 127, 0  };

const uint8_t g_preset_table_PORTAMENTO     [] = {0  , 32 , 32 , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_LEGATO         [] = {0  , 127, 127, 0  , 0  , 127, 0  , 0  };
const uint8_t g_preset_table_KEY_ASSIGN     [] = {0  , 64 , 0  , 127, 127, 127, 127, 0  };
const uint8_t g_preset_table_EXP_BY_VEL     [] = {0  , 0  , 0  , 0  , 0  , 0  , 127, 0  };

const uint8_t g_preset_table_OSC_LEVEL      [] = {80 , 80 , 80 , 80 , 120, 107, 80 , 80 };
const uint8_t g_preset_table_RESONANCE_LIMIT[] = {127, 127, 127, 127, 96 , 96 , 127, 127};
const uint8_t g_preset_table_AMP_LEVEL      [] = {127, 127, 127, 127, 127, 127, 127, 127};
const uint8_t g_preset_table_DAMP_AND_ATTACK[] = {0  , 0  , 0  , 0  , 64 , 64 , 0  , 0  };







#else  // defined(ENABLE_VOLTAGE_CONTROL) && defined(USE_PITCH_CV_IN)

// PRESET for VRA8-N mode-VC                      #0   #1   #2   #3   #4   #5   #6   #7   
const uint8_t g_preset_table_OSC1_2_WAVE    [] = {0  , 0  , 0  , 0  , 127, 127, 0  , 48 };
const uint8_t g_preset_table_OSC_MIX        [] = {64 , 64 , 64 , 64 , 64 , 0  , 32 , 127};
const uint8_t g_preset_table_OSC2_COARSE    [] = {64 , 71 , 64 , 64 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_OSC2_FINE      [] = {68 , 68 , 72 , 66 , 72 , 72 , 48 , 68 };

const uint8_t g_preset_table_SUB_WAVE       [] = {0  , 127, 0  , 0  , 0  , 64 , 127, 0  };
const uint8_t g_preset_table_SUB_LEVEL      [] = {127, 127, 127, 127, 0  , 8  , 0  , 0  };
const uint8_t g_preset_table_EG_TO_PITCH    [] = {64 , 64 , 62 , 64 , 64 , 64 , 80 , 120};
const uint8_t g_preset_table_EG_TO_P_TGT    [] = {0  , 0  , 127, 0  , 0  , 0  , 127, 0  };

const uint8_t g_preset_table_CUTOFF         [] = {64 , 64 , 16 , 16 , 112, 0  , 100, 64 };
const uint8_t g_preset_table_RESONANCE      [] = {64 , 96 , 80 , 127, 0  , 0  , 64 , 64 };
const uint8_t g_preset_table_EG_TO_CUTOFF   [] = {64 , 82 , 112, 94 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_AMP_EG         [] = {127, 127, 0  , 127, 0  , 127, 127, 127};

const uint8_t g_preset_table_ATTACK         [] = {0  , 0  , 0  , 0  , 0  , 32 , 16 , 0  };
const uint8_t g_preset_table_DECAY          [] = {64 , 64 , 96 , 96 , 96 , 32 , 32 , 64 };
const uint8_t g_preset_table_SUSTAIN        [] = {127, 127, 0  , 0  , 0  , 127, 96 , 0  };
const uint8_t g_preset_table_RELEASE        [] = {127, 127, 127, 127, 127, 127, 0  , 127};

const uint8_t g_preset_table_LFO_WAVE       [] = {0  , 0  , 0  , 32 , 127, 32 , 32 , 0  };
const uint8_t g_preset_table_LFO_RATE       [] = {64 , 64 , 64 , 64 , 80 , 64 , 64 , 0  };
const uint8_t g_preset_table_LFO_TO_PITCH   [] = {65 , 66 , 66 , 64 , 88 , 66 , 66 , 64 };
const uint8_t g_preset_table_LFO_TO_CUTOFF  [] = {64 , 64 , 64 , 40 , 64 , 64 , 64 , 112};

const uint8_t g_preset_table_LFO_DEPTH      [] = {16 , 8  , 8  , 0  , 0  , 8  , 8  , 0  };
const uint8_t g_preset_table_EG_TO_LFO_RATE [] = {64 , 64 , 64 , 64 , 72 , 64 , 64 , 64 };
const uint8_t g_preset_table_LFO_TO_P_TGT   [] = {0  , 127, 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_LFO_FADE_TIME  [] = {0  , 64 , 64 , 0  , 0  , 64 , 64 , 0  };

const uint8_t g_preset_table_P_BEND_RANGE   [] = {30 , 2  , 2  , 2  , 2  , 2  , 2  , 12 };
const uint8_t g_preset_table_PITCH_TO_CUTOFF[] = {0  , 0  , 0  , 0  , 0  , 127, 127, 127};
const uint8_t g_preset_table_EXP_TO_CUTOFF  [] = {64 , 64 , 64 , 64 , 64 , 88 , 112, 64 };
const uint8_t g_preset_table_EXP_TO_AMP_LEV [] = {0  , 0  , 0  , 0  , 0  , 127, 127, 127};

const uint8_t g_preset_table_PORTAMENTO     [] = {0  , 0  , 32 , 32 , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_LEGATO         [] = {0  , 0  , 127, 127, 0  , 0  , 127, 0  };
const uint8_t g_preset_table_KEY_ASSIGN     [] = {127, 0  , 64 , 0  , 127, 127, 127, 127};
const uint8_t g_preset_table_EXP_BY_VEL     [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 127};

const uint8_t g_preset_table_OSC_LEVEL      [] = {80 , 80 , 80 , 80 , 80 , 120, 107, 80 };
const uint8_t g_preset_table_RESONANCE_LIMIT[] = {127, 127, 127, 127, 127, 96 , 96 , 127};
const uint8_t g_preset_table_AMP_LEVEL      [] = {127, 127, 127, 127, 127, 127, 127, 127};
const uint8_t g_preset_table_DAMP_AND_ATTACK[] = {0  , 0  , 0  , 0  , 0  , 64 , 64 , 0  };

#endif
