/*road header file
 */
#ifndef ROAD_H
#define ROAD_H

#include <iostream>
#include <ctime>

class Major; //foraward declaration
class Caravan;

class Road
{
private:
  Major* destination1; //one side of the road
  Major* destination2; //the other end of the road

  double length; //distance between the two endpoints
  time_t start_time; //the time of the last transportation

  bool transporting; //whether the road is in use or not
  Caravan* caravan; //pointer to the caravan on the road


public:
  Road(Major* dest1, Major* dest2); //constructor

  Major* getDestination(int num); //return the designated destination
  std::string getDestinationName(int num); //get the name of the designated destination

  double getLength(); //get the distance of the road

  time_t getStartTime(); //get the start time
  void setStartTime(); //reset the start time

  bool isTransporting(); //get the state of the road
  void setTransporting(bool tf); //set the state of the road

  Caravan* getCaravan(); //get the caravan
  void setCaravan(Caravan* caravan_ptr); //set the caravan

  //void startJourney(int caravan_contents, int num, bool caravan_kind, Major* return_label); //start sending a caravan
  void startJourney();
  void endJourney(); //check if the journey is over (are we there yet?)
  //void unload(); //actually add the contents to the destination

  std::string getData(); //get the road data

};

#endif
