#include "application.h"
#include "LEDAnimations.h"

#define BORDER_LED_PIN    D2
#define SHELF_LED_PIN    D3

#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define BRIGHTNESS         200

LEDAnimations *animations;

UDP udpMulticast;
int udpPort = 47555;
IPAddress udpIP(239,1,1,232);

int hueValue = 100;

void setup() {
    /*Serial.begin(11520);*/
    setupCloudModeFunctions();
    connectToRemote();

    animations = new LEDAnimations();

    FastLED.addLeds<LED_TYPE, BORDER_LED_PIN, COLOR_ORDER>(animations->borderLeds, NUM_BORDER_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, SHELF_LED_PIN, COLOR_ORDER>(animations->allShelves, NUM_SHELF_LEDS).setCorrection(TypicalLEDStrip);
  }

void loop() {
    readColorFromRemote();

    animations->runCurrentAnimation();
    FastLED.show();
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
