/* Class Garden header file
 */

#ifndef GARDEN_H
#define GARDEN_H

#include <iostream>

#include "minor.h"
#include "helpful.h"

class Garden: public Minor
{
private:

public:
  Garden(); //constructor

  void endProduction() override; //the overriden endProduction function

};

#endif
