#pragma once

#include "common.h"
#include "program-table.h"

template <uint8_t T>
class Voice {
  static uint8_t  m_count;

  static uint8_t  m_note_queue[4];
  static uint8_t  m_note_on_number[4];
  static uint8_t  m_note_on_count[128];
  static uint8_t  m_note_on_total_count;
  static boolean  m_sustain_pedal;
  static uint8_t  m_voice_mode;

  static uint8_t  m_output_error;
  static uint8_t  m_portamento;
  static uint8_t  m_attack;
  static uint8_t  m_decay;
  static uint8_t  m_sustain;
  static uint8_t  m_amp_env_gen;

  static uint8_t  m_chorus_mode;
  static uint8_t  m_velocity_to_cutoff;

  static uint16_t m_rnd;
  static uint8_t  m_sp_prog_chg_cc_values[8];

public:
  INLINE static void initialize() {
    m_count = 0;

    m_note_queue[0] = 0;
    m_note_queue[1] = 1;
    m_note_queue[2] = 2;
    m_note_queue[3] = 3;
    m_note_on_number[0] = NOTE_NUMBER_INVALID;
    m_note_on_number[1] = NOTE_NUMBER_INVALID;
    m_note_on_number[2] = NOTE_NUMBER_INVALID;
    m_note_on_number[3] = NOTE_NUMBER_INVALID;
    for (uint8_t i = 0; i < sizeof(m_note_on_count); ++i) {
      m_note_on_count[i] = 0;
    }
    m_note_on_total_count = 0;
    m_sustain_pedal = false;
    m_voice_mode = VOICE_PARAPHONIC;

    m_output_error = 0;
    m_portamento = 0;
    IOsc<0>::initialize();
    IOsc<0>::set_mono_mode(m_voice_mode);
    IFilter<0>::initialize();
    IAmp<0>::initialize();

    IEnvGen<0>::initialize();
    IEnvGen<1>::initialize();
    IEnvGen<1>::set_gain(90);

    IDelayFx<0>::initialize();

    m_attack = 0;
    m_decay = 0;
    m_sustain = 127;
    m_amp_env_gen = 127;
    update_env_gen();

    m_chorus_mode = CHORUS_MODE_OFF;
    m_velocity_to_cutoff = 0;

    m_rnd = 1;
  }

  INLINE static void note_on(uint8_t note_number, uint8_t velocity) {
    if (m_note_on_total_count == 255) {
      return;
    }

    int8_t cutoff_offset = 0;
    if (m_velocity_to_cutoff == 128) {
      cutoff_offset = velocity - 100;
    } else {
      cutoff_offset = high_sbyte(static_cast<int8_t>(velocity - 100) * (m_velocity_to_cutoff << 1));
    }

    if (m_voice_mode != VOICE_PARAPHONIC) {

      if (m_voice_mode == VOICE_LEGATO) {
        ++m_note_on_total_count;
        ++m_note_on_count[note_number];

        if (m_note_on_number[0] == NOTE_NUMBER_INVALID) {
          m_note_on_number[0] = note_number;

          IOsc<0>::set_portamento<0>(0);
          IOsc<0>::set_portamento<2>(0);
          IOsc<0>::note_on<0>(note_number);
          IOsc<0>::note_on<2>(note_number);
          IOsc<0>::trigger_lfo();
          IEnvGen<0>::note_on();
          IEnvGen<1>::note_on();
          IFilter<0>::set_cutoff_offset(cutoff_offset);
        } else {
          m_note_on_number[3] = m_note_on_number[2];
          m_note_on_number[2] = m_note_on_number[1];
          m_note_on_number[1] = m_note_on_number[0];
          m_note_on_number[0] = note_number;

          IOsc<0>::set_portamento<0>(m_portamento);
          IOsc<0>::set_portamento<2>(m_portamento);
          IOsc<0>::note_on<0>(note_number);
          IOsc<0>::note_on<2>(note_number);
        }
      } else {
        ++m_note_on_total_count;
        ++m_note_on_count[note_number];

        m_note_on_number[3] = m_note_on_number[2];
        m_note_on_number[2] = m_note_on_number[1];
        m_note_on_number[1] = m_note_on_number[0];
        m_note_on_number[0] = note_number;

        IOsc<0>::set_portamento<0>(m_portamento);
        IOsc<0>::set_portamento<2>(m_portamento);
        IOsc<0>::note_on<0>(note_number);
        IOsc<0>::note_on<2>(note_number);
        IOsc<0>::trigger_lfo();
        IEnvGen<0>::note_on();
        IEnvGen<1>::note_on();
        IFilter<0>::set_cutoff_offset(cutoff_offset);
      }

    } else if (m_note_on_number[0] == note_number) {
      ++m_note_on_total_count;
      ++m_note_on_count[note_number];

      IOsc<0>::set_portamento<0>(m_portamento);
      IOsc<0>::note_on<0>(note_number);
      IOsc<0>::trigger_lfo();
      IEnvGen<0>::note_on();
      IEnvGen<1>::note_on();
      IFilter<0>::set_cutoff_offset(cutoff_offset);
    } else if (m_note_on_number[1] == note_number) {
      ++m_note_on_total_count;
      ++m_note_on_count[note_number];

      IOsc<0>::set_portamento<1>(m_portamento);
      IOsc<0>::note_on<1>(note_number);
      IOsc<0>::trigger_lfo();
      IEnvGen<0>::note_on();
      IEnvGen<1>::note_on();
      IFilter<0>::set_cutoff_offset(cutoff_offset);
    } else if (m_note_on_number[2] == note_number) {
      ++m_note_on_total_count;
      ++m_note_on_count[note_number];

      IOsc<0>::set_portamento<2>(m_portamento);
      IOsc<0>::note_on<2>(note_number);
      IOsc<0>::trigger_lfo();
      IEnvGen<0>::note_on();
      IEnvGen<1>::note_on();
      IFilter<0>::set_cutoff_offset(cutoff_offset);
    } else if (m_note_on_number[3] == note_number) {
      ++m_note_on_total_count;
      ++m_note_on_count[note_number];

      IOsc<0>::set_portamento<3>(m_portamento);
      IOsc<0>::note_on<3>(note_number);
      IOsc<0>::trigger_lfo();
      IEnvGen<0>::note_on();
      IEnvGen<1>::note_on();
      IFilter<0>::set_cutoff_offset(cutoff_offset);
    } else {
      uint8_t note_on_osc_index;
      if        (m_note_on_number[0] == NOTE_NUMBER_INVALID) {
        note_on_osc_index = 0;
      } else if (m_note_on_number[1] == NOTE_NUMBER_INVALID) {
        note_on_osc_index = 1;
      } else if (m_note_on_number[2] == NOTE_NUMBER_INVALID) {
        note_on_osc_index = 2;
      } else if (m_note_on_number[3] == NOTE_NUMBER_INVALID) {
        note_on_osc_index = 3;
      } else {
        note_on_osc_index = m_note_queue[0];
      }

      note_queue_on(note_on_osc_index);

      ++m_note_on_total_count;
      ++m_note_on_count[note_number];

      m_note_on_number[note_on_osc_index] = note_number;

      switch (note_on_osc_index) {
      default:
        IOsc<0>::set_portamento<0>(m_portamento);
        IOsc<0>::note_on<0>(note_number);
        break;
      case 1:
        IOsc<0>::set_portamento<1>(m_portamento);
        IOsc<0>::note_on<1>(note_number);
        break;
      case 2:
        IOsc<0>::set_portamento<2>(m_portamento);
        IOsc<0>::note_on<2>(note_number);
        break;
      case 3:
        IOsc<0>::set_portamento<3>(m_portamento);
        IOsc<0>::note_on<3>(note_number);
        break;
      }

      IOsc<0>::trigger_lfo();
      IEnvGen<0>::note_on();
      IEnvGen<1>::note_on();
      IFilter<0>::set_cutoff_offset(cutoff_offset);
    }
  }

  INLINE static void note_off(uint8_t note_number) {
    if (m_note_on_total_count == 0) {
      return;
    }

    if (m_note_on_count[note_number] == 0) {
      return;
    }

    --m_note_on_total_count;
    --m_note_on_count[note_number];

    if (m_sustain_pedal) {
      return;
    }

    if (m_voice_mode != VOICE_PARAPHONIC) {

      if (m_note_on_total_count == 0) {
        m_note_on_number[0] = NOTE_NUMBER_INVALID;
        m_note_on_number[1] = NOTE_NUMBER_INVALID;
        m_note_on_number[2] = NOTE_NUMBER_INVALID;
        m_note_on_number[3] = NOTE_NUMBER_INVALID;
        m_note_queue[0] = 0;
        m_note_queue[1] = 1;
        m_note_queue[2] = 2;
        m_note_queue[3] = 3;
        IOsc<0>::note_off<0>();
        IOsc<0>::note_off<1>();
        IOsc<0>::note_off<2>();
        IOsc<0>::note_off<3>();
      } else if (m_note_on_number[0] == note_number) {
        m_note_on_number[0] = m_note_on_number[1];
        m_note_on_number[1] = m_note_on_number[2];
        m_note_on_number[2] = m_note_on_number[3];
        m_note_on_number[3] = NOTE_NUMBER_INVALID;

        if (m_note_on_number[0] != NOTE_NUMBER_INVALID) {
          IOsc<0>::set_portamento<0>(m_portamento);
          IOsc<0>::set_portamento<2>(m_portamento);
          IOsc<0>::note_on<0>(m_note_on_number[0]);
          IOsc<0>::note_on<2>(m_note_on_number[0]);

          if (m_voice_mode == VOICE_MONOPHONIC) {
            IOsc<0>::trigger_lfo();
            IEnvGen<0>::note_on();
            IEnvGen<1>::note_on();
          }
        }
      } else if (m_note_on_number[1] == note_number) {
        m_note_on_number[1] = m_note_on_number[2];
        m_note_on_number[2] = m_note_on_number[3];
        m_note_on_number[3] = NOTE_NUMBER_INVALID;
      } else if (m_note_on_number[2] == note_number) {
        m_note_on_number[2] = m_note_on_number[3];
        m_note_on_number[3] = NOTE_NUMBER_INVALID;
      } else if (m_note_on_number[3] == note_number) {
        m_note_on_number[3] = NOTE_NUMBER_INVALID;
      }

    } else if (m_note_on_number[0] == note_number) {
      if (m_note_on_count[note_number] == 0) {
        m_note_on_number[0] = NOTE_NUMBER_INVALID;
        note_queue_off(0);
        IOsc<0>::note_off<0>();
      }
    } else if (m_note_on_number[1] == note_number) {
      if (m_note_on_count[note_number] == 0) {
        m_note_on_number[1] = NOTE_NUMBER_INVALID;
        note_queue_off(1);
        IOsc<0>::note_off<1>();
      }
    } else if (m_note_on_number[2] == note_number) {
      if (m_note_on_count[note_number] == 0) {
        m_note_on_number[2] = NOTE_NUMBER_INVALID;
        note_queue_off(2);
        IOsc<0>::note_off<2>();
      }
    } else if (m_note_on_number[3] == note_number) {
      if (m_note_on_count[note_number] == 0) {
        m_note_on_number[3] = NOTE_NUMBER_INVALID;
        note_queue_off(3);
        IOsc<0>::note_off<3>();
      }
    }

    if (m_note_on_total_count == 0) {
      IEnvGen<0>::note_off();
      IEnvGen<1>::note_off();
    }
  }

  static void all_sound_off() {
    m_sustain_pedal = false;
    m_note_on_number[0] = NOTE_NUMBER_INVALID;
    m_note_on_number[1] = NOTE_NUMBER_INVALID;
    m_note_on_number[2] = NOTE_NUMBER_INVALID;
    m_note_on_number[3] = NOTE_NUMBER_INVALID;
    for (uint8_t i = 0; i < sizeof(m_note_on_count); ++i) {
      m_note_on_count[i] = 0;
    }
    m_note_on_total_count = 0;
    m_note_queue[0] = 0;
    m_note_queue[1] = 1;
    m_note_queue[2] = 2;
    m_note_queue[3] = 3;
    IOsc<0>::note_off<0>();
    IOsc<0>::note_off<1>();
    IOsc<0>::note_off<2>();
    IOsc<0>::note_off<3>();
    IEnvGen<0>::note_off();
    IEnvGen<1>::note_off();
  }

  INLINE static void reset_all_controllers() {
    pitch_bend(0, 64);
    set_modulation(0);
    set_expression(127);
    set_sustain_pedal(0);
  }

  INLINE static void control_change(uint8_t controller_number, uint8_t controller_value) {
    switch (controller_number) {
    case EXPRESSION     :
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

    case SUSTAIN_PEDAL   :
      set_sustain_pedal(controller_value);
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
      {
        uint8_t new_chorus_mode = CHORUS_MODE_STEREO_2;
        if        (controller_value < 16) {
          new_chorus_mode = CHORUS_MODE_OFF;
        } else if (controller_value < 48) {
          new_chorus_mode = CHORUS_MODE_MONO;
        } else if (controller_value < 80) {
          new_chorus_mode = CHORUS_MODE_P_STEREO;
        } else if (controller_value < 112) {
          new_chorus_mode = CHORUS_MODE_STEREO;
        }

        if (m_chorus_mode != new_chorus_mode) {
          m_chorus_mode = new_chorus_mode;

          IDelayFx<0>::attenuate();

          switch (m_chorus_mode) {
          case CHORUS_MODE_OFF      :
            IOsc<0>::set_chorus_mode(CHORUS_MODE_OFF);
            IEnvGen<1>::set_gain(90);
            break;
          case CHORUS_MODE_STEREO   :
            IOsc<0>::set_chorus_mode(CHORUS_MODE_STEREO);
            IEnvGen<1>::set_gain(90);
            break;
          case CHORUS_MODE_P_STEREO   :
            IOsc<0>::set_chorus_mode(CHORUS_MODE_P_STEREO);
            IEnvGen<1>::set_gain(64);
            break;
          case CHORUS_MODE_MONO     :
            IOsc<0>::set_chorus_mode(CHORUS_MODE_MONO);
            IEnvGen<1>::set_gain(64);
            break;
          case CHORUS_MODE_STEREO_2 :
            IOsc<0>::set_chorus_mode(CHORUS_MODE_STEREO_2);
            IEnvGen<1>::set_gain(64);
            break;
          }
        }
      }
      break;

    case OSC_LEVEL      :
      IOsc<0>::set_osc_level(controller_value);
      break;

    case PORTAMENTO     :
      m_portamento = controller_value;
      break;

    case MONO_O2_MIX    :
      IOsc<0>::set_mono_osc2_mix(controller_value);
      break;

    case MONO_O2_PITCH :
      IOsc<0>::set_mono_osc2_pitch(controller_value);
      break;

    case MONO_O2_DETUNE :
      IOsc<0>::set_mono_osc2_detune(controller_value);
      break;

    case EG_TO_PITCH    :
      IOsc<0>::set_pitch_env_amt(controller_value);
      break;

    case LFO_WAVE       :
      IOsc<0>::set_lfo_waveform(controller_value);
      break;

    case LFO_FADE_TIME  :
      IOsc<0>::set_lfo_fade_time(controller_value);
      break;

    case P_BEND_RANGE   :
      IOsc<0>::set_pitch_bend_range(controller_value);
      break;

    case V_TO_CUTOFF    :
      m_velocity_to_cutoff = ((controller_value + 1) >> 1) << 1;
      break;

    case VOICE_MODE     :
      {
        uint8_t new_voice_mode = VOICE_PARAPHONIC;
        if (controller_value > 96) {
          new_voice_mode = VOICE_LEGATO;
        } else if (controller_value > 32) {
          new_voice_mode = VOICE_MONOPHONIC;
        }

        if (m_voice_mode != new_voice_mode) {
          m_voice_mode = new_voice_mode;
          all_sound_off();
          boolean mono_mode = (m_voice_mode != VOICE_PARAPHONIC);
          IOsc<0>::set_mono_mode(mono_mode);
        }
      }
      break;

    case ALL_NOTES_OFF  :
    case OMNI_MODE_OFF  :
    case OMNI_MODE_ON   :
    case MONO_MODE_ON   :
    case POLY_MODE_ON   :
      all_sound_off();  // Strictly speaking, this is a violation of MIDI 1.0 Specification...
      break;

    case ALL_SOUND_OFF  :
      all_sound_off();
      break;

    case RESET_ALL_CTRLS:
      reset_all_controllers();
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

  /* INLINE */ static void program_change(uint8_t program_number) {
#if defined(ENABLE_SPECIAL_PROGRAM_CHANGE)
    if (program_number > (PROGRAM_NUMBER_MAX * 2) + 1) {
      return;
    } else if (program_number > PROGRAM_NUMBER_MAX) {
      program_number -= (PROGRAM_NUMBER_MAX + 1);
    }
#else
    if (program_number > PROGRAM_NUMBER_MAX) {
      return;
    }
#endif

    control_change(OSC_WAVE       , g_preset_table_OSC_WAVE       [program_number]);
    control_change(OSC_LEVEL      , g_preset_table_OSC_LEVEL      [program_number]);
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

    control_change(LFO_WAVE       , g_preset_table_LFO_WAVE       [program_number]);
    control_change(LFO_FADE_TIME  , g_preset_table_LFO_FADE_TIME  [program_number]);

    control_change(CHORUS_DEPTH   , g_preset_table_CHORUS_DEPTH   [program_number]);
    control_change(CHORUS_RATE    , g_preset_table_CHORUS_RATE    [program_number]);
    control_change(CHORUS_DELAY_T , g_preset_table_CHORUS_DELAY_T [program_number]);
    control_change(CHORUS_MODE    , g_preset_table_CHORUS_MODE    [program_number]);

    control_change(P_BEND_RANGE   , g_preset_table_P_BEND_RANGE   [program_number]);
    control_change(V_TO_CUTOFF    , g_preset_table_V_TO_CUTOFF    [program_number]);
    control_change(VOICE_MODE     , g_preset_table_VOICE_MODE     [program_number]);
    control_change(PORTAMENTO     , g_preset_table_PORTAMENTO     [program_number]);

    control_change(MONO_O2_MIX    , g_preset_table_MONO_O2_MIX    [program_number]);
    control_change(MONO_O2_PITCH  , g_preset_table_MONO_O2_PITCH  [program_number]);
    control_change(MONO_O2_DETUNE , g_preset_table_MONO_O2_DETUNE [program_number]);
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

    int8_t eff_sample_0 = IDelayFx<0>::get(IOsc<0>::get_chorus_delay_time<0>());
    int8_t eff_sample_1 = IDelayFx<0>::get(IOsc<0>::get_chorus_delay_time<1>());
    IDelayFx<0>::push(dir_sample);

    if (m_chorus_mode >= CHORUS_MODE_MONO) {
      // For Mono Chorus and Stereo 2-phase Chorus
      right_level = dir_sample + eff_sample_0;
      return        dir_sample + eff_sample_1;
    } else if (m_chorus_mode == CHORUS_MODE_P_STEREO) {
      // For Pseudo-Stereo Chorus
      right_level = dir_sample - eff_sample_0;
      return        dir_sample + eff_sample_0;
    }

    // For Off and Stereo Chorus
    right_level = dir_sample;
    return        eff_sample_0;
  }

private:

  INLINE static void note_queue_on(uint8_t note_on_osc_index) {
    if        (m_note_queue[3] == note_on_osc_index) {
      m_note_queue[3] = note_on_osc_index;
    } else if (m_note_queue[2] == note_on_osc_index) {
      m_note_queue[2] = m_note_queue[3];
      m_note_queue[3] = note_on_osc_index;
    } else if (m_note_queue[1] == note_on_osc_index) {
      m_note_queue[1] = m_note_queue[2];
      m_note_queue[2] = m_note_queue[3];
      m_note_queue[3] = note_on_osc_index;
    } else {
      m_note_queue[0] = m_note_queue[1];
      m_note_queue[1] = m_note_queue[2];
      m_note_queue[2] = m_note_queue[3];
      m_note_queue[3] = note_on_osc_index;
    }
  }

  INLINE static void note_queue_off(uint8_t note_off_osc_index) {
    if        (m_note_queue[1] == note_off_osc_index) {
      m_note_queue[1] = m_note_queue[0];
      m_note_queue[0] = note_off_osc_index;
    } else if (m_note_queue[2] == note_off_osc_index) {
      m_note_queue[2] = m_note_queue[1];
      m_note_queue[1] = m_note_queue[0];
      m_note_queue[0] = note_off_osc_index;
    } else if (m_note_queue[3] == note_off_osc_index) {
      m_note_queue[3] = m_note_queue[2];
      m_note_queue[2] = m_note_queue[1];
      m_note_queue[1] = m_note_queue[0];
      m_note_queue[0] = note_off_osc_index;
    }
  }

  INLINE static void update_decay_release() {
    if (m_amp_env_gen >= 64) {
      IEnvGen<0>::set_decay(m_decay);
      IEnvGen<1>::set_decay(m_decay);
    } else {
      IEnvGen<0>::set_decay(m_decay);
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
    } else {
      IEnvGen<0>::set_attack(m_attack);
      IEnvGen<1>::set_attack(0);
      IEnvGen<0>::set_decay(m_decay);
      IEnvGen<1>::set_decay(0);
      IEnvGen<0>::set_sustain(m_sustain);
      IEnvGen<1>::set_sustain(127);
    }
  }

  static uint8_t get_rnd_7() {
    m_rnd = m_rnd ^ (m_rnd << 5);
    m_rnd = m_rnd ^ (m_rnd >> 9);
    m_rnd = m_rnd ^ (m_rnd << 8);
    return low_byte(m_rnd) >> 1;
  }

  INLINE static void set_modulation(uint8_t controller_value) {
    IOsc<0>::set_lfo_depth<1>(controller_value);
  }

  INLINE static void set_expression(uint8_t controller_value) {
    IEnvGen<1>::set_expression(controller_value);
  }

  INLINE static void set_sustain_pedal(uint8_t controller_value) {
    if ((m_sustain_pedal == false) && (controller_value >= 64)) {
      m_sustain_pedal = true;
    } else if (m_sustain_pedal && (controller_value < 64)) {
      m_sustain_pedal = false;

      if (m_note_on_number[0] != NOTE_NUMBER_INVALID) {
        if (m_note_on_count[m_note_on_number[0]] == 0) {
          m_note_on_number[0] = NOTE_NUMBER_INVALID;
          note_queue_off(0);
          IOsc<0>::note_off<0>();
        }
      }

      if (m_note_on_number[1] != NOTE_NUMBER_INVALID) {
        if (m_note_on_count[m_note_on_number[1]] == 0) {
          m_note_on_number[1] = NOTE_NUMBER_INVALID;
          note_queue_off(1);
          IOsc<0>::note_off<1>();
        }
      }

      if (m_note_on_number[2] != NOTE_NUMBER_INVALID) {
        if (m_note_on_count[m_note_on_number[2]] == 0) {
          m_note_on_number[2] = NOTE_NUMBER_INVALID;
          note_queue_off(2);
          IOsc<0>::note_off<2>();
        }
      }

      if (m_note_on_number[3] != NOTE_NUMBER_INVALID) {
        if (m_note_on_count[m_note_on_number[3]] == 0) {
          m_note_on_number[3] = NOTE_NUMBER_INVALID;
          note_queue_off(3);
          IOsc<0>::note_off<3>();
        }
      }

      if (m_note_on_total_count == 0) {
        IEnvGen<0>::note_off();
        IEnvGen<1>::note_off();
      }
    }
  }
};

template <uint8_t T> uint8_t  Voice<T>::m_count;

template <uint8_t T> uint8_t  Voice<T>::m_note_queue[4];
template <uint8_t T> uint8_t  Voice<T>::m_note_on_number[4];
template <uint8_t T> uint8_t  Voice<T>::m_note_on_count[128];
template <uint8_t T> uint8_t  Voice<T>::m_note_on_total_count;
template <uint8_t T> boolean  Voice<T>::m_sustain_pedal;
template <uint8_t T> uint8_t  Voice<T>::m_voice_mode;

template <uint8_t T> uint8_t  Voice<T>::m_output_error;
template <uint8_t T> uint8_t  Voice<T>::m_portamento;
template <uint8_t T> uint8_t  Voice<T>::m_attack;
template <uint8_t T> uint8_t  Voice<T>::m_decay;
template <uint8_t T> uint8_t  Voice<T>::m_sustain;
template <uint8_t T> uint8_t  Voice<T>::m_amp_env_gen;

template <uint8_t T> uint8_t  Voice<T>::m_chorus_mode;
template <uint8_t T> uint8_t  Voice<T>::m_velocity_to_cutoff;

template <uint8_t T> uint16_t Voice<T>::m_rnd;
template <uint8_t T> uint8_t  Voice<T>::m_sp_prog_chg_cc_values[8];
