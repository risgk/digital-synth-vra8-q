#pragma once

// refs http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt

#include "common.h"
#include "mul-q.h"
#include "filter-table.h"

template <uint8_t T>
class Filter {
  static const uint16_t* m_lpf_table;
  static int16_t         m_b_2_over_a_0;
  static int16_t         m_a_1_over_a_0;
  static int16_t         m_a_2_over_a_0;
  static int16_t         m_x_1;
  static int16_t         m_x_2;
  static int16_t         m_y_1;
  static int16_t         m_y_2;
  static uint8_t         m_cutoff_current;
  static int16_t         m_cutoff_candidate;
  static uint8_t         m_cutoff;
  static int8_t          m_cutoff_env_gen_amt;
  static int8_t          m_cutoff_lfo_amt;
  static int8_t          m_cutoff_pitch_amt;
  static uint8_t         m_cutoff_expression_decrease;
  static int8_t          m_cutoff_exp_amt;
  static uint8_t         m_resonance;
  static uint8_t         m_resonance_limit;

  static const uint8_t AUDIO_FRACTION_BITS = 14;
  static const int16_t MAX_ABS_OUTPUT = ((124 << (AUDIO_FRACTION_BITS - 8)) >> 8) << 8;

public:
  INLINE static void initialize() {
    m_b_2_over_a_0 = 0;
    m_a_1_over_a_0 = 0;
    m_a_2_over_a_0 = 0;
    m_x_1 = 0;
    m_x_2 = 0;
    m_y_1 = 0;
    m_y_2 = 0;
    m_cutoff_current = 127;

    set_cutoff(127);
    set_resonance_limit(127);
    set_resonance(0);
    set_cutoff_env_amt(64);
    set_cutoff_lfo_amt(64);
    set_cutoff_exp_amt(0);
    set_expression(127);

    update_coefs_0th(0);
    update_coefs_1st(0);
    update_coefs_2nd();
    update_coefs_3rd();
  }

  INLINE static void set_cutoff(uint8_t controller_value) {
    uint8_t value = controller_value;
    if (value < 16) {
      value = 16;
    } else if (112 < value) {
      value = 112;
    }

    m_cutoff = value;
  }

  INLINE static void set_resonance(uint8_t controller_value) {
    m_resonance = controller_value;
    update_resonance();
  }

  INLINE static void set_resonance_limit(uint8_t controller_value) {
    m_resonance_limit = controller_value;
    update_resonance();
  }

  INLINE static void set_cutoff_env_amt(uint8_t controller_value) {
    uint8_t value = controller_value;
    if (value < 16) {
      value = 16;
    } else if (112 < value) {
      value = 112;
    }

    m_cutoff_env_gen_amt = (value - 64) << 1;
  }

  INLINE static void set_cutoff_lfo_amt(uint8_t controller_value) {
    uint8_t value = controller_value;
    if (value < 16) {
      value = 16;
    } else if (112 < value) {
      value = 112;
    }

    m_cutoff_lfo_amt = (value - 64) << 1;
  }

  INLINE static void set_cutoff_pitch_amt(uint8_t controller_value) {
    if (controller_value < 32) {
      m_cutoff_pitch_amt = 0;
    } else if (controller_value < 96) {
      m_cutoff_pitch_amt = 1;
    } else {
      m_cutoff_pitch_amt = 2;
    }
  }

  INLINE static void set_cutoff_exp_amt(uint8_t controller_value) {
    uint8_t value = controller_value;
    if (value < 16) {
      value = 16;
    } else if (112 < value) {
      value = 112;
    }

    m_cutoff_exp_amt = (value - 64) << 1;
  }

  INLINE static void set_expression(uint8_t controller_value) {
    m_cutoff_expression_decrease = 254 - (controller_value << 1);
  }

  INLINE static int16_t clock(uint8_t count, int16_t audio_input, uint8_t env_gen_input, int16_t lfo_input) {
    if ((count & (FILTER_CONTROL_INTERVAL - 1)) == 6) {
      #if defined(__cpp_static_assert)
        static_assert(FILTER_CONTROL_INTERVAL_BITS == 3, "FILTER_CONTROL_INTERVAL_BITS must be 3");
      #endif
      if (count & 0x10) {
        if (count & 0x08) {
          update_coefs_3rd();
        } else {
          update_coefs_2nd();
        }
      } else {
        if (count & 0x08) {
          update_coefs_1st(lfo_input);
        } else {
          update_coefs_0th(env_gen_input);
        }
      }
    }

    int16_t x_0  = audio_input >> (16 - AUDIO_FRACTION_BITS);
    int16_t tmp  = mul_q15_q15(x_0 + (m_x_1 << 1) + m_x_2, m_b_2_over_a_0);
    tmp         -= mul_q15_q15(m_y_1,                      m_a_1_over_a_0);
    tmp         -= mul_q15_q15(m_y_2,                      m_a_2_over_a_0);
    int16_t y_0  = tmp << (16 - FILTER_TABLE_FRACTION_BITS);

    if (high_sbyte(y_0) > high_sbyte(MAX_ABS_OUTPUT)) {
      y_0 = MAX_ABS_OUTPUT;
    }
    if (high_sbyte(y_0) < high_sbyte(-MAX_ABS_OUTPUT)) {
      y_0 = -MAX_ABS_OUTPUT;
    }

    m_x_2 = m_x_1;
    m_y_2 = m_y_1;
    m_x_1 = x_0;
    m_y_1 = y_0;

    return y_0 << (16 - AUDIO_FRACTION_BITS);
  }

private:
  INLINE static void update_coefs_0th(uint8_t env_gen_input) {
    m_cutoff_candidate = m_cutoff;
    m_cutoff_candidate -= high_sbyte(m_cutoff_exp_amt * m_cutoff_expression_decrease);
    m_cutoff_candidate += high_sbyte((m_cutoff_env_gen_amt * env_gen_input) << 1);
  }

  INLINE static void update_coefs_1st(int16_t lfo_input) {
    int8_t lfo_mod = high_sbyte(mul_q15_q7(lfo_input, m_cutoff_lfo_amt) << 1);
    m_cutoff_candidate -= lfo_mod;

    // OSC Pitch is processed here (not in Voice) for performance reasons
    uint16_t osc_pitch = IOsc<0>::get_osc_pitch();
    if (m_cutoff_pitch_amt == 1) {
      m_cutoff_candidate += static_cast<int8_t>(high_byte(osc_pitch + 128) - 60);
    } else if (m_cutoff_pitch_amt == 2) {
      m_cutoff_candidate += static_cast<int8_t>(high_byte((osc_pitch << 1) + 128) - 120);
    }
  }

  INLINE static void update_coefs_2nd() {
    uint8_t cutoff_target;
    if (m_cutoff_candidate > 127) {
      cutoff_target = 127;
    } else if (m_cutoff_candidate < 0) {
      cutoff_target = 0;
    } else {
      cutoff_target = m_cutoff_candidate;
    }

    if (m_cutoff_current + FILTER_CUTOFF_THROUGH_RATE < cutoff_target) {
      m_cutoff_current += FILTER_CUTOFF_THROUGH_RATE;
    } else if (m_cutoff_current > cutoff_target + FILTER_CUTOFF_THROUGH_RATE) {
      m_cutoff_current -= FILTER_CUTOFF_THROUGH_RATE;
    } else {
      m_cutoff_current = cutoff_target;
    }
  }

  INLINE static void update_coefs_3rd() {
    const uint16_t* p = m_lpf_table + static_cast<uint8_t>(m_cutoff_current << 1);
    m_b_2_over_a_0 = pgm_read_word(p);
    p++;
    m_a_1_over_a_0 = pgm_read_word(p);
    m_a_2_over_a_0 = (m_b_2_over_a_0 << 2) - m_a_1_over_a_0 -
                     (1 << FILTER_TABLE_FRACTION_BITS);
  }

  INLINE static void update_resonance() {
    uint8_t controller_value = m_resonance;
    if (controller_value > m_resonance_limit) {
      controller_value = m_resonance_limit;
    }

    uint8_t index = (controller_value + 4) >> 4;
    m_lpf_table = g_filter_lpf_tables[index];
  }
};

template <uint8_t T> const uint16_t* Filter<T>::m_lpf_table;
template <uint8_t T> int16_t         Filter<T>::m_b_2_over_a_0;
template <uint8_t T> int16_t         Filter<T>::m_a_1_over_a_0;
template <uint8_t T> int16_t         Filter<T>::m_a_2_over_a_0;
template <uint8_t T> int16_t         Filter<T>::m_x_1;
template <uint8_t T> int16_t         Filter<T>::m_x_2;
template <uint8_t T> int16_t         Filter<T>::m_y_1;
template <uint8_t T> int16_t         Filter<T>::m_y_2;
template <uint8_t T> uint8_t         Filter<T>::m_cutoff_current;
template <uint8_t T> int16_t         Filter<T>::m_cutoff_candidate;
template <uint8_t T> uint8_t         Filter<T>::m_cutoff;
template <uint8_t T> int8_t          Filter<T>::m_cutoff_env_gen_amt;
template <uint8_t T> int8_t          Filter<T>::m_cutoff_lfo_amt;
template <uint8_t T> int8_t          Filter<T>::m_cutoff_pitch_amt;
template <uint8_t T> uint8_t         Filter<T>::m_cutoff_expression_decrease;
template <uint8_t T> int8_t          Filter<T>::m_cutoff_exp_amt;
template <uint8_t T> uint8_t         Filter<T>::m_resonance;
template <uint8_t T> uint8_t         Filter<T>::m_resonance_limit;
