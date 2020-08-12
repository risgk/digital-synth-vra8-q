#pragma once

#include "common.h"
#include "program-table.h"

template <uint8_t T>
class Voice {
  static uint8_t m_count;
  static uint8_t m_last_note_number;
  static uint8_t m_on_note[16];
  static uint8_t m_output_error;
  static uint8_t m_portamento;
  static uint8_t m_attack;
  static uint8_t m_decay;
  static uint8_t m_sustain;
  static uint8_t m_release;
  static uint8_t m_amp_env_gen;
  static uint16_t m_rnd;
  static uint8_t m_sp_prog_chg_cc_values[8];

public:
  INLINE static void initialize() {
    m_count = 0;
    m_last_note_number = NOTE_NUMBER_INVALID;
    for (uint8_t i = 0; i < 16; ++i) {
      m_on_note[i] = 0x00;
    }
    m_output_error = 0;
    m_portamento = 0;
    IOsc<0>::initialize();
    IFilter<0>::initialize();
    IAmp<0>::initialize();
    IEnvGen<0>::initialize();
    IEnvGen<1>::initialize();
    IDelayFx<0>::initialize();
    m_attack = 0;
    m_decay = 0;
    m_sustain = 127;
    m_release = 127;
    m_amp_env_gen = 127;
    update_env_gen();
    m_rnd = 1;
  }

  INLINE
  static void note_on(uint8_t note_number, uint8_t /* velocity */) {
    IOsc<0>::set_portamento(0);
    {
      IOsc<0>::trigger_lfo();
      IEnvGen<0>::note_on();
      IEnvGen<1>::note_on();
    }

    set_on_note(note_number);
    m_last_note_number = note_number;
    IOsc<0>::note_on<0>(m_last_note_number + 0);
    IOsc<0>::note_on<1>(m_last_note_number + 5);
    IOsc<0>::note_on<2>(m_last_note_number + 7);
    IOsc<0>::note_on<3>(m_last_note_number + 12);
  }

  INLINE
  static void note_off(uint8_t note_number) {
    clear_on_note(note_number);
    if (m_last_note_number == note_number) {
      all_note_off();
    }
  }

  INLINE
  static void all_note_off() {
    for (uint8_t i = 0; i < 16; ++i) {
      m_on_note[i] = 0x00;
    }
    m_last_note_number = NOTE_NUMBER_INVALID;
    IOsc<0>::note_off<0>();
    IOsc<0>::note_off<1>();
    IEnvGen<0>::note_off();
    IEnvGen<1>::note_off();
  }

  INLINE static void control_change(uint8_t controller_number, uint8_t controller_value) {
    switch (controller_number) {
    case EXPRESSION     :
      IFilter<0>::set_expression(controller_value);
      IEnvGen<1>::set_expression(controller_value);
      break;
    case MODULATION     :
      IOsc<0>::set_lfo_depth<1>(controller_value);
      break;

    case CUTOFF         :
      IFilter<0>::set_cutoff(controller_value);
      break;
    case RESONANCE      :
      IFilter<0>::set_resonance(controller_value);
      break;
    case EG_TO_CUTOFF   :
      IFilter<0>::set_cutoff_env_amt(controller_value);
      break;
    case DECAY          :
      m_decay = controller_value;
      update_decay_release();
      break;

    case ATTACK         :
      m_attack = controller_value;
      if (m_amp_env_gen >= 64) {
        IEnvGen<0>::set_attack(m_attack);
        IEnvGen<1>::set_attack(m_attack);
      } else {
        IEnvGen<0>::set_attack(m_attack);
      }
      break;

    case OSC_WAVE       :
      IOsc<0>::set_osc_waveforms(controller_value);
      break;

    case SUSTAIN        :
      {
        m_sustain = controller_value;

        if (m_amp_env_gen >= 64) {
          IEnvGen<0>::set_sustain(m_sustain);
          IEnvGen<1>::set_sustain(m_sustain);
        } else {
          IEnvGen<0>::set_sustain(m_sustain);
        }
      }
      break;

    case AMP_EG         :
      m_amp_env_gen = controller_value;
      update_env_gen();
      break;

    case LFO_RATE       :
      IOsc<0>::set_lfo_rate(controller_value);
      break;
    case LFO_DEPTH      :
      IOsc<0>::set_lfo_depth<0>(controller_value);
      break;
    case LFO_TO_PITCH   :
      IOsc<0>::set_pitch_lfo_amt<0>(controller_value);
      break;
    case LFO_TO_CUTOFF  :
      IFilter<0>::set_cutoff_lfo_amt(controller_value);
      break;

    case CHORUS_DEPTH   :
      IOsc<0>::set_chorus_depth(controller_value);
      break;
    case CHORUS_RATE    :
      IOsc<0>::set_chorus_rate(controller_value);
      break;
    case CHORUS_DELAY_T :
      IOsc<0>::set_chorus_delay_time(controller_value);
      break;
    case CHORUS_MODE    :
      IOsc<0>::set_chorus_mode(controller_value);
      break;

    case P_BEND_RANGE   :
      IOsc<0>::set_pitch_bend_minus_range(controller_value);
      IOsc<0>::set_pitch_bend_plus_range(controller_value);
      break;

    case EG_TO_PITCH    :
      IOsc<0>::set_pitch_eg_amt(controller_value);
      break;

    case ALL_NOTES_OFF  :
    case OMNI_MODE_OFF  :
    case OMNI_MODE_ON   :
    case MONO_MODE_ON   :
    case POLY_MODE_ON   :
      all_note_off();
      break;

#if defined(ENABLE_SPECIAL_PROGRAM_CHANGE)
    // Special Program Change
    case SP_PROG_CHG_0  :
    case SP_PROG_CHG_1  :
    case SP_PROG_CHG_2  :
    case SP_PROG_CHG_3  :
    case SP_PROG_CHG_4  :
    case SP_PROG_CHG_5  :
    case SP_PROG_CHG_6  :
    case SP_PROG_CHG_7  :
      {
        uint8_t program_number = controller_number - SP_PROG_CHG_0;
        uint8_t old_value = m_sp_prog_chg_cc_values[program_number];
        m_sp_prog_chg_cc_values[program_number] = controller_value;
        if ((old_value <= 63) && (controller_value >= 64)) {
          program_change(program_number);
        }
      }
      break;
#endif
    }
  }

  INLINE static void pitch_bend(uint8_t lsb, uint8_t msb) {
    int16_t pitch_bend = ((static_cast<uint16_t>(msb) << 8) >> 1) + lsb - 8192;
    IOsc<0>::set_pitch_bend(pitch_bend);
  }

  static void program_change(uint8_t program_number) {
    {
      if (program_number > PROGRAM_NUMBER_MAX) {
        return;
      }

      control_change(OSC_WAVE       , g_preset_table_OSC_WAVE       [program_number]);
      control_change(EG_TO_PITCH    , g_preset_table_EG_TO_PITCH    [program_number]);

      control_change(CUTOFF         , g_preset_table_CUTOFF         [program_number]);
      control_change(RESONANCE      , g_preset_table_RESONANCE      [program_number]);
      control_change(EG_TO_CUTOFF   , g_preset_table_EG_TO_CUTOFF   [program_number]);
      control_change(AMP_EG         , g_preset_table_AMP_EG         [program_number]);

      control_change(ATTACK         , g_preset_table_ATTACK         [program_number]);
      control_change(DECAY          , g_preset_table_DECAY          [program_number]);
      control_change(SUSTAIN        , g_preset_table_SUSTAIN        [program_number]);

      control_change(LFO_DEPTH      , g_preset_table_LFO_DEPTH      [program_number]);
      control_change(LFO_RATE       , g_preset_table_LFO_RATE       [program_number]);
      control_change(LFO_TO_PITCH   , g_preset_table_LFO_TO_PITCH   [program_number]);
      control_change(LFO_TO_CUTOFF  , g_preset_table_LFO_TO_CUTOFF  [program_number]);

      control_change(CHORUS_DEPTH   , g_preset_table_CHORUS_DEPTH   [program_number]);
      control_change(CHORUS_RATE    , g_preset_table_CHORUS_RATE    [program_number]);
      control_change(CHORUS_DELAY_T , g_preset_table_CHORUS_DELAY_T [program_number]);
      control_change(CHORUS_MODE    , g_preset_table_CHORUS_MODE    [program_number]);

      control_change(P_BEND_RANGE   , g_preset_table_P_BEND_RANGE   [program_number]);
    }
  }

  INLINE static int8_t clock(int8_t& right_level) {
    ++m_count;

    uint8_t env_gen_output_0 = IEnvGen<0>::clock(m_count);
    int16_t osc_output = IOsc<0>::clock(m_count, env_gen_output_0);
    int16_t lfo_output = IOsc<0>::get_lfo_level();
    int16_t filter_output = IFilter<0>::clock(m_count, osc_output, env_gen_output_0, lfo_output);
    uint8_t env_gen_output_1 = IEnvGen<1>::clock(m_count);
    int16_t amp_output = IAmp<0>::clock(filter_output, env_gen_output_1);

    // error diffusion
    int16_t output = amp_output + m_output_error;
    m_output_error = low_byte(output);

    int8_t dir_sample = high_sbyte(output);
    IDelayFx<0>::push(dir_sample);

    int8_t eff_sample = IDelayFx<0>::get(IOsc<0>::get_chorus_delay_time());

    right_level = dir_sample;
    return eff_sample;
  }

private:
  INLINE static void set_on_note(uint8_t note_number) {
    m_on_note[note_number >> 3] |= (1 << (note_number & 0x07));
  }

  INLINE static void clear_on_note(uint8_t note_number) {
    m_on_note[note_number >> 3] &= ~(1 << (note_number & 0x07));
  }

  INLINE static uint8_t get_highest_on_note() {
    uint8_t highest_on_note = NOTE_NUMBER_INVALID;
    for (int8_t i = 15; i >= 0; --i) {
      if (m_on_note[i] != 0x00) {
        if        (m_on_note[i] & 0x80) {
          highest_on_note = (i << 3) + 7;
        } else if (m_on_note[i] & 0x40) {
          highest_on_note = (i << 3) + 6;
        } else if (m_on_note[i] & 0x20) {
          highest_on_note = (i << 3) + 5;
        } else if (m_on_note[i] & 0x10) {
          highest_on_note = (i << 3) + 4;
        } else if (m_on_note[i] & 0x08) {
          highest_on_note = (i << 3) + 3;
        } else if (m_on_note[i] & 0x04) {
          highest_on_note = (i << 3) + 2;
        } else if (m_on_note[i] & 0x02) {
          highest_on_note = (i << 3) + 1;
        } else if (m_on_note[i] & 0x01) {
          highest_on_note = (i << 3) + 0;
        }
        break;
      }
    }
    return highest_on_note;
  }

  INLINE static uint8_t get_lowest_on_note() {
    uint8_t lowest_on_note = NOTE_NUMBER_INVALID;
    for (uint8_t i = 0; i < 16; ++i) {
      if (m_on_note[i] != 0x00) {
        if        (m_on_note[i] & 0x01) {
          lowest_on_note = (i << 3) + 0;
        } else if (m_on_note[i] & 0x02) {
          lowest_on_note = (i << 3) + 1;
        } else if (m_on_note[i] & 0x04) {
          lowest_on_note = (i << 3) + 2;
        } else if (m_on_note[i] & 0x08) {
          lowest_on_note = (i << 3) + 3;
        } else if (m_on_note[i] & 0x10) {
          lowest_on_note = (i << 3) + 4;
        } else if (m_on_note[i] & 0x20) {
          lowest_on_note = (i << 3) + 5;
        } else if (m_on_note[i] & 0x40) {
          lowest_on_note = (i << 3) + 6;
        } else if (m_on_note[i] & 0x80) {
          lowest_on_note = (i << 3) + 7;
        }
        break;
      }
    }
    return lowest_on_note;
  }

  INLINE static void update_decay_release() {
    if (m_amp_env_gen >= 64) {
      IEnvGen<0>::set_decay(m_decay);
      IEnvGen<1>::set_decay(m_decay);
      if (m_release >= 64) {
        IEnvGen<0>::set_release(m_decay);
        IEnvGen<1>::set_release(m_decay);
      } else {
        IEnvGen<0>::set_release(0);
        IEnvGen<1>::set_release(0);
      }
    } else {
      IEnvGen<0>::set_decay(m_decay);
      if (m_release >= 64) {
        IEnvGen<0>::set_release(m_decay);
      } else {
        IEnvGen<0>::set_release(0);
      }
    }
  }

  INLINE static void update_env_gen() {
    if (m_amp_env_gen >= 64) {
      IEnvGen<0>::set_attack(m_attack);
      IEnvGen<1>::set_attack(m_attack);
      IEnvGen<0>::set_decay(m_decay);
      IEnvGen<1>::set_decay(m_decay);
      IEnvGen<0>::set_sustain(m_sustain);
      IEnvGen<1>::set_sustain(m_sustain);
      if (m_release >= 64) {
        IEnvGen<0>::set_release(m_decay);
        IEnvGen<1>::set_release(m_decay);
      } else {
        IEnvGen<0>::set_release(0);
        IEnvGen<1>::set_release(0);
      }
    } else {
      IEnvGen<0>::set_attack(m_attack);
      IEnvGen<1>::set_attack(0);
      IEnvGen<0>::set_decay(m_decay);
      IEnvGen<1>::set_decay(0);
      IEnvGen<0>::set_sustain(m_sustain);
      IEnvGen<1>::set_sustain(127);
      if (m_release >= 64) {
        IEnvGen<0>::set_release(m_decay);
      } else {
        IEnvGen<0>::set_release(0);
      }
      IEnvGen<1>::set_release(0);
    }
  }

  static uint8_t get_rnd_7() {
    m_rnd = m_rnd ^ (m_rnd << 5);
    m_rnd = m_rnd ^ (m_rnd >> 9);
    m_rnd = m_rnd ^ (m_rnd << 8);
    return low_byte(m_rnd) >> 1;
  }
};

template <uint8_t T> uint8_t  Voice<T>::m_count;
template <uint8_t T> uint8_t  Voice<T>::m_last_note_number;
template <uint8_t T> uint8_t  Voice<T>::m_on_note[16];
template <uint8_t T> uint8_t  Voice<T>::m_output_error;
template <uint8_t T> uint8_t  Voice<T>::m_portamento;
template <uint8_t T> uint8_t  Voice<T>::m_attack;
template <uint8_t T> uint8_t  Voice<T>::m_decay;
template <uint8_t T> uint8_t  Voice<T>::m_sustain;
template <uint8_t T> uint8_t  Voice<T>::m_release;
template <uint8_t T> uint8_t  Voice<T>::m_amp_env_gen;
template <uint8_t T> uint16_t Voice<T>::m_rnd;
template <uint8_t T> uint8_t  Voice<T>::m_sp_prog_chg_cc_values[8];
