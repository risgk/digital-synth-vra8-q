#pragma once

#include "common.h"
#include "osc-table.h"
#include "mul-q.h"
#include <math.h>
#include <stdio.h>

static const uint8_t OSC_MIX_TABLE_LENGTH = 31;

template <uint8_t T>
class Osc {
  static const uint8_t LFO_FADE_COEF_OFF    = 1;
  static const uint8_t LFO_FADE_COEF_ON_MIN = 2;
  static const uint8_t LFO_FADE_LEVEL_MAX   = 128;

  static uint8_t        m_mix[4];
  static uint8_t        m_portamento_coef;
  static int16_t        m_lfo_mod_level;
  static uint16_t       m_lfo_phase;
  static int8_t         m_lfo_wave_level;
  static int16_t        m_lfo_level;
  static uint16_t       m_lfo_rate_actual;
  static uint8_t        m_lfo_rate;
  static int8_t         m_lfo_rate_eg_amt;
  static uint8_t        m_lfo_fade_coef;
  static uint8_t        m_lfo_fade_cnt;
  static uint8_t        m_lfo_fade_level;
  static uint8_t        m_lfo_depth[2];
  static int8_t         m_pitch_lfo_amt;
  static uint8_t        m_lfo_waveform;
  static uint8_t        m_lfo_sampled;

  static uint16_t       m_lfo1_phase;
  static uint16_t       m_lfo1_rate_actual;
  static int16_t        m_lfo1_level;

  static uint8_t        m_waveform;
  static int16_t        m_pitch_bend;
  static uint8_t        m_pitch_bend_minus_range;
  static uint8_t        m_pitch_bend_plus_range;
  static int16_t        m_pitch_bend_normalized;
  static uint16_t       m_pitch_target[4];
  static uint16_t       m_pitch_current[4];
  static uint16_t       m_pitch_real[4];
  static const uint8_t* m_wave_table[4];
  static const uint8_t* m_wave_table_temp[4];
  static uint16_t       m_freq[4];
  static uint16_t       m_freq_temp[4];
  static uint16_t       m_phase[4];
  static boolean        m_note_on[4];
  static int16_t        m_pitch_eg_amt;

public:
  INLINE static void initialize() {
    m_mix[0] = 0;
    m_mix[1] = 0;
    m_mix[2] = 0;
    m_mix[3] = 0;
    m_portamento_coef = 0;

    m_lfo_mod_level = 0;
    m_lfo_phase = 0;
    m_lfo_wave_level = 0;
    m_lfo_level = 0;
    m_lfo_rate_actual = 0;
    m_lfo_rate = 0;
    m_lfo_rate_eg_amt = 0;
    m_lfo_fade_coef = LFO_FADE_COEF_OFF;
    m_lfo_fade_cnt = m_lfo_fade_coef;
    m_lfo_fade_level = LFO_FADE_LEVEL_MAX;
    m_lfo_depth[0] = 0;
    m_lfo_depth[1] = 0;
    m_lfo_waveform = LFO_WAVEFORM_TRI_ASYNC;
    m_lfo_sampled = 64;

    m_lfo1_phase = 0;
    m_lfo1_rate_actual = 32;
    m_lfo1_level = 0;

    m_waveform = OSC_WAVEFORM_SAW;
    m_pitch_bend_normalized = 0;
    m_pitch_target[0] = NOTE_NUMBER_MIN << 8;
    m_pitch_target[1] = NOTE_NUMBER_MIN << 8;
    m_pitch_target[2] = NOTE_NUMBER_MIN << 8;
    m_pitch_target[3] = NOTE_NUMBER_MIN << 8;
    m_pitch_current[0] = m_pitch_target[0];
    m_pitch_current[1] = m_pitch_target[1];
    m_pitch_current[2] = m_pitch_target[2];
    m_pitch_current[3] = m_pitch_target[3];
    m_pitch_real[0] = m_pitch_current[0];
    m_pitch_real[1] = m_pitch_current[1];
    m_wave_table[0] = g_osc_saw_wave_tables[0];
    m_wave_table[1] = g_osc_saw_wave_tables[0];
    m_wave_table[2] = g_osc_saw_wave_tables[0];
    m_wave_table[3] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[0] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[1] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[2] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[3] = g_osc_saw_wave_tables[0];
    m_freq[0] = g_osc_freq_table[0];
    m_freq[1] = g_osc_freq_table[0];
    m_freq_temp[0] = g_osc_freq_table[0];
    m_freq_temp[1] = g_osc_freq_table[0];
    m_phase[0] = 0;
    m_phase[1] = 0;
    m_phase[2] = 0;
    m_phase[3] = 0;
    m_note_on[0] = false;
    m_note_on[1] = false;
    m_pitch_eg_amt = 0;
    set_pitch_bend_minus_range(30);
    set_pitch_bend_plus_range(30);
  }

  INLINE static void set_osc_waveforms(uint8_t controller_value) {
    if (controller_value < 64) {
      m_waveform = OSC_WAVEFORM_SAW;
    } else {
      m_waveform = OSC_WAVEFORM_SQ;
    }
  }

  INLINE static void set_lfo_waveform(uint8_t controller_value) {
    if        (controller_value < 16) {
      m_lfo_waveform = LFO_WAVEFORM_TRI_ASYNC;
    } else if (controller_value < 48) {
      m_lfo_waveform = LFO_WAVEFORM_TRI_SYNC;
    } else if (controller_value < 80) {
      m_lfo_waveform = LFO_WAVEFORM_SAW_DOWN;
    } else if (controller_value < 112) {
      m_lfo_waveform = LFO_WAVEFORM_RANDOM;
    } else {
      m_lfo_waveform = LFO_WAVEFORM_SQ;
    }
  }

  INLINE static void set_portamento(uint8_t controller_value) {
    if (controller_value < 2) {
      m_portamento_coef = 0;
    } else {
      m_portamento_coef = (controller_value >> 1) + 191;
    }
  }

  INLINE static void set_lfo_rate(uint8_t controller_value) {
    m_lfo_rate = controller_value;
  }

  INLINE static void set_lfo_rate_eg_amt(uint8_t controller_value) {
    m_lfo_rate_eg_amt = (controller_value - 64) << 1;
  }

  INLINE static void set_lfo_fade_time(uint8_t controller_value) {
    m_lfo_fade_coef = high_byte(controller_value * controller_value) + LFO_FADE_COEF_OFF;
  }

  template <uint8_t N>
  INLINE static void set_lfo_depth(uint8_t controller_value) {
    m_lfo_depth[N] = controller_value;
  }

  template <uint8_t N>
  INLINE static void set_pitch_lfo_amt(uint8_t controller_value) {
    if (controller_value < 4) {
      m_pitch_lfo_amt = -60;
    } else if (controller_value < 124) {
      m_pitch_lfo_amt = controller_value - 64;
    } else {
      m_pitch_lfo_amt = 60;
    }
  }

  INLINE static void set_pitch_eg_amt(uint8_t controller_value) {
    if (controller_value < 2) {
      m_pitch_eg_amt = -((30 << 8) << 1);
    } else if (controller_value < 32) {
      m_pitch_eg_amt = -(((32 - controller_value) << 8) << 1);
    } else if (controller_value < 64) {
      m_pitch_eg_amt = -(((64 - controller_value) << 3) << 1);
    } else if (controller_value < 97) {
      m_pitch_eg_amt = ((controller_value - 64) << 3) << 1;
    } else if (controller_value < 127) {
      m_pitch_eg_amt = ((controller_value - 96) << 8) << 1;
    } else {
      m_pitch_eg_amt = (30 << 8) << 1;
    }
  }

  template <uint8_t N>
  INLINE static void note_on(uint8_t note_number) {
    m_pitch_target[N] = note_number << 8;
    m_note_on[N] = true;
  }

  template <uint8_t N>
  INLINE static void note_off() {
    m_note_on[N] = false;
  }

  INLINE static void trigger_lfo() {
    if (m_lfo_waveform != LFO_WAVEFORM_TRI_ASYNC) {
      m_lfo_phase = 0xFFFF;
    }

    if (m_lfo_fade_coef >= LFO_FADE_COEF_ON_MIN) {
      m_lfo_fade_level = 0;
    }
  }

  INLINE static void set_pitch_bend_minus_range(uint8_t controller_value) {
    uint8_t range = controller_value;
    if (range > 30) {
      range = 30;
    }
    m_pitch_bend_minus_range = range;
    update_pitch_bend();
  }

  INLINE static void set_pitch_bend_plus_range(uint8_t controller_value) {
    uint8_t range = controller_value;
    if (range > 30) {
      range = 30;
    }
    m_pitch_bend_plus_range = range;
    update_pitch_bend();
  }

  INLINE static void set_pitch_bend(int16_t pitch_bend) {
    m_pitch_bend = pitch_bend;
    update_pitch_bend();
  }

  INLINE static int16_t get_lfo_level() {
    return m_lfo_level;
  }

  INLINE static int16_t get_lfo1_level() {
    return m_lfo1_level;
  }

  INLINE static int16_t clock(uint8_t count, uint8_t eg_level) {
#if 1
    if ((count & (OSC_CONTROL_INTERVAL - 1)) == 0) {
      switch (count & (0x1F << OSC_CONTROL_INTERVAL_BITS)) {
      case (0x00 << OSC_CONTROL_INTERVAL_BITS): update_freq_0th<0>();               break;
      case (0x01 << OSC_CONTROL_INTERVAL_BITS): update_freq_1st<0>(eg_level);       break;
      case (0x02 << OSC_CONTROL_INTERVAL_BITS): update_freq_2nd<0>();               break;
      case (0x03 << OSC_CONTROL_INTERVAL_BITS): update_freq_3rd<0>();               break;
      case (0x04 << OSC_CONTROL_INTERVAL_BITS): update_freq_4th<0>();               break;
      case (0x05 << OSC_CONTROL_INTERVAL_BITS): update_mix_1st(); update_mix_2nd(); break;
      case (0x06 << OSC_CONTROL_INTERVAL_BITS): update_lfo_1st(eg_level);           break;
      case (0x07 << OSC_CONTROL_INTERVAL_BITS): update_lfo_2nd();                   break;
      case (0x08 << OSC_CONTROL_INTERVAL_BITS): update_freq_0th<1>();               break;
      case (0x09 << OSC_CONTROL_INTERVAL_BITS): update_freq_1st<1>(eg_level);       break;
      case (0x0A << OSC_CONTROL_INTERVAL_BITS): update_freq_2nd<1>();               break;
      case (0x0B << OSC_CONTROL_INTERVAL_BITS): update_freq_3rd<1>();               break;
      case (0x0C << OSC_CONTROL_INTERVAL_BITS): update_freq_4th<1>();               break;
      case (0x0D << OSC_CONTROL_INTERVAL_BITS):                                     break;
      case (0x0E << OSC_CONTROL_INTERVAL_BITS): update_lfo_3rd();                   break;
      case (0x0F << OSC_CONTROL_INTERVAL_BITS): update_lfo_4th();                   break;
      case (0x10 << OSC_CONTROL_INTERVAL_BITS): update_freq_0th<2>();               break;
      case (0x11 << OSC_CONTROL_INTERVAL_BITS): update_freq_1st<2>(eg_level);       break;
      case (0x12 << OSC_CONTROL_INTERVAL_BITS): update_freq_2nd<2>();               break;
      case (0x13 << OSC_CONTROL_INTERVAL_BITS): update_freq_3rd<2>();               break;
      case (0x14 << OSC_CONTROL_INTERVAL_BITS): update_freq_4th<2>();               break;
      case (0x15 << OSC_CONTROL_INTERVAL_BITS):                                     break;
      case (0x16 << OSC_CONTROL_INTERVAL_BITS): update_lfo1();                      break;
      case (0x17 << OSC_CONTROL_INTERVAL_BITS):                                     break;
      case (0x18 << OSC_CONTROL_INTERVAL_BITS): update_freq_0th<3>();               break;
      case (0x19 << OSC_CONTROL_INTERVAL_BITS): update_freq_1st<3>(eg_level);       break;
      case (0x1A << OSC_CONTROL_INTERVAL_BITS): update_freq_2nd<3>();               break;
      case (0x1B << OSC_CONTROL_INTERVAL_BITS): update_freq_3rd<3>();               break;
      case (0x1C << OSC_CONTROL_INTERVAL_BITS): update_freq_4th<3>();               break;
      case (0x1D << OSC_CONTROL_INTERVAL_BITS):                                     break;
      case (0x1E << OSC_CONTROL_INTERVAL_BITS):                                     break;
      case (0x1F << OSC_CONTROL_INTERVAL_BITS):                                     break;
      }
    }
#endif

#if 1
    m_phase[0] += m_freq[0];
    m_phase[1] += m_freq[1];
    m_phase[2] += m_freq[2];
    m_phase[3] += m_freq[3];

    int8_t wave_0 = get_wave_level(m_wave_table[0], m_phase[0]);
    int8_t wave_1 = get_wave_level(m_wave_table[1], m_phase[1]);
    int8_t wave_2 = get_wave_level(m_wave_table[2], m_phase[2]);
    int8_t wave_3 = get_wave_level(m_wave_table[3], m_phase[3]);

    // amp and mix
    int16_t level_0 = wave_0 * m_mix[0];
    int16_t level_1 = wave_1 * m_mix[1];
    int16_t level_2 = wave_2 * m_mix[2];
    int16_t level_3 = wave_3 * m_mix[3];
    int16_t result  = level_0 + level_1 + level_2 + level_3;
#else
    int16_t result  = 0;
#endif

    return result;
  }

private:
  INLINE static const uint8_t* get_wave_table(uint8_t waveform, uint8_t note_number) {
    const uint8_t* result;
#if defined(MAKE_SAMPLE_WAV_FILE)
    if (waveform == OSC_WAVEFORM_SAW) {
      result = g_osc_saw_wave_tables[note_number - NOTE_NUMBER_MIN];
    } else {     // OSC_WAVEFORM_SQ
      result = g_osc_sq_wave_tables[note_number - NOTE_NUMBER_MIN];
    }
#else
    if (waveform == OSC_WAVEFORM_SAW) {
      result = pgm_read_word(g_osc_saw_wave_tables + (note_number - NOTE_NUMBER_MIN));
    } else {     // OSC_WAVEFORM_SQ
      result = pgm_read_word(g_osc_sq_wave_tables + (note_number - NOTE_NUMBER_MIN));
    }
#endif
    return result;
  }

  INLINE static int8_t get_wave_level(const uint8_t* wave_table, uint16_t phase) {
    uint8_t curr_index = high_byte(phase);
    uint8_t next_weight = low_byte(phase);
    uint16_t two_data = pgm_read_word(wave_table + curr_index);
    uint8_t curr_data = low_byte(two_data);
    uint8_t next_data = high_byte(two_data);

    // lerp
    int8_t result = curr_data + high_sbyte(static_cast<int8_t>(next_data - curr_data) * next_weight);

    return result;
  }

  INLINE static int8_t get_lfo_wave_level(uint16_t phase) {
    int8_t level = 0;

    switch (m_lfo_waveform) {
    case LFO_WAVEFORM_TRI_ASYNC:
    case LFO_WAVEFORM_TRI_SYNC:
      level = high_sbyte(phase);
      if (level < -64) {
        level = -64 - (level + 64);
      } else if (level < 64) {
        // do nothing
      } else {
        level = 64 - (level - 64);
      }
      level = -level;
      break;
    case LFO_WAVEFORM_SAW_DOWN:
      {
        uint8_t b = high_byte(phase);
        b >>= 1;
        level = b - 64;
      }
      break;
    case LFO_WAVEFORM_RANDOM:
      level = m_lfo_sampled - 64;
      break;
    case LFO_WAVEFORM_SQ:
      level = high_sbyte(phase);
      if (level >= 0) {
        level = 0;
      } else {
        level = -128;
      }
      break;
    }

    return level;
  }



  template <uint8_t N>
  INLINE static void update_freq_0th() {
//    if (m_note_on[N]) {
//      m_pitch_current[N] = m_pitch_target[N] - mul_q15_q8(m_pitch_target[N] - m_pitch_current[N], m_portamento_coef);
//    }
    m_pitch_current[N] = m_pitch_target[N];

    int16_t t = TRANSPOSE << 8;
    m_pitch_real[N] = (64 << 8) + m_pitch_current[N] + m_pitch_bend_normalized + t;

    uint8_t coarse = high_byte(m_pitch_real[N]);
    if (coarse <= (NOTE_NUMBER_MIN + 64)) {
      m_pitch_real[N] = NOTE_NUMBER_MIN << 8;
    } else if (coarse >= (NOTE_NUMBER_MAX + 64)) {
      m_pitch_real[N] = NOTE_NUMBER_MAX << 8;
    } else {
      m_pitch_real[N] -= (64 << 8);
    }
  }

  template <uint8_t N>
  INLINE static void update_freq_1st(uint8_t eg_level) {
    m_pitch_real[N] += (64 << 8);

    int16_t pitch_eg_mod_level = 0;
    pitch_eg_mod_level = mul_q15_q8(m_pitch_eg_amt, eg_level);
    m_pitch_real[N] += pitch_eg_mod_level;
  }

  template <uint8_t N>
  INLINE static void update_freq_2nd() {
    m_pitch_real[N] += m_lfo_mod_level;

    uint8_t coarse = high_byte(m_pitch_real[N]);
    if (coarse <= (NOTE_NUMBER_MIN + 64)) {
      m_pitch_real[N] = NOTE_NUMBER_MIN << 8;
    } else if (coarse >= (NOTE_NUMBER_MAX + 64)) {
      m_pitch_real[N] = NOTE_NUMBER_MAX << 8;
    } else {
      m_pitch_real[N] -= (64 << 8);
    }

    m_pitch_real[N] += 128;  // For g_osc_tune_table[]
  }

  template <uint8_t N>
  INLINE static void update_freq_3rd() {
    uint8_t coarse = high_byte(m_pitch_real[N]);
    m_freq_temp[N] = g_osc_freq_table[coarse - NOTE_NUMBER_MIN];
    m_wave_table_temp[N] = get_wave_table(m_waveform, coarse);
  }

  template <uint8_t N>
  INLINE static void update_freq_4th() {
    uint8_t fine = low_byte(m_pitch_real[N]);
    uint16_t freq_div_512 = m_freq_temp[N] >> 8;
    freq_div_512 >>= 1;
    int16_t freq_offset = (freq_div_512 * g_osc_tune_table[fine >> (8 - OSC_TUNE_TABLE_STEPS_BITS)]);
    m_freq[N] = m_freq_temp[N] + freq_offset;
    m_wave_table[N] = m_wave_table_temp[N];
  }



  INLINE static void update_lfo_1st(uint8_t eg_level) {
    int8_t lfo_rate_mod = high_sbyte(m_lfo_rate_eg_amt * eg_level);
    int16_t lfo_rate = m_lfo_rate + lfo_rate_mod + lfo_rate_mod;
    if (lfo_rate > 127) {
      lfo_rate = 127;
    } else if (lfo_rate < 0) {
      lfo_rate = 0;
    }

    if (lfo_rate >= 32) {
      m_lfo_rate_actual = (high_byte((lfo_rate << 1) *
                                     (lfo_rate << 1)) + 2) * 12;
    } else {
      m_lfo_rate_actual = ((lfo_rate >> 1) + 2) * 12;
    }
  }

  INLINE static void update_lfo_2nd() {
    --m_lfo_fade_cnt;
    if (m_lfo_fade_cnt == 0) {
      m_lfo_fade_cnt = m_lfo_fade_coef;
      if (m_lfo_fade_level < LFO_FADE_LEVEL_MAX) {
        m_lfo_fade_level += 2;
      }
    }

    m_lfo_phase += m_lfo_rate_actual;
    m_lfo_wave_level = get_lfo_wave_level(m_lfo_phase);
  }

  INLINE static void update_lfo_3rd() {
    uint8_t lfo_depth = high_byte((m_lfo_depth[0] << 1) * m_lfo_fade_level) + m_lfo_depth[1];
    if (lfo_depth > 127) {
      lfo_depth = 127;
    }

    if (lfo_depth == 127) {
      lfo_depth = 128;
    }

    m_lfo_level = (lfo_depth * m_lfo_wave_level) << 1;
  }

  INLINE static void update_lfo_4th() {
    m_lfo_mod_level = -mul_q15_q7(m_lfo_level, m_pitch_lfo_amt);
  }



  INLINE static void update_lfo1() {
    m_lfo1_phase += m_lfo1_rate_actual;
    m_lfo_level = get_lfo1_wave_level(m_lfo1_phase >> 5);
    printf("%d %d\n",m_lfo1_phase >> 5, m_lfo_level);
  }

  INLINE static int16_t get_lfo1_wave_level(uint16_t phase) {
    int16_t level = 0;

    if (phase < 0x0400) {
      level = phase - 512;
    } else {
      level = 1024 + 511 - phase;
    }

    return level;
  }


  INLINE static void update_pitch_bend() {
    int16_t b = m_pitch_bend + 1;
    b >>= 3;
    if (b < 0) {
      m_pitch_bend_normalized = (b * m_pitch_bend_minus_range) >> 2;
    } else {
      m_pitch_bend_normalized = (b * m_pitch_bend_plus_range) >> 2;
    }
  }

  INLINE static void update_mix_1st() {
    m_mix[0] = 64;
    m_mix[1] = 64;
    m_mix[2] = 64;
    m_mix[3] = 64;
  }

  INLINE static void update_mix_2nd() {
  }
};

template <uint8_t T> uint8_t         Osc<T>::m_mix[4];
template <uint8_t T> uint8_t         Osc<T>::m_portamento_coef;
template <uint8_t T> int16_t         Osc<T>::m_lfo_mod_level;
template <uint8_t T> uint16_t        Osc<T>::m_lfo_phase;
template <uint8_t T> int8_t          Osc<T>::m_lfo_wave_level;
template <uint8_t T> int16_t         Osc<T>::m_lfo_level;
template <uint8_t T> uint16_t        Osc<T>::m_lfo_rate_actual;
template <uint8_t T> uint8_t         Osc<T>::m_lfo_rate;
template <uint8_t T> int8_t          Osc<T>::m_lfo_rate_eg_amt;
template <uint8_t T> uint8_t         Osc<T>::m_lfo_fade_coef;
template <uint8_t T> uint8_t         Osc<T>::m_lfo_fade_cnt;
template <uint8_t T> uint8_t         Osc<T>::m_lfo_fade_level;
template <uint8_t T> uint8_t         Osc<T>::m_lfo_depth[2];

template <uint8_t T> uint16_t        Osc<T>::m_lfo1_phase;
template <uint8_t T> uint16_t        Osc<T>::m_lfo1_rate_actual;
template <uint8_t T> int16_t         Osc<T>::m_lfo1_level;

template <uint8_t T> int8_t          Osc<T>::m_pitch_lfo_amt;
template <uint8_t T> uint8_t         Osc<T>::m_lfo_waveform;
template <uint8_t T> uint8_t         Osc<T>::m_lfo_sampled;
template <uint8_t T> uint8_t         Osc<T>::m_waveform;
template <uint8_t T> int16_t         Osc<T>::m_pitch_bend;
template <uint8_t T> uint8_t         Osc<T>::m_pitch_bend_minus_range;
template <uint8_t T> uint8_t         Osc<T>::m_pitch_bend_plus_range;
template <uint8_t T> int16_t         Osc<T>::m_pitch_bend_normalized;
template <uint8_t T> uint16_t        Osc<T>::m_pitch_target[4];
template <uint8_t T> uint16_t        Osc<T>::m_pitch_current[4];
template <uint8_t T> uint16_t        Osc<T>::m_pitch_real[4];
template <uint8_t T> const uint8_t*  Osc<T>::m_wave_table[4];
template <uint8_t T> const uint8_t*  Osc<T>::m_wave_table_temp[4];
template <uint8_t T> uint16_t        Osc<T>::m_freq[4];
template <uint8_t T> uint16_t        Osc<T>::m_freq_temp[4];
template <uint8_t T> uint16_t        Osc<T>::m_phase[4];
template <uint8_t T> boolean         Osc<T>::m_note_on[4];
template <uint8_t T> int16_t         Osc<T>::m_pitch_eg_amt;
