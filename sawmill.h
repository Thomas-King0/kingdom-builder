/* Class Sawmill header file
 */

#ifndef SAWMILL_H
#define SAWMILL_H

#include <iostream>

#include "minor.h"
#include "helpful.h"

class Sawmill: public Minor
{
private:

public:
  Sawmill(); //constructor

  void endProduction() override; //the overriden endProduction function

};

#endif
