//#define DEBUG

#define SERIAL_SPEED (38400)    // For Serial MIDI
//#define SERIAL_SPEED (31250)    // For MIDI Shield

//#define SUBSTITUTE_PIN_D5_FOR_D6_AS_AUDIO_OUT    // For MIDI Shield

#define ENABLE_LFO_LED_OUT           // D5 (or D6)
#define LFO_LED_OUT_ACTIVE (HIGH)    // LOW for MIDI Shield

#define TRANSPOSE (0)    // Min: -24, Max: +24

//#define ENABLE_VOLTAGE_CONTROL    // Make the sketch operate as **VRA8-N mini**
                                  // - Sub Oscillator disabled
                                  // - See "cv-in.h"
#define USE_PITCH_CV_IN           // Comment out this line, if you use a MIDI keyboard



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
