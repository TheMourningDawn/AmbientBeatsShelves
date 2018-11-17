#include "application.h"
#include "LEDAnimations.h"
#include "SpectrumEqualizer.h"

#define BORDER_LED_PIN    D2
#define SHELF_LED_PIN    D3

#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

SpectrumEqualizer *spectrum;
LEDAnimations *animations;

bool poweredOn = true;

UDP udpMulticast;
int udpPort = 47555;
IPAddress udpIP(239,1,1,232);

int hueValue = 200;
int saturationValue = 0;
int brightnessValue = 0;
int modeValue = 0;
int sensitivityValue = 0;

void setup() {
    setupCloudModeFunctions();
    connectToRemote();

    spectrum = new SpectrumEqualizer();
    animations = new LEDAnimations(spectrum);

    FastLED.addLeds<LED_TYPE, BORDER_LED_PIN, COLOR_ORDER>(animations->borderLeds, NUM_BORDER_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, SHELF_LED_PIN, COLOR_ORDER>(animations->allShelves, NUM_SHELF_LEDS).setCorrection(TypicalLEDStrip);
  }

void loop() {
    if(poweredOn) {
//      readColorFromRemote();
        readCurrentValues();

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

void readCurrentValues() {
    hueValue = animations->currentHue;
    saturationValue = animations->currentSaturation;
    brightnessValue = animations->currentBrightness;
    modeValue = animations->currentPattern;
    sensitivityValue = animations->globalSensitivity;
}

void setupCloudModeFunctions() {
    Particle.function("next-mode", nextMode);
    Particle.function("previous-mode", previousMode);
    Particle.function("set-mode", setPattern);

    Particle.function("next-frequency", nextFrequency);
    Particle.function("previous-frequency", previousFrequency);


    Particle.function("set-color", setColor);
    Particle.function("set-hue", setHue);
    Particle.function("set-saturation", setSaturation);
    Particle.function("set-brightness", setBrightness);
    Particle.function("set-sensitivy", setSensitivity);

    Particle.function("reset-device", resetDevice);
    Particle.function("enter-safe-mode", enterSafeMode);
    Particle.function("power-on", powerOn);
    Particle.function("power-off", powerOff);
    Particle.function("pause", pause);

    Particle.variable("current-hue", &hueValue, INT);
    Particle.variable("current-brightness", &brightnessValue, INT);
    Particle.variable("current-saturation", &saturationValue, INT);
    Particle.variable("current-pattern", &modeValue, INT);

    Particle.subscribe("NEXT_MODE", handleNextMode);
    Particle.subscribe("PREVIOUS_MODE", handlePreviousMode);
    Particle.subscribe("RESET", handleReset);
}

int resetDevice(String arg) {
   System.reset();

   return 0;
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

int pause(String arg) {
    poweredOn = !poweredOn;

    return 1;
}

int nextMode(String mode) {
    int currentPattern = animations->nextPattern();
    Particle.publish("Current Pattern", String(currentPattern));
    return currentPattern;
}

void handleNextMode(const char *eventName, const char *data) {
    nextMode("notSureWhyIHaveToDoItLikeThis");
}

int previousMode(String mode) {
    int currentPattern = animations->previousPattern();
    Particle.publish("Current Pattern", String(currentPattern));
    return currentPattern;
}

void handlePreviousMode(const char *eventName, const char *data) {
    previousMode("seriouslyWhy?");
}

int setPattern(String mode) {
    int currentPattern = animations->setPattern(mode.toInt());
    Particle.publish("Current Pattern", String(currentPattern));
    return currentPattern;
}

int nextFrequency(String frequency) {
    int frequencyMode = animations->nextFrequencyMode();

    return frequencyMode;
}

int previousFrequency(String frequency) {
    int frequencyMode = animations->previousFrequencyMode();

    return frequencyMode;
}

void handleReset(const char *eventName, const char *data) {
  resetDevice("whatever");
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
   animations->currentHue = hueValue;

    return hueValue;
}

int setHue(String hue) {
    hueValue = hue.toInt();
    animations->currentHue = hueValue;

    return hueValue;
}

int setSaturation(String saturationString) {
  animations->setCurrentSaturation(saturationString.toInt());

  return 1;
}

int setBrightness(String brightnessString) {
  animations->setCurrentBrightness(brightnessString.toInt());

  return 1;
}

int setSensitivity(String sensitivity) {
    animations->globalSensitivity = sensitivity.toInt();

    return sensitivity.toInt();
}
