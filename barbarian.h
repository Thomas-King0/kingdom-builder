/* Barbarian header file
 */

#ifndef BARBARIAN_H
#define BARBARIAN_H

#include<iostream>
#include<ctime>

class Major;
class Land;

class Barbarian
{
private:
  Major* keep; //the structure that the tribe has taken
  int population; //the number of barbarians
  //time_t elapsed_time; //the time since the barbarians last attacked

public:
  Barbarian(); //default constructor

  Barbarian(Major* major_ptr); //constructor with a keep

  void setKeep(Major* keep_ptr); //set the structure they have occupied
  Major* getKeep(); //get the occupied structure

  Land* getBase(); //get the base tile of the keep

  void setPopulation(int num); //set the population
  int getPopulation(); //get the current population

  void increasePopulation(); //increase the population
  void pillage(); //decrease the host structure of its resources
  Major* getTarget(); //get the next target

  static double spread_chance; //the probability that a tribe will spread
  static void increaseSpreadChance(); //increase the chance of barbarians spreading
  static int tribe_count; //the number of barbarian tribes currently in play

};

#endif
