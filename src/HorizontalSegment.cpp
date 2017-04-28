#ifndef HORIZONTALSEGMENT_CPP
#define HORIZONTALSEGMENT_CPP

#include "HorizontalSegment.h"

HorizontalOrientation::orientation horizontalOrientation;

HorizontalSegment::HorizontalSegment() {}

/**
    Creates a new HorizontalSegment

    @param segment Pointer to the CRGB strip in which the segment is contained
    @param firstIndex the index of the LED closest to the data input (smaller number)
    @param lastIndex The index of the LED furthest away from the data input (later number)
    @param orientation One of the orientations defined in orientation enum. Tells the strip which way is up/down in relation to data flow.
**/
HorizontalSegment::HorizontalSegment(CRGB *segment, int firstLEDIndex, int lastLEDIndex, HorizontalOrientation::orientation horizontalOrientation) : Segment(segment, firstLEDIndex, lastLEDIndex) {
  horizontalOrientation = horizontalOrientation;
}

int HorizontalSegment::getRightLEDIndex() {
  if(horizontalOrientation == HorizontalOrientation::DATA_LEFT) {
    return getFirstLEDIndex();
  }
  return getLastLEDIndex();
}
int HorizontalSegment::getLeftLEDIndex() {
  if(horizontalOrientation == HorizontalOrientation::DATA_RIGHT) {
    return getFirstLEDIndex();
  }
  return getLastLEDIndex();
}

void HorizontalSegment::setRightLEDColor(CRGB color) {}
void HorizontalSegment::setLeftLEDColor(CRGB color) {}

CRGB HorizontalSegment::getRightLEDColor() {}
CRGB HorizontalSegment::getLeftLEDColor() {}

CRGB HorizontalSegment::shiftTowardRight() {
  // if(horizontalOrientation == HorizontalOrientation::DATA_LEFT) {
    return shiftTowardLast();
  // }
  // return shiftTowardRight();
}

CRGB HorizontalSegment::shiftTowardLeft() {
  return shiftTowardLast();
}

void fillFromRight(CRGB color, int numberOfLEDToFIll) {}
void fillFromLeft(CRGB color, int numberOfLEDToFIll) {}

#endif
