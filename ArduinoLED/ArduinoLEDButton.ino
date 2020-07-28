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

int header, channel, velocity;
int buttonVal = 0;
int colorVal = 0;

void setup() {
  pinMode(9, INPUT);
  pinMode(10,OUTPUT);
  digitalWrite(10,HIGH);
  Serial.begin(115200);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(BRIGHTNESS); // master brightness


  for (int i = 0; i < NUM_LEDS; i++) {
    rainbowPos[i] = (gHue + HUE_OFFSET) % 255;
    gHue += rainbowDivision;
  }
}

void loop() {

  // reads button from pin 9 and increases the colorVal (0-6)
  buttonVal = digitalRead(9);
  if(buttonVal == HIGH){
      if(colorVal == 6){
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
  FastLED.delay(100 / FRAMES_PER_SECOND);
}

void handlennOn(byte _channel, byte number, byte value){

  int nn = number - 21;
  nn *= 2;
  byte randColor = random(0, 255);


  leds[nn].setHSV(rainbowPos[nn], 255, map(value, 0, 127, 100, 255)); //HSV
  leds[nn + 1].setHSV(rainbowPos[nn + 1], 255, map(value, 0, 127, 100, 255)); //HSV

    if(colorVal == 0){ // color spectrum
      for (int i = 0; i < 2; i++) {
        leds[nn + i].setHSV(rainbowPos[nn + i], 255, value*2 /*map(value, 0, 127, 100, 255)*/); //HSV
      }
    }
    else if(colorVal == 1){ // random color
      for (int i = 0; i < 2; i++) {
        leds[nn + i].setHSV(randColor,255,value*2);
      }
    }
    else if(colorVal == 2){
      for (int i = 0; i < 2; i++) {
        leds[nn + i].setHSV(0,0,value*2); // white
      }
    }
    else if(colorVal == 3){
      for (int i = 0; i < 2; i++) {
        leds[nn + i].setHSV(193,255,value*2); // purple
      }
    }
    else if(colorVal == 4){
      for (int i = 0; i < 2; i++) {
        leds[nn + i].setHSV(31,224,value*2); // orange
      }
    }
    else if(colorVal == 5){
      for (int i = 0; i < 2; i++) {
        leds[nn + i].setHSV(95,157,value*2); // green
      }
    }
    else if(colorVal == 6){
      for (int i = 0; i < 2; i++) {
        leds[nn + i].setHSV(142,255,value*2); // blue
      }
    }
}
void handlennOff(byte channel, byte number, byte value){
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
