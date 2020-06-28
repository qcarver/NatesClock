#ifndef DBBUTTONH
#define DBBUTTONH
//for uint8_t typedef
#include "Arduino.h"

void setupButtons();

namespace Button{
  bool isLPressed();
  bool isRPressed();
  bool isLHeld();
  bool isRHeld();
  void print();
};

#endif
