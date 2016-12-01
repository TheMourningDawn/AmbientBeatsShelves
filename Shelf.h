#ifndef SHELF_H
#define SHELF_H

#include "application.h"
#include "FastLED.h"

FASTLED_USING_NAMESPACE;

class Shelf {
private:
  CRGB *shelfStrip;
  int left;
  int right;
  int previousPushIndex;
public:
  Shelf();
  Shelf(CRGB *shelves, int leftIndex, int rightIndex);
  void setPixel(int index, CRGB color);
  void setRightPixel(CRGB color);
  void setLeftPixel(CRGB color);
  int getRightPixelIndex();
  int getLeftPixelIndex();
  CRGB getRightPixel();
  CRGB getLeftPixel();
  int length();
  void shiftRight();
  void shiftLeft();
  void shiftMiddle();
  void pushLeft(CRGB color);
  void pushRight(CRGB color);
  void fillLeft(CRGB color, int numToFill);
  void fillRight(CRGB color, int numToFill);
};

#endif
