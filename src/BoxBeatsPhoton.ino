#include "application.h"
#include "LEDAnimations.h"
#include "SpectrumEqualizer.h"

#define BORDER_LED_PIN    D2
#define SHELF_LED_PIN    D3

#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define BRIGHTNESS 200

SpectrumEqualizer *spectrum;
LEDAnimations *animations;

bool poweredOn = true;

UDP udpMulticast;
int udpPort = 47555;
IPAddress udpIP(239,1,1,232);

int hueValue = 200;

void setup() {
    /*Serial.begin(11520);*/
    setupCloudModeFunctions();
    connectToRemote();

    spectrum = new SpectrumEqualizer();
    animations = new LEDAnimations(spectrum);

    FastLED.addLeds<LED_TYPE, BORDER_LED_PIN, COLOR_ORDER>(animations->borderLeds, NUM_BORDER_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, SHELF_LED_PIN, COLOR_ORDER>(animations->allShelves, NUM_SHELF_LEDS).setCorrection(TypicalLEDStrip);
  }

void loop() {
    if(poweredOn) {
      readColorFromRemote();

      animations->runCurrentAnimation();
      FastLED.show();
    }
}

void connectToRemote() {
   udpMulticast.begin(udpPort);
   udpMulticast.joinMulticast(udpIP);
}

void readColorFromRemote() {
   if(udpMulticast.parsePacket() > 0) {
     hueValue = udpMulticast.read() << 8 | udpMulticast.read();
   }
   animations->currentHue = hueValue;
}

void setupCloudModeFunctions() {
    Particle.function("nextMode", nextMode);
    Particle.function("previousMode", previousMode);
    Particle.function("setColor", setColor);

    // Going toward dash delimited names for functions!
    Particle.function("set-saturation", setSaturation);
    Particle.function("set-brightness", setBrightness);
    Particle.function("reset-device", resetDevice);
    Particle.function("enter-safe-mode", enterSafeMode);
    Particle.function("power-on", powerOn);
    Particle.function("power-off", powerOff);

    Particle.variable("current_hue", &hueValue, INT);

    Particle.subscribe("NEXT_MODE", handleNextMode);
    Particle.subscribe("PREVIOUS_MODE", handlePreviousMode);
}

int resetDevice(String arg) {
   System.reset();

   return 1;
}

int enterSafeMode(String arg) {
  System.enterSafeMode();

  return 1;
}

int powerOn(String arg) {
    poweredOn = true;

    return 1;
}

int powerOff(String arg) {
  poweredOn = false;

  animations->clearAllLeds();
  FastLED.show();

  return 1;
}

int nextMode(String mode) {
    int currentPattern = animations->nextPattern();
    Particle.publish("Current Pattern", String(currentPattern));
    return 1;
}

void handleNextMode(const char *eventName, const char *data) {
    nextMode("notSureWhyIHaveToDoItLikeThis");
}

int previousMode(String mode) {
    int currentPattern = animations->previousPattern();
    Particle.publish("Current Pattern", String(currentPattern));
    return 1;
}

void handlePreviousMode(const char *eventName, const char *data) {
    previousMode("seriouslyWhy?");
}

//Expects rgb values to be in r,g,b format e.g. 140,200,90
int setColor(String rgbString) {
   char buffer[12];
   rgbString.toCharArray(buffer, 12);
   String r = "";
   String g = "";
   String b = "";

   int rgbItem = 0;
   for(int i=0;i<12;i++) {
     if(buffer[i] != ',') {
       if(rgbItem == 0) {
         r.concat(buffer[i]);
       } else if(rgbItem == 1) {
         g.concat(buffer[i]);
       } else if(rgbItem == 2) {
         b.concat(buffer[i]);
       }
     } else {
       rgbItem++;
     }
   }

   CRGB rgb = CRGB(r.toInt(),g.toInt(),b.toInt());
   CHSV hsv = rgb2hsv_approximate(rgb);

   hueValue = hsv.hue;

   return 1;
}

int setSaturation(String saturationString) {
  animations->setCurrentSaturation(saturationString.toInt());

  return 1;
}

int setBrightness(String brightnessString) {
  animations->setCurrentBrightness(brightnessString.toInt());

  return 1;
}
