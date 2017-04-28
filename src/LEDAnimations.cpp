#ifndef LEDANIMATIONS_CPP
#define LEDANIMATIONS_CPP

#include "application.h"
#include "LEDAnimations.h"
#include "HorizontalSegment.h"
#include "VerticalSegment.h"

CRGB borderLeds[NUM_BORDER_LEDS];
CRGB allShelves[NUM_SHELF_LEDS];

HorizontalSegment *topBorder;
HorizontalSegment *topShelf;
HorizontalSegment *middleShelf;
HorizontalSegment *bottomShelf;

VerticalSegment *leftBorder;
VerticalSegment *rightBorder;

uint16_t globalSensitivity = 500;
uint8_t frequencyMode[7] = {0, 1, 2, 3, 4, 5, 6};
uint8_t numberOfPatterns;
int currentPattern;
int currentHue = 0;

typedef void (LEDAnimations::*AnimationList)();

AnimationList animationList[] = {&LEDAnimations::randomSilon};

//TODO: Rename to LEDController?
//Interface?
LEDAnimations::LEDAnimations() {
    topBorder = new HorizontalSegment(borderLeds, 61, 79, HorizontalOrientation::DATA_RIGHT);

    // topShelf = new HorizontalSegment(allShelves, 39, 58, HorizontalOrientation::DATA_RIGHT);
    // middleShelf = new HorizontalSegment(allShelves, 19, 38, HorizontalOrientation::DATA_LEFT);
    // bottomShelf = new HorizontalSegment(allShelves, 0, 18, HorizontalOrientation::DATA_RIGHT);

    leftBorder = new VerticalSegment(borderLeds, 0, 60, VerticalOrientation::DATA_UP);
    rightBorder = new VerticalSegment(borderLeds, 80, 120, VerticalOrientation::DATA_DOWN);

    // topBorder->addIntersection(topBorder->getLeftLEDIndex(), leftBorder, leftBorder->getTopLEDIndex());
    // topBorder->addIntersection(topBorder->getRightLEDIndex(), rightBorder, rightBorder->getTopLEDIndex());
    // // TODO: I dont really want to declare the opposite border, if possible.
    // leftBorder->addIntersection(leftBorder->getTopLEDIndex(), topBorder, topBorder->getLeftLEDIndex());
    // rightBorder->addIntersection(rightBorder->getTopLEDIndex(), topBorder, topBorder->getRightLEDIndex());

    // numberOfPatterns = ARRAY_SIZE(animationList) - 1;
    numberOfPatterns = 1;
    currentPattern = 0;
}

int LEDAnimations::runCurrentAnimation() {
    readAudioFrequencies();
    (this->*animationList[currentPattern])();
    Serial.printf("%s\n", "Finshed with animation in runCurrentAnimation");
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

bool modeReset = true;
// int position = 0;
void LEDAnimations::randomSilon() {
    if(modeReset == true) {
      clearAllLeds();
      FastLED.show();
      delay(10000);
      // CRGB *color = &CRGB(120,240,170);
      // leftBorder->setBottomLEDColor(color);
      borderLeds[26] = CHSV(20, 255, 255);
      FastLED.show();

      // leftBorder->setColor(CRGB(160,10,55), 14);
      // borderLeds[38] = CHSV(0, 200, 220);
      // borderLeds[0] = CHSV(currentHue, 255, 255);
      // position = leftBorder->getBottomLEDIndex();
    }
    // leftBorder->shiftTowardTop();
    // CRGB rightBorderRunoff = rightBorder->shiftTowardBottom();
    // CRGB topBorderRunoff = topBorder->shiftTowardRight();
    //
    // rightBorder->setColor(topBorderRunoff, rightBorder->getTopLEDIndex());
    // topBorder->setColor(leftBorderRunoff, topBorder->getLeftLEDIndex());
    // modeReset = false;
}

#endif
