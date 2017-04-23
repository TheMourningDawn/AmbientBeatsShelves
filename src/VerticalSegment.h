#ifndef VERTICALSEGMENT_H
#define VERTICALSEGMENT_H

#include "application.h"
#include "Segment.h"

class VerticalSegment : public Segment
{
  private:
    enum orientation : char;

  public:
    VerticalSegment();
    VerticalSegment(CRGB *segment, int firstLEDIndex, int lastLEDIndex, orientation verticalOrientation);

    int getTopLEDIndex();
    int getBottomLEDIndex();

    void setTopLEDColor(CRGB color);
    void setBottomLEDColor(CRGB color);

    void getTopLEDColor();
    void getBottomLEDColor();

    void shiftTowardTop();
    void shiftTowardBottom();

    void fillFromTop(CRGB color, int numberOfLEDToFIll);
    void fillFromBottom(CRGB color, int numberOfLEDToFIll);
};

#endif
