// refs https://www.g200kg.com/archives/2012/10/adsr.html
// refs https://www.g200kg.com/archives/2012/10/adsr2.html
// refs https://www.g200kg.com/archives/2020/07/adsr-1.html

#pragma once

#include "common.h"
#include "mul-q.h"

template <uint8_t T>
class EnvGen {
  static const uint8_t STATE_ATTACK  = 0;
  static const uint8_t STATE_SUSTAIN = 1;
  static const uint8_t STATE_IDLE    = 2;

  static const uint8_t NO_DECAY_UPDATE_COEF = 255;
  static const uint8_t RELEASE_UPDATE_COEF  = 1;

  static uint8_t  m_state;
  static uint16_t m_level;
  static uint8_t  m_level_out;
  static uint8_t  m_attack_update_coef;
  static uint8_t  m_decay_update_coef;
  static uint16_t m_sustain;
  static uint8_t  m_rest;
  static uint8_t  m_gain;
  static uint8_t  m_expression;
  static uint8_t  m_amp_exp_amt;
  static uint8_t  m_gain_coef;

public:
  INLINE static void initialize() {
    m_state = STATE_IDLE;
    m_level = 0;
    m_level_out = 0;
    set_attack(0);
    set_decay(0);
    set_sustain(127);
    set_gain(127);
    set_expression(127);
    set_amp_exp_amt(127);
  }

  INLINE static void set_attack(uint8_t controller_value) {
    if (controller_value == 127) {
      m_attack_update_coef = 64;
    } else {
      m_attack_update_coef = (controller_value + 3) >> 1;
    }
  }

  INLINE static void set_decay(uint8_t controller_value) {
    if (controller_value == 127) {
      m_decay_update_coef = NO_DECAY_UPDATE_COEF;
    } else {
      m_decay_update_coef = (controller_value + 3) >> 1;
    }
  }

  INLINE static void set_sustain(uint8_t controller_value) {
    m_sustain = (((controller_value + 1) >> 1) << 1) << 8;
  }

  INLINE static void set_gain(uint8_t controller_value) {
    m_gain = (controller_value << 1) + 1;
    update_gain_coef();
  }

  // EXPRESSION is processed here (not in Amp) for performance reasons
  INLINE static void set_expression(uint8_t controller_value) {
    m_expression = (controller_value << 1) + 1;
    update_gain_coef();
  }

  INLINE static void set_amp_exp_amt(uint8_t controller_value) {
    m_amp_exp_amt = (controller_value << 1) + 1;
    update_gain_coef();
  }

  INLINE static void note_on() {
    m_state = STATE_ATTACK;
    m_rest = m_attack_update_coef;
  }

  INLINE static void note_off() {
    m_state = STATE_IDLE;
    m_rest = RELEASE_UPDATE_COEF;
  }

  INLINE static uint8_t clock(uint8_t count) {
#if 1
    if ((count & (ENV_GEN_CONTROL_INTERVAL - 1)) == ((T == 0) ? 3 : 11)) {
      //printf("%d EnvGen\n", count);
      switch (m_state) {
      case STATE_ATTACK:
        --m_rest;
        if (m_rest == 0) {
          m_rest = m_attack_update_coef;

          uint8_t coef;
          coef = 188 + m_attack_update_coef;

          m_level = ENV_GEN_LEVEL_MAX_X_1_5 - mul_uq16_uq8(ENV_GEN_LEVEL_MAX_X_1_5 - m_level, coef);
          if (m_level >= ENV_GEN_LEVEL_MAX) {
            m_level = ENV_GEN_LEVEL_MAX;
            m_state = STATE_SUSTAIN;
            m_rest = m_decay_update_coef;
          }
        }
        break;

      case STATE_SUSTAIN:
        --m_rest;
        if (m_rest == 0) {
          m_rest = m_decay_update_coef;

          if ((m_level > m_sustain) && (m_decay_update_coef != NO_DECAY_UPDATE_COEF)) {
            uint8_t coef;
            coef = 188 + m_decay_update_coef;

            m_level = m_sustain + mul_uq16_uq8(m_level - m_sustain, coef);
            if (m_level < m_sustain) {
              m_level = m_sustain;
            }
          }
        }
        break;

      case STATE_IDLE:
        --m_rest;
        if (m_rest == 0) {
          m_rest = RELEASE_UPDATE_COEF;

          if (m_level > 0) {
            uint8_t coef;
            coef = 188 + RELEASE_UPDATE_COEF;

            m_level = mul_uq16_uq8(m_level, coef);
            if (m_level < 0x0100) {
              m_level = 0;
            }
          }
        }
        break;
      }

      if (T == 1) {
        m_level_out = high_byte(high_byte(m_level) * m_gain_coef) << 1;
        if (m_level_out < 4) {
          m_level_out = 0;
        }
      } else {
        m_level_out = high_byte(m_level);
      }
    }
#endif

    return m_level_out;
  }

private:
  INLINE static void update_gain_coef() {
    uint8_t expression = 255 - high_byte(static_cast<uint8_t>(255 - m_expression) * m_amp_exp_amt);
    m_gain_coef = high_byte((high_byte(expression * expression) + 1) * m_gain);
  }
};

template <uint8_t T> uint8_t  EnvGen<T>::m_state;
template <uint8_t T> uint16_t EnvGen<T>::m_level;
template <uint8_t T> uint8_t  EnvGen<T>::m_level_out;
template <uint8_t T> uint8_t  EnvGen<T>::m_attack_update_coef;
template <uint8_t T> uint8_t  EnvGen<T>::m_decay_update_coef;
template <uint8_t T> uint16_t EnvGen<T>::m_sustain;
template <uint8_t T> uint8_t  EnvGen<T>::m_rest;
template <uint8_t T> uint8_t  EnvGen<T>::m_gain;
template <uint8_t T> uint8_t  EnvGen<T>::m_expression;
template <uint8_t T> uint8_t  EnvGen<T>::m_amp_exp_amt;
template <uint8_t T> uint8_t  EnvGen<T>::m_gain_coef;
