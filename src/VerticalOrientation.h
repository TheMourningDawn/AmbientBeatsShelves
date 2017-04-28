#ifndef VERTICALORIENTATION_H
#define VERTICALORIENTATION_H
/*
   enum that defines which direction the data is going with respect to the ground
   DATA_DOWN would be the source of data at the top, or the arrows pointed down. The data is traveling down.
*/
struct VerticalOrientation {
  enum orientation { DATA_DOWN = 1, DATA_UP = 2 };
};

#endif
