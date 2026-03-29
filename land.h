/*Class Land for kingdom_builder game
 */


#ifndef LAND_H
#define LAND_H

#include <iostream>
#include "helpful.h"

class Major; //forward declaration



class Land
{
private:
  int positionX; //position of the land tile in X (c_cen)
  int positionY; //position of the land tile in Y (c_cen)

  Major* keep; //pointer to the keep (governing structure)

  Land_type type; //what type of land it is
  bool explored; //boolean containing whether or not the tile is visible

public://still need to decide what arguments constructor should take
  Land(); //defualt constructor

  Land(int pos_x, int pos_y); //constructor for a specific position

  //position functions
  void setX(int pos); //set the X coordinate
  int getX(); //get the X coordinate

  void setY(int pos); //set the Y coordinate
  int getY(); //get the Y coordinate

  //keep functions
  void setKeep(Major* major); //set the keep
  Major* getKeep(); //get the address of the keep

  //type functions
  void setType(Land_type biome); //set the land type
  Land_type getType(); //get the land type
  std::string getTypeString(); //get the land type as a string

  //visibility functions
  void setExplored(bool tf); //set the visible variable
  bool isExplored();

  //other functions
  bool isAdjacent(Land* neighbor); //return if the tile is adjacent to the input

  std::string getData(); //return a string containing the data of the tile

};

#endif
