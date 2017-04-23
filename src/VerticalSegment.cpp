#ifndef VERTICALSEGMENT_CPP
#define VERTICALSEGMENT_CPP

#include "VerticalSegment.h"
/*
   enum that defines which direction the data is going with respect to the ground
   DATA_DOWN would be the source of data at the top, or the arrows pointed down. The data is traveling down.
*/
enum orientation : char
{
  DATA_DOWN,
  DATA_UP
};

orientation segmentOrientation;

VerticalSegment::VerticalSegment() {}

/**
    Creates a new VerticalSegment

    @param segment Pointer to the CRGB strip in which the segment is contained
    @param firstIndex the index of the LED closest to the data input (smaller number)
    @param lastIndex The index of the LED furthest away from the data input (later number)
    @param orientation One of the orientations defined in orientation enum. Tells the strip which way is up/down in relation to data flow.
**/
VerticalSegment::VerticalSegment(CRGB *segment, int firstLEDIndex, int lastLEDIndex, orientation segmentOrientation) : Segment(segment, firstLEDIndex, lastLEDIndex) {
  segmentOrientation = segmentOrientation;
}

int VerticalSegment::getTopLEDIndex() {
  if(segmentOrientation == DATA_DOWN) {
    return getFirstLEDIndex();
  } else {
    return getLastLEDIndex();
  }
}
int VerticalSegment::getBottomLEDIndex() {
  if(segmentOrientation == DATA_UP) {
    return getFirstLEDIndex();
  } else {
    return getLastLEDIndex();
  }
}

void VerticalSegment::setTopLEDColor(CRGB color) {}
void VerticalSegment::setBottomLEDColor(CRGB color) {}

void VerticalSegment::getTopLEDColor() {}
void VerticalSegment::getBottomLEDColor() {}

void VerticalSegment::shiftTowardTop() {}
void VerticalSegment::shiftTowardBottom() {}

void fillFromTop(CRGB color, int numberOfLEDToFIll) {}
void fillFromBottom(CRGB color, int numberOfLEDToFIll) {}

#endif
