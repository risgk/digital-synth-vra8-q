#pragma once

#include "common.h"

template <uint8_t T>
class DelayFx {
  static const uint8_t TODO = 0;

  static uint8_t m_todo;

public:
  INLINE static void initialize() {
  }

  INLINE static int8_t clock(int8_t audio_input) {
    return audio_input;
  }
};

template <uint8_t T> uint8_t DelayFx<T>::m_todo;
