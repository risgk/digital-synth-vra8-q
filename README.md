# Digital Synth VRA8-Q v0.0.0

- 2020-01-12 ISGK Instruments
- <https://github.com/risgk/digital-synth-vra8-q>


## Concept

- Quadraphonic Synthesizer (MIDI Sound Module) for Arduino Uno


## Caution about Arduino

- We *strongly recommend* **Arduino AVR Boards core 1.8.3**
    - If you use other than AVR Boards core 1.8.1 (or modified this sketch), the sketch *may not work well*
        - CPU Busy LED (LED L) *may almost always lit*, rather than flashing occasionally


## Features

- Sampling Rate: 31.25 kHz, Bit Depth: 8 bit, LPF Attenuation Slope: -12 dB/oct
- Serial MIDI In (38.4 kbps)
- PWM Audio Out (**L/Mono: Pin D5** or D6, **R: Pin D11**, Unipolar, Line Level), PWM Rate: 62.5 kHz
    - We recommend adding a RC filter circuit to reduce PWM ripples
        - A cutoff frequency 15.9 kHz (R: 100 ohm, C: 100 nF) works well
- We recommend [Hairless MIDI<->Serial Bridge](https://projectgus.github.io/hairless-midiserial/) to connect PC
    - **NOTE**: A combination of a **MIDI Shield** (or MIDI Breakout) and a **power supply adapter** is *better* to avoiding USB noise
        - To use MIDI Shield (or MIDI Breakout), take `#define SERIAL_SPEED (31250)` (31.25 kbps) in `"DigitalSynthVRA8Q.ino"`
        - Even using only the power supply adapter *significantly* reduces USB noise
- Files
    - `"DigitalSynthVRA8Q.ino"` is a sketch for Arduino/Genuino Uno Rev3 (ATmega328P)
    - `"make-sample-wav-file.cc"` is for Debugging on PC
        - Requiring GCC (g++) or other
        - `"make-sample-wav-file-cc.bat"` makes a sample WAV file (working on Windows)
    - `"generate-*.rb"` generates source files
        - Requiring a Ruby execution environment


## VRA8-Q CTRL

- MIDI Controller (Editor) for VRA8-Q, HTML5 App (Web App)
- VRA8-Q CTRL converts Program Changes (#0-7 for PRESET) into Control Changes
- VRA8-Q CTRL stores the current control values and the user programs (#8-15) in a Web browser (localStorage)
- We recommend Google Chrome, which implements Web MIDI API
- We recommend [loopMIDI](https://www.tobias-erichsen.de/software/loopmidi.html) (virtual loopback MIDI cable) to connect VRA8-Q


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


## MIDI Implementation Chart

      [Monophonic Synthesizer]                                        Date: 2020-01-12       
      Model: Digital Synth VRA8-Q     MIDI Implementation Chart       Version: 0.0.0         
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
