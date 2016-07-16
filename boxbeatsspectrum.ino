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
#define FRAMES_PER_SECOND  240

LEDAnimations *animations;
SpectrumEqualizer *spectrum;

void setup() {
    spectrum = new SpectrumEqualizer();
    animations = new LEDAnimations(spectrum);
    FastLED.addLeds<LED_TYPE, BORDER_LED_PIN, COLOR_ORDER>(animations->borderLeds, NUM_BORDER_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.addLeds<LED_TYPE, SHELF_LED_PIN, COLOR_ORDER>(animations->allShelves, NUM_SHELF_LEDS).setCorrection(TypicalLEDStrip);

    Particle.function("nextMode", nextMode);
    Particle.function("previousMode", previousMode);

    for(int j=0;j<NUM_BORDER_LEDS;j++) {
        animations->borderLeds[j] = 0;
    }
    for(int j=0;j<NUM_SHELF_LEDS;j++) {
        animations->allShelves[j] = 0;
    }
    FastLED.show();
}

void loop() {
    animations->runCurrentAnimation();
    FastLED.show();
    FastLED.delay(1000 / FRAMES_PER_SECOND);

    EVERY_N_MILLISECONDS(20) { animations->hueCounter++; } // slowly cycle the "base color" through the rainbow
}

int nextMode(String mode) {
    int currentPattern = animations->nextPattern();
    char currentPatternString[40];
    sprintf(currentPatternString, "%i", currentPattern);
    Particle.publish("Current Pattern", currentPatternString);
    return 1;
}

int previousMode(String mode) {
    int currentPattern = animations->previousPattern();
    char currentPatternString[40];
    sprintf(currentPatternString, "%i", currentPattern);
    Particle.publish("Current Pattern", currentPatternString);
    return 1;
}
