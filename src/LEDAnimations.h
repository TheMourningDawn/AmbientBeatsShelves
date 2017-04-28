#ifndef LEDANIMATIONS_H
#define LEDANIMATIONS_H

#include "application.h"
#include "SpectrumEqualizer.h"
#include "FastLED.h"
#include "HorizontalSegment.h"
#include "VerticalSegment.h"

FASTLED_USING_NAMESPACE;

#define NUM_SHELF_LEDS 59
#define LEDS_PER_SHELF 20
#define NUM_BORDER_LEDS 147
#define NUM_TOTAL_LEDS NUM_BORDER_LEDS + NUM_SHELF_LEDS

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

class LEDAnimations : public SpectrumEqualizer
{
  private:
    HorizontalSegment *topBorder;
    HorizontalSegment *topShelf;
    HorizontalSegment *middleShelf;
    HorizontalSegment *bottomShelf;

    VerticalSegment *rightBorder;
    VerticalSegment *leftBorder;
  public:
    CRGB borderLeds[NUM_BORDER_LEDS];
    CRGB allShelves[NUM_SHELF_LEDS];
    uint8_t numberOfPatterns;
    uint16_t globalSensitivity;
    int currentPattern;
    int currentHue;
    bool modeReset;

    LEDAnimations();

    int runCurrentAnimation();
    int nextPattern();
    int previousPattern();
    void nextFrequencyMode();
    void previousFrequencyMode();

    int clampToRange(int numberToClamp, int lowerBound, int upperBound);
    int clampSensitivity(int sensitivity);
    int wrapToRange(int numberToWrap, int lowerBound, int upperBound);

    void randomSilon();

    void clearAllLeds();
};

#endif
