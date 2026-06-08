/*Class Castle for kingdom builder
 *
 * this is a major structure
 */

#ifndef CASTLE_H
#define CASTLE_H

#include <iostream>
#include <vector>

#include "major.h"
#include "helpful.h"

//forward declarations
class Land;
class Minor;

class Castle: public Major
{
private:
public:
  Castle(); //constructor

  static bool isValid(Land* base); //check if the land tile is a valid base tile

  void setVacancy(int num) override; //overriden setVacancy function

  void trainTownsfolk() override; //train the trainTownsfolk

  double getDefense() override; //get the defense ability for the castle

  std::string getData() override; //return data on village

};

#endif
