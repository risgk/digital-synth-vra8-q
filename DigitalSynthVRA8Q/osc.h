#pragma once

#include "common.h"
#include "osc-table.h"
#include "mul-q.h"
#include <math.h>

static const uint8_t OSC_MIX_TABLE_LENGTH = 31;

template <uint8_t T>
class Osc {
  static const uint8_t LFO_FADE_COEF_OFF    = 1;
  static const uint8_t LFO_FADE_COEF_ON_MIN = 2;
  static const uint8_t LFO_FADE_LEVEL_MAX   = 128;

  static uint8_t        m_portamento_coef;
  static int16_t        m_lfo_mod_level;
  static uint16_t       m_lfo_phase;
  static int8_t         m_lfo_wave_level;
  static int16_t        m_lfo_level;
  static uint16_t       m_lfo_rate_actual;
  static uint8_t        m_lfo_rate;
  static uint8_t        m_lfo_depth[2];
  static int8_t         m_pitch_lfo_amt;
  static uint8_t        m_lfo_waveform;
  static uint8_t        m_lfo_sampled;

  static uint8_t        m_chorus_depth_control;
  static uint8_t        m_chorus_rate_control;
  static uint8_t        m_chorus_delay_time_control;
  static uint8_t        m_chorus_mode_control;
  static uint8_t        m_chorus_depth_control_actual;
  static uint16_t       m_chorus_lfo_phase;
  static int16_t        m_chorus_lfo_wave_level;
  static int16_t        m_chorus_lfo_level;
  static uint16_t       m_chorus_delay_time;

  static uint8_t        m_waveform;
  static int16_t        m_pitch_bend;
  static uint8_t        m_pitch_bend_range;
  static int16_t        m_pitch_bend_normalized;
  static uint16_t       m_pitch_target[4];
  static uint16_t       m_pitch_current[4];
  static uint16_t       m_pitch_real[4];
  static const uint8_t* m_wave_table[4];
  static const uint8_t* m_wave_table_temp[4];
  static uint16_t       m_freq[4];
  static uint16_t       m_freq_temp[4];
  static uint16_t       m_phase[4];
  static boolean        m_osc_on[4];
  static uint8_t        m_osc_gain[4];
  static uint8_t        m_osc_level;

public:
  INLINE static void initialize() {
    m_portamento_coef = 0;

    m_lfo_mod_level = 0;
    m_lfo_phase = 0;
    m_lfo_wave_level = 0;
    m_lfo_level = 0;
    m_lfo_rate_actual = 0;
    m_lfo_rate = 0;
    m_lfo_depth[0] = 0;
    m_lfo_depth[1] = 0;
    m_lfo_waveform = LFO_WAVEFORM_TRI_ASYNC;
    m_lfo_sampled = 64;

    set_chorus_depth     (32 );
    set_chorus_rate      (16 );
    set_chorus_delay_time(80 );
    set_chorus_mode      (127);

    m_chorus_depth_control_actual = 64;
    m_chorus_lfo_phase = 0;
    m_chorus_lfo_wave_level = 0;
    m_chorus_lfo_level = 0;
    m_chorus_delay_time = 0;

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
    m_pitch_real[2] = m_pitch_current[2];
    m_pitch_real[3] = m_pitch_current[3];
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
    m_freq[2] = g_osc_freq_table[0];
    m_freq[3] = g_osc_freq_table[0];
    m_freq_temp[0] = g_osc_freq_table[0];
    m_freq_temp[1] = g_osc_freq_table[0];
    m_freq_temp[2] = g_osc_freq_table[0];
    m_freq_temp[3] = g_osc_freq_table[0];
    m_phase[0] = 0;
    m_phase[1] = 0;
    m_phase[2] = 0;
    m_phase[3] = 0;
    m_osc_on[0] = false;
    m_osc_on[1] = false;
    m_osc_on[2] = false;
    m_osc_on[3] = false;
    m_osc_gain[0] = 0;
    m_osc_gain[1] = 0;
    m_osc_gain[2] = 0;
    m_osc_gain[3] = 0;
    m_osc_level = 32;

    set_pitch_bend_range(2);
    set_pitch_bend_range(2);
  }

  INLINE static void set_osc_waveforms(uint8_t controller_value) {
    if (controller_value < 64) {
      m_waveform = OSC_WAVEFORM_SAW;
    } else {
      m_waveform = OSC_WAVEFORM_PUL;
    }
  }

  INLINE static void set_osc_level(uint8_t controller_value) {
    if (controller_value < 32) {
      m_osc_level = (32 >> 1);
    } else {
      m_osc_level = (controller_value >> 1);
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

  template <uint8_t N>
  INLINE static void set_lfo_depth(uint8_t controller_value) {
    m_lfo_depth[N] = controller_value;
  }

  template <uint8_t N>
  INLINE static void set_pitch_lfo_amt(uint8_t controller_value) {
    if (controller_value < 16) {
      m_pitch_lfo_amt = -96;
    } else if (controller_value < 112) {
      m_pitch_lfo_amt = ((controller_value - 64) << 1);
    } else {
      m_pitch_lfo_amt = 96;
    }
  }


  INLINE static void set_chorus_depth(uint8_t controller_value) {
    if (controller_value < 8) {
      m_chorus_depth_control = 8;
    } else {
      m_chorus_depth_control = controller_value;
    }
  }

  INLINE static void set_chorus_rate(uint8_t controller_value) {
    if (controller_value < 2) {
      m_chorus_rate_control = 1;
    } else {
      m_chorus_rate_control = (controller_value + 2) >> 2;
    }
  }

  INLINE static void set_chorus_delay_time(uint8_t controller_value) {
    if (controller_value < 8) {
      m_chorus_delay_time_control = 8;
    } else if (controller_value < 119) {
      m_chorus_delay_time_control = controller_value;
    } else {
      m_chorus_delay_time_control = 119;
    }
  }

  INLINE static void set_chorus_mode(uint8_t controller_value) {
    m_chorus_mode_control = controller_value;
  }

  INLINE static void note_on(uint8_t osc_index, uint8_t note_number) {
    m_pitch_target[osc_index] = note_number << 8;
    m_osc_on[osc_index] = true;
  }

  INLINE static void note_off(uint8_t osc_index) {
    m_osc_on[osc_index] = false;
  }

  INLINE static void trigger_lfo() {
    if (m_lfo_waveform != LFO_WAVEFORM_TRI_ASYNC) {
      m_lfo_phase = 0xFFFF;
    }
  }

  INLINE static void set_pitch_bend_range(uint8_t controller_value) {
    uint8_t range = controller_value;
    if (range > 24) {
      range = 24;
    }
    m_pitch_bend_range = range;
    update_pitch_bend();
  }

  INLINE static void set_pitch_bend(int16_t pitch_bend) {
    m_pitch_bend = pitch_bend;
    update_pitch_bend();
  }

  INLINE static int16_t get_lfo_level() {
    return m_lfo_level;
  }

  INLINE static int16_t get_chorus_delay_time() {
    return m_chorus_delay_time;
  }

  INLINE static int16_t clock(uint8_t count) {
#if 1
    if ((count & (OSC_CONTROL_INTERVAL - 1)) == 0) {
      //printf("%d Osc\n", count);
      switch (count & (0x1F << OSC_CONTROL_INTERVAL_BITS)) {
      case (0x00 << OSC_CONTROL_INTERVAL_BITS): update_freq_0th<0>();               break;
      case (0x01 << OSC_CONTROL_INTERVAL_BITS): update_freq_1st<0>();               break;
      case (0x02 << OSC_CONTROL_INTERVAL_BITS): update_freq_2nd<0>();               break;
      case (0x03 << OSC_CONTROL_INTERVAL_BITS): update_freq_3rd<0>();               break;
      case (0x04 << OSC_CONTROL_INTERVAL_BITS): update_gate<0>();                   break;
      case (0x05 << OSC_CONTROL_INTERVAL_BITS):                                     break;
      case (0x06 << OSC_CONTROL_INTERVAL_BITS): update_lfo_1st();                   break;
      case (0x07 << OSC_CONTROL_INTERVAL_BITS): update_lfo_2nd();                   break;
      case (0x08 << OSC_CONTROL_INTERVAL_BITS): update_freq_0th<1>();               break;
      case (0x09 << OSC_CONTROL_INTERVAL_BITS): update_freq_1st<1>();               break;
      case (0x0A << OSC_CONTROL_INTERVAL_BITS): update_freq_2nd<1>();               break;
      case (0x0B << OSC_CONTROL_INTERVAL_BITS): update_freq_3rd<1>();               break;
      case (0x0C << OSC_CONTROL_INTERVAL_BITS): update_gate<1>();                   break;
      case (0x0D << OSC_CONTROL_INTERVAL_BITS):                                     break;
      case (0x0E << OSC_CONTROL_INTERVAL_BITS): update_lfo_3rd();                   break;
      case (0x0F << OSC_CONTROL_INTERVAL_BITS): update_lfo_4th();                   break;
      case (0x10 << OSC_CONTROL_INTERVAL_BITS): update_freq_0th<2>();               break;
      case (0x11 << OSC_CONTROL_INTERVAL_BITS): update_freq_1st<2>();               break;
      case (0x12 << OSC_CONTROL_INTERVAL_BITS): update_freq_2nd<2>();               break;
      case (0x13 << OSC_CONTROL_INTERVAL_BITS): update_freq_3rd<2>();               break;
      case (0x14 << OSC_CONTROL_INTERVAL_BITS): update_gate<2>();                   break;
      case (0x15 << OSC_CONTROL_INTERVAL_BITS):                                     break;
      case (0x16 << OSC_CONTROL_INTERVAL_BITS): update_chorus_lfo_0th();            break;
      case (0x17 << OSC_CONTROL_INTERVAL_BITS): update_chorus_lfo_1st();            break;
      case (0x18 << OSC_CONTROL_INTERVAL_BITS): update_freq_0th<3>();               break;
      case (0x19 << OSC_CONTROL_INTERVAL_BITS): update_freq_1st<3>();               break;
      case (0x1A << OSC_CONTROL_INTERVAL_BITS): update_freq_2nd<3>();               break;
      case (0x1B << OSC_CONTROL_INTERVAL_BITS): update_freq_3rd<3>();               break;
      case (0x1C << OSC_CONTROL_INTERVAL_BITS): update_gate<3>();                   break;
      case (0x1D << OSC_CONTROL_INTERVAL_BITS):                                     break;
      case (0x1E << OSC_CONTROL_INTERVAL_BITS): update_chorus_lfo_2nd();            break;
      case (0x1F << OSC_CONTROL_INTERVAL_BITS): update_chorus_lfo_3rd();            break;
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
    int16_t level_0 = wave_0 * m_osc_gain[0];
    int16_t level_1 = wave_1 * m_osc_gain[1];
    int16_t level_2 = wave_2 * m_osc_gain[2];
    int16_t level_3 = wave_3 * m_osc_gain[3];
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
    } else {     // OSC_WAVEFORM_PUL
      result = g_osc_sq_wave_tables[note_number - NOTE_NUMBER_MIN];
    }
#else
    if (waveform == OSC_WAVEFORM_SAW) {
      result = pgm_read_word(g_osc_saw_wave_tables + (note_number - NOTE_NUMBER_MIN));
    } else {     // OSC_WAVEFORM_PUL
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
    if (m_osc_on[N]) {
      m_pitch_current[N] = m_pitch_target[N] - mul_q15_q8(m_pitch_target[N] - m_pitch_current[N], m_portamento_coef);
    }

    m_pitch_real[N] = (64 << 8) + m_pitch_current[N] + m_pitch_bend_normalized;

    uint8_t coarse = high_byte(m_pitch_real[N]);
    if (coarse <= (NOTE_NUMBER_MIN + 64)) {
      m_pitch_real[N] = ((NOTE_NUMBER_MIN + 64) << 8);
    } else if (coarse >= (NOTE_NUMBER_MAX + 64)) {
      m_pitch_real[N] = ((NOTE_NUMBER_MAX + 64) << 8);
    }
  }

  template <uint8_t N>
  INLINE static void update_freq_1st() {
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
  INLINE static void update_freq_2nd() {
    uint8_t coarse = high_byte(m_pitch_real[N]);
    m_freq_temp[N] = g_osc_freq_table[coarse - NOTE_NUMBER_MIN];
    m_wave_table_temp[N] = get_wave_table(m_waveform, coarse);
  }

  template <uint8_t N>
  INLINE static void update_freq_3rd() {
    uint8_t fine = low_byte(m_pitch_real[N]);
    uint16_t freq_div_2 = (m_freq_temp[N] >> 1);
    int8_t freq_offset = high_sbyte(freq_div_2 * g_osc_tune_table[fine >> (8 - OSC_TUNE_TABLE_STEPS_BITS)]);
    m_freq[N] = m_freq_temp[N] + freq_offset;
    m_wave_table[N] = m_wave_table_temp[N];
  }

  template <uint8_t N>
  INLINE static void update_gate() {
    if (m_osc_on[N]) {
      m_osc_gain[N] = m_osc_level;
    }
    else {
      m_osc_gain[N] = 0;
    }
  }

  INLINE static void update_lfo_1st() {
    if (m_lfo_rate >= 32) {
      m_lfo_rate_actual = (high_byte((m_lfo_rate << 1) *
                                     (m_lfo_rate << 1)) + 2) * 12;
    } else {
      m_lfo_rate_actual = ((m_lfo_rate >> 1) + 2) * 12;
    }
  }

  INLINE static void update_lfo_2nd() {
    m_lfo_phase += m_lfo_rate_actual;
    m_lfo_wave_level = get_lfo_wave_level(m_lfo_phase);
  }

  INLINE static void update_lfo_3rd() {
    uint8_t lfo_depth = m_lfo_depth[0] + m_lfo_depth[1];
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



  INLINE static void update_chorus_lfo_0th() {
    if (m_chorus_delay_time_control < 64) {
      if (m_chorus_depth_control > m_chorus_delay_time_control) {
        m_chorus_depth_control_actual = m_chorus_delay_time_control;
      } else {
        m_chorus_depth_control_actual = m_chorus_depth_control;
      }
    } else {
      if (m_chorus_depth_control > (127 - m_chorus_delay_time_control)) {
        m_chorus_depth_control_actual = (127 - m_chorus_delay_time_control);
      } else {
        m_chorus_depth_control_actual = m_chorus_depth_control;
      }
    }

    m_chorus_lfo_phase += m_chorus_rate_control;
  }

  INLINE static void update_chorus_lfo_1st() {
    m_chorus_lfo_wave_level = get_chorus_lfo_wave_level(m_chorus_lfo_phase);
  }

  INLINE static void update_chorus_lfo_2nd() {
    m_chorus_lfo_level = mul_q15_q8(m_chorus_lfo_wave_level, m_chorus_depth_control_actual);
  }

  INLINE static void update_chorus_lfo_3rd() {
    if (m_chorus_mode_control < 64) {
      m_chorus_delay_time = 0;
    } else {
      m_chorus_delay_time = (m_chorus_delay_time_control * 4) + m_chorus_lfo_level;
    }
  }

  INLINE static int16_t get_chorus_lfo_wave_level(uint16_t phase) {
    int16_t triangle_wave_level = 0;
    phase &= 0x1FFC;
    phase = (phase >> 2);

    if (phase < 0x0400) {
      triangle_wave_level = phase - 512;
    } else {
      triangle_wave_level = 1535 - phase;
    }

    return triangle_wave_level;
  }


  INLINE static void update_pitch_bend() {
    int16_t b = m_pitch_bend + 1;
    b >>= 3;
    m_pitch_bend_normalized = (b * m_pitch_bend_range) >> 2;
  }
};

template <uint8_t T> uint8_t         Osc<T>::m_portamento_coef;
template <uint8_t T> int16_t         Osc<T>::m_lfo_mod_level;
template <uint8_t T> uint16_t        Osc<T>::m_lfo_phase;
template <uint8_t T> int8_t          Osc<T>::m_lfo_wave_level;
template <uint8_t T> int16_t         Osc<T>::m_lfo_level;
template <uint8_t T> uint16_t        Osc<T>::m_lfo_rate_actual;
template <uint8_t T> uint8_t         Osc<T>::m_lfo_rate;
template <uint8_t T> uint8_t         Osc<T>::m_lfo_depth[2];

template <uint8_t T> uint8_t         Osc<T>::m_chorus_depth_control;
template <uint8_t T> uint8_t         Osc<T>::m_chorus_rate_control;
template <uint8_t T> uint8_t         Osc<T>::m_chorus_delay_time_control;
template <uint8_t T> uint8_t         Osc<T>::m_chorus_mode_control;
template <uint8_t T> uint8_t         Osc<T>::m_chorus_depth_control_actual;
template <uint8_t T> uint16_t        Osc<T>::m_chorus_lfo_phase;
template <uint8_t T> int16_t         Osc<T>::m_chorus_lfo_wave_level;
template <uint8_t T> int16_t         Osc<T>::m_chorus_lfo_level;
template <uint8_t T> uint16_t        Osc<T>::m_chorus_delay_time;

template <uint8_t T> int8_t          Osc<T>::m_pitch_lfo_amt;
template <uint8_t T> uint8_t         Osc<T>::m_lfo_waveform;
template <uint8_t T> uint8_t         Osc<T>::m_lfo_sampled;
template <uint8_t T> uint8_t         Osc<T>::m_waveform;
template <uint8_t T> int16_t         Osc<T>::m_pitch_bend;
template <uint8_t T> uint8_t         Osc<T>::m_pitch_bend_range;
template <uint8_t T> int16_t         Osc<T>::m_pitch_bend_normalized;
template <uint8_t T> uint16_t        Osc<T>::m_pitch_target[4];
template <uint8_t T> uint16_t        Osc<T>::m_pitch_current[4];
template <uint8_t T> uint16_t        Osc<T>::m_pitch_real[4];
template <uint8_t T> const uint8_t*  Osc<T>::m_wave_table[4];
template <uint8_t T> const uint8_t*  Osc<T>::m_wave_table_temp[4];
template <uint8_t T> uint16_t        Osc<T>::m_freq[4];
template <uint8_t T> uint16_t        Osc<T>::m_freq_temp[4];
template <uint8_t T> uint16_t        Osc<T>::m_phase[4];
template <uint8_t T> boolean         Osc<T>::m_osc_on[4];
template <uint8_t T> uint8_t         Osc<T>::m_osc_gain[4];
template <uint8_t T> uint8_t         Osc<T>::m_osc_level;
