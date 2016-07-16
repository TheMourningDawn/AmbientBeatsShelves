#ifndef LEDANIMATIONS_CPP
#define LEDANIMATIONS_CPP

#include "application.h"
#include "LEDAnimations.h"

CRGB borderLeds[NUM_BORDER_LEDS];
CRGB allShelves[NUM_SHELF_LEDS];
CRGB* bottomShelfLeds = allShelves + 0;
CRGB* middleShelfLeds = allShelves + LEDS_PER_SHELF;
CRGB* topShelfLeds = allShelves + 2 * LEDS_PER_SHELF;

SpectrumEqualizer *equalizer;

int globalSensitivity = 0;
uint8_t frequencyMode[7] = {0, 1, 2, 3, 4, 5, 6};
int currentPattern = 0;
int numberOfPatterns = 7;
uint8_t hueCounter = 0;

typedef void (*SimplePatternList[])();

// SimplePatternList patterns = {waterfallCascading, waterfall, confetti, equalizerRightToLeftBottomToTop, equalizerLeftToRightBottomToTop, rainbow, sinelon, juggle, bpm};

LEDAnimations::LEDAnimations() : equalizer(new SpectrumEqualizer()) {
    equalizer->init();
}

LEDAnimations::LEDAnimations(SpectrumEqualizer *eq) : equalizer(eq) {
    equalizer->init();
}

int LEDAnimations::runCurrentAnimation() {
    equalizer->readAudioFrequencies();

    if(currentPattern == 0) {
        waterfall();
    } else if(currentPattern == 1) {
        rainbow();
    } else if (currentPattern == 2) {
        juggle(equalizer->frequenciesLeft[4]);
    } else if (currentPattern == 3) {
        confetti(equalizer->frequenciesLeft[4]);
    } else if (currentPattern == 4) {
        sinelon(equalizer->frequenciesLeft[4]);
    } else if (currentPattern == 5) {
        bpm();
    } else if (currentPattern == 6) {
        equalizerLeftToRightBottomToTop();
    } else {
        for(int j=0;j<NUM_BORDER_LEDS;j++) {
            borderLeds[j] = 0;
        }
    }
}

int LEDAnimations::nextPattern() {
    currentPattern = (currentPattern + 1);
    return currentPattern;
}

int LEDAnimations::previousPattern() {
    currentPattern = (currentPattern - 1);
    return currentPattern;
}

void LEDAnimations::nextFrequencyMode() {
    int wrapEnd = frequencyMode[6];
    for(int i=6;i>0;i--) {
        frequencyMode[i] = frequencyMode[i-1];
    }
    frequencyMode[0] = wrapEnd;
}

void LEDAnimations::previousFrequencyMode() {
    int wrapBegining = frequencyMode[0];
    for(int i=0;i<6;i++) {
        frequencyMode[i] = frequencyMode[i+1];
    }
    frequencyMode[6] = wrapBegining;
}

int LEDAnimations::clampToRange(int numberToClamp, int lowerBound, int upperBound) {
    if (numberToClamp > upperBound) {
        return upperBound;
    } else if (numberToClamp < lowerBound) {
        return lowerBound;
    }
    return numberToClamp;
}

int LEDAnimations::clampSensitivity(int sensitivity) {
    return clampToRange(sensitivity, 0, 1023);
}

int LEDAnimations::wrapToRange(int numberToWrap, int lowerBound, int upperBound) {
    if (numberToWrap > upperBound) {
        return lowerBound;
    } else if (numberToWrap < lowerBound) {
        return upperBound;
    }
    return numberToWrap;
}


void LEDAnimations::rainbow() {
    if (equalizer->frequenciesLeft[frequencyMode[2]] > clampSensitivity(globalSensitivity + 600)) {
        fill_rainbow(borderLeds, NUM_TOTAL_LEDS, hueCounter, 7);
    }
}

// random colored speckles that blink in and fade smoothly
void LEDAnimations::confetti(int frequencyValue) {
    uint8_t position = random16(NUM_TOTAL_LEDS);
    uint16_t frequencyThreshold = clampSensitivity(globalSensitivity + 600);

    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 10);
    fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 10);

    if (frequencyValue > frequencyThreshold) {
        if (position > NUM_BORDER_LEDS) {
            allShelves[position % NUM_BORDER_LEDS] += CHSV(hueCounter + random8(64), 200, 255);
        } else {
            borderLeds[position] += CHSV(hueCounter + random8(64), 200, 255);
        }
    }
}

// a colored dot sweeping back and forth, with fading trails
void LEDAnimations::sinelon(int frequencyValue) {
    uint16_t frequencyThreshold = clampSensitivity(globalSensitivity + 600);

    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 5);
    fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 5);
    int pos = beatsin16(13, 0, NUM_TOTAL_LEDS);
    if (frequencyValue > frequencyThreshold) {
        if (pos > NUM_BORDER_LEDS) {
            allShelves[pos % NUM_BORDER_LEDS] += CHSV(hueCounter, 255, 192);
        } else {
            borderLeds[pos] += CHSV(hueCounter, 255, 192);
        }
    }
}

// colored stripes pulsing at a defined Beats-Per-Minute (BPM)
void LEDAnimations::bpm() {
    uint8_t BeatsPerMinute = 120;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
    for (int i = 0; i < NUM_BORDER_LEDS; i++) { //9948
        borderLeds[i] = ColorFromPalette(palette, hueCounter + (i * 2), beat - hueCounter + (i * 10));
    }
}

// eight colored dots, weaving in and out of sync with each other
void LEDAnimations::juggle(int frequencyValue) {
    uint16_t frequencyThreshold = clampSensitivity(globalSensitivity + 600);

    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 20);
    byte dothue = 0;
    if (frequencyValue > frequencyThreshold) {
        for (int i = 0; i < 8; i++) {
            borderLeds[beatsin16(i + 7, 0, NUM_BORDER_LEDS)] |= CHSV(dothue, 200, 255);
            dothue += 32;
        }
    }
}

void LEDAnimations::waterfall() {
    waterfallShelf(topShelfLeds, equalizer->frequenciesLeft[frequencyMode[6]], clampSensitivity(globalSensitivity + 500));
    waterfallShelf(middleShelfLeds, equalizer->frequenciesLeft[frequencyMode[1]], clampSensitivity(globalSensitivity + 500));
    waterfallShelf(bottomShelfLeds, equalizer->frequenciesLeft[frequencyMode[0]], clampSensitivity(globalSensitivity + 500));
    waterfallBorder(equalizer->frequenciesLeft[frequencyMode[4]], 500);
}

void LEDAnimations::waterfallCascading() {
    waterfallBorderCascading(equalizer->frequenciesLeft[frequencyMode[4]], clampSensitivity((globalSensitivity + 500)));
    waterfallShelf(topShelfLeds, equalizer->frequenciesLeft[frequencyMode[6]], clampSensitivity(globalSensitivity + 500));
    waterfallShelf(middleShelfLeds, equalizer->frequenciesLeft[frequencyMode[1]], clampSensitivity(globalSensitivity + 500));
    waterfallShelf(bottomShelfLeds, equalizer->frequenciesLeft[frequencyMode[0]], clampSensitivity(globalSensitivity + 500));
}

void LEDAnimations::waterfallShelf(CRGB shelf[], int frequencyValue, int frequencyThreshold) {
    if (frequencyValue > frequencyThreshold) {
        int mappedFrequencyValue = map(frequencyValue, frequencyThreshold, 1023, 0, 255);
        shelf[LEDS_PER_SHELF / 2] = CHSV(mappedFrequencyValue, 200, 255);
        shelf[LEDS_PER_SHELF / 2 + 1] = CHSV(mappedFrequencyValue, 200, 255);
    } else {
        shelf[LEDS_PER_SHELF / 2] = CRGB(0, 0, 0);
        shelf[LEDS_PER_SHELF / 2 + 1] = CRGB(0, 0, 0);
    }
    memmove(&shelf[0], &shelf[1], LEDS_PER_SHELF / 2 * sizeof(CRGB));
    memmove(&shelf[LEDS_PER_SHELF / 2], &shelf[LEDS_PER_SHELF / 2 - 1], LEDS_PER_SHELF / 2 * sizeof(CRGB));
}

void LEDAnimations::waterfallBorder(int frequencyValue, int frequencyThreshold) {
    if (frequencyValue > frequencyThreshold) {
        int mappedFrequencyValue = map(frequencyValue, frequencyThreshold, 1023, 0, 255);
        // EVERY_N_SECONDS(1) {
        // char howLong[40];
        // sprintf(howLong, "%i",frequencyValue);
        // char thisLong[40];
        // sprintf(thisLong, "%i", mappedFrequencyValue);
        // Particle.publish("4: ", howLong);
        // Particle.publish("4Mapped", thisLong);
        // }
        borderLeds[NUM_BORDER_LEDS / 2] = CHSV(mappedFrequencyValue, 200, 255);
    } else {
        borderLeds[NUM_BORDER_LEDS / 2] = CRGB(0, 0, 0);
    }
    memmove(&borderLeds[0], &borderLeds[1], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
    memmove(&borderLeds[NUM_BORDER_LEDS / 2], &borderLeds[NUM_BORDER_LEDS / 2 - 1], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
}

void LEDAnimations::waterfallBorderCascading(int frequencyValue, int frequencyThreshold) {
    if (frequencyValue > frequencyThreshold) {
        borderLeds[NUM_BORDER_LEDS / 2] = CHSV(map(frequencyValue, frequencyThreshold, 1023, 0, 255), 200, 255);
    } else {
        borderLeds[NUM_BORDER_LEDS / 2] = CRGB(0, 0, 0);
    }
    if(topShelfLeds[0]){
        borderLeds[42]+=topShelfLeds[0];
    }
    if(topShelfLeds[LEDS_PER_SHELF-1]) {
        borderLeds[104]+=topShelfLeds[LEDS_PER_SHELF-1];
    }
    if(middleShelfLeds[0]){
        borderLeds[21]+=middleShelfLeds[0];
    }
    if(middleShelfLeds[LEDS_PER_SHELF-1]) {
        borderLeds[125]+=middleShelfLeds[LEDS_PER_SHELF-1];
    }
    memmove(&borderLeds[0], &borderLeds[1], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
    memmove(&borderLeds[NUM_BORDER_LEDS / 2], &borderLeds[NUM_BORDER_LEDS / 2 - 1], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
}

void LEDAnimations::equalizerLeftToRightBottomToTop() {
    equalizerLeftBorder(equalizer->frequenciesLeft[frequencyMode[0]], clampSensitivity(globalSensitivity + 200), false);
    equalizerRightBorder(equalizer->frequenciesLeft[frequencyMode[6]], clampSensitivity(globalSensitivity + 200), false);
    equalizerTopBorder(equalizer->frequenciesLeft[frequencyMode[5]], clampSensitivity(globalSensitivity + 400), true);
    equalizerShelf(topShelfLeds, equalizer->frequenciesLeft[frequencyMode[4]], clampSensitivity(globalSensitivity + 400), false);
    equalizerShelf(middleShelfLeds, equalizer->frequenciesLeft[frequencyMode[3]], clampSensitivity(globalSensitivity + 400), true);
    equalizerShelf(bottomShelfLeds, equalizer->frequenciesLeft[frequencyMode[2]], clampSensitivity(globalSensitivity + 400), false);
}

void LEDAnimations::equalizerRightToLeftBottomToTop() {
    equalizerLeftBorder(equalizer->frequenciesLeft[frequencyMode[0]], clampSensitivity(globalSensitivity + 200), false);
    equalizerRightBorder(equalizer->frequenciesLeft[frequencyMode[6]], clampSensitivity(globalSensitivity + 200), false);
    equalizerTopBorder(equalizer->frequenciesLeft[frequencyMode[5]], clampSensitivity(globalSensitivity + 400), false);
    equalizerShelf(topShelfLeds, equalizer->frequenciesLeft[frequencyMode[4]], clampSensitivity(globalSensitivity + 400), true);
    equalizerShelf(middleShelfLeds, equalizer->frequenciesLeft[frequencyMode[3]], clampSensitivity(globalSensitivity + 400), false);
    equalizerShelf(bottomShelfLeds, equalizer->frequenciesLeft[frequencyMode[2]], clampSensitivity(globalSensitivity + 400), true);
}

void LEDAnimations::equalizerLeftBorder(int frequencyValue, int sensitivityThreshold, bool direction) {
    int ledsInSection = 62;
    for(int i = 0;i<ledsInSection;i++) {
        borderLeds[i].fadeToBlackBy(40);
    }
    if (frequencyValue > sensitivityThreshold) {
        int numberToLight = map(frequencyValue, sensitivityThreshold, 1023, 0, ledsInSection);
        CRGB color = CHSV(map(frequencyValue, sensitivityThreshold, 1023, 0, 255), 200, 255);
        if (direction == true) {
            for(int i = ledsInSection - numberToLight;i<ledsInSection;i++) {
                borderLeds[i] = color;
            }
        } else {
            for(int i = 0;i<numberToLight;i++) {
                borderLeds[i] = color;
            }
        }
    }
}

void LEDAnimations::equalizerRightBorder(int frequencyValue, int sensitivityThreshold, bool direction) {
    int ledsInSection = 62;
    int locationOffset = 84;
    for(int i = locationOffset;i<ledsInSection + locationOffset;i++) {
        borderLeds[i].fadeToBlackBy(40);
    }
    if (frequencyValue > sensitivityThreshold) {
        int numberToLight = map(frequencyValue, sensitivityThreshold, 1023, 0, ledsInSection);
        CRGB color = CHSV(map(frequencyValue, sensitivityThreshold, 1023, 0, 255), 200, 255);
        if (direction == true) {
            for(int i = locationOffset;i<locationOffset + numberToLight;i++) {
                borderLeds[i] = color;
            }
        } else {
            for(int i = locationOffset + ledsInSection - numberToLight;i<locationOffset + ledsInSection;i++) {
                borderLeds[i] = color;
            }
        }
    }
}

void LEDAnimations::equalizerTopBorder(int frequencyValue, int sensitivityThreshold, bool direction) {
    int ledsInSection = 20;
    int locationOffset = 63;
    for(int i = locationOffset;i<ledsInSection + locationOffset;i++) {
        borderLeds[i].fadeToBlackBy(45);
    }
    if (frequencyValue > sensitivityThreshold) {
        int numberToLight = map(frequencyValue, sensitivityThreshold, 1023, 0, ledsInSection);
        CRGB color = CHSV(map(frequencyValue, sensitivityThreshold, 1023, 0, 255), 200, 255);
        if (direction == true) {
            for(int i = locationOffset;i<locationOffset + numberToLight;i++) {
                borderLeds[i] = color;
            }
        } else {
            for(int i = locationOffset + ledsInSection - numberToLight;i<locationOffset + ledsInSection;i++) {
                borderLeds[i] = color;
            }
        }
    }
}

void LEDAnimations::equalizerShelf(CRGB *shelf, int frequencyValue, int sensitivityThreshold, bool direction) {
    fadeToBlackBy(shelf, LEDS_PER_SHELF, 45);
    if (frequencyValue > sensitivityThreshold) {
        int numberToLight = map(frequencyValue, sensitivityThreshold, 1023, 0, LEDS_PER_SHELF);
        CRGB color = CHSV(map(frequencyValue, sensitivityThreshold, 1023, 0, 255), 200, 255);
        if (direction == true) {
            for(int i = LEDS_PER_SHELF - numberToLight - 1;i<LEDS_PER_SHELF - 1;i++) {
                shelf[i] = color;
            }
        } else {
            for(int i = 0;i<numberToLight;i++) {
                shelf[i] = color;
            }
        }
    }
}

#endif