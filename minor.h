/*Class Minor for kingdom builder
 * This is the base class for minor structures
 */

#ifndef MINOR_H
#define MINOR_H


#include <iostream>
#include <ctime>
#include "helpful.h"

class Major; //forward declaration


class Minor
{
private:

  Minor_Structure minor_type; //type of structure
  Major* keep; //the keep of the minor structure
  time_t start_time; //the time that production last started
  bool producing; //is the structure producing

public:
  Minor(); //constructor
  
  void copy(Minor* minor_ptr); //copy the data of another minor

  void setKeep(Major* keep); //set the keep for the minor
  Major* getKeep(); //get the address of the keep

  void setProducing(bool tf); //set whether or not the structure is producing
  bool getProducing(); //get the state of the minor

  void markTime(); //set the current time as start_time
  time_t getTime(); //get the time that production last started

  void setType(Minor_Structure kind); //set which type of minor structure the minor is
  Minor_Structure getType(); //get which type of minor structure the minor is
  std::string getTypeString(); //get which type the minor is but return it as a string

  std::string getData(); //get all the data about the minor

  void startProduction(); //try to start the production process
  virtual void endProduction()=0;

  static bool isValid(Minor_Structure kind, Major* keep_ptr);
  //check if the structure can be built on the designated keep

};

#endif
