/*Class Fort for kingdom builder
 *
 * this is a major structure
 */

#ifndef FORT_H
#define FORT_H

#include <iostream>
#include <vector>

#include "major.h"
#include "helpful.h"

//forward declarations
class Land;
class Minor;

class Fort: public Major
{
private:
  std::vector<int> materials; //this vector stores how much of each material the fort has
public:
  Fort(); //constructor

  static bool isValid(Land* base); //check if the land tile is a valid base tile

  void setVacancy(int num) override; //overriden setVacancy function

  void setMaterials(Material resource, int num) override; //set how much of a material a village has
  int getMaterials(Material resource) override; //get the how much of a material a village has
  int getMaterials(int resource) override; //get how much of a material a village has

  void trainTownsfolk() override; //train the trainTownsfolk

  double getDefense() override; //get the defense ability for the fort

  std::string getData() override; //return data on village

};

#endif
