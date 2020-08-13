#pragma once

#include "common.h"
#include "mul-q.h"

template <uint8_t T>
class Gate {
  static const uint8_t STATE_OFF = 0;
  static const uint8_t STATE_ON  = 1;

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
#if 1
    uint8_t match_count = 1;
    switch (T) {
    case 0:
      match_count = 1;
      break;
    case 1:
      match_count = 5;
      break;
    case 2:
      match_count = 9;
      break;
    case 3:
      match_count = 13;
      break;
    }

    if ((count & (GATE_CONTROL_INTERVAL - 1)) == match_count) {
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
