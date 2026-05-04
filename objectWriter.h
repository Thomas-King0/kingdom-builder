//header file for objectWriter class
//this allows the program to easily write objects to the save file

#ifndef OBJECT_WRITER
#define OBJECT_WRITER

#include <iostream>
#include <fstream>
#include <string>

#include "helpful.h"
#include "land.h"
#include "major.h"
#include "minor.h"
#include "barbarian.h"
#include "settings.h"

/*
Object id numbers:
1. land
2. barbarian
3. major
4. minor
5. road
*/

class ObjectWriter
{
private:
  std::fstream* file;

public:
  ObjectWriter(); //default constructor
  ObjectWriter(std::fstream* file_ptr); //constructor with a file attached

  void setFile(std::fstream* file_ptr);
  std::fstream* getFile();
  
  //string functions
  void write(std::string str); //write a string to save
  std::string readString(); //read a string

  //land file functions
  void write(Land* land_tile); //write a land tile to save
  Land* readLand(std::string* name); //read the land data 

  //barbarian file functions
  void write(Barbarian* tribe); //write a barbarian to save
  Barbarian* readBarbarian(std::string* keep_name); //read the barbarian data

  //major file functions
  void write(Major* major_ptr); //write any major to save
  Major* readMajor(Land* land_ptr); //read the major data

  //minor file functions
  void write(Minor* minor_ptr); //write a minor to save
  Minor* readMinor(std::string* keep_name); //read minor data

  //road file functions
  void write(Road* road_ptr); //write a road to save
  Road* readRoad(std::string* dest1, std::string* dest2); //read road data

/*
  //caravan file functions
  void write(Road* road_ptr); //write a caravan to save (via a road)
  Caravan* readCaravan(std::string* origin, std::string* dest); //read a caravan from memory
*/
};
#endif
