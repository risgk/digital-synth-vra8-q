#pragma once

#include "common.h"
//#include <stdio.h>

template <uint8_t T>
class DelayFx {
  static const uint8_t TODO = 0;

  static uint8_t  m_buff[1024];
  static uint16_t m_wp;

public:
  INLINE static void initialize() {
    for (uint16_t i = 0; i < 1024; ++i) {
      m_buff[i] = 0;
    }
    m_wp = 1023;
  }

  INLINE static void push(int8_t audio_input) {
    m_wp = (m_wp + 1) & 0x03FF;
    m_buff[m_wp] = audio_input;
//printf("%u\n",m_wp);
  }

  INLINE static int8_t get(uint16_t sample_delay) {
    uint16_t rp = (m_wp - sample_delay) & 0x03FF;
    return m_buff[rp];
  }
};

template <uint8_t T> uint8_t  DelayFx<T>::m_buff[1024];
template <uint8_t T> uint16_t DelayFx<T>::m_wp;
