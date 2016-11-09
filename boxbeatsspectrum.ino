#include "application.h"
#include "LEDAnimations.h"
#include "SpectrumEqualizer.h"

#define BORDER_LED_PIN    D2
#define SHELF_LED_PIN    D3

#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define NUM_SHELF_LEDS 60
#define LEDS_PER_SHELF 20
#define NUM_BORDER_LEDS 147
#define NUM_TOTAL_LEDS NUM_BORDER_LEDS + NUM_SHELF_LEDS

#define BRIGHTNESS         120
#define FRAMES_PER_SECOND  360

LEDAnimations *animations;
SpectrumEqualizer *spectrum;

TCPClient client;
byte server[] = { 192, 168, 1, 103 };
int port = 54555;

void setup() {
    Particle.function("nextMode", nextMode);
    Particle.function("previousMode", previousMode);
    // Serial.begin(9600);

    Particle.subscribe("NEXT_MODE", handleNextMode);
    Particle.subscribe("PREVIOUS_MODE", handlePreviousMode);

    if (client.connect(server, port)) {
        Serial.println("Connected");
        Particle.publish("Connected");
    } else {
        Serial.println("Connection failed");
    }

    spectrum = new SpectrumEqualizer();
    animations = new LEDAnimations(spectrum);
    FastLED.addLeds<LED_TYPE, BORDER_LED_PIN, COLOR_ORDER>(animations->borderLeds, NUM_BORDER_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, SHELF_LED_PIN, COLOR_ORDER>(animations->allShelves, NUM_SHELF_LEDS).setCorrection(TypicalLEDStrip);

    //  for(int i=0;i<LEDS_PER_SHELF;i++) {
    //     animations->bottomShelfLeds[i] = animations->allShelves[i];
    // }
    for(int j=0;j<NUM_BORDER_LEDS;j++) {
      animations->borderLeds[j] = 0;
    }
    for(int j=0;j<NUM_SHELF_LEDS;j++) {
      animations->allShelves[j] = 0;
    }
    FastLED.show();

    animations->currentPattern = 0;
}

int hueValue = 100;
void loop() {
    if(client.connected()) {
        client.println("s");
        if(client.available()) {
            char hueFromController[3];
            int readCount = 0;
            while(client.available()) {
                char c = client.read();
                hueFromController[readCount] = c;
                readCount++;
            }
            hueValue = atoi(hueFromController);
        }
        animations->hueCounter = hueValue;
    } else {
        if (client.connect(server, port)) {
            Serial.println("Reconnected");
            Particle.publish("Reconnected");
        } else {
            Serial.println("Reconnection failed");
        }
    }
    animations->runCurrentAnimation();
    FastLED.show();
    // FastLED.delay(1000 / FRAMES_PER_SECOND);

    // EVERY_N_MILLISECONDS(20) { animations->hueCounter++; } // slowly cycle the "base color" through the rainbow
}

int nextMode(String mode) {
    int currentPattern = animations->nextPattern();
    char currentPatternString[5];
    sprintf(currentPatternString, "%i", currentPattern);
    Particle.publish("Current Pattern", currentPatternString);
    return 1;
}

void handleNextMode(const char *eventName, const char *data) {
    nextMode("fuckoff");
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
