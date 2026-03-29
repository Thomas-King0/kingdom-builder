/* class Road object file
 */

#include <iostream>
#include <ctime>
#include <cstdio>

#include "road.h"
#include "major.h"
#include "caravan.h"
#include "helpful.h"
#include "settings.h"


//constructor
Road::Road(Major* dest1, Major* dest2)
{
  /*destinations 1 and 2 are set with no data validation because they have no setters. So if they are not initialized
   * properly right away they cannot be changed. So data validation needs to happen before the road is constructed or
   * else a possibly misconstructed road could take up space in a major's road list.
   */
  destination1=dest1;
  destination2=dest2;

  dest1->addRoad(this);
  dest2->addRoad(this);

  length=dist(dest1, dest2);

  setStartTime(); //set the start time
  setTransporting(false); //set the state of the road
  //setIsMaterials(false); //the default state of this will be false
  //setContents(0); //there is no null material so this is set to 0 by default
  //setAmount(0); //set the amount
}

//getDestination
Major* Road::getDestination(int num)
{
  if (num==1)
  {
    return destination1;
  }
  else if (num==2)
  {
    return destination2;
  }
  else
  {
    std::cout<<"Invalid destination: ["<<num<<"]\n";
    return nullptr;
  }
}

//getDestinationName
std::string Road::getDestinationName(int num)
{
  if (num==1)
  {
    return destination1->getName();
  }
  else if (num==2)
  {
    return destination2->getName();
  }
  else
  {
    std::cout<<"Invalid destination: ["<<num<<"]\n";
    return "";
  }
}

//getLength
double Road::getLength()
{
  return dist(getDestination(1), getDestination(2));
}

//getStartTime
time_t Road::getStartTime()
{
  return start_time;
}

//setStartTime
void Road::setStartTime()
{
  start_time=time(0);
}

//isTransporting
bool Road::isTransporting()
{
  return transporting;
}


//setTransporting
void Road::setTransporting(bool tf)
{
  transporting=tf;
}

//getCaravan
Caravan* Road::getCaravan()
{
  return caravan;
}

//setCaravan
void Road::setCaravan(Caravan* caravan_ptr)
{
  caravan=caravan_ptr;
}

//startJourney
void Road::startJourney()
{
  if (getCaravan()==nullptr)
  {
    std::cout<<"tried to start a journey with no caravan\n";
    return;
  }

  if (isTransporting())
  {
    std::cout<<"tried to start a second journey on a road that was already transporting a caravan\n";
    return;
  }

  setStartTime();
  setTransporting(true);
}

//endJourney
void Road::endJourney()
{
  if (!isTransporting())
  {
    return;
  }

  time_t now=time(0);
  if (now-getStartTime()>(ROAD_COEFF*getLength()))
  //ROAD_COEFF times the length of the road gives the time needed to elapse
  {
    Major* destination=getCaravan()->getDestination();
    if (!(destination->isOccupied()))
    {
      destination->unload(getCaravan());
    }
    else
    {
      std::cout<<"could not unload caravan at "<<destination->getName()<<" due to barbarian occupation\n";
    }
    setTransporting(false);
    setCaravan(nullptr);
  }
}

//getData
std::string Road::getData()
{
  
  std::string data="";

  //distance between endpoints
  data+=("  "+fillString("Road length:",20)+std::to_string(getLength())+"\n");

  //how long a journey takes
  data+=("  "+fillString("Time to transport:",20)+int2string(static_cast<int>(ROAD_COEFF*getLength()+1))+"\n");
  //the +1 is there so that the user gets the "rounded up" time

  //whether the road is transporting
  data+=("  "+fillString("In use:",20));

  //if the road is transporting
  if (isTransporting())
  {
    data+="yes\n";
    //when did the caravan start its journey
    time_t now=time(0);
    //data+=("  "+fillString("Time since journey began:",20)+int2string(static_cast<int>(now-getStartTime()))+"\n");

    //when will it end
    data+=("  "+fillString("Time left in journey:",20)+int2string(static_cast<int>(ROAD_COEFF*getLength()-(now-getStartTime())+1))+"\n");

    //what is in the caravan
    data+=("\n"+getCaravan()->getData());
  }
  else
  {
    data+="no\n";
  }

  return data;
}
