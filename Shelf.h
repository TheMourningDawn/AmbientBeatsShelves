#ifndef SHELF_H
#define SHELF_H

#include "application.h"
#include "FastLED.h"

FASTLED_USING_NAMESPACE;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)

class Shelf {
private:
  CRGB *shelfStrip;
  int left;
  int right;
public:
  Shelf();
  Shelf(CRGB *shelves, int leftIndex, int rightIndex);
  void setRightPixel(CRGB color);
  void setLeftPixel(CRGB color);
//   void shiftRight();
//   void sheftRightBy(int shiftAmount);
//   void shiftLeft();
//   void sheftLeftBy(int shiftAmount);
};

#endif
