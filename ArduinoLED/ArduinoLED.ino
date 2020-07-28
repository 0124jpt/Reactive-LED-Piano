#include "MIDIUSB.h"
#include "FastLED.h" 

FASTLED_USING_NAMESPACE

#define DATA_PIN    6
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    176
#define HUE_OFFSET 90
CRGB leds[NUM_LEDS];

#define BRIGHTNESS         255
#define FRAMES_PER_SECOND  120

uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint8_t rainbowPos[NUM_LEDS] = {0};
uint8_t rainbowDivision = 255 / NUM_LEDS;

int header, channel, note, velocity;
int colorVal = 0;

void setup() {
  Serial.begin(115200);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(255); // max brightness


  for (int i = 0; i < NUM_LEDS; i++) {
    rainbowPos[i] = (gHue + HUE_OFFSET) % 255;
    gHue += rainbowDivision;
  }

}

void loop() {
  
  MIDIread();

  FastLED.show();
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



    if(_channel == 0){ // color spectrum
      for (int i = 0; i < 2; i++) {
        leds[nn + i].setHSV(rainbowPos[nn + i], 255, value*2 /*map(value, 0, 127, 100, 255)*/); //HSV
      }
    }
    else if(_channel == 1){ // random color
      for (int i = 0; i < 2; i++) {
        leds[nn + i].setHSV(randColor,255,value*2);
      }
    }
    else if(_channel == 2){
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::Crimson;
      }
    }
    else if(_channel == 3){
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::Coral;
      }
    }
    else if(_channel == 4){
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::Orange;
      }
    }
    else if(_channel == 5){
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::Gold;
      }
    }
    else if(_channel == 6){
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::Yellow;
      }
    }
    else if(_channel == 7){
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::GreenYellow;
      }
    }
    else if(_channel == 8){
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::ForestGreen;
      }
    }
    else if(_channel == 9){
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::DarkGreen;
      }
    }
    else if(_channel == 10){
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::LightBlue;
      }
    }
    else if(_channel == 11){
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::Blue;
      }
    }
    else if(_channel == 12){
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::BlueViolet;
      }
    }
    else if(_channel == 13){
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::Fuchsia;
      }
    }
    else if(_channel == 14){
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::Tan;
      }
    }
    else if(_channel == 15){
      for (int i = 0; i < 2; i++) {
        leds[nn + i] = CRGB::White;
      }
    }
}

void handlennOff(byte channel, byte number, byte value)
{
   int nn = number - 21;
    nn *= 2;
    for (int i = 0; i < 2; i++) {
      leds[nn + i].setRGB(0, 0, 0);
    }
}



void MIDIread() {

  midiEventPacket_t ep = MidiUSB.read();
 

  if(ep.header == 0x9)
      handlennOn(
        ep.byte1 & 0xF,  //channel
        ep.byte2,        //pitch
        ep.byte3         //velocity
      );

  if(ep.header == 0x8)
      handlennOff(
        ep.byte1 & 0xF,  //channel
        ep.byte2,        //pitch
        ep.byte3         //velocity
      );
 
  if (ep.header != 0) {
    Serial.print(ep.byte1 & 0xF, DEC);
    Serial.print("-");
    Serial.print(ep.byte1, DEC);
    Serial.print("-");
    Serial.print(ep.byte2, DEC);
    Serial.print("-");
    Serial.println(ep.byte3, DEC);
  
}
}
