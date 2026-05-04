/*Class Major for kingdom_builder
 * This is the base class for major structures
 */

#ifndef MAJOR_H
#define MAJOR_H

#include <iostream>
#include <vector>
#include "helpful.h"

class Land; //forward declaration
class Minor;
class Road;
class Caravan;
class Barbarian;

class Major
{
private:
  std::string name; //the name of the location
  Land* land; //pointer to the land that the structure sits in
  Barbarian* occupants; //pointer to the occupying barbarian tribe

  Major_Structure major_type;
  std::vector<Road*> highways; //pointers to all of the roads connected to the structure

  std::vector<Land*> claimed; //pointers to the other land claimed by the structure
  int land_limit; //the limit of how much claimed land a structure can have

  std::vector<Minor*> minor_structures; //stores the addresses of all minor structures attached to the major

  std::vector<int> population; //each element is the population of that particular trade
  int vacancy; //how many more people can be in this village

  std::vector<int> materials; //this vector stores how much of each material the major has
public:
  Major(); //constructor

  void copy(Major* major_ptr); //copy the contents of another major pointer (this is useful for loading saved data)

  //name functions=================================================
  void setName(std::string givenName);
  std::string getName();
  void changeName();

  //inventory functions
  void setMaterials(Material resource, int num); //set how much of a material a village has
  int getMaterials(Material resource); //get the how much of a material a village has
  int getMaterials(int resource); //get how much of a material a village has

  //type functions=================================================
  void setType(Major_Structure kind); //set the type of major
  Major_Structure getType(); //get the type of major
  std::string getTypeString(); //get the type of major but return it as a string

  //base functions====================================================
  void setBase(Land* base); //set the base tile
  Land* getBase(); //get the address of the base tile
  static bool isValid(Land* base); //check if the land tile is a valid base tile

  //position functions==================================================
  int getX(); //get the x coordinate
  int getY(); //get the y coordinate

  double dist(int x, int y); //get the distance between the structure and input coordinates

  //claim functions=================================================
  void addClaim(Land* claim); //add a claimed tile
  //need to make this so that you can only add adjacent tiles

  Land* getClaim(int num); //get the address of a claimed tile
  int getClaimSize(); //get the number of claims

  bool canClaim(Land* potential); //return if the major can claim the input land tile

  //limit functions===================================================
  void setLimit(int num); //set the land limit of the structure
  int getLimit(); //get the land limit of the structure

  //population functions=============================================
  void setPopulation(Career trade, int num); //set the population of a particular trade
  int getPopulation(Career trade); //get the population of a particular trade
  int getPopluation(Career trade); //this is because I often make this mistake
  virtual void trainTownsfolk(); //train the trainTownsfolk
  void addTownsfolk();

  virtual void setVacancy(int num); //set how much room the structure has left
  int getVacancy(); //get how much room the structure has left

  //support functions================================================
  virtual void addSupport(Minor* support); //add a minor structure to the list
  Minor* getSupport(int num); //get the address of a support structure at the specified index
  int getSupportSize(); //get the number of supports

  //production functions
  bool canProduce(); //return whether the major can produce
  void startProduction(); //start producing at all minor structures that can
  void endProduction(); //try to end production in finished supports

  //virtual void setMaterials(Material resource, int num)=0; //set how much of a material a major has
  //virtual int getMaterials(Material resource)=0; //get the how much of a material a major has
  //virtual int getMaterials(int resource)=0;
  //overload the getMaterials function so it can handle either an int or a Material input

  //road and caravan functions======================================================
  void addRoad(Road* road);
  Road* getRoad(int num);
  Road* getRoad(std::string name);
  int getHighwaySize();
  Major* getSister(int index);
  Major* getSister(std::string name);
  std::string getSisterList();
  bool isConnected(std::string name);
  bool isConnected(Major* major_ptr);

  void load(); //load a caravan
  bool packageCaravan(Caravan*); //take the contents from the major and add them to the caravan
  void unload(Caravan* haul); //unload a caravan

  //barbarian functions===============================================================
  void setOccupants(Barbarian* tribe); //set the tribe that is occupying the structure
  Barbarian* getOccupants(); //get the occupying tribe
  bool isSurrounded(); //return whether the structure has any sister cities that are unoccupied
  bool isOccupied(); //return whether the structure is occupied

  //combat functions======================================
  int attack(Major* target_ptr); //attack the input land tile
  int getWarriors(); //get the soldiers who are going to fight in a battle
  void setAftermath(int num);
  virtual double getDefense(); //get the defense value for a stsructure

  //miscellaneous functions
  virtual std::string getData(); //return the data contained in the major and all its supports and land as a string

  ~Major(); //destructor
};

#endif
