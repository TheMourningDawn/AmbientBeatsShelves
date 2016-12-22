#include "application.h"
#include "LEDAnimations.h"
#include "SpectrumEqualizer.h"

#define BORDER_LED_PIN    D2
#define SHELF_LED_PIN    D3

#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define BRIGHTNESS         200

LEDAnimations *animations;
SpectrumEqualizer *spectrum;

UDP udpMulticast;
int udpPort = 47555;
IPAddress udpIP(239,1,1,232);

int hueValue = 100;

void setup() {
    /*Serial.begin(11520);*/
    setupModeFunctions();
    connectToRemote();

    spectrum = new SpectrumEqualizer();
    animations = new LEDAnimations(spectrum);

    FastLED.addLeds<LED_TYPE, BORDER_LED_PIN, COLOR_ORDER>(animations->borderLeds , NUM_BORDER_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, SHELF_LED_PIN, COLOR_ORDER>(animations->allShelves, NUM_SHELF_LEDS).setCorrection(TypicalLEDStrip);

    animations->currentPattern = 0;
  }

void loop() {
    readColorFromRemote();

    animations->runCurrentAnimation();
    FastLED.show();

    // EVERY_N_MILLISECONDS(20) { animations->hueCounter++; } // slowly cycle the "base color" through the rainbow
}

void readColorFromRemote() {
   if(udpMulticast.parsePacket() > 0) {
     hueValue = udpMulticast.read() << 8 | udpMulticast.read();
   }
   animations->currentHue = hueValue;
}

void connectToRemote() {
   udpMulticast.begin(udpPort);
   udpMulticast.joinMulticast(udpIP);
}

void setupModeFunctions() {
    Particle.function("nextMode", nextMode);
    Particle.function("previousMode", previousMode);

    Particle.subscribe("NEXT_MODE", handleNextMode);
    Particle.subscribe("PREVIOUS_MODE", handlePreviousMode);
}

int nextMode(String mode) {
    int currentPattern = animations->nextPattern();
    char currentPatternString[5];
    sprintf(currentPatternString, "%i", currentPattern);
    Particle.publish("Current Pattern", currentPatternString);
    return 1;
}

void handleNextMode(const char *eventName, const char *data) {
    nextMode("notSureWhyIHaveToDoItLikeThis");
}

int previousMode(String mode) {
    int currentPattern = animations->previousPattern();
    char currentPatternString[5];
    sprintf(currentPatternString, "%i", currentPattern);
    Particle.publish("Current Pattern", currentPatternString);
    return 1;
}

void handlePreviousMode(const char *eventName, const char *data) {
    previousMode("seriouslyWhy?");
}
