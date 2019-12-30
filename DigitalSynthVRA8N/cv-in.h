#pragma once

#include "common.h"

//#define USE_GATE_IN   // Use GATE Signal Input: Active High (V-Trigger, 5V), SCALE MODE is fixed to Linear (5Oct / 5V)

#define USE_CV_0      // CUTOFF       (Connect a potentiometer or a CV input)
#define USE_CV_1      // RESONANCE    (Connect a potentiometer or a CV input)
#define USE_CV_2      // OSC MIX      (Connect a potentiometer or a CV input)  // For MIDI Shield, comment out this line
#define USE_CV_3      // Pitch CV     (Connect a potentiometer or a CV input)  // For MIDI Shield, comment out this line
//#define USE_CV_4      // OSC WAVE     (Connect a potentiometer or a CV input)  // For MIDI Shield, comment out this line
//#define USE_CV_5      // GATE Signal  (Connect a potentiometer or a CV input)  // For MIDI Shield, comment out this line

#define CV_0_ADC_NO (0)
#define CV_1_ADC_NO (1)
#define CV_2_ADC_NO (2)
#define CV_3_ADC_NO (3) // (4)
#define CV_4_ADC_NO (4) // (3)
#define CV_5_ADC_NO (5)

#define ANALOG_INPUT_REVERSED (false)

#define USE_INPUT_D2    // Change the PROGRAM    (Connect a button)
#define USE_INPUT_D4    // Change the SCALE MODE (Connect a button)
#define DIGITAL_INPUT_ACTIVE (HIGH)                                           // LOW for MIDI Shield

#define INPUT_D2_ACTIVE      ((DIGITAL_INPUT_ACTIVE == HIGH) ? _BV(2) : 0)
#define INPUT_D2_INACTIVE    ((DIGITAL_INPUT_ACTIVE == HIGH) ? 0 : _BV(2))
#define INPUT_D4_ACTIVE      ((DIGITAL_INPUT_ACTIVE == HIGH) ? _BV(4) : 0)
#define INPUT_D4_INACTIVE    ((DIGITAL_INPUT_ACTIVE == HIGH) ? 0 : _BV(4))

template <uint8_t T>
class CVIn {
  static const uint8_t CV_IN_CONTROL_INTERVAL_BITS = 1;
  static const uint8_t CV_IN_CONTROL_INTERVAL      = 0x01 << CV_IN_CONTROL_INTERVAL_BITS;

  static const uint8_t DIGITAL_INPUT_ANTICHATTERING_WAIT = 25;

  static const uint8_t SCALE_MODE_0_1_NOTE_NUMBER_MIN = 48;
  static const uint8_t SCALE_MODE_2_NOTE_NUMBER_MID   = 54;

  static uint8_t  m_count;
  static uint16_t m_analog_value[6];
  static uint16_t m_temp_value;
  static uint8_t  m_input_level_d2;
  static uint8_t  m_antichattering_rest_d2;
  static uint8_t  m_input_level_d4;
  static uint8_t  m_antichattering_rest_d4;
  static uint8_t  m_new_note_number;
  static uint8_t  m_note_number;
  static uint8_t  m_program_number;
  static uint8_t  m_scale_mode;

public:
  INLINE static void initialize() {
#if defined(ENABLE_VOLTAGE_CONTROL)
    m_count = 0;
    m_analog_value[CV_0_ADC_NO] = 0;
    m_analog_value[CV_1_ADC_NO] = 0;
    m_analog_value[CV_2_ADC_NO] = 0;
    m_analog_value[CV_3_ADC_NO] = 0;
    m_analog_value[CV_4_ADC_NO] = 0;
    m_analog_value[CV_5_ADC_NO] = 0;
    m_temp_value = 0;
    m_antichattering_rest_d2 = 0;
    m_input_level_d2 = INPUT_D2_INACTIVE;
    m_antichattering_rest_d4 = 0;
    m_input_level_d4 = INPUT_D4_INACTIVE;
    m_new_note_number = SCALE_MODE_0_1_NOTE_NUMBER_MIN;
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
      uint16_t value;
      #if defined(__cpp_static_assert)
        static_assert(CV_IN_CONTROL_INTERVAL_BITS == 1, "CV_IN_CONTROL_INTERVAL_BITS must be 1");
      #endif
      switch (m_count & (0x1F << CV_IN_CONTROL_INTERVAL_BITS)) {

      #if defined(USE_CV_0)
        case (0x00 << CV_IN_CONTROL_INTERVAL_BITS):
          adc_start<CV_0_ADC_NO>();
          break;
      #endif

      #if defined(USE_INPUT_D2)
        case (0x01 << CV_IN_CONTROL_INTERVAL_BITS):
            if (m_antichattering_rest_d2 > 0) {
              --m_antichattering_rest_d2;
            } else {
              value = PIND & _BV(2);    // Read D2
              if (m_input_level_d2 != value) {
                m_input_level_d2 = value;
                m_antichattering_rest_d2 = DIGITAL_INPUT_ANTICHATTERING_WAIT;
                if (value == INPUT_D2_ACTIVE) {
                  if (m_program_number < PROGRAM_NUMBER_MAX) {
                    m_program_number++;
                  } else {
                    m_program_number = PROGRAM_NUMBER_RANDOM_CONTROL;
                  }

                  IVoice<0>::program_change(m_program_number);
                }
              }
            }
          break;
      #endif

      #if defined(USE_INPUT_D4)
        case (0x03 << CV_IN_CONTROL_INTERVAL_BITS):
          if (m_antichattering_rest_d4 > 0) {
            --m_antichattering_rest_d4;
          } else {
            value = PIND & _BV(4);    // Read D4
            if (m_input_level_d4 != value) {
              m_input_level_d4 = value;
              m_antichattering_rest_d4 = DIGITAL_INPUT_ANTICHATTERING_WAIT;
              if (value == INPUT_D4_ACTIVE) {
                if (m_scale_mode == 0) {
                  m_scale_mode = 1;
                } else if (m_scale_mode == 1) {
                  m_scale_mode = 2;
                } else {
                  m_scale_mode = 0;
                }
                set_note_number(NOTE_NUMBER_INVALID);
                IOsc<0>::set_pitch_bend(0);
              }
            }
          }
          break;
      #endif

      #if defined(USE_CV_1)
        case (0x04 << CV_IN_CONTROL_INTERVAL_BITS):
          adc_start<CV_1_ADC_NO>();
          break;
      #endif

      #if defined(USE_CV_0)
        case (0x05 << CV_IN_CONTROL_INTERVAL_BITS):
          value = adc_read();
          if (((m_analog_value[CV_0_ADC_NO] + 1) != value) &&
              ((m_analog_value[CV_0_ADC_NO] - 1) != value)) {
            m_analog_value[CV_0_ADC_NO] = value;
          }
          break;
        case (0x06 << CV_IN_CONTROL_INTERVAL_BITS):
          m_temp_value = static_cast<uint8_t>(m_analog_value[CV_0_ADC_NO] >> 3);
          break;
        case (0x07 << CV_IN_CONTROL_INTERVAL_BITS):
          IVoice<0>::control_change(CUTOFF         , static_cast<uint8_t>(m_temp_value));
          break;
      #endif

      #if defined(USE_CV_2)
        case (0x08 << CV_IN_CONTROL_INTERVAL_BITS):
          adc_start<CV_2_ADC_NO>();
          break;
      #endif

      #if defined(USE_CV_1)
        case (0x09 << CV_IN_CONTROL_INTERVAL_BITS):
          value = adc_read();
          if (((m_analog_value[CV_1_ADC_NO] + 1) != value) &&
              ((m_analog_value[CV_1_ADC_NO] - 1) != value)) {
            m_analog_value[CV_1_ADC_NO] = value;
          }
          break;
        case (0x0A << CV_IN_CONTROL_INTERVAL_BITS):
          m_temp_value = static_cast<uint8_t>(m_analog_value[CV_1_ADC_NO] >> 3);
          break;
        case (0x0B << CV_IN_CONTROL_INTERVAL_BITS):
          IVoice<0>::control_change(RESONANCE      , static_cast<uint8_t>(m_temp_value));
          break;
      #endif

      #if defined(USE_CV_3) && defined(USE_PITCH_CV_IN)
        case (0x0C << CV_IN_CONTROL_INTERVAL_BITS):
          adc_start<CV_3_ADC_NO>();
          break;
      #endif

      #if defined(USE_CV_2)
        case (0x0D << CV_IN_CONTROL_INTERVAL_BITS):
          value = adc_read();
          if (((m_analog_value[CV_2_ADC_NO] + 1) != value) &&
              ((m_analog_value[CV_2_ADC_NO] - 1) != value)) {
            m_analog_value[CV_2_ADC_NO] = value;
          }
          break;
        case (0x0E << CV_IN_CONTROL_INTERVAL_BITS):
          m_temp_value = static_cast<uint8_t>(m_analog_value[CV_2_ADC_NO] >> 3);
          break;
        case (0x0F << CV_IN_CONTROL_INTERVAL_BITS):
          IVoice<0>::control_change(OSC_MIX        , static_cast<uint8_t>(m_temp_value));
          break;
      #endif

      #if defined(USE_CV_3) && defined(USE_PITCH_CV_IN)
        case (0x10 << CV_IN_CONTROL_INTERVAL_BITS):
          value = adc_read();
          if (((m_analog_value[CV_3_ADC_NO] + 1) != value) &&
              ((m_analog_value[CV_3_ADC_NO] - 1) != value)) {
            m_analog_value[CV_3_ADC_NO] = value;
          }
          break;
        case (0x12 << CV_IN_CONTROL_INTERVAL_BITS):
          m_temp_value = (m_analog_value[CV_3_ADC_NO] << 2);
          break;
        case (0x13 << CV_IN_CONTROL_INTERVAL_BITS):
          m_temp_value = (m_temp_value << 2) - 8192;
          break;
      #endif

      #if defined(USE_CV_4)
        case (0x14 << CV_IN_CONTROL_INTERVAL_BITS):
          adc_start<CV_4_ADC_NO>();
          break;
      #endif

      #if defined(USE_CV_3) && defined(USE_PITCH_CV_IN)
        case (0x15 << CV_IN_CONTROL_INTERVAL_BITS):
          #if defined(USE_GATE_IN)
            IOsc<0>::set_pitch_bend(m_temp_value);
          #else
            if ((m_scale_mode == 0) || (m_scale_mode == 1)) {
              m_new_note_number = high_byte((m_analog_value[CV_3_ADC_NO] * 6) + 128) + SCALE_MODE_0_1_NOTE_NUMBER_MIN;
            } else {
              IOsc<0>::set_pitch_bend(m_temp_value);
            }
          #endif
          break;
        case (0x16 << CV_IN_CONTROL_INTERVAL_BITS):
          #if !defined(USE_GATE_IN)
            if (m_scale_mode == 0) {
              static const uint8_t map[] = {
                 0,  0,  2,  2,  4,  5,  5,  7,  7,  9,  9, 11,
                12, 12, 14, 14, 16, 17, 17, 19, 19, 21, 21, 23,
                24,
              };
              m_new_note_number = map[m_new_note_number - SCALE_MODE_0_1_NOTE_NUMBER_MIN] + SCALE_MODE_0_1_NOTE_NUMBER_MIN;
            }
          #endif
          break;
        case (0x17 << CV_IN_CONTROL_INTERVAL_BITS):
          #if !defined(USE_GATE_IN)
            if (m_analog_value[CV_3_ADC_NO] < 3) {
              // 0V: Note OFF
              set_note_number(NOTE_NUMBER_INVALID);
            } else {
              if (m_scale_mode == 0) {
                // SCALE MODE 0: "C Major" (2Oct / 5V)
                set_note_number(m_new_note_number);
              } else if (m_scale_mode == 1) {
                // SCALE MODE 1: "Chromatic" (2Oct / 5V)
                set_note_number(m_new_note_number);
              } else {
                // SCALE MODE 2: "Linear" (5Oct / 5V)
                set_note_number(SCALE_MODE_2_NOTE_NUMBER_MID);
              }
            }
          #endif
          break;
      #endif

      #if defined(USE_CV_5) && defined(USE_PITCH_CV_IN) && defined(USE_GATE_IN)
        case (0x18 << CV_IN_CONTROL_INTERVAL_BITS):
          adc_start<CV_5_ADC_NO>();
          break;
      #endif

      #if defined(USE_CV_4)
        case (0x19 << CV_IN_CONTROL_INTERVAL_BITS):
          value = adc_read();
          if (((m_analog_value[CV_4_ADC_NO] + 1) != value) &&
              ((m_analog_value[CV_4_ADC_NO] - 1) != value)) {
            m_analog_value[CV_4_ADC_NO] = value;
          }
          break;
        case (0x1A << CV_IN_CONTROL_INTERVAL_BITS):
          m_temp_value = static_cast<uint8_t>(m_analog_value[CV_4_ADC_NO] >> 3);
          break;
        case (0x1B << CV_IN_CONTROL_INTERVAL_BITS):
          IVoice<0>::control_change(OSC1_2_WAVE    , static_cast<uint8_t>(m_temp_value));
          break;
      #endif

      #if defined(USE_CV_5) && defined(USE_PITCH_CV_IN) && defined(USE_GATE_IN)
        case (0x1D << CV_IN_CONTROL_INTERVAL_BITS):
          value = adc_read();
          if (((m_analog_value[CV_5_ADC_NO] + 1) != value) &&
              ((m_analog_value[CV_5_ADC_NO] - 1) != value)) {
            m_analog_value[CV_5_ADC_NO] = value;
          }
          break;
        case (0x1E << CV_IN_CONTROL_INTERVAL_BITS):
          m_temp_value = static_cast<uint8_t>(m_analog_value[CV_5_ADC_NO] >> 3);
          break;
        case (0x1F << CV_IN_CONTROL_INTERVAL_BITS):
          // GATE Signal Input: Active High (V-Trigger, 5V)
          if (m_analog_value[CV_5_ADC_NO] < 512) {
            set_note_number(NOTE_NUMBER_INVALID);
          } else {
            set_note_number(SCALE_MODE_2_NOTE_NUMBER_MID);
          }
          break;
      #endif

      }
    }
#endif
  }

private:
  // Start the conversion.
  template <uint8_t ADC_NO>
  INLINE static void adc_start() {
#if defined(ENABLE_VOLTAGE_CONTROL)
    ADMUX = _BV(REFS0) | ADC_NO;  // analogReference(DEFAULT)
    ADCSRA = _BV(ADEN) | _BV(ADSC) | 0b111;
#endif
  }

  // CAUTION: Call after the conversion is finishd.
  INLINE static uint16_t adc_read(bool reversed = false) {
#if defined(ENABLE_VOLTAGE_CONTROL)
    uint8_t adcLow  = ADCL;
    uint8_t adcHigh = ADCH;
    uint16_t adc = ((adcHigh << 8) | adcLow);

    if (ANALOG_INPUT_REVERSED != reversed) {
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
template <uint8_t T> uint16_t CVIn<T>::m_analog_value[6];
template <uint8_t T> uint16_t CVIn<T>::m_temp_value;
template <uint8_t T> uint8_t  CVIn<T>::m_input_level_d2;
template <uint8_t T> uint8_t  CVIn<T>::m_antichattering_rest_d2;
template <uint8_t T> uint8_t  CVIn<T>::m_input_level_d4;
template <uint8_t T> uint8_t  CVIn<T>::m_antichattering_rest_d4;
template <uint8_t T> uint8_t  CVIn<T>::m_new_note_number;
template <uint8_t T> uint8_t  CVIn<T>::m_note_number;
template <uint8_t T> uint8_t  CVIn<T>::m_program_number;
template <uint8_t T> uint8_t  CVIn<T>::m_scale_mode;
