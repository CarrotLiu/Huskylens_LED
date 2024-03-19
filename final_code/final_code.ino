#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include <FastLED.h>

#define NUM_LEDS  30
#define LED_PIN   3

CRGB leds[NUM_LEDS];
uint8_t colorIndex[NUM_LEDS];

CRGBPalette16 greenblue = greenblue_gp;
CRGBPalette16 dobamine = dobamine_gp;

int timeCnt = 200;

int amax = 100;
int a = 0;
uint8_t max_bright = 100; //0~255

HUSKYLENS huskylens;
SoftwareSerial mySerial(10, 11); // RX, TX
//HUSKYLENS green line >> Pin 10; blue line >> Pin 11
void printResult(HUSKYLENSResult result);

void setup() {
    Serial.begin(115200);
    mySerial.begin(9600);
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    for (int i = 0; i < NUM_LEDS; i++) {
      colorIndex[i] = random8();
    }
    while (!huskylens.begin(mySerial))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }
}

void loop() {
    if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    else if(!huskylens.isLearned()){
      Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
    } 
    else if(!huskylens.available()){
      if(a >0){
        for(int i = a; i > 0; i-- ){
          a--;
          delay(100);
          FastLED.setBrightness(a);
          for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = ColorFromPalette( PartyColors_p, random8(255), a, LINEARBLEND);
          }
          FastLED.show();
        }
      }else{
        FastLED.setBrightness(a);
        fill_solid(leds,(NUM_LEDS-1),CRGB::Black);
        FastLED.show();
      }
      Serial.println(F("No block or arrow appears on the screen!")); 
    } 
    else{
        Serial.println(F("###########"));
        if (huskylens.available())
        {
          HUSKYLENSResult result = huskylens.read();
          printResult(result);
          Serial.println(huskylens.available());
          
          if(a <= 0){
            for(int i = 0; i<amax; i++){
              a++;
              delay(100);
              FastLED.setBrightness(a);
              for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = ColorFromPalette( PartyColors_p, random8(255), a, LINEARBLEND);
              }
              FastLED.show();
              delay(100);
            }
          }
        }
      }    
    }


void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{
        Serial.println("Object unknown!");
    }
}