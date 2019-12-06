#ifndef LEDANIMATIONS_CPP
#define LEDANIMATIONS_CPP

#include "LEDAnimations.h"

CRGB borderLeds[NUM_BORDER_LEDS];
CRGB allShelves[NUM_SHELF_LEDS];

Shelf *topShelf;
Shelf *middleShelf;
Shelf *bottomShelf;

typedef void (LEDAnimations::*AnimationList)();

AnimationList animationsAudioReactive[] = {&LEDAnimations::waterfall,
                                           &LEDAnimations::colorBump,
                                           &LEDAnimations::seizureCity,
                                           &LEDAnimations::flashyBump,
                                           &LEDAnimations::waterfallLeftToRight,
                                           &LEDAnimations::waterfallRightToLeft,
                                           &LEDAnimations::equalizerLeftToRightBottomToTop,
                                           &LEDAnimations::equalizerRightToLeftBottomToTop,
                                           &LEDAnimations::equalizerBorderOnly,
                                           &LEDAnimations::equalizerBorderOnlyReversed};

// WARNING: I tried to name this animations, and the particle compiler kept timing out. wtf, eh?
AnimationList animationsRails[] = {&LEDAnimations::randomSilon, &LEDAnimations::fillColor, &LEDAnimations::confetti,
                                   &LEDAnimations::juggle, &LEDAnimations::rainbow, &LEDAnimations::waterfallBorderRemote,
                                   &LEDAnimations::waterfallRainbowBorder};

LEDAnimations::LEDAnimations(SpectrumEqualizerClient *eq) : AmbientBeatsLEDAnimations(eq) {
    topShelf = new Shelf(allShelves, TOP_SHELF_LEFT, TOP_SHELF_RIGHT);
    middleShelf = new Shelf(allShelves, MIDDLE_SHELF_LEFT, MIDDLE_SHELF_RIGHT);
    bottomShelf = new Shelf(allShelves, BOTTOM_SHELF_LEFT, BOTTOM_SHELF_RIGHT);
    animationCount = ARRAY_SIZE(animationsAudioReactive) - 1;
}

int LEDAnimations::runAnimation() {
    equalizer->readAudioFrequencies();
    if (audioReactiveOn) {
        (this->*animationsAudioReactive[animation])();
    } else {
        (this->*animationsRails[animation])();
    }
}

int position = 0;
boolean direction = true;
String ledStripToUse = "border";
void LEDAnimations::randomSilon() {
    if (ledStripToUse == "border") {
        borderLeds[position] = CHSV(hue, saturation, brightness);
    } else if (ledStripToUse == "shelf") {
        allShelves[position] = CHSV(hue, saturation, brightness);
    }
    if (position == NUM_BORDER_LEDS) {
        position = BOTTOM_SHELF_RIGHT;
        ledStripToUse = "shelf";
        direction = false;
    } else if (position == BOTTOM_SHELF_LEFT && ledStripToUse == "border" && !direction) {
        position = -1;
        direction = true;
        ledStripToUse = "shelf";
    } else if (position == 21 && ledStripToUse == "border") {
        if (random8(10) > 5) {
            ledStripToUse = "shelf";
            direction = false;
            position = MIDDLE_SHELF_LEFT;
        }
    } else if (position == 42 && ledStripToUse == "border") {
        if (random8(10) > 5) {
            ledStripToUse = "shelf";
            direction = true;
            position = TOP_SHELF_LEFT;
        }
    } else if (position == 104 && ledStripToUse == "border") {
        if (random8(10) > 5) {
            ledStripToUse = "shelf";
            direction = false;
            position = TOP_SHELF_RIGHT;
        }
    } else if (position == 125 && ledStripToUse == "border") {
        if (random8(10) > 5) {
            ledStripToUse = "shelf";
            direction = true;
            position = MIDDLE_SHELF_RIGHT;
        }
    } else if (position == BOTTOM_SHELF_RIGHT && ledStripToUse == "shelf" && direction) {
        position = NUM_BORDER_LEDS - 1;
        direction = false;
        ledStripToUse = "border";
    } else if (position == MIDDLE_SHELF_RIGHT && ledStripToUse == "shelf" && !direction) {
        if (random8(10) > 5) {
            ledStripToUse = "border";
            direction = true;
            position = 125;
        } else {
            ledStripToUse = "border";
            direction = false;
            position = 125;
        }
    } else if (position == BOTTOM_SHELF_LEFT && ledStripToUse == "shelf" && direction == false) {
        position = -1;
        direction = true;
        ledStripToUse = "border";
    } else if (position == TOP_SHELF_LEFT && ledStripToUse == "shelf" && direction == false) {
        if (random8(10) > 5) {
            ledStripToUse = "border";
            direction = true;
            position = 42;
        } else {
            ledStripToUse = "border";
            direction = false;
            position = 42;
        }
    } else if (position == TOP_SHELF_RIGHT && ledStripToUse == "shelf" && direction == true) {
        if (random8(10) > 5) {
            ledStripToUse = "border";
            direction = true;
            position = 104;
        } else {
            ledStripToUse = "border";
            direction = false;
            position = 104;
        }
    } else if (position == MIDDLE_SHELF_LEFT && ledStripToUse == "shelf" && direction == true) {
        if (random8(10) > 5) {
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
    if (direction == true) {
        position++;
    } else {
        position--;
    }
    if (position < 0) { position = 0; }
    if (position > NUM_BORDER_LEDS) { position = NUM_BORDER_LEDS; };
}

int LEDAnimations::toggleAudioReactive() {
    audioReactiveOn = !audioReactiveOn;
    animation = 0;

    if (audioReactiveOn) {
        animationCount = ARRAY_SIZE(animationsAudioReactive) - 1;
    } else {
        animationCount = ARRAY_SIZE(animationsRails) - 1;
    }

    return audioReactiveOn;
}

void LEDAnimations::clearAllLeds() {
    for (uint8_t j = 0; j < NUM_TOTAL_LEDS; j++) {
        borderLeds[j] = 0;
        if (j > NUM_BORDER_LEDS) {
            allShelves[j % NUM_BORDER_LEDS] = 0;
        }
    }
}

void LEDAnimations::fillColor() {
    fill_solid(borderLeds, NUM_BORDER_LEDS, CHSV(hue, saturation, brightness));
    fill_solid(allShelves, NUM_SHELF_LEDS, CHSV(hue, saturation, brightness));
}

void LEDAnimations::rainbow() {
    fill_rainbow(borderLeds, NUM_BORDER_LEDS, hue, 7);
    fill_rainbow(allShelves, NUM_SHELF_LEDS, hue, 7);
}

// random colored speckles that blink in and fade smoothly
void LEDAnimations::confetti() {
    uint8_t position = random16(NUM_TOTAL_LEDS);
    uint16_t frequencyThreshold = clampSensitivity(sensitivity + 600);

    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 10);
    fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 10);

    // if (frequencyValue > frequencyThreshold) {
    if (position > NUM_BORDER_LEDS) {
        allShelves[position % NUM_BORDER_LEDS] += CHSV(hue + random8(64), saturation, brightness);
    } else {
        borderLeds[position] += CHSV(hue + random8(64), saturation, brightness);
    }
    // }
}

// eight colored dots, weaving in and out of sync with each other
void LEDAnimations::juggle() {
    int frequencyValue = equalizer->frequenciesLeftChannel[frequencyMode[0]];
    uint16_t frequencyThreshold = clampSensitivity(sensitivity + 600);

    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 20);
    fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 20);
    byte dothue = 0;
    // if (frequencyValue > frequencyThreshold) {
    for (int i = 0; i < 8; i++) {
        int currentLocation = beatsin16(i + 7, 0, NUM_TOTAL_LEDS);
        if (currentLocation > NUM_BORDER_LEDS) {
            allShelves[currentLocation % NUM_BORDER_LEDS] |= CHSV(dothue, saturation, brightness);
        } else {
            borderLeds[currentLocation] |= CHSV(dothue, saturation, brightness);
        }
        dothue += 32;
    }
    // }
}

int previousBrightness = 0;
void LEDAnimations::colorBump() {
    int thisBrightness = map(equalizer->frequenciesLeftChannel[frequencyMode[6]], 0, 4096, 0, 255);

    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 5);
    fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 5);

    if (thisBrightness > previousBrightness) {
        fill_solid(borderLeds, NUM_BORDER_LEDS, CHSV(hue, saturation, thisBrightness));
        fill_solid(allShelves, NUM_SHELF_LEDS, CHSV(hue, saturation, thisBrightness));
    }
    previousBrightness = thisBrightness;
}

void LEDAnimations::flashyBump() {
    int thisBrightness = map(equalizer->frequenciesLeftChannel[frequencyMode[6]], 0, 4096, 0, 255);

    fill_solid(borderLeds, NUM_BORDER_LEDS, CHSV(hue, saturation, thisBrightness));
    fill_solid(allShelves, NUM_SHELF_LEDS, CHSV(hue, saturation, thisBrightness));
}

void LEDAnimations::seizureCity() {
    int thisBrightness = map(equalizer->frequenciesLeftChannel[frequencyMode[6]], sensitivity, 4096, 0, 255);
    if (thisBrightness > 240) {
        hue += 20;
        wrapToRange(hue += 20, 0, 255);
    }

    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 2);
    fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 2);

    if (thisBrightness > previousBrightness) {
        fill_solid(borderLeds, NUM_BORDER_LEDS, CHSV(hue, saturation, thisBrightness));
        fill_solid(allShelves, NUM_SHELF_LEDS, CHSV(hue, saturation, thisBrightness));
    }

    previousBrightness = thisBrightness;
}

void LEDAnimations::waterfall() {
    waterfallShelf(topShelf, equalizer->frequenciesLeftChannel[frequencyMode[6]], sensitivity, brightness, hue);
    waterfallShelf(middleShelf, equalizer->frequenciesLeftChannel[frequencyMode[2]], sensitivity, brightness, hue);
    waterfallShelf(bottomShelf, equalizer->frequenciesLeftChannel[frequencyMode[0]], sensitivity, brightness, hue);
    waterfallBorder(equalizer->frequenciesLeftChannel[frequencyMode[4]], sensitivity, brightness, hue);
}

void LEDAnimations::waterfallShelf(Shelf *shelf, int frequencyValue, int frequencyThreshold, int brightness,
                                   int baseColorOffset) {
    int middlePixel = (shelf->getRightPixelIndex() + shelf->getLeftPixelIndex()) / 2;
    if (frequencyValue > frequencyThreshold) {
        int mappedFrequencyValue = map(frequencyValue, frequencyThreshold, 4096, 0, 255);
        mappedFrequencyValue = (mappedFrequencyValue + baseColorOffset) % 255; //offsetting the base color...
        shelf->setPixel(middlePixel, CHSV(mappedFrequencyValue, saturation, brightness));
        shelf->setPixel(middlePixel + 1, CHSV(mappedFrequencyValue, saturation, brightness));
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
        shelf->setRightPixel(CHSV(mappedFrequencyValue, saturation, brightness));
    } else {
        shelf->setRightPixel(CRGB(0, 0, 0));
    }
    shelf->shiftLeft();
}

void LEDAnimations::waterfallShelfRight(Shelf *shelf, int frequencyValue, int frequencyThreshold, int brightness, int baseColorOffset) {
    if (frequencyValue > frequencyThreshold) {
        int mappedFrequencyValue = map(frequencyValue, frequencyThreshold, 4096, 0, 255);
        mappedFrequencyValue = (mappedFrequencyValue + baseColorOffset) % 255; //offsetting the base color...
        shelf->setLeftPixel(CHSV(mappedFrequencyValue, saturation, brightness));
    } else {
        shelf->setLeftPixel(CRGB(0, 0, 0));
    }
    shelf->shiftRight();
}

void LEDAnimations::waterfallBorder(int frequencyValue, int frequencyValueMinThreshold, int brightness, int baseColorOffset) {
    if (frequencyValue > frequencyValueMinThreshold) {
        int mappedFrequencyValue = map(frequencyValue, frequencyValueMinThreshold, 4096, 0, 255);
        mappedFrequencyValue = (mappedFrequencyValue + baseColorOffset) % 255; //offsetting the base color...
        borderLeds[NUM_BORDER_LEDS / 2] = CHSV(mappedFrequencyValue, saturation, brightness);
    } else {
        borderLeds[NUM_BORDER_LEDS / 2] = CRGB(0, 0, 0);
    }
    memmove(&borderLeds[0], &borderLeds[1], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
    memmove(&borderLeds[NUM_BORDER_LEDS / 2 + 1], &borderLeds[NUM_BORDER_LEDS / 2], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
}

void LEDAnimations::waterfallLeftToRight() {
    int sensitivityValueMinThreshold = clampSensitivity(sensitivity + 700);
    waterfallShelfLeft(topShelf, equalizer->frequenciesLeftChannel[frequencyMode[6]], sensitivityValueMinThreshold, brightness, hue);
    waterfallShelfLeft(middleShelf, equalizer->frequenciesLeftChannel[frequencyMode[2]], sensitivityValueMinThreshold, brightness, hue);
    waterfallShelfLeft(bottomShelf, equalizer->frequenciesLeftChannel[frequencyMode[0]], sensitivityValueMinThreshold, brightness, hue);
    waterfallBorder(equalizer->frequenciesLeftChannel[frequencyMode[4]], sensitivityValueMinThreshold, brightness, hue);
}

void LEDAnimations::waterfallRightToLeft() {
    int sensitivityValueMinThreshold = clampSensitivity(sensitivity + 700);
    waterfallShelfRight(topShelf, equalizer->frequenciesLeftChannel[frequencyMode[6]], sensitivityValueMinThreshold, brightness, 180);
    waterfallShelfRight(middleShelf, equalizer->frequenciesLeftChannel[frequencyMode[2]], sensitivityValueMinThreshold, brightness, 80);
    waterfallShelfRight(bottomShelf, equalizer->frequenciesLeftChannel[frequencyMode[0]], sensitivityValueMinThreshold, brightness, 40);
    waterfallBorder(equalizer->frequenciesLeftChannel[frequencyMode[4]], sensitivityValueMinThreshold, brightness, hue);
}

void LEDAnimations::waterfallBorderRemote() {
    borderLeds[NUM_BORDER_LEDS / 2] = CHSV(hue, saturation, brightness);
    memmove(&borderLeds[0], &borderLeds[1], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
    memmove(&borderLeds[NUM_BORDER_LEDS / 2 + 1], &borderLeds[NUM_BORDER_LEDS / 2], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
}

uint8_t hueCounter = 0;

void LEDAnimations::waterfallRainbowBorder() {
    borderLeds[NUM_BORDER_LEDS / 2] = CHSV(hueCounter, saturation, brightness);
    memmove(&borderLeds[0], &borderLeds[1], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
    memmove(&borderLeds[NUM_BORDER_LEDS / 2 + 1], &borderLeds[NUM_BORDER_LEDS / 2], NUM_BORDER_LEDS / 2 * sizeof(CRGB));
    hueCounter++;
}

void LEDAnimations::equalizerLeftToRightBottomToTop() {
    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 10);
    fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 10);
    equalizerLeftBorder(equalizer->frequenciesLeftChannel[frequencyMode[0]], clampSensitivity(sensitivity + 400), false);
    equalizerTopBorder(equalizer->frequenciesLeftChannel[frequencyMode[2]], clampSensitivity(sensitivity + 400), true);
    equalizerRightBorder(equalizer->frequenciesLeftChannel[frequencyMode[6]], clampSensitivity(sensitivity + 400), false);
    equalizerShelf(topShelf, equalizer->frequenciesLeftChannel[frequencyMode[4]], clampSensitivity(sensitivity + 400), true);
    equalizerShelf(middleShelf, equalizer->frequenciesLeftChannel[frequencyMode[3]], clampSensitivity(sensitivity + 400), true);
    equalizerShelf(bottomShelf, equalizer->frequenciesLeftChannel[frequencyMode[5]], clampSensitivity(sensitivity + 400), true);
}

void LEDAnimations::equalizerRightToLeftBottomToTop() {
    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 40);
    fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 45);
    equalizerLeftBorder(equalizer->frequenciesLeftChannel[frequencyMode[0]], clampSensitivity(sensitivity + 400), false);
    equalizerTopBorder(equalizer->frequenciesLeftChannel[frequencyMode[2]], clampSensitivity(sensitivity + 400), false);
    equalizerRightBorder(equalizer->frequenciesLeftChannel[frequencyMode[6]], clampSensitivity(sensitivity + 400), false);
    equalizerShelf(topShelf, equalizer->frequenciesLeftChannel[frequencyMode[4]], clampSensitivity(sensitivity + 400), false);
    equalizerShelf(middleShelf, equalizer->frequenciesLeftChannel[frequencyMode[3]], clampSensitivity(sensitivity + 400), false);
    equalizerShelf(bottomShelf, equalizer->frequenciesLeftChannel[frequencyMode[5]], clampSensitivity(sensitivity + 400), false);
}


// TODO: This is broken. Falling off an array somewhere, I think.
//void LEDAnimations::equalizerRightToLeftTopToBottom() {
//    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 40);
//    fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 45);
//    equalizerLeftBorder(equalizer->frequenciesLeftChannel[frequencyMode[0]], clampSensitivity(sensitivity + 400), true);
//    equalizerTopBorder(equalizer->frequenciesLeftChannel[frequencyMode[2]], clampSensitivity(sensitivity + 400), false);
//    equalizerRightBorder(equalizer->frequenciesLeftChannel[frequencyMode[6]], clampSensitivity(sensitivity + 400), true);
//    equalizerShelf(topShelf, equalizer->frequenciesLeftChannel[frequencyMode[4]], clampSensitivity(sensitivity + 400), false);
//    equalizerShelf(middleShelf, equalizer->frequenciesLeftChannel[frequencyMode[3]], clampSensitivity(sensitivity + 400), false);
//    equalizerShelf(bottomShelf, equalizer->frequenciesLeftChannel[frequencyMode[5]], clampSensitivity(sensitivity + 400), false);
//}

void LEDAnimations::equalizerBorderOnly() {
    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 40);
    fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 40);
    equalizerLeftBorder(equalizer->frequenciesLeftChannel[frequencyMode[0]], clampSensitivity(sensitivity + 400), false);
    equalizerRightBorder(equalizer->frequenciesLeftChannel[frequencyMode[6]], clampSensitivity(sensitivity + 400), false);
}

void LEDAnimations::equalizerBorderOnlyReversed() {
    fadeToBlackBy(borderLeds, NUM_BORDER_LEDS, 40);
    fadeToBlackBy(allShelves, NUM_SHELF_LEDS, 40);
    equalizerLeftBorder(equalizer->frequenciesLeftChannel[frequencyMode[6]], clampSensitivity(sensitivity + 400), false);
    equalizerRightBorder(equalizer->frequenciesLeftChannel[frequencyMode[0]], clampSensitivity(sensitivity + 400), false);
}

void LEDAnimations::equalizerLeftBorder(int frequencyValue, int sensitivityThreshold, bool direction) {
    int ledsInSection = 62;
    if (frequencyValue > sensitivityThreshold) {
        int numberToLight = map(frequencyValue, 0, 3500, 0, ledsInSection);
        int mappedFrequencyValue = map(frequencyValue, sensitivityThreshold, 4096, 0, 255);
        mappedFrequencyValue = (mappedFrequencyValue + hue) % 255; //offsetting the base color...
        CRGB color = CHSV(mappedFrequencyValue, saturation, brightness);
        if (direction) {
            for (int i = ledsInSection - numberToLight; i < ledsInSection; i++) {
                borderLeds[i] = color;
            }
        } else {
            for (int i = 0; i < numberToLight; i++) {
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
        int mappedFrequencyValue = map(frequencyValue, sensitivityThreshold, 4096, 0, 255);
        mappedFrequencyValue = (mappedFrequencyValue + hue) % 255; //offsetting the base color...
        CRGB color = CHSV(mappedFrequencyValue, saturation, brightness);
        if (direction) {
            for (int i = locationOffset; i < locationOffset + numberToLight; i++) {
                borderLeds[i] = color;
            }
        } else {
            for (int i = locationOffset + ledsInSection - numberToLight; i < locationOffset + ledsInSection; i++) {
                borderLeds[i] = color;
            }
        }
    }
}

void LEDAnimations::equalizerTopBorder(int frequencyValue, int sensitivityThreshold, bool direction) {
    int ledsInSection = 20;
    int locationOffset = 63;
    for (int i = locationOffset; i < ledsInSection + locationOffset; i++) {
        borderLeds[i].fadeToBlackBy(45);
    }
    if (frequencyValue > sensitivityThreshold) {
        int numberToLight = map(frequencyValue, sensitivityThreshold, 3500, 0, ledsInSection);
        int mappedFrequencyValue = map(frequencyValue, sensitivityThreshold, 4096, 0, 255);
        mappedFrequencyValue = (mappedFrequencyValue + hue) % 255; //offsetting the base color...
        CRGB color = CHSV(mappedFrequencyValue, saturation, brightness);
        if (direction) {
            for (int i = locationOffset; i < locationOffset + numberToLight; i++) {
                borderLeds[i] = color;
            }
        } else {
            for (int i = locationOffset + ledsInSection - numberToLight; i < locationOffset + ledsInSection; i++) {
                borderLeds[i] = color;
            }
        }
    }
}

void LEDAnimations::equalizerShelf(Shelf *shelf, int frequencyValue, int sensitivityThreshold, bool direction) {
    if (frequencyValue > sensitivityThreshold) {
        int numberToLight = map(frequencyValue, sensitivityThreshold, 3500, 0, LEDS_PER_SHELF);
        int mappedFrequencyValue = map(frequencyValue, sensitivityThreshold, 4096, 0, 255);
        mappedFrequencyValue = (mappedFrequencyValue + hue) % 255; //offsetting the base color...
        CRGB color = CHSV(mappedFrequencyValue, saturation, brightness);
        if (direction) {
            shelf->fillLeft(color, numberToLight);
        } else {
            shelf->fillRight(color, numberToLight);
        }
    }
}

#endif
