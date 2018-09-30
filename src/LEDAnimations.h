#ifndef LEDANIMATIONS_H
#define LEDANIMATIONS_H

#include "application.h"
#include "SpectrumEqualizerClient.h"
#include "AmbientBeatsLEDAnimations.h"
#include "FastLED.h"
#include "Shelf.h"

FASTLED_USING_NAMESPACE;

#define NUM_SHELF_LEDS 59
#define LEDS_PER_SHELF 20
#define NUM_BORDER_LEDS 147
#define NUM_TOTAL_LEDS NUM_BORDER_LEDS + NUM_SHELF_LEDS

#define BOTTOM_SHELF_LEFT 0
#define BOTTOM_SHELF_RIGHT 18
#define MIDDLE_SHELF_LEFT 38
#define MIDDLE_SHELF_RIGHT 19
#define TOP_SHELF_LEFT 39
#define TOP_SHELF_RIGHT 58

class LEDAnimations : public AmbientBeatsLEDAnimations
{
  private:
    Shelf *topShelf;
    Shelf *middleShelf;
    Shelf *bottomShelf;
  public:
    CRGB borderLeds[NUM_BORDER_LEDS];
    CRGB allShelves[NUM_SHELF_LEDS];

    LEDAnimations(SpectrumEqualizerClient* eq);

    int runAnimation();
    int toggleAudioReactive();

    void clearAllLeds();
    void fillColor();

    void randomSilon();
    void rainbow();
    void confetti();
    void juggle();

    void colorBump();
    void seizureCity();
    void flashyBump();

    void waterfall();
    void waterfallCascading();
    void waterfallBorder(int frequencyValue, int sensitivityValueMinThreshold, int brightness, int baseColorOffset);
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
