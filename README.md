
<br>

<div align = center>

[![Badge License]][License]   
[![Badge Version]][Releases]

<br>

# Digital Synth VRA8-Q

*A Quadraphonic (4-voice Paraphonic) Synthesizer* <br>
*(MIDI Sound Module) with Stereo Chorus Effector* <br>
*for Arduino Uno.*

*2022 - 06 - 26 ISGK Instruments*

<br>
<br>

[![Button Design]][Design]   
[![Button Sample]][Sample]   
[![Button Midi]][Midi]   
[![Button Changes]][Changes]
  
</div>

<br>
<br>


<br>

## Caution ( Arduino )

-   We strongly recommend **Arduino AVR Boards Core 1.8.3**, <br>
    other cores or modifying this sketch may not work well.

-   The CPU Busy LED ( L ) may almost <br>
    always be lit, rather than flashing.

<br>
<br>

## Features

-   Serial MIDI In ( `38.4 kbps` )

    We recommend **[Hairless MIDI ⟷ Serial Bridge]** to connect PC
    
    ### Note
    
    A combination of a **MIDI Shield** / **Breakout** and a <br>
    **power supply adapter** is *better* to avoiding USB noise.
    
    -   To use MIDI Shield / Breakout, remove the <br>
        following from [`DigitalSynthVRA8Q.ino`].
    
        ```cpp
        #define SERIAL_SPEED (31250) // 31.25 kbps
        ```
    
    -   Even using only the power supply adapter <br>
        *significantly* reduces USB noise

    <br>

-   PWM Audio Out (Unipolar, Line Level) **L/Mono**: **Pin D5** (or D6), **R**: **Pin D11**
    
    -   Sampling Rate: `31.25 kHz`
    
    -   PWM Rate: `62.5 kHz`
    
    -   Bit Depth: `8 bit`
    
    -   We recommend adding an RC filter (post LPF) circuit to reduce PWM ripples
        
        -   A 1st-order LPF with a cutoff frequency `7.2 kHz` works well
        
            R = `220 Ohm`
            
            C = `100 nF`
        
        -   A 2nd-order LPF with a cutoff frequency `33.9 kHz` works well, too
        
            R1 = R2 = `47 ohm`
            
            C1 = C2 = `100 nF`

<br>
<br>

## Files

-   [`DigitalSynthVRA8Q.ino`]

    A sketch for the Arduino Uno Rev3
    
    *ATmega328P*

-   [`make-sample-wav-file.cc`]

    Debugging tool for PC
    
    -   Requiring GCC (g++) or similar
    
    -   [`make-sample-wav-file-cc.bat`]
    
        Makes a sample WAV file
        
        *Windows Only*

-   [`generate-*.rb`]

    Generates source files

    Requiring a Ruby execution environment

<br>
<br>

## VRA8-Q CTRL

-   MIDI Controller (Editor) Application <br>
    for VRA8-Q, HTML App (Web App)

-   VRA8-Q CTRL converts Program Changes <br>
    (#0-7 for PRESET) into Control Changes

-   VRA8-Q CTRL stores the current control <br>
    values and the user programs (#8-15) <br>
    in a Web browser (localStorage)

-   We recommend using Chromium, <br>
    which implements Web MIDI API

-   We recommend **[LoopMIDI]** - virtual <br>
    loopback MIDI cable - to connect VRA8-Q

<br>

<!----------------------------------------------------------------------------->

[Hairless MIDI ⟷ Serial Bridge]: https://projectgus.github.io/hairless-midiserial/
[Releases]: https://github.com/risgk/digital-synth-vra8-q/releases
[LoopMIDI]: https://www.tobias-erichsen.de/software/loopmidi.html

[Changes]: Documentation/Changelog.md
[License]: LICENSE
[Sample]: Documentation/Sample%20Settings.md
[Design]: Documentation/Design.md
[Midi]: Documentation/Midi%20Implementation.md

[`make-sample-wav-file-cc.bat`]: make-sample-wav-file-cc.bat
[`make-sample-wav-file.cc`]: make-sample-wav-file.cc
[`DigitalSynthVRA8Q.ino`]: DigitalSynthVRA8Q/DigitalSynthVRA8Q.ino
[`generate-*.rb`]: generate-sample-midi-stream.rb


<!----------------------------------[ Badges ]--------------------------------->

[Badge License]: https://img.shields.io/badge/-Zero-ae6c18.svg?style=for-the-badge&labelColor=EF9421&logoColor=white&logo=CreativeCommons
[Badge Version]: https://img.shields.io/github/v/release/risgk/digital-synth-vra8-q?include_prereleases&style=for-the-badge&labelColor=009639&logoColor=white&logo=GoogleAnalytics&color=036c2b


<!---------------------------------[ Buttons ]--------------------------------->

[Button Changes]: https://img.shields.io/badge/Changelog-EF2D5E?style=for-the-badge&logoColor=white&logo=GitBook
[Button Sample]: https://img.shields.io/badge/Sample_Settings-3884FF?style=for-the-badge&logoColor=white&logo=Serverless
[Button Design]: https://img.shields.io/badge/Design-179287?style=for-the-badge&logoColor=white&logo=Cloudsmith
[Button Midi]: https://img.shields.io/badge/Midi_Implementation-F28D1A?style=for-the-badge&logoColor=white&logo=MIDI
