
# MIDI Implementation Chart

      [Quadraphonic Synthesizer]                                      Date: 2022-06-26       
      Model: Digital Synth VRA8-Q     MIDI Implementation Chart       Version: 5.2.0         
    +-------------------------------+---------------+---------------+-------------------------+
    | Function...                   | Transmitted   | Recognized    | Remarks                 |
    +-------------------------------+---------------+---------------+-------------------------+
    | Basic        Default          | x             | 1             |                         |
    | Channel      Changed          | x             | x             |                         |
    +-------------------------------+---------------+---------------+-------------------------+
    | Mode         Default          | x             | Mode 3        |                         |
    |              Messages         | x             | x             |                         |
    |              Altered          | ************* |               |                         |
    +-------------------------------+---------------+---------------+-------------------------+
    | Note                          | x             | 0-127         |                         |
    | Number       : True Voice     | ************* | 24-96         |                         |
    +-------------------------------+---------------+---------------+-------------------------+
    | Velocity     Note ON          | x             | o (V=1-127)   | For VELOCITY > CUTOFF   |
    |              Note OFF         | x             | x             |                         |
    +-------------------------------+---------------+---------------+-------------------------+
    | After        Key's            | x             | x             |                         |
    | Touch        Ch's             | x             | x             |                         |
    +-------------------------------+---------------+---------------+-------------------------+
    | Pitch Bend                    | x             | o             |                         |
    +-------------------------------+---------------+---------------+-------------------------+
    | Control                     1 | x             | o             | MODULATION              |
    | Change                     11 | x             | o             | EXPRESSION              |
    |                            64 | x             | o             | SUSTAIN PEDAL (OFF/ON)  |
    |                               |               |               |                         |
    |                            24 | x             | o             | OSC WAVE (SAW/PLS)      |
    |                           108 | x             | o             | OSC LEVEL               |
    |                           104 | x             | o             | EG > PITCH (-/+)        |
    |                               |               |               |                         |
    |                            16 | x             | o             | CUTOFF                  |
    |                            17 | x             | o             | RESONANCE               |
    |                            18 | x             | o             | EG > CUTOFF (-/+)       |
    |                            31 | x             | o             | AMP EG (OFF/ON)         |
    |                               |               |               |                         |
    |                            23 | x             | o             | ATTACK                  |
    |                            19 | x             | o             | DECAY                   |
    |                            27 | x             | o             | SUSTAIN                 |
    |                               |               |               |                         |
    |                            81 | x             | o             | LFO DEPTH               |
    |                            80 | x             | o             | LFO RATE                |
    |                            82 | x             | o             | LFO > PITCH (-/+)       |
    |                            83 | x             | o             | LFO > CUTOFF (-/+)      |
    |                               |               |               |                         |
    |                            14 | x             | o             | LFO WAVE (T/T2/S/RND/P) |
    |                            15 | x             | o             | LFO FADE TIME           |
    |                               |               |               |                         |
    |                            60 | x             | o             | CHORUS DEPTH            |
    |                            61 | x             | o             | CHORUS RATE             |
    |                            62 | x             | o             | CHORUS DELAY TIME       |
    |                            63 | x             | o             | CHORUS (-/M/PS/S/S2)    |
    |                               |               |               |                         |
    |                            85 | x             | o             | PITCH BEND RANGE        |
    |                            89 | x             | o             | VELOCITY > CUTOFF       |
    |                            87 | x             | o             | VOICE (PARA/MONO/LGT)   |
    |                            22 | x             | o             | PORTAMENTO              |
    |                               |               |               |                         |
    |                            25 | x             | o             | MONO OSC2 MIX (-/HF/FL) |
    |                            20 | x             | o             | MONO OSC2 PITCH         |
    |                            21 | x             | o             | MONO OSC2 DETUNE        |
    |                               |               |               |                         |
    |                   90, 112-119 | x             | x             | (RESERVED)              |
    +-------------------------------+---------------+---------------+-------------------------+
    | Program                       | x             | o             |                         |
    | Change       : True #         | ************* | 0-7           |                         |
    +-------------------------------+---------------+---------------+-------------------------+
    | System Exclusive              | x             | x             |                         |
    +-------------------------------+---------------+---------------+-------------------------+
    | System       : Song Pos       | x             | x             |                         |
    | Common       : Song Sel       | x             | x             |                         |
    |              : Tune           | x             | x             |                         |
    +-------------------------------+---------------+---------------+-------------------------+
    | System       : Clock          | x             | x             |                         |
    | Real Time    : Commands       | x             | x             |                         |
    +-------------------------------+---------------+---------------+-------------------------+
    | Aux          : All Sound OFF  | x             | o 120         |                         |
    | Messages     : Reset All      | x             | o 121         |                         |
    |                Controllers    |               |               |                         |
    |              : Local ON/OFF   | x             | x             |                         |
    |              : All Notes OFF  | x             | o 123-127     |                         |
    |              : Active Sense   | x             | x             |                         |
    |              : Reset          | x             | x             |                         |
    +-------------------------------+---------------+---------------+-------------------------+
    | Notes                         |                                                         |
    |                               |                                                         |
    +-------------------------------+---------------------------------------------------------+
      Mode 1: Omni On,  Poly          Mode 2: Omni On,  Mono          o: Yes                 
      Mode 3: Omni Off, Poly          Mode 4: Omni Off, Mono          x: No                  

<br>