/* Class pasture header file
 */

#ifndef PASTURE_H
#define PASTURE_H

#include <iostream>

#include "minor.h"
#include "helpful.h"

class Pasture: public Minor
{
private:

public:
  Pasture(); //constructor

  void endProduction() override; //the overriden endProduction function
};

#endif
