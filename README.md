# Digital Synth VRA8-Q v3.0.0

- 2021-01-24 ISGK Instruments
- <https://github.com/risgk/digital-synth-vra8-q>

## Concept

- Quadraphonic (4-voice Paraphonic) Synthesizer (MIDI Sound Module) with Stereo Chorus Effector for Arduino Uno

## Caution about Arduino

- We *strongly recommend* **Arduino AVR Boards core 1.8.3**
    - If you use other than AVR Boards core 1.8.3 (or modified this sketch), the sketch *may not work well*
        - CPU Busy LED (LED L) *may almost always lit*, rather than flashing occasionally

## Change History

- v3.1.0: Add OSC MODE (Paraphonic/Monophonic); Fix the instability of the pitch when a note (Gate) is turned on
- v3.0.0: Improve sound quality; Fix Oscillator pitch; Increase the CUTOFF frequency slightly (min: 880 Hz, max: 14.08 kHz); Change the Q range of RESONANCE (mix: 0.7, max: 8.0); Increasing RESONANCE decreases the filter gain; Support Velocity > CUTOFF (OFF/ON); Reduce pop noise when turning notes ON and OFF; Enable Short Delay FX; Change PRESET programs; Support All Sound OFF and Reset All Controllers; Other changes
- v2.2.0: Change PRESET; Other changes
- v2.1.0: Improve sound quality; Change PRESET; Other changes
- v2.0.0: Add Mono and Stereo 2-phase Chorus modes; Improve the pitch; Introduce No Decay (DECAY = 127); Improve the recommended circuit; Other changes
- v1.1.2: Change the oscillator drift
- v1.1.1: Modify the PRESET programs
- v1.1.0: Change the behavior of the oscillators
- v1.0.3: Fix the problems with bass or treble note on
- v1.0.2: Fix the oscillator frequency
- v1.0.1: Fix the release date

## Features

- Serial MIDI In (38.4 kbps)
    - We recommend [Hairless MIDI<->Serial Bridge](https://projectgus.github.io/hairless-midiserial/) to connect PC
    - **NOTE**: A combination of a **MIDI Shield** (or MIDI Breakout) and a **power supply adapter** is *better* to avoiding USB noise
        - To use MIDI Shield (or MIDI Breakout), take `#define SERIAL_SPEED (31250)` (31.25 kbps) in `"DigitalSynthVRA8Q.ino"`
        - Even using only the power supply adapter *significantly* reduces USB noise
- PWM Audio Out (Unipolar, Line Level) **L/Mono**: **Pin D5** (or D6), **R**: **Pin D11**
    - Sampling Rate: 31.25 kHz, PWM Rate: 62.5 kHz, Bit Depth: 8 bit
    - We recommend adding a RC filter (post LPF) circuit to reduce PWM ripples
        - A 2nd-order LPF with a cutoff frequency 33.9 kHz (R1 = R2 = 47 ohm, C1 = C2 = 100 nF) works *very* well
            - We recommend using film capacitors (K = +-10% or less)
        - A 1st-order LPF with a cutoff frequency 15.9 kHz (R = 100 ohm, C: 100 nF) works well
    - We recommend adding AC coupling capacitors (10 uF, electrolytic capacitors) to reduce DC components
- Files
    - `"DigitalSynthVRA8Q.ino"` is a sketch for Arduino Uno Rev3 (ATmega328P)
    - `"make-sample-wav-file.cc"` is for Debugging on PC
        - Requiring GCC (g++) or other
        - `"make-sample-wav-file-cc.bat"` makes a sample WAV file (working on Windows)
    - `"generate-*.rb"` generates source files
        - Requiring a Ruby execution environment

## VRA8-Q CTRL

- MIDI Controller (Editor) for VRA8-Q, HTML5 App (Web App)
- VRA8-Q CTRL converts Program Changes (#0-7 for PRESET) into Control Changes
- VRA8-Q CTRL stores the current control values and the user programs (#8-15) in a Web browser (localStorage)
- We recommend using Google Chrome, which implements Web MIDI API
- We recommend [loopMIDI](https://www.tobias-erichsen.de/software/loopmidi.html) (virtual loopback MIDI cable) to connect VRA8-Q

## Details of Control Change

- OSC WAVE (SAW/PUL):
    - Values 0 (0-63): Saw Wave
    - Values 127 (64-127): Pulse Wave (Square Wave)
- DECAY:
    - Values 0-126: Decay Time
    - Values 127: No Decay
- CHORUS DEPTH:
    - Value 0: Delay Time +/- 0.0 ms (min)
    - Value 32: Delay Time +/- 2.0 ms
    - Value 64: Delay Time +/- 4.1 ms
    - Value 126: Delay Time +/- 8.1 ms (max)
- CHORUS RATE:
    - Value 4: LFO Frequency 0.06 Hz (min)
    - Value 32: LFO Frequency 0.48 Hz
    - Value 64: LFO Frequency 0.95 Hz
    - Value 127: LFO Frequency 1.9 Hz (max)
- CHORUS DELAY TIME:
    - Value 0: 0.03 ms (min)
    - Value 64: 8.2 ms
    - Value 80: 10.3 ms
    - Value 127: 16.3 ms (max)
- CHORUS (-/-/M/S/S2):
    - Value 0 (0-15): Off
    - Value 32 (16-47): Off
    - Value 64 (48-79): Mono Chorus
    - Value 96 (80-111): Stereo Chorus
    - Value 127 (112-127): Stereo 2-phase Chorus
- V > CUTOFF (OFF/ON):
    - Values 0 (0-63): Velocity > CUTOFF Modulation OFF
    - Values 127 (64-127): Velocity > CUTOFF Modulation ON
- OSC MODE (P/M)
    - Values 0 (0-63): Paraphonic Mode
    - Values 127 (64-127): Monophonic Mode

## Sample Chorus Settings

- Setting Q1 -- CHORUS DEPTH: 32, CHORUS RATE: 32, CHORUS DELAY TIME: 80, CHORUS: 127
- Setting D1 -- CHORUS DEPTH: 32, CHORUS RATE: 16, CHORUS DELAY TIME: 80, CHORUS: 127
- Setting D2 -- CHORUS DEPTH: 40, CHORUS RATE: 16, CHORUS DELAY TIME: 60, CHORUS: 127
- Setting D3 -- CHORUS DEPTH: 24, CHORUS RATE: 32, CHORUS DELAY TIME: 60, CHORUS: 127
- Setting U1 -- CHORUS DEPTH: 32, CHORUS RATE: 32, CHORUS DELAY TIME: 20, CHORUS: 127
- Setting U2 -- CHORUS DEPTH: 32, CHORUS RATE: 48, CHORUS DELAY TIME: 20, CHORUS: 127

## MIDI Implementation Chart

      [Quadraphonic Synthesizer]                                      Date: 2021-01-24       
      Model: Digital Synth VRA8-Q     MIDI Implementation Chart       Version: 3.0.0         
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
    | Number       : True Voice     | ************* | 24-96         |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | Velocity     Note ON          | x             | x             |                       |
    |              Note OFF         | x             | x             |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | After        Key's            | x             | x             |                       |
    | Touch        Ch's             | x             | x             |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | Pitch Bend                    | x             | o             |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | Control                     1 | x             | o             | MODULATION            |
    | Change                     11 | x             | o             | EXPRESSION            |
    |                            64 | x             | o             | SUSTAIN PEDAL         |
    |                               |               |               |                       |
    |                            24 | x             | o             | OSC WAVE (SAW/PUL)    |
    |                           108 | x             | o             | OSC LEVEL             |
    |                               |               |               |                       |
    |                            16 | x             | o             | CUTOFF                |
    |                            17 | x             | o             | RESONANCE             |
    |                            18 | x             | o             | EG > CUTOFF (-/+)     |
    |                            31 | x             | o             | AMP EG (OFF/ON)       |
    |                               |               |               |                       |
    |                            23 | x             | o             | ATTACK                |
    |                            19 | x             | o             | DECAY                 |
    |                            27 | x             | o             | SUSTAIN               |
    |                               |               |               |                       |
    |                            81 | x             | o             | LFO DEPTH             |
    |                            80 | x             | o             | LFO RATE              |
    |                            82 | x             | o             | LFO > PITCH (-/+)     |
    |                            83 | x             | o             | LFO > CUTOFF (-/+)    |
    |                               |               |               |                       |
    |                            60 | x             | o             | CHORUS DEPTH          |
    |                            61 | x             | o             | CHORUS RATE           |
    |                            62 | x             | o             | CHORUS DELAY TIME     |
    |                            63 | x             | o             | CHORUS (-/-/M/S/S2)   |
    |                               |               |               |                       |
    |                            85 | x             | o             | PITCH BEND RANGE      |
    |                            89 | x             | o             | V > CUTOFF (OFF/ON)   |
    |                            87 | x             | o             | OSC MODE (P/M)        |
    |                               |               |               |                       |
    |                   90, 112-119 | x             | x             | (RESERVED)            |
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
    | Aux          : All Sound OFF  | x             | o 120         |                       |
    | Messages     : Reset All      | x             | o 121         |                       |
    |                Controllers    |               |               |                       |
    |              : Local ON/OFF   | x             | x             |                       |
    |              : All Notes OFF  | x             | o 123-127     |                       |
    |              : Active Sense   | x             | x             |                       |
    |              : Reset          | x             | x             |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | Notes                         |                                                       |
    |                               |                                                       |
    +-------------------------------+-------------------------------------------------------+
      Mode 1: Omni On,  Poly          Mode 2: Omni On,  Mono          o: Yes                 
      Mode 3: Omni Off, Poly          Mode 4: Omni Off, Mono          x: No                  
