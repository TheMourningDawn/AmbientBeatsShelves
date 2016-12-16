#ifndef LEDANIMATIONS_CPP
#define LEDANIMATIONS_CPP

#include "application.h"
#include "LEDAnimations.h"
#include "Shelf.h"

CRGB borderLeds[NUM_BORDER_LEDS];
CRGB allShelves[NUM_SHELF_LEDS];

Shelf *topShelf;
Shelf *middleShelf;
Shelf *bottomShelf;

SpectrumEqualizer *equalizer;

int globalSensitivity = 500;
uint8_t frequencyMode[7] = {0, 1, 2, 3, 4, 5, 6};
uint8_t numberOfPatterns = 0;
int currentPattern = 0;
uint8_t currentHue = 0;

typedef void (LEDAnimations::*AnimationList)();

AnimationList animationList[] = {&LEDAnimations::waterfall, &LEDAnimations::waterfallRainbowBorder, &LEDAnimations::waterfallBorderRemoteAndSpectrum, &LEDAnimations::randomSilon, &LEDAnimations::waterfallBorderRemote, &LEDAnimations::confetti, &LEDAnimations::bpm, &LEDAnimations::juggle, &LEDAnimations::rainbow, &LEDAnimations::equalizerRightToLeftBottomToTop, &LEDAnimations::equalizerLeftToRightBottomToTop };

LEDAnimations::LEDAnimations() : equalizer(new SpectrumEqualizer()) {
    topShelf = new Shelf(allShelves, 39, 58);
    middleShelf = new Shelf(allShelves, 38, 19);
    bottomShelf = new Shelf(allShelves, 0, 18);
    numberOfPatterns = ARRAY_SIZE(animationList) - 1;
}

LEDAnimations::LEDAnimations(SpectrumEqualizer *eq) : equalizer(eq) {
    topShelf = new Shelf(allShelves, 39, 58);
    middleShelf = new Shelf(allShelves, 38, 19);
    bottomShelf = new Shelf(allShelves, 0, 18);
    numberOfPatterns = ARRAY_SIZE(animationList) - 1;
}

int LEDAnimations::runCurrentAnimation() {
    equalizer->readAudioFrequencies();
    (this->*animationList[currentPattern])();
}

int position = 0;
boolean direction = true;
String ledStripToUse = "border";
void LEDAnimations::randomSilon() {
    if(ledStripToUse == "border") {
      borderLeds[position] = CHSV(currentHue, 255, 255);
    }
    else if(ledStripToUse == "shelf") {
      allShelves[position] = CHSV(currentHue, 255, 255);
    }
    if(position == NUM_BORDER_LEDS) {
       position = 18;
       ledStripToUse = "shelf";
       direction = false;
    }
    else if(position == 0 && ledStripToUse == "border" && direction == false) {
       position = -1;
       direction = true;
       ledStripToUse = "shelf";
    }
    else if(position == 21 && ledStripToUse == "border") {
        if(random8(10) > 4) {
            ledStripToUse = "shelf";
            direction = false;
            position = 38;
        }
    }
    else if(position == 42 && ledStripToUse == "border") {
        if(random8(10) > 4) {
            ledStripToUse = "shelf";
            direction = true;
            position = 38;
        }
    }
    else if(position == 104 && ledStripToUse == "border") {
        if(random8(10) > 4) {
            ledStripToUse = "shelf";
            direction = false;
            position = 58;
        }
    }
    else if(position == 125 && ledStripToUse == "border") {
        if(random8(10) > 4) {
            ledStripToUse = "shelf";
            direction = true;
            position = 19;
        }
    }
    else if(position == 18 && ledStripToUse == "shelf" && direction == true) {
      position = NUM_BORDER_LEDS - 1;
      direction = false;
      ledStripToUse = "border";
    }
    else if(position == 19 && ledStripToUse == "shelf" && direction == false) {
        if(random8(10) > 4) {
            ledStripToUse = "border";
            direction = true;
            position = 125;
        } else {
            ledStripToUse = "border";
            direction = false;
            position = 125;
        }
    }
    else if(position == 0 && ledStripToUse == "shelf" && direction == false) {
       position = -1;
       direction = true;
       ledStripToUse = "border";
    }
    else if(position == 39 && ledStripToUse == "shelf" && direction == false) {
        if(random8(10) > 4) {
            ledStripToUse = "border";
            direction = true;
            position = 42;
        } else {
          ledStripToUse = "border";
          direction = false;
          position = 42;
        }
    }
    else if(position == 59-1 && ledStripToUse == "shelf" && direction == true) {
        if(random8(10) > 4) {
            ledStripToUse = "border";
            direction = true;
            position = 104;
        } else {
          ledStripToUse = "border";
          direction = false;
          position = 104;
        }
    }
    else if(position == 38 && ledStripToUse == "shelf" && direction == true) {
        if(random8(10) > 4) {
            ledStripToUse = "border";
            direction = true;
            position = 21;
        } else {
          ledStripToUse = "border";
          direction = false;
          position = 21;
        }
    }
    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 1);
    fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 1);
    if(direction == true) {
        position++;
    } else {
      position--;
    }
    if(position < 0) { position = 0; }
    if(position > NUM_BORDER_LEDS) { position = NUM_BORDER_LEDS; };
}

int LEDAnimations::nextPattern() {
    currentPattern++;
    currentPattern = wrapToRange(currentPattern, 0, numberOfPatterns);
    clearAllLeds();
    return currentPattern;
}

int LEDAnimations::previousPattern() {
    currentPattern--;
    currentPattern = wrapToRange(currentPattern, 0, numberOfPatterns);
    clearAllLeds();
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
    return clampToRange(sensitivity, 0, 4096);
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
    fill_rainbow(borderLeds, NUM_TOTAL_LEDS, currentHue, 7);
}

// random colored speckles that blink in and fade smoothly
void LEDAnimations::confetti() {
    uint8_t position = random16(NUM_TOTAL_LEDS);
    uint16_t frequencyThreshold = clampSensitivity(globalSensitivity + 600);

    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 10);
    fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 10);

    // if (frequencyValue > frequencyThreshold) {
        if (position > NUM_BORDER_LEDS) {
            allShelves[position % NUM_BORDER_LEDS] += CHSV(currentHue + random8(64), 200, 255);
        } else {
            borderLeds[position] += CHSV(currentHue + random8(64), 200, 255);
        }
    // }
}

// a colored dot sweeping back and forth, with fading trails
void LEDAnimations::sinelon() {
    uint16_t frequencyThreshold = clampSensitivity(globalSensitivity + 600);

    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 5);
    fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 5);
    int pos = beatsin16(13, 0, NUM_TOTAL_LEDS);
    // if (frequencyValue > frequencyThreshold) {
        if (pos > NUM_BORDER_LEDS) {
            allShelves[pos % NUM_BORDER_LEDS] += CHSV(currentHue, 255, 192);
        } else {
            borderLeds[pos] += CHSV(currentHue, 255, 192);
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
            allShelves[i] = ColorFromPalette(palette, currentHue + (i * 2), beat - currentHue + (i * 10));
        } else {
            borderLeds[i]= ColorFromPalette(palette, currentHue + (i * 2), beat - currentHue + (i * 10));
        }
    }
}

// eight colored dots, weaving in and out of sync with each other
void LEDAnimations::juggle() {
    int frequencyValue = equalizer->frequenciesLeft[frequencyMode[0]];
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
    int sensitivityValueMinThreshold = clampSensitivity(globalSensitivity + 700);
    int brightness = 220;
    waterfallShelf(topShelf, equalizer->frequenciesLeft[frequencyMode[6]], sensitivityValueMinThreshold, brightness, 180);
    waterfallShelf(middleShelf, equalizer->frequenciesLeft[frequencyMode[2]], sensitivityValueMinThreshold, brightness, 80);
    waterfallShelf(bottomShelf, equalizer->frequenciesLeft[frequencyMode[0]], sensitivityValueMinThreshold, brightness, 40);
    waterfallBorder(equalizer->frequenciesLeft[frequencyMode[4]], sensitivityValueMinThreshold, brightness);
}

void LEDAnimations::waterfallShelf(Shelf *shelf, int frequencyValue, int frequencyThreshold, int brightness, int baseColorOffset) {
    int middlePixel = (shelf->getRightPixelIndex() + shelf->getLeftPixelIndex())/2;
    if (frequencyValue > frequencyThreshold) {
        int mappedFrequencyValue = map(frequencyValue, frequencyThreshold, 4096, 0, 255);
        mappedFrequencyValue = (mappedFrequencyValue + baseColorOffset) % 255; //offsetting the base color...
        shelf->setPixel(middlePixel, CHSV(mappedFrequencyValue, brightness, 255));
        shelf->setPixel(middlePixel + 1, CHSV(mappedFrequencyValue, brightness, 255));
    } else {
      shelf->setPixel(middlePixel, CRGB(0, 0, 0));
      shelf->setPixel(middlePixel + 1, CRGB(0, 0, 0));
    }
    shelf->shiftMiddle();
}

void LEDAnimations::waterfallBorder(int frequencyValue, int frequencyValueMinThreshold, int brightness) {
    if (frequencyValue > frequencyValueMinThreshold) {
        int mappedFrequencyValue = map(frequencyValue, frequencyValueMinThreshold, 4096, 0, 255);
        mappedFrequencyValue = (mappedFrequencyValue + 120) % 255; //offsetting the base color...
        borderLeds[NUM_BORDER_LEDS / 2] = CHSV(mappedFrequencyValue, brightness, 255);
    } else {
        borderLeds[NUM_BORDER_LEDS / 2] = CRGB(0, 0, 0);
    }
    memmove(&borderLeds[0], &borderLeds[1], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
    memmove(&borderLeds[NUM_BORDER_LEDS / 2 + 1], &borderLeds[NUM_BORDER_LEDS / 2], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
}

void LEDAnimations::waterfallBorderRemoteAndSpectrum(int frequencyValue, int frequencyThreshold) {
    if (frequencyValue > frequencyThreshold) {
        int mappedFrequencyValue = currentHue;
        borderLeds[NUM_BORDER_LEDS / 2] = CHSV(mappedFrequencyValue, 200, 255);
    } else {
        borderLeds[NUM_BORDER_LEDS / 2] = CRGB(0, 0, 0);
    }
    memmove(&borderLeds[0], &borderLeds[1], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
    memmove(&borderLeds[NUM_BORDER_LEDS / 2 + 1], &borderLeds[NUM_BORDER_LEDS / 2], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
}

void LEDAnimations::waterfallBorderRemote() {
    borderLeds[NUM_BORDER_LEDS / 2] = CHSV(currentHue, 200, 255);
    memmove(&borderLeds[0], &borderLeds[1], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
    memmove(&borderLeds[NUM_BORDER_LEDS / 2 + 1], &borderLeds[NUM_BORDER_LEDS / 2], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
}

void LEDAnimations::waterfallRainbowBorder() {
    borderLeds[NUM_BORDER_LEDS / 2] = CHSV(currentHue, 200, 255);
    memmove(&borderLeds[0], &borderLeds[1], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
    memmove(&borderLeds[NUM_BORDER_LEDS / 2 + 1], &borderLeds[NUM_BORDER_LEDS / 2], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
    currentHue++;
}

int maxFrequencyValue = 0;
void LEDAnimations::equalizerLeftToRightBottomToTop() {
    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 40);
    fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 45);
    equalizerLeftBorder(equalizer->frequenciesLeft[frequencyMode[0]], clampSensitivity(globalSensitivity + 400), false);
    equalizerTopBorder(equalizer->frequenciesLeft[frequencyMode[2]], clampSensitivity(globalSensitivity + 400), true);
    equalizerRightBorder(equalizer->frequenciesLeft[frequencyMode[6]], clampSensitivity(globalSensitivity + 400), false);
    equalizerShelf(topShelf, equalizer->frequenciesLeft[frequencyMode[4]], clampSensitivity(globalSensitivity + 400), true);
    equalizerShelf(middleShelf, equalizer->frequenciesLeft[frequencyMode[3]], clampSensitivity(globalSensitivity + 400), true);
    equalizerShelf(bottomShelf, equalizer->frequenciesLeft[frequencyMode[5]], clampSensitivity(globalSensitivity + 400), true);
}

void LEDAnimations::equalizerRightToLeftBottomToTop() {
  fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 40);
  fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 45);
  equalizerLeftBorder(equalizer->frequenciesLeft[frequencyMode[0]], clampSensitivity(globalSensitivity + 400), true);
  equalizerTopBorder(equalizer->frequenciesLeft[frequencyMode[2]], clampSensitivity(globalSensitivity + 400), false);
  equalizerRightBorder(equalizer->frequenciesLeft[frequencyMode[6]], clampSensitivity(globalSensitivity + 400), true);
  equalizerShelf(topShelf, equalizer->frequenciesLeft[frequencyMode[4]], clampSensitivity(globalSensitivity + 400), false);
  equalizerShelf(middleShelf, equalizer->frequenciesLeft[frequencyMode[3]], clampSensitivity(globalSensitivity + 400), false);
  equalizerShelf(bottomShelf, equalizer->frequenciesLeft[frequencyMode[5]], clampSensitivity(globalSensitivity + 400), false);
}

void LEDAnimations::equalizerLeftBorder(int frequencyValue, int sensitivityThreshold, bool direction) {
    int ledsInSection = 62;
    if (frequencyValue > sensitivityThreshold) {
        if(frequencyValue > maxFrequencyValue) {
          maxFrequencyValue = frequencyValue;
        }
        int numberToLight = map(frequencyValue, 0, 3500, 0, ledsInSection);
        CRGB color = CHSV(map(frequencyValue, sensitivityThreshold, 4096, 0, 255), 200, 255);
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
        int numberToLight = map(frequencyValue, sensitivityThreshold, 3500, 0, ledsInSection);
        CRGB color = CHSV(map(frequencyValue, sensitivityThreshold, 4096, 0, 255), 200, 255);
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
        int numberToLight = map(frequencyValue, sensitivityThreshold, 3500, 0, ledsInSection);
        CRGB color = CHSV(map(frequencyValue, sensitivityThreshold, 4096, 0, 255), 200, 255);
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

void LEDAnimations::equalizerShelf(Shelf *shelf, int frequencyValue, int sensitivityThreshold, bool direction) {
    if (frequencyValue > sensitivityThreshold) {
        int numberToLight = map(frequencyValue, sensitivityThreshold, 3500, 0, LEDS_PER_SHELF);
        CRGB color = CHSV(map(frequencyValue, sensitivityThreshold, 4096, 0, 255), 200, 255);
        if (direction == true) {
          shelf->fillLeft(color, numberToLight);
        } else {
          shelf->fillRight(color, numberToLight);
        }
    }
}

#endif
