#ifndef LEDANIMATIONS_CPP
#define LEDANIMATIONS_CPP

#include "application.h"
#include "LEDAnimations.h"

CRGB borderLeds[NUM_BORDER_LEDS];
CRGB allShelves[NUM_SHELF_LEDS];

CRGB *bottomShelfLeds;
CRGB *middleShelfLeds;
CRGB *topShelfLeds;

SpectrumEqualizer *equalizer;

int globalSensitivity = 0;
uint8_t frequencyMode[7] = {0, 1, 2, 3, 4, 5, 6};
uint8_t numberOfPatterns = 7;
uint8_t currentPattern = 0;
uint8_t hueCounter = 0;

LEDAnimations::LEDAnimations() : equalizer(new SpectrumEqualizer()) {
    equalizer->init();
}

LEDAnimations::LEDAnimations(SpectrumEqualizer *eq) : equalizer(eq) {
    equalizer->init();
}

int LEDAnimations::runCurrentAnimation() {
    equalizer->readAudioFrequencies();

    switch (currentPattern) {
      case 0:
        waterfall();
        break;
      case 1:
        waterfallBorderControllerOnly();
        break;
      case 2:
        rainbow();
        break;
      case 3:
        juggle(equalizer->frequenciesLeft[4]);
        break;
      case 4:
        confetti(equalizer->frequenciesLeft[4]);
        break;
      case 5:
        sinelon(equalizer->frequenciesLeft[4]);
        break;
      case 7:
        clearAllLeds();
        break;
      case 6:
        for(uint8_t j=0;j<NUM_BORDER_LEDS;j++) {
            borderLeds[j].setHue(hueCounter);
        }
        for(uint8_t j=0;j<NUM_SHELF_LEDS;j++) {
            allShelves[j].setHue(hueCounter);
        }
        break;
      case 8:
        borderLeds[22] = CRGB(200,0,0);
        waterfallBottomShelf(equalizer->frequenciesLeft[frequencyMode[4]], clampSensitivity(globalSensitivity + 500));
        break;
      case 9:
        equalizerLeftToRightBottomToTop();
        break;
      case 10:
        equalizerRightToLeftBottomToTop();
        break;
      default:
        clearAllLeds();
        break;
    }
}

int LEDAnimations::nextPattern() {
    currentPattern = (currentPattern + 1);
    currentPattern = wrapToRange(currentPattern, 0, 10);
    return currentPattern;
}

int LEDAnimations::previousPattern() {
    currentPattern = (currentPattern - 1);
    currentPattern = wrapToRange(currentPattern, 0, 10);
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

void LEDAnimations::clearAllLeds() {
  for(uint8_t j=0;j<NUM_TOTAL_LEDS;j++) {
      borderLeds[j] = 0;
      if(j > NUM_BORDER_LEDS) {
          allShelves[j%NUM_BORDER_LEDS] = 0;
      }
  }
}

int previousHue = 0;
void LEDAnimations::rainbow() {
    fill_rainbow(borderLeds, NUM_TOTAL_LEDS, hueCounter, 7);
}

// random colored speckles that blink in and fade smoothly
void LEDAnimations::confetti(int frequencyValue) {
    uint8_t position = random16(NUM_TOTAL_LEDS);
    uint16_t frequencyThreshold = clampSensitivity(globalSensitivity + 600);

    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 10);
    fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 10);

    // if (frequencyValue > frequencyThreshold) {
        if (position > NUM_BORDER_LEDS) {
            allShelves[position % NUM_BORDER_LEDS] += CHSV(hueCounter + random8(64), 200, 255);
        } else {
            borderLeds[position] += CHSV(hueCounter + random8(64), 200, 255);
        }
    // }
}

// a colored dot sweeping back and forth, with fading trails
void LEDAnimations::sinelon(int frequencyValue) {
    uint16_t frequencyThreshold = clampSensitivity(globalSensitivity + 600);

    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 5);
    fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 5);
    int pos = beatsin16(13, 0, NUM_TOTAL_LEDS);
    // if (frequencyValue > frequencyThreshold) {
        if (pos > NUM_BORDER_LEDS) {
            allShelves[pos % NUM_BORDER_LEDS] += CHSV(hueCounter, 255, 192);
        } else {
            borderLeds[pos] += CHSV(hueCounter, 255, 192);
        }
    // }
}

// colored stripes pulsing at a defined Beats-Per-Minute (BPM)
void LEDAnimations::bpm() {
    uint8_t BeatsPerMinute = 120;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
    for (int i = 0; i < NUM_TOTAL_LEDS; i++) { //9948
        if (i > NUM_BORDER_LEDS) {
            allShelves[i] = ColorFromPalette(palette, hueCounter + (i * 2), beat - hueCounter + (i * 10));
        } else {
            borderLeds[i]= ColorFromPalette(palette, hueCounter + (i * 2), beat - hueCounter + (i * 10));
        }
    }
}

// eight colored dots, weaving in and out of sync with each other
void LEDAnimations::juggle(int frequencyValue) {
    uint16_t frequencyThreshold = clampSensitivity(globalSensitivity + 600);

    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 20);
    fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 20);
    byte dothue = 0;
    if (frequencyValue > frequencyThreshold) {
        for (int i = 0; i < 8; i++) {
            int currentLocation = beatsin16(i + 7, 0, NUM_TOTAL_LEDS);
            if(currentLocation > NUM_BORDER_LEDS) {
                allShelves[currentLocation % NUM_BORDER_LEDS] |= CHSV(dothue, 200, 255);
            } else {
                borderLeds[currentLocation] |= CHSV(dothue, 200, 255);
            }
            dothue += 32;
        }
    }
}

void LEDAnimations::waterfall() {
    // waterfallShelf(topShelfLeds, equalizer->frequenciesLeft[frequencyMode[6]], clampSensitivity(globalSensitivity + 500));
    // waterfallShelf(middleShelfLeds, equalizer->frequenciesLeft[frequencyMode[1]], clampSensitivity(globalSensitivity + 500));
    // waterfallShelf(bottomShelfLeds, equalizer->frequenciesLeft[frequencyMode[0]], clampSensitivity(globalSensitivity + 500));?
    waterfallShelf(allShelves, equalizer->frequenciesLeft[frequencyMode[0]], clampSensitivity(globalSensitivity + 500));
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
        // shelf[NUM_SHELF_LEDS / 2] = CHSV(mappedFrequencyValue, 200, 255);
        // shelf[NUM_SHELF_LEDS / 2 + 1] = CHSV(mappedFrequencyValue, 200, 255);
        shelf[NUM_SHELF_LEDS / 2].setHue(mappedFrequencyValue);
        shelf[NUM_SHELF_LEDS / 2 + 1].setHue(mappedFrequencyValue);
    } else {
        shelf[NUM_SHELF_LEDS / 2] = CRGB(0, 0, 0);
        shelf[NUM_SHELF_LEDS / 2 + 1] = CRGB(0, 0, 0);
    }
    memmove(&shelf[0], &shelf[1], NUM_SHELF_LEDS / 2 * sizeof(CRGB));
    memmove(&shelf[NUM_SHELF_LEDS / 2 + 1], &shelf[NUM_SHELF_LEDS / 2], NUM_SHELF_LEDS / 2 * sizeof(CRGB));
}

void LEDAnimations::waterfallBottomShelf(int frequencyValue, int frequencyThreshold) {
  // if (frequencyValue > frequencyThreshold) {
      int mappedFrequencyValue = map(frequencyValue, frequencyThreshold, 1023, 0, 255);
      setBottomShelf(19 / 2, CHSV(mappedFrequencyValue, 200, 255));
  // } else {
  //   setBottomShelf((NUM_SHELF_LEDS - 1) / 2, CHSV(0,0,0));
  // }
  memmove(&allShelves[0], &allShelves[1], LEDS_PER_SHELF / 2 * sizeof(CRGB));
  memmove(&allShelves[LEDS_PER_SHELF / 2 + 1], &allShelves[LEDS_PER_SHELF / 2], LEDS_PER_SHELF  / 2 * sizeof(CRGB));
}

void LEDAnimations::setBottomShelf(int index, CHSV color) {
  if(index <= 19) {
    allShelves[index] = color;
  }
}

void LEDAnimations::setMiddleShelf(int index, CHSV color) {
    allShelves[index+19] = color;
}

void LEDAnimations::setTopShelf(int index, CHSV color) {
    allShelves[index+39] = color;
}

void LEDAnimations::waterfallBorder(int frequencyValue, int frequencyThreshold) {
    if (frequencyValue > frequencyThreshold) {
        int mappedFrequencyValue = map(frequencyValue, frequencyThreshold, 1023, 0, 255);
        borderLeds[NUM_BORDER_LEDS / 2] = CHSV(mappedFrequencyValue, 200, 255);
    } else {
        borderLeds[NUM_BORDER_LEDS / 2] = CRGB(0, 0, 0);
    }
    memmove(&borderLeds[0], &borderLeds[1], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
    memmove(&borderLeds[NUM_BORDER_LEDS / 2 + 1], &borderLeds[NUM_BORDER_LEDS / 2], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
}

void LEDAnimations::waterfallBorderControllerToo(int frequencyValue, int frequencyThreshold) {
    if (frequencyValue > frequencyThreshold) {
        int mappedFrequencyValue = hueCounter;
        borderLeds[NUM_BORDER_LEDS / 2] = CHSV(mappedFrequencyValue, 200, 255);
    } else {
        borderLeds[NUM_BORDER_LEDS / 2] = CRGB(0, 0, 0);
    }
    memmove(&borderLeds[0], &borderLeds[1], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
    memmove(&borderLeds[NUM_BORDER_LEDS / 2 + 1], &borderLeds[NUM_BORDER_LEDS / 2], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
}

void LEDAnimations::waterfallBorderControllerOnly() {
    borderLeds[NUM_BORDER_LEDS / 2] = CHSV(hueCounter, 200, 255);
    memmove(&borderLeds[0], &borderLeds[1], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
    memmove(&borderLeds[NUM_BORDER_LEDS / 2 + 1], &borderLeds[NUM_BORDER_LEDS / 2], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
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
    memmove(&borderLeds[NUM_BORDER_LEDS / 2 + 1], &borderLeds[NUM_BORDER_LEDS / 2], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
}
int maxFrequencyValue = 0;
void LEDAnimations::equalizerLeftToRightBottomToTop() {
  EVERY_N_SECONDS(5) {
    char numAsString[40];
    sprintf(numAsString, "%d", maxFrequencyValue);
    Particle.publish("frequencyValue", numAsString);
  }
    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 40);
    equalizerLeftBorder(equalizer->frequenciesLeft[frequencyMode[0]], 300, false);
    equalizerTopBorder(equalizer->frequenciesLeft[frequencyMode[5]], clampSensitivity(globalSensitivity + 400), true);
    equalizerRightBorder(equalizer->frequenciesLeft[frequencyMode[6]], clampSensitivity(globalSensitivity + 200), false);
    // equalizerShelf(topShelfLeds, equalizer->frequenciesLeft[frequencyMode[4]], clampSensitivity(globalSensitivity + 400), false);
    // equalizerShelf(middleShelfLeds, equalizer->frequenciesLeft[frequencyMode[3]], clampSensitivity(globalSensitivity + 400), true);
    // equalizerShelf(bottomShelfLeds, equalizer->frequenciesLeft[frequencyMode[2]], clampSensitivity(globalSensitivity + 400), false);
}

void LEDAnimations::equalizerRightToLeftBottomToTop() {
    equalizerLeftBorder(equalizer->frequenciesLeft[frequencyMode[0]], clampSensitivity(globalSensitivity + 200), false);
    equalizerRightBorder(equalizer->frequenciesLeft[frequencyMode[6]], clampSensitivity(globalSensitivity + 200), false);
    equalizerTopBorder(equalizer->frequenciesLeft[frequencyMode[5]], clampSensitivity(globalSensitivity + 400), false);
    // equalizerShelf(topShelfLeds, equalizer->frequenciesLeft[frequencyMode[4]], clampSensitivity(globalSensitivity + 400), true);
    // equalizerShelf(middleShelfLeds, equalizer->frequenciesLeft[frequencyMode[3]], clampSensitivity(globalSensitivity + 400), false);
    // equalizerShelf(bottomShelfLeds, equalizer->frequenciesLeft[frequencyMode[2]], clampSensitivity(globalSensitivity + 400), true);
}


void LEDAnimations::equalizerLeftBorder(int frequencyValue, int sensitivityThreshold, bool direction) {
    int ledsInSection = 62;
    if (frequencyValue > sensitivityThreshold) {
        if(frequencyValue > maxFrequencyValue) {
          maxFrequencyValue = frequencyValue;
        }
        int numberToLight = map(frequencyValue, 0, 3500, 0, ledsInSection);
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
    if (frequencyValue > sensitivityThreshold) {
        int numberToLight = map(frequencyValue, 0, 3500, 0, ledsInSection);
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
        int numberToLight = map(frequencyValue, 0, 3500, 0, ledsInSection);
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
