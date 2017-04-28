#ifndef HORIZONTALSEGMENT_H
#define HORIZONTALSEGMENT_H

#include "application.h"
#include "Segment.h"
#include "HorizontalOrientation.h"

class HorizontalSegment : public Segment
{
  public:
    HorizontalOrientation::orientation horizontalOrientation;
    HorizontalSegment();
    HorizontalSegment(CRGB *segment, int firstLEDIndex, int lastLEDIndex, HorizontalOrientation::orientation horizontalOrientation);

    int getRightLEDIndex();
    int getLeftLEDIndex();

    void setRightLEDColor(CRGB color);
    void setLeftLEDColor(CRGB color);

    CRGB getRightLEDColor();
    CRGB getLeftLEDColor();

    CRGB shiftTowardRight();
    CRGB shiftTowardLeft();

    void fillFromRight(CRGB color, int numberOfLEDToFIll);
    void fillFromLeft(CRGB color, int numberOfLEDToFIll);
};

#endif
