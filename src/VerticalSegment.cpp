#ifndef VERTICALSEGMENT_CPP
#define VERTICALSEGMENT_CPP

#include "VerticalSegment.h"
VerticalOrientation::orientation segmentOrientation;

VerticalSegment::VerticalSegment() {}

/**
    Creates a new VerticalSegment

    @param segment Pointer to the CRGB strip in which the segment is contained
    @param firstIndex the index of the LED closest to the data input (smaller number)
    @param lastIndex The index of the LED furthest away from the data input (later number)
    @param orientation One of the orientations defined in orientation enum. Tells the strip which way is up/down in relation to data flow.
**/
VerticalSegment::VerticalSegment(CRGB *segment, int firstLEDIndex, int lastLEDIndex, VerticalOrientation::orientation segmentOrientation) : Segment(segment, firstLEDIndex, lastLEDIndex) {
  segmentOrientation = segmentOrientation;
}


int VerticalSegment::getTopLEDIndex() {
  if(segmentOrientation == VerticalOrientation::DATA_DOWN) {
    return getFirstLEDIndex();
  } else {
    return getLastLEDIndex();
  }
}
int VerticalSegment::getBottomLEDIndex() {
  if(segmentOrientation == VerticalOrientation::DATA_UP) {
    return getFirstLEDIndex();
  } else {
    return getLastLEDIndex();
  }
}

void VerticalSegment::setTopLEDColor(CRGB color) {}
void VerticalSegment::setBottomLEDColor(CRGB color) {}

CRGB VerticalSegment::getTopLEDColor() {
  return getFirstLEDColor();
}
CRGB VerticalSegment::getBottomLEDColor() {
  return getLastLEDColor();
}

CRGB VerticalSegment::shiftTowardTop() {
    return shiftTowardLast();
}
CRGB VerticalSegment::shiftTowardBottom() {
  return shiftTowardFirst();
}

void fillFromTop(CRGB color, int numberOfLEDToFIll) {}
void fillFromBottom(CRGB color, int numberOfLEDToFIll) {}

#endif
