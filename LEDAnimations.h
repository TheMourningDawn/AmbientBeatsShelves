#ifndef LEDANIMATIONS_H
#define LEDANIMATIONS_H

#include "application.h"
#include "SpectrumEqualizer.h"
#include "FastLED.h"

FASTLED_USING_NAMESPACE;

#define NUM_SHELF_LEDS 59
#define LEDS_PER_SHELF 20
#define NUM_BORDER_LEDS 147
#define NUM_TOTAL_LEDS NUM_BORDER_LEDS + NUM_SHELF_LEDS

#define BRIGHTNESS         120
#define FRAMES_PER_SECOND  240

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)

class LEDAnimations
{
  private:
    SpectrumEqualizer *equalizer;
  public:
    CRGB borderLeds[NUM_BORDER_LEDS];
    CRGB allShelves[NUM_SHELF_LEDS];
    // CRGB *bottomShelfLeds[LEDS_PER_SHELF];
    // CRGB *middleShelfLeds[LEDS_PER_SHELF];
    // CRGB *topShelfLeds[LEDS_PER_SHELF];
    CRGB *bottomShelfLeds;
    CRGB *middleShelfLeds;
    CRGB *topShelfLeds;
    uint8_t currentPattern;
    uint8_t numberOfPatterns;
    uint8_t hueCounter;

    LEDAnimations();
    LEDAnimations(SpectrumEqualizer* eq);

    int runCurrentAnimation();
    int nextPattern();
    int previousPattern();
    void nextFrequencyMode();
    void previousFrequencyMode();

    int clampToRange(int numberToClamp, int lowerBound, int upperBound);
    int clampSensitivity(int sensitivity);
    int wrapToRange(int numberToWrap, int lowerBound, int upperBound);

    void setBottomShelf(int index, CHSV color);
    void setMiddleShelf(int index, CHSV color);
    void setTopShelf(int index, CHSV color);
    void waterfallBottomShelf(int frequencyValue, int frequencyThreshold);

    void clearAllLeds();
    void rainbow();
    void fuckinShit();
    void confetti(int frequencyValue);
    void sinelon(int frequencyValue);
    void bpm();
    void juggle(int frequencyValue);
    void waterfall();
    void waterfallCascading();
    void waterfallShelf(CRGB shelf[], int frequencyValue, int sensitivityThreshold);
    void waterfallBorder(int frequencyValue, int sensitivityThreshold);
    void waterfallBorderControllerToo(int frequencyValue, int sensitivityThreshold);
    void waterfallBorderControllerOnly();
    void waterfallBorderCascading(int frequencyValue, int sensitivityThreshold);
    void equalizerLeftToRightBottomToTop();
    void equalizerRightToLeftBottomToTop();
    void equalizerLeftBorder(int frequencyValue, int sensitivityThreshold, bool direction);
    void equalizerRightBorder(int frequencyValue, int sensitivityThreshold, bool direction);
    void equalizerTopBorder(int frequencyValue, int sensitivityThreshold, bool direction);
    void equalizerShelf(CRGB shelf[], int frequencyValue, int sensitivityThreshold, bool direction);
};

#endif
