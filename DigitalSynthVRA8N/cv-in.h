#pragma once

#include "common.h"

#define USE_INPUT_A0    // CUTOFF    (Connect a potentiometer or a CV input)
#define USE_INPUT_A1    // RESONANCE (Connect a potentiometer or a CV input)
#define USE_INPUT_A2    // OSC MIX   (Connect a potentiometer or a CV input)  // For MIDI Shield, comment out this line
#define USE_INPUT_A3    // PITCH     (Connect a potentiometer or a CV input)  // For MIDI Shield, comment out this line
#define ANALOG_INPUT_REVERSED (false)

#define USE_INPUT_D2    // Change the PROGRAM    (Connect a button)
#define USE_INPUT_D4    // Change the SCALE MODE (Connect a button)
#define DIGITAL_INPUT_ACTIVE (HIGH)                                           // LOW for MIDI Shield

template <uint8_t T>
class CVIn {
  static const uint8_t CV_IN_CONTROL_INTERVAL_BITS = 1;
  static const uint8_t CV_IN_CONTROL_INTERVAL      = 0x01 << CV_IN_CONTROL_INTERVAL_BITS;

  static const uint8_t DIGITAL_INPUT_ANTICHATTERING_WAIT = 25;

  static const uint8_t SCALE_MODE_0_NOTE_NUMBER_MIN = 48;
  static const uint8_t SCALE_MODE_1_NOTE_NUMBER_MID = 54;

  static uint8_t  m_count;
  static uint16_t m_analog_value[4];
  static uint8_t  m_input_level_d2;
  static uint8_t  m_antichattering_rest_d2;
  static uint8_t  m_input_level_d4;
  static uint8_t  m_antichattering_rest_d4;
  static uint8_t  m_note_number;
  static uint8_t  m_program_number;
  static uint8_t  m_scale_mode;

public:
  INLINE static void initialize() {
#if defined(ENABLE_VOLTAGE_CONTROL)
    m_count = 0;
    m_analog_value[0] = 0;
    m_analog_value[1] = 0;
    m_analog_value[2] = 0;
    m_analog_value[3] = 0;
    m_antichattering_rest_d2 = 0;
    m_input_level_d2 = (DIGITAL_INPUT_ACTIVE == HIGH) ? LOW : HIGH;
    m_antichattering_rest_d4 = 0;
    m_input_level_d4 = (DIGITAL_INPUT_ACTIVE == HIGH) ? LOW : HIGH;
    m_note_number = NOTE_NUMBER_INVALID;
    m_program_number = PROGRAM_NUMBER_DEFAULT;
    m_scale_mode = 0;

  #if defined(USE_INPUT_D2)
    pinMode(2, INPUT);
  #endif
  #if defined(USE_INPUT_D4)
    pinMode(4, INPUT);
  #endif
#endif
  }

  INLINE static void clock() {
#if defined(ENABLE_VOLTAGE_CONTROL)
    ++m_count;

    if ((m_count & (CV_IN_CONTROL_INTERVAL - 1)) == 1) {
      uint8_t idx = (m_count >> CV_IN_CONTROL_INTERVAL_BITS) & 0x1F;

      uint16_t value;
      switch (idx) {
      case 0x0:
  #if defined(USE_INPUT_A0)
        adc_start<0>();
  #endif
        break;
      case 0x4:
  #if defined(USE_INPUT_A0)
        value = adc_read();    // Read A0
        if (((m_analog_value[0] + 1) != value) &&
            ((m_analog_value[0] - 1) != value)) {
          m_analog_value[0] = value;
        }
  #endif
  #if defined(USE_INPUT_A1)
        adc_start<1>();
  #endif
        break;
      case 0x6:
  #if defined(USE_INPUT_A0)
        IVoice<0>::control_change(FILTER_CUTOFF, m_analog_value[0] >> 3);
  #endif
        break;
      case 0x8:
  #if defined(USE_INPUT_A1)
        value = adc_read();    // Read A1
        if (((m_analog_value[1] + 1) != value) &&
            ((m_analog_value[1] - 1) != value)) {
          m_analog_value[1] = value;
        }
  #endif
  #if defined(USE_INPUT_A2)
        adc_start<2>();
  #endif
        break;
      case 0xA:
  #if defined(USE_INPUT_A1)
        IVoice<0>::control_change(FILTER_RESO, m_analog_value[1] >> 3);
  #endif
  #if defined(USE_INPUT_A2)
        adc_start<2>();
  #endif
        break;
      case 0xC:
  #if defined(USE_INPUT_A2)
        value = adc_read();    // Read A2
        if (((m_analog_value[2] + 1) != value) &&
            ((m_analog_value[2] - 1) != value)) {
          m_analog_value[2] = value;
        }
  #endif
  #if defined(USE_INPUT_A3)
        adc_start<3>();
  #endif
        break;
      case 0xE:
  #if defined(USE_INPUT_A2)
        IVoice<0>::control_change(OSC_MIX, m_analog_value[2] >> 3);
  #endif
  #if defined(USE_INPUT_A3)
        adc_start<3>();
  #endif
        break;
      case 0x10:
  #if defined(USE_INPUT_A3)
        value = adc_read();    // Read A3
        if (((m_analog_value[3] + 1) != value) &&
            ((m_analog_value[3] - 1) != value)) {
          m_analog_value[3] = value;
        }
  #endif
        break;
      case 0x12:
  #if defined(USE_INPUT_A3)
    #if defined(USE_PITCH_CV_IN)
        if (m_analog_value[3] < 3) {
          // 0V: Note OFF
          set_note_number(NOTE_NUMBER_INVALID);
        } else {
          if (m_scale_mode == 0) {
            // Chromatic (2Oct / 5V)
            set_note_number(high_byte((m_analog_value[3] * 6) + 128) + SCALE_MODE_0_NOTE_NUMBER_MIN);
          } else {
            // Linear (5Oct / 5V)
            IOsc<0>::set_pitch_bend((m_analog_value[3] << 4) - 8192);
            set_note_number(SCALE_MODE_1_NOTE_NUMBER_MID);
          }
        }
    #endif
  #endif
        break;
      case 0x14:
  #if defined(USE_INPUT_D2)
        if (m_antichattering_rest_d2 > 0) {
          --m_antichattering_rest_d2;
        } else {
          value = digitalRead(2);    // Read D2
          if (m_input_level_d2 != value) {
            m_input_level_d2 = value;
            m_antichattering_rest_d2 = DIGITAL_INPUT_ANTICHATTERING_WAIT;
            if (value == DIGITAL_INPUT_ACTIVE) {
              if (m_program_number < PROGRAM_NUMBER_MAX) {
                m_program_number++;
              } else {
                m_program_number = PROGRAM_NUMBER_RANDOM_CONTROL;
              }

              IVoice<0>::program_change(m_program_number);
            }
          }
        }
  #endif
        break;
      case 0x18:
  #if defined(USE_INPUT_D4)
        if (m_antichattering_rest_d4 > 0) {
          --m_antichattering_rest_d4;
        } else {
          value = digitalRead(4);    // Read D4
          if (m_input_level_d4 != value) {
            m_input_level_d4 = value;
            m_antichattering_rest_d4 = DIGITAL_INPUT_ANTICHATTERING_WAIT;
            if (value == DIGITAL_INPUT_ACTIVE) {
              if (m_scale_mode == 0) {
                m_scale_mode = 1;
              } else {
                m_scale_mode = 0;
              }
              set_note_number(NOTE_NUMBER_INVALID);
              IOsc<0>::set_pitch_bend(0);
            }
          }
        }
  #endif
        break;
      case 0x1C:
        break;
      }
    }
#endif
  }

private:
  // Start the conversion.
  template <uint8_t N>
  INLINE static void adc_start() {
#if defined(ENABLE_VOLTAGE_CONTROL)
    ADMUX = _BV(REFS0) | N;  // analogReference(DEFAULT)
    ADCSRA = _BV(ADEN) | _BV(ADSC) | 0b111;
#endif
  }

  // CAUTION: Call after the conversion is finishd.
  INLINE static uint16_t adc_read() {
#if defined(ENABLE_VOLTAGE_CONTROL)
    uint8_t adcLow  = ADCL;
    uint8_t adcHigh = ADCH;
    uint16_t adc = ((adcHigh << 8) | adcLow);

    if (ANALOG_INPUT_REVERSED) {
      return 1023 - adc;
    }
#else
    uint16_t adc = 0;
#endif
    return adc;
  }

  INLINE static void set_note_number(uint8_t note_number) {
#if defined(ENABLE_VOLTAGE_CONTROL)
    if (m_note_number != note_number) {
      if (note_number != NOTE_NUMBER_INVALID) {
        IVoice<0>::note_on(note_number, 127);
      }
      IVoice<0>::note_off(m_note_number);
      m_note_number = note_number;
    }
#endif
  }
};

template <uint8_t T> uint8_t  CVIn<T>::m_count;
template <uint8_t T> uint16_t CVIn<T>::m_analog_value[4];
template <uint8_t T> uint8_t  CVIn<T>::m_input_level_d2;
template <uint8_t T> uint8_t  CVIn<T>::m_antichattering_rest_d2;
template <uint8_t T> uint8_t  CVIn<T>::m_input_level_d4;
template <uint8_t T> uint8_t  CVIn<T>::m_antichattering_rest_d4;
template <uint8_t T> uint8_t  CVIn<T>::m_note_number;
template <uint8_t T> uint8_t  CVIn<T>::m_program_number;
template <uint8_t T> uint8_t  CVIn<T>::m_scale_mode;
