#ifndef SHELF_H
#define SHELF_H

#include "application.h"
#include "FastLED.h"

FASTLED_USING_NAMESPACE;

//TODO: Change the idea of a Shelf to a Segment
// Sements will be given a strip in which the segment is contained
// Segments will be given the led indexes indicating the begin and end
// Segments could be created via a factory
// Either:
//  Segment factory will need to know all the above,
//  as well as an orientation (Horizontal, Verticle),
//  and which direction the data came from
//      This part is difficult, because I don't know how to easily ask the question.
//      It will depend on where the microcontroller was placed
//  also, it would ideal to be able to know how and where segments intersect.
// Or:
//  Make seperate classes for a Horizontal and Verticle Segements, and decide the above in the constructor.
//

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
