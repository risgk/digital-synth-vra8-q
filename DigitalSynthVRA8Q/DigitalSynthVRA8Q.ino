//////////////////////////////////////////////////////////////////////////////////
// Digital Synth VRA8-Q -- We strongly recommend Arduino IDE 1.8.10             //
//                               (more precisely Arduino AVR Boards core 1.8.1) //
//////////////////////////////////////////////////////////////////////////////////

//#define DEBUG

#define SERIAL_SPEED (38400)    // For Serial MIDI
//#define SERIAL_SPEED (31250)    // For MIDI Shield (MIDI Breakout)

#define L_MONO_AUDIO_OUT_PIN (5)    // Pin 5 or 6
#define R_AUDIO_OUT_PIN (11)        // Pin 11

#define TRANSPOSE (0)    // Min: -24, Max: +24

//#define ENABLE_SPECIAL_PROGRAM_CHANGE    // Program Change by Control Change (112-119, 90)
#define ENABLE_OSCILLATOR_2



#include "common.h"
#include "audio-out.h"
#include "synth.h"
#include "serial-in.h"

void setup() {
  noInterrupts();
  Synth<0>::initialize();
  SerialIn<0>::open(SERIAL_SPEED);
  AudioOut<0>::open();    // D6 (or D5)
}

void loop() {
  while (true) {
    if (SerialIn<0>::available()) {
      uint8_t b = SerialIn<0>::read();
      Synth<0>::receive_midi_byte(b);
    }
    int8_t level = Synth<0>::clock();
    AudioOut<0>::write(level);
  }
}