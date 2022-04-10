
<h1>

Digital Synth VRA8-Q  
[![Badge License]][License]
![Badge Version]

</h1>

- 2022-06-26 ISGK Instruments
- <https://github.com/risgk/digital-synth-vra8-q>

## Concept

- Quadraphonic (4-voice Paraphonic) Synthesizer (MIDI Sound Module) with Stereo Chorus Effector for Arduino Uno

## Caution about Arduino

- We *strongly recommend* **Arduino AVR Boards core 1.8.3**
    - If you use other than AVR Boards core 1.8.3 (or modified this sketch), the sketch *may not work well*
        - CPU Busy LED (LED L) *may almost always lit*, rather than flashing occasionally


## Features

- Serial MIDI In (38.4 kbps)
    - We recommend [Hairless MIDI<->Serial Bridge](https://projectgus.github.io/hairless-midiserial/) to connect PC
    - **NOTE**: A combination of a **MIDI Shield** (or MIDI Breakout) and a **power supply adapter** is *better* to avoiding USB noise
        - To use MIDI Shield (or MIDI Breakout), take `#define SERIAL_SPEED (31250)` (31.25 kbps) in `"DigitalSynthVRA8Q.ino"`
        - Even using only the power supply adapter *significantly* reduces USB noise
- PWM Audio Out (Unipolar, Line Level) **L/Mono**: **Pin D5** (or D6), **R**: **Pin D11**
    - Sampling Rate: 31.25 kHz, PWM Rate: 62.5 kHz, Bit Depth: 8 bit
    - We recommend adding a RC filter (post LPF) circuit to reduce PWM ripples
        - A 1st-order LPF with a cutoff frequency 7.2 kHz (R = 220 ohm, C = 100 nF) works well
        - A 2nd-order LPF with a cutoff frequency 33.9 kHz (R1 = R2 = 47 ohm, C1 = C2 = 100 nF) works well, too
- Files
    - `"DigitalSynthVRA8Q.ino"` is a sketch for Arduino Uno Rev3 (ATmega328P)
    - `"make-sample-wav-file.cc"` is for Debugging on PC
        - Requiring GCC (g++) or other
        - `"make-sample-wav-file-cc.bat"` makes a sample WAV file (working on Windows)
    - `"generate-*.rb"` generates source files
        - Requiring a Ruby execution environment

## VRA8-Q CTRL

- MIDI Controller (Editor) Application for VRA8-Q, HTML App (Web App)
- VRA8-Q CTRL converts Program Changes (#0-7 for PRESET) into Control Changes
- VRA8-Q CTRL stores the current control values and the user programs (#8-15) in a Web browser (localStorage)
- We recommend using Google Chrome, which implements Web MIDI API
- We recommend [loopMIDI](https://www.tobias-erichsen.de/software/loopmidi.html) (virtual loopback MIDI cable) to connect VRA8-Q


<!----------------------------------------------------------------------------->

[Badge License]: https://img.shields.io/badge/License-CC0_1.0-lightgrey.svg
[Badge Version]: https://badge.fury.io/gh/risgk%2Fdigital-synth-vra8-q.svg

[License]: LICENSE
