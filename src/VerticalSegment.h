#ifndef VERTICALSEGMENT_H
#define VERTICALSEGMENT_H

#include "application.h"
#include "Segment.h"
#include "VerticalOrientation.h"

class VerticalSegment : public Segment
{
  public:
    VerticalSegment();
    VerticalSegment(CRGB *segment, int firstLEDIndex, int lastLEDIndex, VerticalOrientation::orientation verticalOrientation);

    int getTopLEDIndex();
    int getBottomLEDIndex();

    void setTopLEDColor(CRGB color);
    void setBottomLEDColor(CRGB color);

    CRGB getTopLEDColor();
    CRGB getBottomLEDColor();

    CRGB shiftTowardTop();
    CRGB shiftTowardBottom();

    void fillFromTop(CRGB color, int numberOfLEDToFIll);
    void fillFromBottom(CRGB color, int numberOfLEDToFIll);
};

#endif
