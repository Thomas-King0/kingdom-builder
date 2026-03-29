/*Class Land object file
 */

#include <iostream>
#include <cmath>

#include "land.h"
#include "major.h"
#include "barbarian.h"
#include "helpful.h"
#include "settings.h"



Land::Land() //defualt constructor
{
  //initialize the tile to the origin
  setX(0);
  setY(0);

  //set the keep
  setKeep(nullptr);

  //set the type
  setType(SPACE);

  //set the visibility
  setExplored(false);
}


Land::Land(int pos_x, int pos_y) //constructor for a specific position
{
  Land(); //call the default constructor

  //initialize the tile to the position
  setX(pos_x);
  setY(pos_y);
}

//*********************coordinate functions***********************

//setX
void Land::setX(int pos) //set the X coordinate (c_cen)
{
  /*the map extends MAP_SIZE tiles to either side of the origin. This means that the side length of the map is
   * MAP_SIZE+origin+MAP_SIZE, which is 2*MAP_SIZE+1. So if the entered position is greater than that it is off
   * the map.
   */
  if (pos<(2*MAP_SIZE+1))
  {
    positionX=pos;
  }
  else
  {
    std::cout<<"Tried to set X coordinate to an out of bounds value["<<pos<<"]\n setting coordinate to 0\n";
    positionX=0;
  }
}

//getX
int Land::getX() //get the X coordinate (c_cen)
{
  return positionX;
}

//setY
void Land::setY(int pos) //set the coordinate (c_cen)
{
  /*Like for setX(), the map has a height of 2*MAP_SIZE+1. This means that pos must be in [0, 2*MAP_SIZE+1]
   */
  if (pos<(2*MAP_SIZE+1))
  {
    positionY=pos;
  }
  else
  {
    std::cout<<"Tried to set Y coordinate to an out of bounds value["<<pos<<"]\nsetting coordinate to 0\n";
    positionY=0;
  }
}

//getY
int Land::getY() //get the Y coordinate (c_cen)
{
  return positionY;
}


//******************keep functions*********************************


//setKeep
void Land::setKeep(Major* major) //set the keep
{
  //always allow for a keep to be set to the nullptr
  if (major==nullptr) //if the program is trying to set the keep to the nullptr
  {
    keep=nullptr;
    return;
  }

  /*check to make sure that the keep is the nullptr. If it is not the nullptr then the tile already has a keep and
   * resetting the keep would mean that two structures are claiming this tile as a base.
   */
  if (keep==nullptr)
  {
    keep=major;
  }
  else
  {
    std::cout<<"tried setting a keep for a land tile that already has a keep\n";
    std::cout<<"\tkeep is "<<keep<<"\n";
  }
}

//getKeep
Major* Land::getKeep() //get the address of the keep
{
  return keep;
}



//*****************type functions*******************************

//setType
void Land::setType(Land_type biome) //set the land type
{
  type=biome;
}

//getType
Land_type Land::getType() //get the land type
{
  return type;
}

std::string Land::getTypeString() //get the land type as a string
{
  Land_type kind=getType();
  std::string kind_str;
  switch (kind)
  {
    case (FOREST):
    {
      kind_str="forest";
      break;
    }
    case (FIELDS):
    {
      kind_str="fields";
      break;
    }
    case (MOUNTAINS):
    {
      kind_str="mountains";
      break;
    }
    case (TUNDRA):
    {
      kind_str="tundra";
      break;
    }
    case (SEA):
    {
      kind_str="sea";
      break;
    }
    case (SPACE):
    {
      kind_str="space";
      break;
    }
    default:
    {
      break;
    }
  }

  return kind_str;
}

//********************visiblity functions*****************************

//setVisible
void Land::setExplored(bool tf)
{
  explored=tf;
}

//isVisible
bool Land::isExplored()
{
  return explored;
}

//************************other functions***********************************

//isAdjacent
bool Land::isAdjacent(Land* neighbor) //check if another tile is adjacent to this tile
{
  /*
  int neighbor_x=static_cast<int>(neighbor->getX()); //get the neighbor's X coordinate
  int neighbor_y=static_cast<int>(neighbor->getY()); //get the neighbor's Y coordinate

  int diff_x=getX()-neighbor_x; //get the difference in the coordinates
  int diff_y=getY()-neighbor_y;
  */
  double distance=dist(this, neighbor); //calculate the distance between land tiles

  if (distance<2) //as long as the distance is less than 2, that means that the tile is adjacent
  {
    return true;
  }
  else
  {
    return false;
  }
}

//getData
std::string Land::getData()
{
  std::string data="";
  //std::string data="Land\n";
  //location
  data+=("  "+fillString("Location:",20)+displayCoords(getX(), getY())+"\n");

  //land type
  data+=("  "+fillString("Type:",20)+getTypeString()+"\n");

  return data;
}
