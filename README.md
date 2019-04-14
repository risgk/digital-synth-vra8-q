# Digital Synth VRA8-N v2.1.2

- 2019-04-14 ISGK Instruments
- <https://github.com/risgk/digital-synth-vra8-n>

## Concept

- Monophonic Synthesizer (MIDI Sound Module) for Arduino Uno

## Change History

- v2.1.2
    - Add USE_PITCH_CV_IN option for **VRA8-N mini** (Undef it, if you use a MIDI keyboard)
- v2.1.1
    - Change the assignment of the potentiometers for **VRA8-N mini**
- v2.1.0 (Major Changes)
    - Narrow the CUTOFF frequency range from 5 oct (G3-G8) to 4 oct (G4-G8) for sound quality
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
    - **NOTE**: A combination of **MIDI Shield** (MIDI Breakout) and a power supply adapter is **more desirable** to avoiding USB noise
        - To use MIDI Shield, edit `SERIAL_SPEED`, `LFO_LED_OUT_ACTIVE`, and `SUBSTITUTE_PIN_D5_FOR_D6_AS_AUDIO_OUT` in `DigitalSynthVRA8N.ino`
- Files
    - `DigitalSynthVRA8N.ino` is a sketch for Arduino/Genuino Uno Rev3 (ATmega328P)
        - Arduino/Genuino Nano 3.x (ATmega328P) can also be used
    - `make-sample-wav-file.cc` is for Debugging on PC
        - Requiring GCC (g++) or other
        - `make-sample-wav-file-cc.bat` makes a sample WAV file (working on Windows)
    - `generate-*.rb` generates source files
        - Requiring a Ruby execution environment
- **CAUTION**: We *strongly recommend* **Arduino IDE 1.8.5**
    - `DigitalSynthVRA8N.ino` *does not work well* with Arduino IDE 1.8.6 or later
    - There is no restriction on a version of Arduino AVR Core
        - You can install the Arduino AVR Core 1.16.21 or later (in the Board Manager) for new Arduino Nano bootloader

## VRA8-N CTRL

- MIDI Controller (Editor) for VRA8-N, Web App
- VRA8-N CTRL converts Program Changes (#0-7 for PRESET) into Control Changes
- VRA8-N CTRL memorizes USER Programs (#8-15)
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
- "LFO FADE TIME": This affects "LFO DEPTH" but not "MODULATION DEPTH".
- "LEGATO (OFF/ON)": LEGATO Portamento
    - When LEGATO Portamento is ON, Single Trigger is forced
- "K. ASN (L/L/P/H/LST)": Key ASSIGN / Trigger Mode
    - Values 0-47: Lowest Note / Single Trigger
    - Values 48-79: Paraphonic (Lowest and Highest Notes) / Single Trigger
    - Values 80-111: Highest Note / Single Trigger
    - Values 112-127: LAST One Note / Multi Trigger
- "EXP BY VEL (OFF/ON)": EXPRESSION Control By (Note ON) VELOCITY

## A Sample Setting of a Physical Controller (8-Knob)

    +-------------------+---------------+---------------+---------------+
    | CC #16            | CC #17        | CC #23        | CC #19        |
    | CUTOFF            | RESONANCE     | ATTACK        | DECAY         |
    +-------------------+---------------+---------------+---------------+
    | CC #24            | CC #25        | CC #26        | CC #22        |
    | OSC (SAW/SQ)      | OSC MIX (1/2) | SUB LEVEL     | PORTAMENTO    |
    +-------------------+---------------+---------------+---------------+

## **VRA8-N mini** (Operation Mode)

- Voltage controlled (0-5V), Sub Oscillator disabled
- You need 4 potentiometers and 2 buttons
- To make the sketch operate as **VRA8-N mini**, edit `ENABLE_VOLTAGE_CONTROL` in `DigitalSynthVRA8N.ino`
- See "cv-in.h"

## MIDI Implementation Chart

      [Monophonic Synthesizer]                                        Date: 2019-04-14       
      Model: Digital Synth VRA8-N     MIDI Implementation Chart       Version: 2.1.2         
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
    |                            86 | x             | x             | (RESERVED)            |
    |                           106 | x             | o             | EXP > CUTOFF (-/+)    |
    |                           107 | x             | o             | EXP > AMP LEVEL       |
    |                               |               |               |                       |
    |                            22 | x             | o             | PORTAMENTO            |
    |                            30 | x             | o             | LEGATO (OFF/ON)       |
    |                            87 | x             | o             | K. ASN (L/L/P/H/LST)  |
    |                            89 | x             | o             | EXP BY VEL (OFF/ON)   |
    |                               |               |               |                       |
    |                   112-119, 90 | x             | x             | (RESERVED)            |
    +-------------------------------+---------------+---------------+-----------------------+
    | Program                       | x             | o             |                       |
    | Change       : True #         | ************* | 0-7           |                       |
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
