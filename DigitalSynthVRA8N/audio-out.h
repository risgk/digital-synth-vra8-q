// refs http://playground.arduino.cc/Code/PCMAudio

#pragma once

#include "common.h"

template <uint8_t T>
class AudioOut {
#if !defined(SUBSTITUTE_PIN_D5_FOR_D6_AS_AUDIO_OUT)
  static const int AUDIO_OUT_PIN   = 6;   // PD6 (OC0A)
  static const int LFO_LED_OUT_PIN = 5;   // PD5 (OC0B)
#else
  static const int AUDIO_OUT_PIN   = 5;   // PD5 (OC0B)
  static const int LFO_LED_OUT_PIN = 6;   // PD6 (OC0A)
#endif

  static const int CPU_BUSY_LED_OUT_PIN  = 13;  // PB5

  static uint8_t m_count;
  static uint8_t m_maxTcnt;
  static uint8_t m_busy;
  static uint8_t m_busyCont;

public:
  INLINE static void open() {
    pinMode(AUDIO_OUT_PIN, OUTPUT);
    pinMode(CPU_BUSY_LED_OUT_PIN, OUTPUT);
    pinMode(LFO_LED_OUT_PIN, OUTPUT);

    // Timer/Counter0 (8-bit Fast PWM, Inverting, 62500 Hz)
    TCCR0A = 0xF3;
    TCCR0B = 0x01;
#if !defined(SUBSTITUTE_PIN_D5_FOR_D6_AS_AUDIO_OUT)
    OCR0A  = 0x7F;
    OCR0B  = 0xFF;
#else
    OCR0B  = 0x7F;
    OCR0A  = 0xFF;
#endif

    // Timer/Counter1 (9-bit Fast PWM, 31250 Hz)
    TCCR1A = 0x02;
    TCCR1B = 0x09;

    m_count = 0;
    m_maxTcnt = 0;
    m_busy = 0;
    m_busyCont = 0;
  }

  INLINE static void write(int8_t level) {
#if defined(DEBUG)
    // Output Elapsed Time as Channel Pressure (of Channel 16)
    ++m_count;
    if (m_count == 0x7F) {
      UDR0 = 0xDF;
    } else if (m_count == 0xFF) {
#if 0
      uint8_t tcnt = TCNT1 >> 2;
      if ((tcnt < 64) && (tcnt > m_maxTcnt)) {
        m_maxTcnt = tcnt;
      }
      tcnt = m_maxTcnt;
#elif 0
      uint8_t tcnt = m_busyCont;
      tcnt &= 0x7F;
#elif 1
      uint8_t tcnt = TCNT1 >> 2;
      if (tcnt >= 64) {
        tcnt = 99;   // Not Over
      }
#else
      uint8_t tcnt = TCNT1 >> 2;
#endif
      UDR0 = tcnt;
      m_count = 0;
    }
#endif
    if (TIFR1 & _BV(TOV1)) {
      // CPU BUSY
      PORTB = _BV(5);
#if defined(DEBUG)
      if (m_busy) {
        ++m_busyCont;
      }
      m_busy = 1;
#endif
    } else {
      PORTB = 0x00;
#if defined(DEBUG)
      m_busy = 0;
#endif
      while ((TIFR1 & _BV(TOV1)) == 0);
    }
    TIFR1 = _BV(TOV1);
#if !defined(SUBSTITUTE_PIN_D5_FOR_D6_AS_AUDIO_OUT)
    OCR0A = 0x7F - level;
#else
    OCR0B = 0x7F - level;
#endif
  }

  INLINE static void setLFOLed(int8_t level) {
#if defined(ENABLE_LFO_LED_OUT)
  #if !defined(SUBSTITUTE_PIN_D5_FOR_D6_AS_AUDIO_OUT)
    #if (LFO_LED_OUT_ACTIVE == LOW)
      OCR0B = 0x7F - level;
    #else // (LFO_LED_OUT_ACTIVE == HIGH)
      OCR0B = 0x80 + level;
    #endif
  #else
    #if (LFO_LED_OUT_ACTIVE == LOW)
      OCR0A = 0x7F - level;
    #else // (LFO_LED_OUT_ACTIVE == HIGH)
      OCR0A = 0x80 + level;
    #endif
  #endif
#else // !defined(ENABLE_LFO_LED_OUT)
    static_cast<void>(level);
#endif
  }
};

template <uint8_t T> uint8_t AudioOut<T>::m_count;
template <uint8_t T> uint8_t AudioOut<T>::m_maxTcnt;
template <uint8_t T> uint8_t AudioOut<T>::m_busy;
template <uint8_t T> uint8_t AudioOut<T>::m_busyCont;
