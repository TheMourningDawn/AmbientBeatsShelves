#ifndef LEDANIMATIONS_H
#define LEDANIMATIONS_H

#include "application.h"
#include "SpectrumEqualizer.h"
#include "FastLED.h"
#include "Shelf.h"

FASTLED_USING_NAMESPACE;

#define NUM_SHELF_LEDS 60
#define LEDS_PER_SHELF 20
#define NUM_BORDER_LEDS 151
#define NUM_TOTAL_LEDS NUM_BORDER_LEDS + NUM_SHELF_LEDS

#define BOTTOM_SHELF_LEFT 0
#define BOTTOM_SHELF_RIGHT 19
#define MIDDLE_SHELF_LEFT 39
#define MIDDLE_SHELF_RIGHT 20
#define TOP_SHELF_LEFT 40
#define TOP_SHELF_RIGHT 59

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

class LEDAnimations
{
  private:
    SpectrumEqualizer *equalizer;
    Shelf *topShelf;
    Shelf *middleShelf;
    Shelf *bottomShelf;
  public:
    CRGB borderLeds[NUM_BORDER_LEDS];
    CRGB allShelves[NUM_SHELF_LEDS];
    bool audioReactive = true;
    uint8_t animationCount;
    int globalSensitivity = 0;
    int currentAnimation = 0;
    int currentHue = 120;
    int currentSaturation = 255;
    int currentBrightness = 255;

    LEDAnimations();
    LEDAnimations(SpectrumEqualizer* eq);

    int getCurrentAnimation();

    void setCurrentBrightness(int brightness);
    void setCurrentSaturation(int saturation);

    int runCurrentAnimation();
    int nextAnimation();
    int previousAnimation();
    int setAnimation(int animationNumber);
    int nextFrequencyMode();
    int previousFrequencyMode();
    int toggleAudioReactive();

    int clampToRange(int numberToClamp, int lowerBound, int upperBound);
    int clampSensitivity(int sensitivity);
    int wrapToRange(int numberToWrap, int lowerBound, int upperBound);

    void randomSilon();

    void clearAllLeds();
    void fillColor();
    void rainbow();
    void confetti();
    void juggle();

    void colorBump();
    void seizureCity();
    void flashyBump();

    void waterfall();
    void waterfallCascading();
    void waterfallBorder(int frequencyValue, int sensitivityValueMinThreshold, int brightness);
    void waterfallShelf(Shelf *shelf, int frequencyValue, int sensitivityThreshold, int brightness, int baseColorOffset);
    void waterfallShelfRight(Shelf *shelf, int frequencyValue, int frequencyThreshold, int brightness, int baseColorOffset);
    void waterfallShelfLeft(Shelf *shelf, int frequencyValue, int frequencyThreshold, int brightness, int baseColorOffset);

    void waterfallBorderRemoteAndSpectrum(int frequencyValue, int sensitivityThreshold);
    void waterfallBorderRemote();
    void waterfallBorderCascading(int frequencyValue, int sensitivityThreshold);
    void waterfallRainbowBorder();
    void waterfallRightToLeft();
    void waterfallLeftToRight();

    void equalizerBorderOnly();
    void equalizerBorderOnlyReversed();
    void equalizerLeftToRightBottomToTop();
    void equalizerRightToLeftBottomToTop();
    void equalizerRightToLeftTopToBottom();
    void equalizerLeftBorder(int frequencyValue, int sensitivityThreshold, bool direction);
    void equalizerRightBorder(int frequencyValue, int sensitivityThreshold, bool direction);
    void equalizerTopBorder(int frequencyValue, int sensitivityThreshold, bool direction);
    void equalizerShelf(Shelf *shelf, int frequencyValue, int sensitivityThreshold, bool direction);
};

#endif
