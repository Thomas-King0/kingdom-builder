/* Class Quarry header file
 */

#ifndef QUARRY_H
#define QUARRY_H

#include <iostream>

#include "minor.h"
#include "helpful.h"

class Quarry: public Minor
{
private:

public:
  Quarry(); //constructor

  void endProduction() override; //the overriden endProduction function

};

#endif
