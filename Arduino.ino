#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
SoftwareSerial s(5,6); // (RX, TX)
FASTLED_USING_NAMESPACE


// Aurduino

#define DATA_PIN            7
#define NUM_LEDS            300
#define LED_TYPE            WS2812B
#define COLOR_ORDER         GRB


CRGB leds[NUM_LEDS];

String operationType = "effect";
uint8_t brightness = 60;
int effectID = 0;



CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;
static uint8_t startIndex = 0;
int timeDelay = 100;


long previousMillis = 0;
long interval = 30;



void setup() {
  s.begin(9600);     
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS);
  LEDS.setBrightness(brightness);
  FastLED.clear(true);
  

  currentPalette = PartyColors_p;
  currentBlending = LINEARBLEND;
}



void loop() {
    //setupRainBowPaletteSlow();
    
    //startIndex = startIndex + 1; /* motion speed */
    //FillLEDsFromPaletteColors(startIndex);
    
    //FastLED.show();
    //FastLED.delay(timeDelay);

    unsigned long currentMillis = millis();

    
    
    if (s.available() > 0) {
        String data = "";
        data = s.readStringUntil('\r');
    
        if (contains(data, ',')) {
            String hue = getValueSplitBy(data, ',', 0);
            String sat = getValueSplitBy(data, ',', 1);
            String bri = getValueSplitBy(data, ',', 2);
    
            s.write(hue.toInt());
            s.write(sat.toInt());
            s.write(bri.toInt());


            operationType = "rgb";
            allRGB(hue.toInt(), sat.toInt(), bri.toInt());
        }
    
        else if (contains(data, ';')) {
            String brigth = getValueSplitBy(data, ';', 0);
      
            s.write(brigth.toInt());
            s.write(brigth.toInt());

            brightness = brigth.toInt();
            LEDS.setBrightness(brightness);

            FastLED.show();
        }

        else if (contains(data, ':')) {
            effectID = getValueSplitBy(data, ';', 0).toInt();

            s.write(effectID);

            operationType = "effect";
        }
  

    } else {
        if (operationType == "effect") {
            if (effectID == 0) {
                if (currentMillis - previousMillis > interval) {
                    previousMillis = currentMillis;         // save the last time you blinked the LED 
                    setupRainBowPaletteSlow();
                    
                    startIndex = startIndex + 1; /* motion speed */
                    FillLEDsFromPaletteColors(startIndex);
                    
                    FastLED.show();
                }
            }

        }
    }

}



















String getValueSplitBy(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}





boolean contains(String data, char separator) {
  boolean found = false;
  int maxIndex = data.length();

  for(int i=0; i < maxIndex; i++){
    if (data.charAt(i) == separator){
        found = true;
        break;
    }
  }

  return found;
}


















void FillLEDsFromPaletteColors(uint8_t colorIndex) {
    uint8_t brightness = 70;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
    
}


DEFINE_GRADIENT_PALETTE(heatmap_gp ) {
  0,     0,  0,  0,   //black
128,   255,  0,  0,   //red
224,   255,255,  0,   //bright yellow
255,   255,255,255 }; //full white

void setupPurpleAndGreenPalette() {
    CRGB green1  = Color("550057");
    CRGB green2  = Color("320033");
    CRGB darkBlue  = Color("0C4B97");
    CRGB darkBlue2  = Color("114B55");
    
    currentPalette = CRGBPalette16(
                                   green1,  green1,  darkBlue2,  darkBlue2,
                                   green2, green2, darkBlue,  darkBlue,
                                   darkBlue2,  darkBlue2,  green2,  green2,
                                   green1, green1, darkBlue,  darkBlue );

    currentPalette = heatmap_gp;

    currentBlending = LINEARBLEND; 
}






void setupRainBowPaletteSlow() {
    currentPalette = RainbowColors_p;        
    currentBlending = LINEARBLEND;
    timeDelay=30;
}

void setupRainBowLinearFast() {
    currentPalette = RainbowStripeColors_p;        
    currentBlending = LINEARBLEND;
    timeDelay=10;
}

void setupRainBowLinearSuperFast() {
    currentPalette = PartyColors_p;        
    currentBlending = LINEARBLEND;
    timeDelay=1;
}


void setupEyeCrusherPalette() {
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV(random8(), 255, random8());
    }
    timeDelay=300;
}










void check() {
  for (int i=70; i < NUM_LEDS+70; i++) {
      leds[i-70] = CHSV(i, i, 90);
      FastLED.show();
      delay(10);
  }
}


void allRGB(double hue, double saturation, double bright) {
  //Calculate hue from 360 to FastLED's 255
  double hueProcent = (hue*100)/360;
  double finalHue = (hueProcent*255)/100;

  //Calculate saturation from 100 to FastLED's 255
  double finalSaturation = (saturation*255)/100;
  
  //Calculate brightness from 100 to FastLED's 255
  double finalBright = (bright*255)/100;


  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(finalHue, finalSaturation, finalBright);         //Update all leds
  }

  FastLED.show();        //Show all leds, after they're loaded
}





void rainbow(int color) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(color, 255, brightness);
        color -= 3;
    }
}







long Color(String hexadecimal) {
  long color = strtol(hexadecimal.c_str(), NULL, 16);
  return color;
}
