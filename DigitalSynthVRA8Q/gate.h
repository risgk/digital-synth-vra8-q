#pragma once

#include "common.h"
#include "mul-q.h"

template <uint8_t T>
class Gate {
  static const uint8_t STATE_OFF = 0;
  static const uint8_t STATE_ON  = 48;

  static uint8_t m_state;
  static uint8_t m_level;

public:
  INLINE static void initialize() {
    m_state = STATE_OFF;
    m_level = 0;
  }

  INLINE static void note_on() {
    m_state = STATE_ON;
  }

  INLINE static void note_off() {
    m_state = STATE_OFF;
  }

  INLINE static uint8_t clock(uint8_t count) {
    return m_state; // TENTATIVE CODE
#if 1
    if ((count & (GATE_CONTROL_INTERVAL - 1)) == ((T * 4) + 1)) {
      //printf("%d Gate\n", count);
      if (m_state == STATE_ON) {
        if (m_level < GATE_LEVEL_MAX) {
          m_level += (GATE_LEVEL_MAX >> 2);
        }
      } else {
        if (m_level > 0) {
          m_level -= (GATE_LEVEL_MAX >> 2);
        }
      }
    }
#endif

    return m_level;
  }
};

template <uint8_t T> uint8_t Gate<T>::m_state;
template <uint8_t T> uint8_t Gate<T>::m_level;
