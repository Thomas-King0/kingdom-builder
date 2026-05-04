/*Class Minor object code
 */

#include <iostream>
#include <ctime>

#include "minor.h"
#include "major.h"
#include "land.h"
#include "helpful.h"
#include "settings.h"


class Land; //forward declaration

//*******************************************************constructor*******************************************
Minor::Minor()
{
  setKeep(nullptr); //set the keep
  setProducing(false); //the minor does not start off setProducing
  markTime(); //mark the time so that the variable is not empty
}

//copy
void Minor::copy(Minor* minor_ptr)
{
  setType(minor_ptr->getType());
  markTime();
  setProducing(minor_ptr->getProducing());
  setKeep(minor_ptr->getKeep());
}

//**********************************setting/getting functions*****************************************


//setKeep
void Minor::setKeep(Major* major)
{
  //always allow for setting the nullptr as the keep
  if (major==nullptr)
  {
    keep=nullptr; //set the keep
  }

  //only allow resetting the keep if it is the nullptr. Otherwise the minor structure is claimed by two keeps
  if (keep==nullptr)
  {
    keep=major;
  }
  else
  {
    std::cout<<"tried setting a keep for a minor that already has a keep\n";
  }
}

//getKeep
Major* Minor::getKeep() //get the keep of the minor
{
  return keep;
}

//setProducing
void Minor::setProducing(bool tf) //set the production status
{
  producing=tf; //set producing
}

//getProducing
bool Minor::getProducing() //get the production status
{
  return producing;
}

//markTime
void Minor::markTime() //reset the start time of last production
{
  start_time=time(0);
}

//getTime
time_t Minor::getTime() //get the start time of the last production
{
  return start_time;
}


//**********************************************type & data functions****************************

//setType
void Minor::setType(Minor_Structure kind) //set what kind of minor it is
{
  minor_type=kind;
  /*no data validation happens because it would have to recieve a lot of information anyways about the area around
   * the structure. The game will check if a structure is valid in the main function before it instantiates it.
   */
}

//getType
Minor_Structure Minor::getType() //return the type of the minor
{
  return minor_type;
}

//getTypeString
std::string Minor::getTypeString() //return the type of the minor as a string
{
  switch (getType())
  {
    case (MINE):
    {
      return "mine";
      break;
    }
    case (QUARRY):
    {
      return "quarry";
      break;
    }
    case (SAWMILL):
    {
      return "sawmill";
      break;
    }
    case (PASTURE):
    {
      return "pasture";
      break;
    }
    case (GARDEN):
    {
      return "garden";
      break;
    }
    default:
    {
      return "unkown";
      break;
    }
  }
}

//getData
std::string Minor::getData() //get the data from the minor
{
  std::string data="";

  //type
  data+=("  "+fillString("Type:",20)+getTypeString()+"\n");

  //keep type
  //data+=("  "+fillString("Keep type:",20)+((getKeep()==nullptr)?"none":(getKeep()->getTypeString()))+"\n");

  //position
  data+=("  Attached to a keep at position ");
  if (getKeep()==nullptr)
  {
    data+="N/A";
  }
  else
  {
    Land *base_ptr=getKeep()->Major::getBase();
    data+=displayCoords(base_ptr->getX(), base_ptr->getY());
  }
  data+="\n";

  //production status
  data+="  "+fillString("Production status:", 20);
  if (getProducing())
  {
    data+=("producing");
  }
  else
  {
    data+=("not producing");
  }
  data+="\n";

  return data;
}

//**********************************producing functions**************************************


//startProduction
void Minor::startProduction() //try to start producing in the structure
{
  if (getProducing()==true) //if the minor is already producing
  {
    return; //stop the function
  }

  markTime(); //mark the time
  setProducing(true); //turn production on
}

/*
//endProduction
void Minor::endProduction() //try to end production in the minor
{}
/*this is virtual because all of the minors need to add different things in different amounts after different
*lengths of time
*/


//**********************************misc***********************************************

bool Minor::isValid(Minor_Structure kind, Major* keep_ptr)
{
  for (int i=0; i<keep_ptr->getClaimSize();i+=1)
  {
    Land* tile_ptr=keep_ptr->getClaim(i); //get the address of the ith claim of the major
    Land_type tile_kind=tile_ptr->getType(); //get the land type of the claim
    switch (kind)
    {
      case (MINE):
      {
        if ((tile_kind==FOREST)||(tile_kind==FIELDS)||(tile_kind==MOUNTAINS)) //if that claim is of the right type
        {
          return true;
        }
        else
        {
          break;
        }
      }
      case (QUARRY):
      {
        if ((tile_kind==FIELDS)||(tile_kind==MOUNTAINS)||(tile_kind==TUNDRA))
        {
          return true;
        }
        else
        {
          break;
        }
      }
      case (SAWMILL):
      {
        if (tile_kind==FOREST)
        {
          return true;
        }
        else
        {
          break;
        }
      }
      case (PASTURE):
      {
        if ((tile_kind==FIELDS)||(tile_kind==TUNDRA))
        {
          return true;
        }
        else
        {
          break;
        }
      }
      case (GARDEN):
      {
        if (tile_kind==FIELDS)
        {
          return true;
        }
        else
        {
          break;
        }
      }
      default:
      {
      }
    }
  }
  return false;
}
