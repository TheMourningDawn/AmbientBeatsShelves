#include "application.h"
#include "LEDAnimations.h"
#include "SpectrumEqualizer.h"

#define BORDER_LED_PIN    D2
#define SHELF_LED_PIN    D3

#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

SpectrumEqualizer *audioEqualizer;
LEDAnimations *animations;

UDP udpMulticast;
int udpPort = 47555;
IPAddress udpIP(239,1,1,232);

bool poweredOn = true;

int hue = 200;
int saturation = 0;
int brightness = 0;
int animation = 0;
int audioSensitivity = 0;

void setup() {
    setupCloudModeFunctions();
    connectToRemote();

    audioEqualizer = new SpectrumEqualizer();
    animations = new LEDAnimations(audioEqualizer);

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
       animations->currentHue = udpMulticast.read() << 8 | udpMulticast.read();
   }
}

void readCurrentValues() {
    hue = animations->currentHue;
    saturation = animations->currentSaturation;
    brightness = animations->currentBrightness;
    animation = animations->currentAnimation;
    audioSensitivity = animations->globalSensitivity;
}

void setupCloudModeFunctions() {
    // Up to 15 cloud functions may be registered and each function name is limited to a maximum of 12 characters.
    Particle.function("toggle-audio-reactive", toggleAudioReactive);

    Particle.function("next-animation", nextAnimation);
    Particle.function("previous-animation", previousAnimation);
    Particle.function("set-animation", setAnimation);

    Particle.function("cycle-frequency", nextFrequency);

    Particle.function("set-color", setColor);
    Particle.function("set-hue", setHue);
    Particle.function("set-saturation", setSaturation);
    Particle.function("set-brightness", setBrightness);
    Particle.function("set-sensitivity", setSensitivity);

    Particle.function("reset-device", resetDevice);
    Particle.function("enter-safe-mode", enterSafeMode);
    Particle.function("power-on", powerOn);
    Particle.function("power-off", powerOff);
    Particle.function("pause", pause);

    Particle.variable("current-hue", &hue, INT);
    Particle.variable("current-brightness", &brightness, INT);
    Particle.variable("current-saturation", &saturation, INT);
    Particle.variable("current-animation", &animation, INT);

    Particle.subscribe("NEXT_MODE", handleNextAnimation);
    Particle.subscribe("PREVIOUS_MODE", handlePreviousAnimation);
    Particle.subscribe("RESET", handleReset);
}

int resetDevice(String arg) {
   System.reset();

   return 0;
}

void handleReset(const char *eventName, const char *data) {
    resetDevice("whatever");
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

int toggleAudioReactive(String arg) {
    return animations->toggleAudioReactive();
}

int nextAnimation(String arg) {
    int currentAnimation = animations->nextAnimation();
    Particle.publish("Animation #", String(currentAnimation));
    return currentAnimation;
}

void handleNextAnimation(const char *eventName, const char *data) {
    nextAnimation("notSureWhyIHaveToDoItLikeThis");
}

int previousAnimation(String arg) {
    int currentAnimation = animations->previousAnimation();
    Particle.publish("Animation #", String(currentAnimation));
    return currentAnimation;
}

void handlePreviousAnimation(const char *eventName, const char *data) {
    previousAnimation("seriouslyWhy?");
}

int setAnimation(String animationNumber) {
    int currentAnimation = animations->setAnimation(animationNumber.toInt());
    Particle.publish("Animation #", String(currentAnimation));
    return currentAnimation;
}

int nextFrequency(String frequency) {
    int frequencyMode = animations->nextFrequencyMode();

    return frequencyMode;
}

int previousFrequency(String frequency) {
    int frequencyMode = animations->previousFrequencyMode();

    return frequencyMode;
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

   hue = hsv.hue;
   animations->currentHue = hue;

    return hue;
}

int setHue(String newHue) {
    hue = newHue.toInt();
    animations->currentHue = hue;

    return hue;
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
