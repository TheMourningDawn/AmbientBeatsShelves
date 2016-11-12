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

int globalSensitivity = 0;
uint8_t frequencyMode[7] = {0, 1, 2, 3, 4, 5, 6};
uint8_t numberOfPatterns = 9;
int currentPattern = 0;
uint8_t hueCounter = 0;

LEDAnimations::LEDAnimations() : equalizer(new SpectrumEqualizer()) {
    equalizer->init();
    topShelf = new Shelf(allShelves, 39, 58);
    middleShelf = new Shelf(allShelves, 38, 19);
    bottomShelf = new Shelf(allShelves, 0, 18);
}

LEDAnimations::LEDAnimations(SpectrumEqualizer *eq) : equalizer(eq) {
    equalizer->init();
    topShelf = new Shelf(allShelves, 39, 58);
    middleShelf = new Shelf(allShelves, 38, 19);
    bottomShelf = new Shelf(allShelves, 0, 18);
}

int LEDAnimations::runCurrentAnimation() {
    equalizer->readAudioFrequencies();

    switch (currentPattern) {
      case 0:
        waterfall();
        break;
      case 1:
      middleShelf->setRightPixel(CRGB(255,0,0));
      middleShelf->setLeftPixel(CRGB(0,255,0));
      bottomShelf->setRightPixel(CRGB(255,0,0));
      bottomShelf->setLeftPixel(CRGB(0,255,0));
      topShelf->setRightPixel(CRGB(255,0,0));
      topShelf->setLeftPixel(CRGB(0,255,0));
        // waterfallBorderControllerOnly();
        break;
      case 2:
        randomSilon();
        break;
      case 3:
        juggle(equalizer->frequenciesLeft[4]);
        break;
      case 4:
        confetti(equalizer->frequenciesLeft[4]);
        break;
      case 5:
        rainbow();
        break;
      case 6:
        for(uint8_t j=0;j<NUM_BORDER_LEDS;j++) {
            borderLeds[j].setHue(hueCounter);
        }
        for(uint8_t j=0;j<NUM_SHELF_LEDS;j++) {
            allShelves[j].setHue(hueCounter);
        }
        break;
      case 7:
        equalizerRightToLeftBottomToTop();
        break;
      case 8:
        equalizerLeftToRightBottomToTop();
        break;
      case 9:
        borderLeds[22] = CRGB(200,0,0);
        waterfallBottomShelf(equalizer->frequenciesLeft[frequencyMode[4]], clampSensitivity(globalSensitivity + 500));
        break;
      default:
        break;
    }
}

int position = 0;
boolean direction = true;
String ledStripToUse = "border";
int LEDAnimations::randomSilon() {
    if(ledStripToUse == "border") {
      borderLeds[position] = CHSV(hueCounter, 255, 255);
    }
    else if(ledStripToUse == "shelf") {
      allShelves[position] = CHSV(hueCounter, 255, 255);
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
    // EVERY_N_MILLISECONDS(15) {
      if(direction == true) {
          position++;
      } else {
        position--;
      }
      if(position < 0) { position = 0; }
      if(position > NUM_BORDER_LEDS) { position = NUM_BORDER_LEDS; };
    // }
}

int LEDAnimations::nextPattern() {
    currentPattern++;
    currentPattern = wrapToRange(currentPattern, 0, 10);
    clearAllLeds();
    return currentPattern;
}

int LEDAnimations::previousPattern() {
    currentPattern--;
    currentPattern = wrapToRange(currentPattern, 0, 10);
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
    waterfallShelf(allShelves, equalizer->frequenciesLeft[frequencyMode[0]], clampSensitivity(globalSensitivity + 500));
    waterfallBorder(equalizer->frequenciesLeft[frequencyMode[4]], 500, 200);
}

void LEDAnimations::waterfallCascading() {
    waterfallBorderCascading(equalizer->frequenciesLeft[frequencyMode[4]], clampSensitivity((globalSensitivity + 500)));
    // waterfallShelf(topShelfLeds, equalizer->frequenciesLeft[frequencyMode[6]], clampSensitivity(globalSensitivity + 500));
    // waterfallShelf(middleShelfLeds, equalizer->frequenciesLeft[frequencyMode[1]], clampSensitivity(globalSensitivity + 500));
    // waterfallShelf(bottomShelfLeds, equalizer->frequenciesLeft[frequencyMode[0]], clampSensitivity(globalSensitivity + 500));
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

void LEDAnimations::waterfallBorder(int frequencyValue, int frequencyValueMinThreshold, int brightness) {
    if (frequencyValue > frequencyValueMinThreshold) {
        int mappedFrequencyValue = map(frequencyValue, frequencyValueMinThreshold, 1023, 0, 255);
        borderLeds[NUM_BORDER_LEDS / 2] = CHSV(mappedFrequencyValue, brightness, 255);
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

int maxFrequencyValue = 0;
void LEDAnimations::equalizerLeftToRightBottomToTop() {
    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 40);
    equalizerLeftBorder(equalizer->frequenciesLeft[frequencyMode[0]], 300, false);
    equalizerTopBorder(equalizer->frequenciesLeft[frequencyMode[5]], clampSensitivity(globalSensitivity + 400), true);
    equalizerRightBorder(equalizer->frequenciesLeft[frequencyMode[6]], clampSensitivity(globalSensitivity + 200), false);
}

void LEDAnimations::equalizerRightToLeftBottomToTop() {
    equalizerLeftBorder(equalizer->frequenciesLeft[frequencyMode[0]], clampSensitivity(globalSensitivity + 200), false);
    equalizerRightBorder(equalizer->frequenciesLeft[frequencyMode[6]], clampSensitivity(globalSensitivity + 200), false);
    equalizerTopBorder(equalizer->frequenciesLeft[frequencyMode[5]], clampSensitivity(globalSensitivity + 400), false);
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
