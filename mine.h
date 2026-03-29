/* Class Mine header file
 */

#ifndef MINE_H
#define MINE_H

#include <iostream>

#include "minor.h"
#include "helpful.h"

class Mine: public Minor
{
private:

public:
  Mine(); //constructor

  void endProduction() override; //the overriden endProduction function

};

#endif
