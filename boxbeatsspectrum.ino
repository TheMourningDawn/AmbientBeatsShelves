#include "application.h"
#include "LEDAnimations.h"
#include "SpectrumEqualizer.h"

#define BORDER_LED_PIN    D2
#define SHELF_LED_PIN    D3

#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define BRIGHTNESS         120
#define FRAMES_PER_SECOND  360

LEDAnimations *animations;
SpectrumEqualizer *spectrum;

TCPClient client;
byte server[] = { 192, 168, 1, 125 };
int tcpPort = 54555;

int hueValue = 100;

/*SYSTEM_MODE(MANUAL);*/
/*SYSTEM_THREAD(ENABLED);*/

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
    if(client.connected()) {
        readColorFromRemote();
    } else {
      connectToRemote();
    }

    animations->runCurrentAnimation();
    FastLED.show();

    /*FastLED.delay(1000 / FRAMES_PER_SECOND);*/

    // EVERY_N_MILLISECONDS(20) { animations->hueCounter++; } // slowly cycle the "base color" through the rainbow
}

void readColorFromRemote() {
   if(client.available()) {
     hueValue = client.read() << 8 | client.read();
     /*Serial.printlnf("Reading from remote: %d", hueValue);*/
   }
   while(client.read() != -1) {
     /*Serial.println("Shelf lost bytes");*/
   }
   client.flush();
   /*Serial.printlnf("Final from remote: %d", hueValue);*/
   animations->hueCounter = hueValue;
}

void connectToRemote() {
 if (client.connect(server, tcpPort)) {
     Serial.println("Connected");
     Particle.publish("Connected");
 } else {
     Serial.println("Connection failed");
     Particle.publish("Connection failed");
 }
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
