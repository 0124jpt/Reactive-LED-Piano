#include "MIDIUSB.h" // by Gary Grewal
#include "FastLED.h" // by Daniel Garcia

FASTLED_USING_NAMESPACE

#define DATA_PIN    6
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    176
#define HUE_OFFSET 90
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          255
#define FRAMES_PER_SECOND  120
//////////////////////////////////////

uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t rainbowPos[NUM_LEDS] = {0};
uint8_t rainbowDivision = 255 / NUM_LEDS;

int header, channel, note, velocity;
bool sustain = 0;
int buttonVal = 0;
int colorVal = 0;
//int held[NUM_LEDS];

void setup() {
  pinMode(9, INPUT);
  pinMode(10,OUTPUT);
  digitalWrite(10,HIGH);
  Serial.begin(115200);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);


  for (int i = 0; i < NUM_LEDS; i++) {
    rainbowPos[i] = (gHue + HUE_OFFSET) % 255;
    gHue += rainbowDivision;
    //Serial.println(rainbowPos[i]);
  }

}

void loop() {
  buttonVal = digitalRead(9);
if(buttonVal == HIGH){
    if(colorVal == 4){
      colorVal = 0;
      delay(500);
    }
     else{
      colorVal++;
      delay(500);
     }
  }
  
  MIDIread();

  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(100 / FRAMES_PER_SECOND);
}

void handlennOn(byte _channel, byte number, byte value)
{

  int nn = number - 21;
  note = number;
  nn *= 2;
  byte randColor = random(0, 255);


  leds[nn].setHSV(rainbowPos[nn], 255, map(value, 0, 127, 100, 255)); //HSV
  leds[nn + 1].setHSV(rainbowPos[nn + 1], 255, map(value, 0, 127, 100, 255)); //HSV


  //Serial.println(channel);

  switch (colorVal) {
    case 0:
      //      leds[nn].setHSV(rainbowPos[nn], 255, map(value, 0, 127, 100, 255)); //HSV
      //      leds[nn + 1].setHSV(rainbowPos[nn + 1], 255, map(value, 0, 127, 100, 255)); //HSV
      for (int i = 0; i < 2; i++) {
        leds[nn + i].setHSV(rainbowPos[nn + i], 255, value*2 /*map(value, 0, 127, 100, 255)*/); //HSV
      }
      break;
    case 1:
      for (int i = 0; i < 2; i++) {
        leds[nn + i].setHSV(randColor,255,value*2);
      }
      // leds[nn + 1].setHue(randColor);
      break;
    case 2:
      for (int i = 0; i < 2; i++) {
        leds[nn + i].setHSV(0, 0, value*2);
        //held[nn+i] = 1;
      }
      // leds[nn + 1] = CRGB::Blue;
      break;
    case 3:
      for (int i = 0; i < 2; i++) {
        leds[nn + i].setHSV(300, 100, value*2);
        //held[nn+i] = 1;

      }
      // leds[nn + 1] = CRGB::Green;
      break;
    case 4:
      for (int i = 0; i < 2; i++) {
        leds[nn + i].setHSV(16, 69, value*2);
      }
      //leds[nn + 1] = CRGB::Red;
      break;
    /*case 5:
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::HotPink;
      }
      //leds[nn + 1] = CRGB::HotPink;
      break;
    case 6:
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::Purple;
      }
      //leds[nn + 1] = CRGB::Purple;
      break;
    case 7:
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::Aqua;
      }
      //leds[nn + 1] = CRGB::Aqua;
      break;
    case 8:
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::Aquamarine;
      }
      //leds[nn + 1] = CRGB::Aquamarine;
      break;
    case 9:
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::Coral;
      }
      // leds[nn + 1] = CRGB::Coral;
      break;
    case 10:
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::DarkOrange;
      }
      //leds[nn + 1] = CRGB::DarkOrange;
      break;
    case 11:
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::DeepPink;
      }
      //leds[nn + 1] = CRGB::Blue;
      break;
    case 12:
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::Magenta;
      }
      //leds[nn + 1] = CRGB::Green;
      break;
    case 13:
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::Red;
      }
      //leds[nn + 1] = CRGB::Cyan;
      break;
    case 14:
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::Green;
      }
      //leds[nn + 1] = CRGB::Yellow;
      break;
    case 15:
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::White;
      }
      //leds[nn + 1] = CRGB::White;
      break;*/

  }
 if (value == 0 && sustain == 0) {
    for (int i = 0; i < 2; i++) {
      leds[nn + i].setRGB(0, 0, 0);
    }
    leds[nn + 1].setRGB(0, 0, 0);
  }
  //leds[nn].setRGB( 255, 68, 221); //RGB
  //leds[nn + 1] = CRGB::HotPink;*/
}

void handlennOff(byte channel, byte number, byte value)
{
  //if(sustain == 0){
   int nn = number - 21;
    nn *= 2;
    for (int i = 0; i < 2; i++) {
      //held[nn + i] = 0;
      leds[nn + i].setRGB(0, 0, 0);
    }
    //leds[nn + 1].setRGB(0, 0, 0);
  //}

}

void handleControlChange(byte channel, byte number, byte value)
{
    /*if(number==64 && value==0){
      for(int i=0; i<NUM_LEDS; i++){
        if(held[i] == 0) leds[i].setRGB(0,0,0);
      }
      sustain = 0;
    }
    else sustain = 1;*/
}

void MIDIread() {

  midiEventPacket_t rx = MidiUSB.read();
  switch (rx.header) {
    case 0:
      break;//No pending events

    case 0x9:
      handlennOn(
        rx.byte1 & 0xF,  //channel
        rx.byte2,        //pitch
        rx.byte3         //velocity
      );
      break;

    case 0x8:
      handlennOff(
        rx.byte1 & 0xF,  //channel
        rx.byte2,        //pitch
        rx.byte3         //velocity
      );
      break;
      /*
      case 0xB:
        handleControlChange(
        rx.byte1 & 0xF,  //channel
        rx.byte2,        //cc number
        rx.byte3         //value
      );
      break;*/
  }

  if (rx.header != 0) {
    //Serial.print("Unhandled MIDI message: ");
     
    
    Serial.print(rx.byte1 & 0xF, DEC);
    Serial.print("-");
    Serial.print(rx.byte1, DEC);
    Serial.print("-");
    Serial.print(rx.byte2, DEC);
    Serial.print("-");
    Serial.println(rx.byte3, DEC);
  
}
}

/////////////////////////////////////////////
// Arduino (pro)micro midi functions MIDIUSB Library
void nnOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t nnOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(nnOn);
}

void nnOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t nnOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(nnOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}