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

uint16_t globalSensitivity = 500;
uint8_t frequencyMode[7] = {0, 1, 2, 3, 4, 5, 6};
uint8_t numberOfPatterns;

typedef void (LEDAnimations::*AnimationList)();

AnimationList animationList[] = {&LEDAnimations::waterfall, &LEDAnimations::randomSilon,
            &LEDAnimations::fillColor, &LEDAnimations::colorBump, &LEDAnimations::seizureCity, &LEDAnimations:: flashyBump,
            &LEDAnimations::confetti, &LEDAnimations::juggle, &LEDAnimations::rainbow,
            &LEDAnimations::waterfallBorderRemote, &LEDAnimations::waterfallRainbowBorder,
            &LEDAnimations::waterfallLeftToRight, &LEDAnimations::waterfallRightToLeft, &LEDAnimations::equalizerLeftToRightBottomToTop,
            &LEDAnimations::equalizerRightToLeftBottomToTop, &LEDAnimations::equalizerRightToLeftTopToBottom,
            &LEDAnimations::equalizerBorderOnly, &LEDAnimations::equalizerBorderOnlyReversed};

LEDAnimations::LEDAnimations() : equalizer(new SpectrumEqualizer()) {
    topShelf = new Shelf(allShelves, TOP_SHELF_LEFT, TOP_SHELF_RIGHT);
    middleShelf = new Shelf(allShelves, MIDDLE_SHELF_LEFT, MIDDLE_SHELF_RIGHT);
    bottomShelf = new Shelf(allShelves, BOTTOM_SHELF_LEFT, BOTTOM_SHELF_RIGHT);
    numberOfPatterns = ARRAY_SIZE(animationList) - 1;
}

LEDAnimations::LEDAnimations(SpectrumEqualizer *eq) : equalizer(eq) {
    topShelf = new Shelf(allShelves, TOP_SHELF_LEFT, TOP_SHELF_RIGHT);
    middleShelf = new Shelf(allShelves, MIDDLE_SHELF_LEFT, MIDDLE_SHELF_RIGHT);
    bottomShelf = new Shelf(allShelves, BOTTOM_SHELF_LEFT, BOTTOM_SHELF_RIGHT);
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
      borderLeds[position] = CHSV(currentHue, currentSaturation, currentBrightness);
    }
    else if(ledStripToUse == "shelf") {
      allShelves[position] = CHSV(currentHue, currentSaturation, currentBrightness);
    }
    if(position == NUM_BORDER_LEDS) {
       position = BOTTOM_SHELF_RIGHT;
       ledStripToUse = "shelf";
       direction = false;
    }
    else if(position == BOTTOM_SHELF_LEFT && ledStripToUse == "border" && direction == false) {
       position = -1;
       direction = true;
       ledStripToUse = "shelf";
    }
    else if(position == 21 && ledStripToUse == "border") {
        if(random8(10) > 5) {
            ledStripToUse = "shelf";
            direction = false;
            position = MIDDLE_SHELF_LEFT;
        }
    }
    else if(position == 42 && ledStripToUse == "border") {
        if(random8(10) > 5) {
            ledStripToUse = "shelf";
            direction = true;
            position = TOP_SHELF_LEFT;
        }
    }
    else if(position == 104 && ledStripToUse == "border") {
        if(random8(10) > 5) {
            ledStripToUse = "shelf";
            direction = false;
            position = TOP_SHELF_RIGHT;
        }
    }
    else if(position == 125 && ledStripToUse == "border") {
        if(random8(10) > 5) {
            ledStripToUse = "shelf";
            direction = true;
            position = MIDDLE_SHELF_RIGHT;
        }
    }
    else if(position == BOTTOM_SHELF_RIGHT && ledStripToUse == "shelf" && direction == true) {
      position = NUM_BORDER_LEDS - 1;
      direction = false;
      ledStripToUse = "border";
    }
    else if(position == MIDDLE_SHELF_RIGHT && ledStripToUse == "shelf" && direction == false) {
        if(random8(10) > 5) {
            ledStripToUse = "border";
            direction = true;
            position = 125;
        } else {
            ledStripToUse = "border";
            direction = false;
            position = 125;
        }
    }
    else if(position == BOTTOM_SHELF_LEFT && ledStripToUse == "shelf" && direction == false) {
       position = -1;
       direction = true;
       ledStripToUse = "border";
    }
    else if(position == TOP_SHELF_LEFT && ledStripToUse == "shelf" && direction == false) {
        if(random8(10) > 5) {
            ledStripToUse = "border";
            direction = true;
            position = 42;
        } else {
          ledStripToUse = "border";
          direction = false;
          position = 42;
        }
    }
    else if(position == TOP_SHELF_RIGHT && ledStripToUse == "shelf" && direction == true) {
        if(random8(10) > 5) {
            ledStripToUse = "border";
            direction = true;
            position = 104;
        } else {
          ledStripToUse = "border";
          direction = false;
          position = 104;
        }
    }
    else if(position == MIDDLE_SHELF_LEFT && ledStripToUse == "shelf" && direction == true) {
        if(random8(10) > 5) {
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

int LEDAnimations::setPattern(int patternNumber) {
    currentPattern = wrapToRange(patternNumber, 0, numberOfPatterns);
    clearAllLeds();
    return currentPattern;
}

int LEDAnimations::getCurrentPattern() {
  return currentPattern;
}

void LEDAnimations::setCurrentBrightness(int brightness) {
  currentBrightness = brightness;
}

void LEDAnimations::setCurrentSaturation(int saturation) {
  currentSaturation = saturation;
}

int LEDAnimations::nextFrequencyMode() {
    int wrapEnd = frequencyMode[6];
    for(int i=6;i>0;i--) {
        frequencyMode[i] = frequencyMode[i-1];
    }
    frequencyMode[0] = wrapEnd;

    return wrapEnd;
}

int LEDAnimations::previousFrequencyMode() {
    int wrapBegining = frequencyMode[0];
    for(int i=0;i<6;i++) {
        frequencyMode[i] = frequencyMode[i+1];
    }
    frequencyMode[6] = wrapBegining;

    return wrapBegining;
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

void LEDAnimations::fillColor() {
    fill_solid(borderLeds, NUM_BORDER_LEDS, CHSV(currentHue, currentSaturation, currentBrightness));
    fill_solid(allShelves, NUM_SHELF_LEDS, CHSV(currentHue, currentSaturation, currentBrightness));
}

void LEDAnimations::rainbow() {
    fill_rainbow(borderLeds, NUM_BORDER_LEDS, currentHue, 7);
    fill_rainbow(allShelves, NUM_SHELF_LEDS, currentHue, 7);
}

// random colored speckles that blink in and fade smoothly
void LEDAnimations::confetti() {
    uint8_t position = random16(NUM_TOTAL_LEDS);
    uint16_t frequencyThreshold = clampSensitivity(globalSensitivity + 600);

    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 10);
    fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 10);

    // if (frequencyValue > frequencyThreshold) {
        if (position > NUM_BORDER_LEDS) {
            allShelves[position % NUM_BORDER_LEDS] += CHSV(currentHue + random8(64), currentSaturation, currentBrightness);
        } else {
            borderLeds[position] += CHSV(currentHue + random8(64), currentSaturation, currentBrightness);
        }
    // }
}

// eight colored dots, weaving in and out of sync with each other
void LEDAnimations::juggle() {
    int frequencyValue = equalizer->frequenciesLeft[frequencyMode[0]];
    uint16_t frequencyThreshold = clampSensitivity(globalSensitivity + 600);

    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 20);
    fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 20);
    byte dothue = 0;
    // if (frequencyValue > frequencyThreshold) {
        for (int i = 0; i < 8; i++) {
            int currentLocation = beatsin16(i + 7, 0, NUM_TOTAL_LEDS);
            if(currentLocation > NUM_BORDER_LEDS) {
                allShelves[currentLocation % NUM_BORDER_LEDS] |= CHSV(dothue, currentSaturation, currentBrightness);
            } else {
                borderLeds[currentLocation] |= CHSV(dothue, currentSaturation, currentBrightness);
            }
            dothue += 32;
        }
    // }
}

int previousBrightness = 0;
void LEDAnimations::colorBump() {
    int thisBrightness = map(equalizer->frequenciesLeft[frequencyMode[6]], 500, 3800, 0, 255);

    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 5);
    fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 5);

    if(thisBrightness > previousBrightness) {
        fill_solid(borderLeds, NUM_BORDER_LEDS, CHSV(currentHue, currentSaturation, thisBrightness));
        fill_solid(allShelves, NUM_SHELF_LEDS, CHSV(currentHue, currentSaturation, thisBrightness));
    }
}

void LEDAnimations::flashyBump() {
    int thisBrightness = map(equalizer->frequenciesLeft[frequencyMode[6]], 0, 3800, 0, 255);

    fill_solid(borderLeds, NUM_BORDER_LEDS, CHSV(currentHue, currentSaturation, thisBrightness));
    fill_solid(allShelves, NUM_SHELF_LEDS, CHSV(currentHue, currentSaturation, thisBrightness));
}

void LEDAnimations::seizureCity() {
    int thisBrightness = map(equalizer->frequenciesLeft[frequencyMode[6]], globalSensitivity, 3200, 0, 255);
    if(thisBrightness > 240) {
        currentHue += 20;
        wrapToRange(currentHue+=20, 0, 255);
    }

    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 2);
    fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 2);

    if(thisBrightness > previousBrightness) {
        fill_solid(borderLeds, NUM_BORDER_LEDS, CHSV(currentHue, currentSaturation, thisBrightness));
        fill_solid(allShelves, NUM_SHELF_LEDS, CHSV(currentHue, currentSaturation, thisBrightness));
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

void LEDAnimations::waterfallShelfLeft(Shelf *shelf, int frequencyValue, int frequencyThreshold, int brightness, int baseColorOffset) {
  if (frequencyValue > frequencyThreshold) {
      int mappedFrequencyValue = map(frequencyValue, frequencyThreshold, 4096, 0, 255);
      mappedFrequencyValue = (mappedFrequencyValue + baseColorOffset) % 255; //offsetting the base color...
      shelf->setRightPixel(CHSV(mappedFrequencyValue, brightness, 255));
  } else {
    shelf->setRightPixel(CRGB(0, 0, 0));
  }
  shelf->shiftLeft();
}

void LEDAnimations::waterfallShelfRight(Shelf *shelf, int frequencyValue, int frequencyThreshold, int brightness, int baseColorOffset) {
  if (frequencyValue > frequencyThreshold) {
      int mappedFrequencyValue = map(frequencyValue, frequencyThreshold, 4096, 0, 255);
      mappedFrequencyValue = (mappedFrequencyValue + baseColorOffset) % 255; //offsetting the base color...
      shelf->setLeftPixel(CHSV(mappedFrequencyValue, brightness, 255));
  } else {
    shelf->setLeftPixel(CRGB(0, 0, 0));
  }
  shelf->shiftRight();
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

void LEDAnimations::waterfallLeftToRight() {
  int sensitivityValueMinThreshold = clampSensitivity(globalSensitivity + 700);
  int brightness = 220;
  waterfallShelfLeft(topShelf, equalizer->frequenciesLeft[frequencyMode[6]], sensitivityValueMinThreshold, brightness, 180);
  waterfallShelfLeft(middleShelf, equalizer->frequenciesLeft[frequencyMode[2]], sensitivityValueMinThreshold, brightness, 80);
  waterfallShelfLeft(bottomShelf, equalizer->frequenciesLeft[frequencyMode[0]], sensitivityValueMinThreshold, brightness, 40);
  waterfallBorder(equalizer->frequenciesLeft[frequencyMode[4]], sensitivityValueMinThreshold, brightness);
}

void LEDAnimations::waterfallRightToLeft() {
  int sensitivityValueMinThreshold = clampSensitivity(globalSensitivity + 700);
  int brightness = 220;
  waterfallShelfRight(topShelf, equalizer->frequenciesLeft[frequencyMode[6]], sensitivityValueMinThreshold, brightness, 180);
  waterfallShelfRight(middleShelf, equalizer->frequenciesLeft[frequencyMode[2]], sensitivityValueMinThreshold, brightness, 80);
  waterfallShelfRight(bottomShelf, equalizer->frequenciesLeft[frequencyMode[0]], sensitivityValueMinThreshold, brightness, 40);
  waterfallBorder(equalizer->frequenciesLeft[frequencyMode[4]], sensitivityValueMinThreshold, brightness);
}

void LEDAnimations::waterfallBorderRemote() {
    borderLeds[NUM_BORDER_LEDS / 2] = CHSV(currentHue, 200, 255);
    memmove(&borderLeds[0], &borderLeds[1], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
    memmove(&borderLeds[NUM_BORDER_LEDS / 2 + 1], &borderLeds[NUM_BORDER_LEDS / 2], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
}

uint8_t hueCounter = 0;
void LEDAnimations::waterfallRainbowBorder() {
    borderLeds[NUM_BORDER_LEDS / 2] = CHSV(hueCounter, 200, 255);
    memmove(&borderLeds[0], &borderLeds[1], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
    memmove(&borderLeds[NUM_BORDER_LEDS / 2 + 1], &borderLeds[NUM_BORDER_LEDS / 2], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
    hueCounter++;
}

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
    equalizerLeftBorder(equalizer->frequenciesLeft[frequencyMode[0]], clampSensitivity(globalSensitivity + 400), false);
    equalizerTopBorder(equalizer->frequenciesLeft[frequencyMode[2]], clampSensitivity(globalSensitivity + 400), false);
    equalizerRightBorder(equalizer->frequenciesLeft[frequencyMode[6]], clampSensitivity(globalSensitivity + 400), false);
    equalizerShelf(topShelf, equalizer->frequenciesLeft[frequencyMode[4]], clampSensitivity(globalSensitivity + 400), false);
    equalizerShelf(middleShelf, equalizer->frequenciesLeft[frequencyMode[3]], clampSensitivity(globalSensitivity + 400), false);
    equalizerShelf(bottomShelf, equalizer->frequenciesLeft[frequencyMode[5]], clampSensitivity(globalSensitivity + 400), false);
}

void LEDAnimations::equalizerRightToLeftTopToBottom() {
  fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 40);
  fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 45);
  equalizerLeftBorder(equalizer->frequenciesLeft[frequencyMode[0]], clampSensitivity(globalSensitivity + 400), true);
  equalizerTopBorder(equalizer->frequenciesLeft[frequencyMode[2]], clampSensitivity(globalSensitivity + 400), false);
  equalizerRightBorder(equalizer->frequenciesLeft[frequencyMode[6]], clampSensitivity(globalSensitivity + 400), true);
  equalizerShelf(topShelf, equalizer->frequenciesLeft[frequencyMode[4]], clampSensitivity(globalSensitivity + 400), false);
  equalizerShelf(middleShelf, equalizer->frequenciesLeft[frequencyMode[3]], clampSensitivity(globalSensitivity + 400), false);
  equalizerShelf(bottomShelf, equalizer->frequenciesLeft[frequencyMode[5]], clampSensitivity(globalSensitivity + 400), false);
}

void LEDAnimations::equalizerBorderOnly() {
  fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 40);
  fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 40);
  equalizerLeftBorder(equalizer->frequenciesLeft[frequencyMode[0]], clampSensitivity(globalSensitivity + 400), false);
  equalizerRightBorder(equalizer->frequenciesLeft[frequencyMode[6]], clampSensitivity(globalSensitivity + 400), false);
}

void LEDAnimations::equalizerBorderOnlyReversed() {
  fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 40);
  fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 40);
  equalizerLeftBorder(equalizer->frequenciesLeft[frequencyMode[6]], clampSensitivity(globalSensitivity + 400), false);
  equalizerRightBorder(equalizer->frequenciesLeft[frequencyMode[0]], clampSensitivity(globalSensitivity + 400), false);
}

void LEDAnimations::equalizerLeftBorder(int frequencyValue, int sensitivityThreshold, bool direction) {
    int ledsInSection = 62;
    if (frequencyValue > sensitivityThreshold) {
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
