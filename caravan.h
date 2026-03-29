/* Caravan header file
 */

#ifndef CARAVAN_H
#define CARAVAN_H

#include <string>

class Major;
//class Road;

class Caravan
{
private:

  Major* destination; //pointer to the destination
  bool is_materials; //is the caravan people or materials
  int contents; //contains the integer corresponding to the trade or material
  int amount; //the amount in the caravan

public:

  Caravan(Major* destination_ptr);

  bool isMaterials(); //get the type of caravan (sending people or materials)
  void setIsMaterials(bool tf); //set the type of caravan

  Major* getDestination(); //get the direction
  void setDestination(Major* major_ptr); //set the direction

  int getContents(); //get the contents of the caravan
  void setContents(int num); //set what the caravan is carrying

  int getAmount(); //get how much the caravan is carrying
  void setAmount(int num); //set how much the carvan is carrying

  std::string getData(); //get the data for the caravan
};


#endif
