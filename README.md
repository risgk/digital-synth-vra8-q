# Digital Synth VRA8-N v2.4.4

- 2019-12-29 ISGK Instruments
- <https://github.com/risgk/digital-synth-vra8-n>


## Concept

- Monophonic Synthesizer (MIDI Sound Module) for Arduino Uno


## Caution about Arduino: Sorry for Mac users

- We *strongly recommend* Arduino IDE 1.8.5 (more precisely **Arduino AVR Boards core 1.6.20**)
    - If you use other than AVR Boards core 1.6.20 (or modified this sketch), the sketch *may not work well*
        - CPU Busy LED (LED L) *may almost always lit*, rather than flashing occasionally
- However, with **macOS Catalina 10.15**, only IDE 1.8.10 (AVR Boards core 1.8.1) or later can be used!
- If you have to use **Arduino AVR Boards core 1.8.1**, it is recommended to *reduce the program size*. Please do the following:
    - Change `REDUCE_OSC_TABLE_SIZE_1 = false` to `REDUCE_OSC_TABLE_SIZE_1 = true` in `"generate-osc-table.rb"`, and execute this
        - Requiring a Ruby execution environment
    - Or delete `"osc-table.h"`, and rename `"osc-table.h.REDUCE_OSC_TABLE_SIZE_1.txt"` to `"osc-table.h"`

## Change History

- v2.4.4
    - Prevent increase of CPU load in AVR Boards core 1.8.1
        - Change comments for macOS Catalina 10.15 and Arduino IDE 1.8.10 (AVR Boards core 1.8.1)
    - Improve portability of "make-sample-wav-file.cc"
    - Add "osc-table.h.REDUCE_OSC_TABLE_SIZE_1.txt"
- v2.4.3
    - Fix `"make-sample-wav-file.cc"` (for Debugging on PC): Add `#define ENABLE_OSC_2`
- v2.4.2 (Major Changes)
    - Improve `ENABLE_SPECIAL_PROGRAM_CHANGE` option: Works when the CC value changes from <= 63 to >= 64
- v2.4.1 (Major Changes)
    - Fix explanation about Arduino AVR Boards (Core)
        - "There is no restriction on a version of Arduino AVR Core" was a mistake!
    - Add comments for macOS Catalina 10.15 and Arduino IDE 1.8.10 (AVR Boards 1.8.1)
    - Change "LEGATO" of "PRESET #4 PAN FLUTE" from ON to OFF
    - Reset "EXPRESSION" when "EXP BY VEL" turns OFF
    - Add `ENABLE_SPECIAL_PROGRAM_CHANGE` option: Program Change by Control Change #112-119, and #90
- v2.4.0 (Major Changes)
    - Add the JSON file IMPORT/EXPORT function to VRA8-N CTRL
    - Rename the constants for CCs
    - Introduce NewEnvGen (Level changes are smoother than the original EnvGen)
    - Improve the ANALOG_INPUT_REVERSED option of mode-VC
- v2.3.0 (Major Changes)
    - Do not support Arduino Nano 3.x (because the sketch uses more than 30720 bytes of program storage space)
    - Rename **VRA8-N mini** to **VRA8-N mode-VC**
    - Improve **VRA8-N mode-VC**: Add the SCALE MODE "C Major" and the GATE Signal support
    - Halve the send level of LFO > CUTOFF for smoothness
    - Change the change range of CUTOFF frequency from 28-124 to 16-112 (64: the center)
    - Change the Q of RESONANCE 64 from 2.8 to 2.0 (No change in the range of Q from 0.7 to 8.0)
    - Add "PITCH > CUTOFF", "OSC LEVEL", "RESONANCE LIMIT", "AMP LEVEL", and "DAMP AND ATTACK" controls
    - Reset EXPRESSION at Random Control
- v2.2.1
    - Fix the issue that RANDOM of LFO and the fluctuation of OSCs do not work
- v2.2.0 (Major Changes)
    - Enable Sub Oscillator in **VRA8-N mini** mode
    - Slow down the change speed of OSC MIX and SUB LEVEL
    - Improve stability: Reduce distortion of audio output when processing MIDI data
- v2.1.2
    - Add USE_PITCH_CV_IN option for **VRA8-N mini** (Undef it, if you use a MIDI keyboard)
- v2.1.1
    - Change the assignment of the potentiometers for **VRA8-N mini**
- v2.1.0 (Major Changes)
    - Narrow the CUTOFF frequency range from 5 oct (G4-G9) to 4 oct (G5-G9) for sound quality
    - Add OSC2 NOISE; Change "OSC1/2 (SAW/SQ)" to "OSC1/2 (SAW/N/SQ)"
    - Add "LFO FADE TIME" control
    - Add LFO LED Out option (Enabled by default, Pin D5)
    - Add Pin D5 Audio Out option for MIDI Shield (Disabled by default)
    - Add the operation mode **VRA8-N mini** option (Voltage controlled, Sub Oscillator disabled)
    - Improve stability
- v2.0.1
    - Fix sound quality degradation in v2.0.0
    - Revert "SUSTAIN (OFF/ON)" to "SUSTAIN"
- v2.0.0 (Major Changes)
    - Change "OSC (SAW/SQ)" to "OSC1/2 (SAW/SQ)"
    - Add "EXPRESSION", "EXP > CUTOFF", "EXP > AMP LEVEL", and "EXP BY VEL (OFF/ON)"
- v1.2.1
    - Improve sound quality
    - Improve VRA8-N CTRL compatibility between versions
- v1.2.0 (Major Changes)
    - Add "EG > PITCH(-/+)", "EG > P. TGT (1&2/2)", "RELEASE (OFF/ON)", and "EG > LFO RATE (-/+)"
    - Change "OSC2 MIX" (OSC2 Max 50%) to "OSC MIX (1/2)" (OSC2 Max 100%)
    - Change "SUB WAVE (SIN/SQ)" to "SUB (SIN/NOISE/SQ)"
    - Change "AMP (GATE+RLS/EG)" to "AMP EG (OFF/ON)" (No Release)
    - Change "SUSTAIN" to "SUSTAIN (OFF/ON)"
    - Change "LFO WAVE (TRI/SQ)" to "LFO (T/2/SA/RND/SQ)"
    - Change "LFO > PITCH (2/1+2)" to "LFO > PITCH (-/+)" and "LFO > P. TGT (1&2/2)"
    - Change "KEY ASGN (LO/LAST)" to "K. ASN (L/L/P/H/LST)"
    - Change the variation range of "CUTOFF" 7-127 to 4-124
    - Change the names of controllers
    - Change the layout and the display of VRA8-N CTRL
    - Change PRESETs
- v1.1.0 (Major Changes)
    - Add "SUB WAVE (SIN/SQ)" and "LFO WAVE (TRI/SQ)"


## Features

- Sampling Rate: 31.25 kHz, Bit Depth: 8 bit, LPF Attenuation Slope: -12 dB/oct
- Serial MIDI In (38.4 kbps), PWM Audio Out (Pin D6), Optional PWM LFO LED Out (Pin D5), PWM Rate: 62.5 kHz
    - We recommend adding a RC filter circuit to reduce PWM ripples
        - A cutoff frequency 15.9 kHz (R: 100 ohm, C: 100 nF) works well
    - **CAUTION**: The Arduino PWM audio output is a unipolar Line Out
        - Please connect the output to a active speaker/a power amp/a headphone amp
        - Do not connect the output to a passive speaker/a headphone directly
    - **CAUTION**: Click sounds may occur when you connect the audio out to an amp or reset the board
- We recommend [Hairless MIDI<->Serial Bridge](http://projectgus.github.io/hairless-midiserial/) to connect PC
    - **NOTE**: A combination of a **MIDI Shield** (MIDI Breakout) and a **power supply adapter** is *recommended* to avoiding USB noise
        - To use MIDI Shield, edit `SERIAL_SPEED`, `LFO_LED_OUT_ACTIVE`, and `SUBSTITUTE_PIN_D5_FOR_D6_AS_AUDIO_OUT` in `"DigitalSynthVRA8N.ino"`
        - Even using only the **power supply adapter** *significantly* reduces USB noise
- Files
    - `"DigitalSynthVRA8N.ino"` is a sketch for Arduino/Genuino Uno Rev3 (ATmega328P)
    - `"make-sample-wav-file.cc"` is for Debugging on PC
        - Requiring GCC (g++) or other
        - `"make-sample-wav-file-cc.bat"` makes a sample WAV file (working on Windows)
    - `"generate-*.rb"` generates source files
        - Requiring a Ruby execution environment


## VRA8-N CTRL

- MIDI Controller (Editor) for VRA8-N, HTML5 App (Web App)
- VRA8-N CTRL converts Program Changes (#0-7 for PRESET) into Control Changes
- VRA8-N CTRL stores the current control values and the user programs (#8-15) in a Web browser (localStorage)
- We recommend Google Chrome, which implements Web MIDI API
- We recommend [loopMIDI](http://www.tobias-erichsen.de/software/loopmidi.html) (virtual loopback MIDI cable) to connect VRA8-N
- **CAUTION**: Low CUTOFF with high RESONANCE can damage the speakers
- **CAUTION**: Click sounds may occur when you change the control values


## Details of Controllers

- "OSC1/2 (SAW/N/SQ)": OSC1 Wave / OSC2 Wave
    - Values 0-15: OSC1 SAW / OSC2 SAW
    - Values 16-63: OSC1 SAW / OSC2 SQUARE
    - Values 40-63: OSC1 SAW / OSC2 NOISE
    - Values 64-87: OSC1 SQUARE / OSC2 NOISE
    - Values 88-111: OSC1 SQUARE / OSC2 SAW
    - Values 112-127: OSC1 SQUARE / OSC2 SQUARE
- "SUB (SIN/NOISE/SQ)": SUB Osc Wave
    - Values 0-31: SIN
    - Values 32-95: NOISE
    - Values 96-127: SQUARE
- "LFO (T/2/SA/S&H/SQ)": LFO Wave
    - Values 0-15: Triangle (Key Trigger: Off)
    - Values 16-47: Triangle 2 (Key Trigger: On)
    - Values 48-79: SAW Down (Key Trigger: On)
    - Values 80-111: RANDOM (Key Trigger: On)
    - Values 112-127: SQUARE Up (Key Trigger: On)
- "LFO FADE TIME": This affects "LFO DEPTH" but not "MODULATION DEPTH"
- "PITCH > CUTOFF": In other words, Filter CUTOFF Keyboard Tracking (Off/Half/Full)
- "LEGATO (OFF/ON)": LEGATO Portamento
    - When LEGATO Portamento is ON, Single Trigger is forced
- "K. ASN (L/L/P/H/LST)": Key ASSIGN / Trigger Mode
    - Values 0-47: Lowest Note / Single Trigger
    - Values 48-79: Paraphonic (Lowest and Highest Notes, Duophonic) / Single Trigger
    - Values 80-111: Highest Note / Single Trigger
    - Values 112-127: LAST One Note / Multi Trigger
- "EXP BY VEL (OFF/ON)": EXPRESSION Control By (Note ON) VELOCITY
- "DAMP AND ATTACK": DAMP the EG level to zero before ATTACK (0-63: Off, 64: Slowest, 127: Fastest)


## A Sample Setting of a 4-Knob Physical Controller

    +---------------+---------------+---------------+---------------+
    | CC #16        | CC #17        | CC #23        | CC #19        |
    | CUTOFF        | RESONANCE     | ATTACK        | DECAY         |
    +---------------+---------------+---------------+---------------+


## **VRA8-N mode-VC** (Alternative Operation Mode)

- VRA8-N is **Voltage Controlled** by 0-5V signals
- By default, you need 4 potentiometers and 2 buttons
    - A0: CUTOFF
    - A1: RESONANCE
    - A2: OSC MIX
    - A3: Pitch CV In (0V: Note OFF)
    - D2: Change the PROGRAM (#0, ..., #7, Random Control)
    - D4: Change the SCALE MODE
        - SCALE MODE 0: "C Major" (2Oct / 5V)
        - SCALE MODE 1: "Chromatic" (2Oct / 5V)
        - SCALE MODE 2: "Linear" (5Oct / 5V)
- To make the sketch operate as **VRA8-N mode-VC**, edit `ENABLE_VOLTAGE_CONTROL` in `"DigitalSynthVRA8N.ino"`
    - If you use a MIDI keyboard, comment out the line `#define USE_PITCH_CV_IN`
- See `"cv-in.h"`
    - If you use a GATE Signal (A5), cancel comment out of the line `//#define USE_GATE_IN`
- **NOTE**: A **power supply adapter** is *strongly recommended* to avoiding the swings of voltage values


## MIDI Implementation Chart

      [Monophonic Synthesizer]                                        Date: 2019-12-29       
      Model: Digital Synth VRA8-N     MIDI Implementation Chart       Version: 2.4.4         
    +-------------------------------+---------------+---------------+-----------------------+
    | Function...                   | Transmitted   | Recognized    | Remarks               |
    +-------------------------------+---------------+---------------+-----------------------+
    | Basic        Default          | x             | 1             |                       |
    | Channel      Changed          | x             | x             |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | Mode         Default          | x             | 3             |                       |
    |              Messages         | x             | x             |                       |
    |              Altered          | ************* |               |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | Note                          | x             | 0-127         |                       |
    | Number       : True Voice     | ************* | 0-120         |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | Velocity     Note ON          | x             | o (V=1-127)   | When EXP BY VEL is ON |
    |              Note OFF         | x             | x             |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | After        Key's            | x             | x             |                       |
    | Touch        Ch's             | x             | x             |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | Pitch Bend                    | x             | o             |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | Control                     1 | x             | o             | MODULATION DEPTH      |
    | Change                     11 | x             | o             | EXPRESSION            |
    |                               |               |               |                       |
    |                            24 | x             | o             | OSC1/2 (SAW/SQ)       |
    |                            25 | x             | o             | OSC MIX (1/2)         |
    |                            20 | x             | o             | OSC2 COARSE (-/+)     |
    |                            21 | x             | o             | OSC2 FINE (-/+)       |
    |                               |               |               |                       |
    |                            29 | x             | o             | SUB (SIN/NOISE/SQ)    |
    |                            26 | x             | o             | SUB LEVEL             |
    |                           104 | x             | o             | EG > PITCH (-/+)      |
    |                           105 | x             | o             | EG > P. TGT (1&2/2)   |
    |                               |               |               |                       |
    |                            16 | x             | o             | CUTOFF                |
    |                            17 | x             | o             | RESONANCE             |
    |                            18 | x             | o             | EG > CUTOFF (-/+)     |
    |                            31 | x             | o             | AMP EG (OFF/ON)       |
    |                               |               |               |                       |
    |                            23 | x             | o             | ATTACK                |
    |                            19 | x             | o             | DECAY                 |
    |                            27 | x             | o             | SUSTAIN               |
    |                            28 | x             | o             | RELEASE (OFF/ON)      |
    |                               |               |               |                       |
    |                            14 | x             | o             | LFO (T/2/SA/RND/SQ)   |
    |                            80 | x             | o             | LFO RATE              |
    |                            82 | x             | o             | LFO > PITCH (-/+)     |
    |                            83 | x             | o             | LFO > CUTOFF (-/+)    |
    |                               |               |               |                       |
    |                            81 | x             | o             | LFO DEPTH             |
    |                             3 | x             | o             | EG > LFO RATE (-/+)   |
    |                             9 | x             | o             | LFO > P. TGT (1&2/2)  |
    |                            15 | x             | o             | LFO FADE TIME         |
    |                               |               |               |                       |
    |                            85 | x             | o             | P. BEND RANGE         |
    |                            86 | x             | o             | PITCH > CUTOFF        |
    |                           106 | x             | o             | EXP > CUTOFF (-/+)    |
    |                           107 | x             | o             | EXP > AMP LEVEL       |
    |                               |               |               |                       |
    |                            22 | x             | o             | PORTAMENTO            |
    |                            30 | x             | o             | LEGATO (OFF/ON)       |
    |                            87 | x             | o             | K. ASN (L/L/P/H/LST)  |
    |                            89 | x             | o             | EXP BY VEL (OFF/ON)   |
    |                               |               |               |                       |
    |                           108 | x             | o             | OSC LEVEL             |
    |                           109 | x             | o             | RESONANCE LIMIT       |
    |                           110 | x             | o             | AMP LEVEL             |
    |                           111 | x             | o             | DAMP AND ATTACK       |
    |                               |               |               |                       |
    |                   90, 112-119 | x             | x             | (RESERVED)            |
    +-------------------------------+---------------+---------------+-----------------------+
    | Program                       | x             | o             |                       |
    | Change       : True #         | ************* | 0-7, 127      | 127: Random Control   |
    +-------------------------------+---------------+---------------+-----------------------+
    | System Exclusive              | x             | x             |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | System       : Song Pos       | x             | x             |                       |
    | Common       : Song Sel       | x             | x             |                       |
    |              : Tune           | x             | x             |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | System       : Clock          | x             | x             |                       |
    | Real Time    : Commands       | x             | x             |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | Aux          : Local ON/OFF   | x             | x             |                       |
    | Messages     : All Notes OFF  | x             | o 123-127     |                       |
    |              : Active Sense   | x             | x             |                       |
    |              : Reset          | x             | x             |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | Notes                         |                                                       |
    |                               |                                                       |
    +-------------------------------+-------------------------------------------------------+
      Mode 1: Omni On,  Poly          Mode 2: Omni On,  Mono          o: Yes                 
      Mode 3: Omni Off, Poly          Mode 4: Omni Off, Mono          x: No                  
