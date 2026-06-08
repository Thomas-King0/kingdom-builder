//object file for class ObjectWriter

#include <iostream>
#include <fstream>
#include <string>

#include "objectWriter.h"
#include "helpful.h"
#include "land.h"
#include "major.h"
#include "minor.h"
#include "road.h"
#include "caravan.h"
#include "barbarian.h"
#include "castle.h"
#include "village.h"
#include "fort.h"
#include "garden.h"
#include "pasture.h"
#include "sawmill.h"
#include "quarry.h"
#include "mine.h"
#include "settings.h"


//default constructor
ObjectWriter::ObjectWriter()
{
  setFile(nullptr);
}

//constructor with file pointer
ObjectWriter::ObjectWriter(std::fstream* file_ptr)
{
  setFile(file_ptr);
}

//setFile
void ObjectWriter::setFile(std::fstream* file_ptr)
{
  file=file_ptr;
}

//getFile
std::fstream* ObjectWriter::getFile()
{
  return file;
}

//write(string)
void ObjectWriter::write(std::string str)
{
  std::fstream* file_ptr=getFile(); //get the file
  int length=str.size(); //get the size of the string

  file_ptr->write(reinterpret_cast<const char *>(&length), sizeof(int));
  for (int i=0; i<length; i+=1)
  {
    char current=str.at(i);
    file_ptr->write(reinterpret_cast<const char *>(&current), sizeof(char));
  }
}

//readString
std::string ObjectWriter::readString()
{
  std::fstream* file_ptr=getFile(); //get the file
  int length;
  std::string str;
  file_ptr->read(reinterpret_cast<char *>(&length), sizeof(int)); //read the length of the string
  for (int i=0; i<length; i+=1)
  {
    char current;
    file_ptr->read(reinterpret_cast<char *>(&current), sizeof(char)); //read the current character
    str=str+current;
  }
  return str;
}

//write (land)
void ObjectWriter::write(Land* land_tile)
{
/*
Land objects are written in the following order:
x-position (int)
y-position (int)
land type (enumerated type)
//exploration status (bool) actually this will not be stored as it is stored in the type
keep name (string)
*/
  //data contained in the land tile
  int positionX=land_tile->getX(); //position of the land tile in X (c_cen)
  int positionY=land_tile->getY(); //position of the land tile in Y (c_cen)
  std::string keep_name;
  Major* keep=land_tile->getKeep(); //pointer to the keep (governing structure)
  if (keep!=nullptr)
  {
    keep_name=keep->getName(); //get the name of the keep
  }
  else
  {
    keep_name="";
  }
  Land_type type=land_tile->getType(); //what type of land it is
  //bool explored=land_tile->isExplored(); //boolean containing whether or not the tile is visible

  //get the file
  std::fstream* file_ptr=getFile();

  int id=LAND_ID;
  file_ptr->write(reinterpret_cast<const char *>(&id), sizeof(int)); //write id number
  file_ptr->write(reinterpret_cast<const char *>(&positionX), sizeof(int)); //write x-position
  file_ptr->write(reinterpret_cast<const char *>(&positionY), sizeof(int)); //write y-position
  file_ptr->write(reinterpret_cast<const char *>(&type), sizeof(Land_type)); //write land type
  //file_ptr->write(reinterpret_cast<const char *>(&explored), sizeof(bool)); //write exploration status
  write(keep_name); //write the name as a string
}

//readLand
Land* ObjectWriter::readLand(std::string* name)
{
  std::fstream* file_ptr=getFile();
  int pos_x;
  int pos_y;
  Land_type type;
  //bool explored;
  file_ptr->read(reinterpret_cast< char *>(&pos_x), sizeof(int)); //read x-position
  file_ptr->read(reinterpret_cast< char *>(&pos_y), sizeof(int)); //read y-position
  file_ptr->read(reinterpret_cast< char *>(&type), sizeof(Land_type)); //read land type
  //file_ptr->read(reinterpret_cast< char *>(&explored), sizeof(bool)); //read exploration status
  *name=readString(); //read the name

  //std::cout<<"x: "<<pos_x<<"\ny: "<<pos_y<<"\n"<<"type: "<<type<<"\n"<<(explored?"explored":"not explored")<<"\n";
  Land* land=new Land();
  land->setX(pos_x);
  land->setY(pos_y);
  land->setType(type);
  //land->setExplored(explored);
  return land;
}

//write (barbarian)
void ObjectWriter::write(Barbarian* tribe)
{
/*
Barbarian objects are written in the following order:
population number (int)
keep name (string)
*/
  //data contained in the land tile
  int population=tribe->getPopulation();
  std::string keep_name;
  Major* keep=tribe->getKeep(); //pointer to the keep (governing structure)
  if (keep!=nullptr)
  {
    keep_name=keep->getName(); //get the name of the keep
  }
  else
  {
    keep_name="";
  }

  //get the file
  std::fstream* file_ptr=getFile();

  int id=BARBARIAN_ID;
  file_ptr->write(reinterpret_cast<const char *>(&id), sizeof(int)); //write id number
  file_ptr->write(reinterpret_cast<const char *>(&population), sizeof(int)); //write x-position
  write(keep_name); //write the name as a string
}

//readBarbarian
Barbarian* ObjectWriter::readBarbarian(std::string* keep_name)
{
  std::fstream* file_ptr=getFile();
  int population;
  file_ptr->read(reinterpret_cast<char *>(&population), sizeof(int)); //read the population
  *keep_name=readString();
  Barbarian* tribe=new Barbarian();
  tribe->setPopulation(population);
  return tribe;
    
}

//write (major)
void ObjectWriter::write(Major* major_ptr)
{
/*
Major objects are  written in the following order:
name (string)
x-position (int)
y-position (int)
major type (enumerated type)
population (vector<int>)
vacancy (int)
materials (vector<int>)

Note: majors do not store their own position. That will be stored from the base land tile. The read function will take a pointer to a land tile and set that
tile's position equal to the major's position. After all data has been read from memory the main program will then go through and link up all objects to their
appropriate major.
*/

  //data contained in the major
  std::string name=major_ptr->getName();
  int positionX=major_ptr->getX(); //position of the major
  int positionY=major_ptr->getY(); //position of the major
  Major_Structure type=major_ptr->getType(); //what type of major it is
  std::vector<int> people;
  for (int i=FARMER; i<=TOWNSFOLK; i+=1)
  {
    people.push_back(major_ptr->getPopulation(static_cast<Career>(i)));
  }
  int vacancy=major_ptr->getVacancy();
  std::vector<int> inventory;
  for (int i=PRODUCE; i<VACUUM; i+=1)
  {
    inventory.push_back(major_ptr->getMaterials(static_cast<Material>(i)));
  }

  //get the file
  std::fstream* file_ptr=getFile();

  int id=MAJOR_ID;
  file_ptr->write(reinterpret_cast<const char *>(&id), sizeof(int)); //write id number
  write(name); //write the name of the major
  file_ptr->write(reinterpret_cast<const char *>(&positionX), sizeof(int)); //write x-position
  file_ptr->write(reinterpret_cast<const char *>(&positionY), sizeof(int)); //write y-position
  file_ptr->write(reinterpret_cast<const char *>(&type), sizeof(Major_Structure)); //write land type
  int length=people.size();
  file_ptr->write(reinterpret_cast<const char *>(&length), sizeof(int)); //the length of the vector (technically known but this seems like good practice)
  for (int i=0; i<length; i+=1)
  {
    int current=people.at(i);
    file_ptr->write(reinterpret_cast<const char *>(&current), sizeof(int)); //write the population data
  }
  file_ptr->write(reinterpret_cast<const char *>(&vacancy), sizeof(int)); //write the vacancy
  length=inventory.size();
  file_ptr->write(reinterpret_cast<const char *>(&length), sizeof(int)); //the length of the vector (again)
  for (int i=0; i<length; i+=1)
  {
    int current=inventory.at(i);
    file_ptr->write(reinterpret_cast<const char *>(&current), sizeof(int)); //write the population data
  }
}

//readMajor
Major* ObjectWriter::readMajor(Land* land_ptr)
{
  //data contained in the major
  std::string name;
  int positionX;
  int positionY;
  Major_Structure type; //what type of major it is
  std::vector<int> people;
  int vacancy;
  std::vector<int> inventory;

  //get the file
  std::fstream* file_ptr=getFile();
  name=readString(); //write the name of the major
  file_ptr->read(reinterpret_cast<char *>(&positionX), sizeof(int)); //write x-position
  file_ptr->read(reinterpret_cast<char *>(&positionY), sizeof(int)); //write y-position
  file_ptr->read(reinterpret_cast<char *>(&type), sizeof(Major_Structure)); //write land type
  int length;
  file_ptr->read(reinterpret_cast<char *>(&length), sizeof(int)); //the length of the vector (technically known but this seems like good practice)
  for (int i=0; i<length; i+=1)
  {
    int current;
    file_ptr->read(reinterpret_cast<char *>(&current), sizeof(int)); //write the population data
    people.push_back(current);
  }
  //std::cout<<"file pointer position7: "<<file_ptr->tellg()<<"\n";
  file_ptr->read(reinterpret_cast<char *>(&vacancy), sizeof(int)); //write the vacancy
  file_ptr->read(reinterpret_cast<char *>(&length), sizeof(int)); //the length of the vector (again)
  for (int i=0; i<length; i+=1)
  {
    int current;
    file_ptr->read(reinterpret_cast<char *>(&current), sizeof(int)); //write the population data
    inventory.push_back(current);
  }

  //create new major
  Major* major_ptr=nullptr;
  switch (type)
  {
    case (CASTLE):
    {
      major_ptr=new Castle();
      break;
    }
    case (VILLAGE):
    {
      major_ptr=new Village();
      break;
    }
    case (FORT):
    {
      major_ptr=new Fort();
      break;
    }
    default:
    {
      std::cout<<"unrecognized id when reading Major object\n";
      major_ptr=new Major();
    }
  }
  major_ptr->setName(name);
  for (int i=FARMER; i<=TOWNSFOLK; i+=1)
  {
    int current=people.at(i);
    major_ptr->setPopulation(static_cast<Career>(i), current);
  }
  major_ptr->setVacancy(vacancy);
  for (int i=PRODUCE; i<VACUUM; i+=1)
  {
    int current=inventory.at(i);
    major_ptr->setMaterials(static_cast<Material>(i), current);
  }
  land_ptr->setX(positionX);
  land_ptr->setY(positionY);

  return major_ptr;
}

//write (minor)
void ObjectWriter::write(Minor* minor_ptr)
{
  /*
  minors are written in the following order:
  type (enumerated)
  //time (time_t) //actually I may not write this since I don't have a function to set the start time to whatever I want
                  //Also I like the idea that if you save in the middle of production then the production has to start over
  production status (bool)
  keep name (string) 
  */

  //get minor data
  Minor_Structure type=minor_ptr->getType();
  //time_t start=minor_ptr->getTime();
  bool producing=minor_ptr->getProducing();
  std::string name=minor_ptr->getKeep()->getName();

  //get file writer
  std::fstream* file_ptr=getFile();

  int id=MINOR_ID;

  file_ptr->write(reinterpret_cast<const char*>(&id), sizeof(int)); //write the id number
  file_ptr->write(reinterpret_cast<const char*>(&type), sizeof(Minor_Structure)); //write the minor type
  //file_ptr->write(reinterpret_cast<const char*>(&start), sizeof(time_t)); //write the time since last production
  file_ptr->write(reinterpret_cast<const char*>(&producing), sizeof(bool)); //write the production status
  write(name); //write the name of the keep
}

//readMinor
Minor* ObjectWriter::readMinor(std::string* keep_name)
{
  //minor data
  Minor_Structure type;
  //time_t start;
  bool producing;
  
  std::fstream* file_ptr=getFile();

  //read data
  file_ptr->read(reinterpret_cast<char *>(&type), sizeof(Minor_Structure)); //read the type
  //file_ptr->read(reinterpret_cast<char *>(&start), sizeof(time_t)); //read the type
  file_ptr->read(reinterpret_cast<char *>(&producing), sizeof(bool)); //read the type
  *keep_name=readString();

  //initialize new minor
  Minor* minor_ptr;
  switch (type)
  {
    case (GARDEN):
    {
      minor_ptr=new Garden();
      break;
    }
    case (PASTURE):
    {
      minor_ptr=new Pasture();
      break;
    }
    case (SAWMILL):
    {
      minor_ptr=new Sawmill();
      break;
    }
    case (QUARRY):
    {
      minor_ptr=new Quarry();
      break;
    }
    case (MINE):
    {
      minor_ptr=new Mine();
      break;
    }
  }
  //minor_ptr->setType(type); I don't need this line now that I have the above switch statement
  minor_ptr->setProducing(producing);

  return minor_ptr;  
}

//write (road)
void ObjectWriter::write(Road* road_ptr)
{
/*
Roads will be written in the following order
destination1 (string)
destination2 (string)
transportation status (bool)
caravan (caravan) [if applicable]
*/

  //get data from road
  std::string dest1=road_ptr->getDestinationName(1);
  std::string dest2=road_ptr->getDestinationName(2);
  bool transporting=road_ptr->isTransporting();
  Caravan* caravan_ptr=road_ptr->getCaravan();

  //get file pointer
  std::fstream* file_ptr=getFile();

  int id=ROAD_ID;
  //write data
  file_ptr->write(reinterpret_cast<const char *>(&id), sizeof(int)); //write the id
  write(dest1); //first destination name
  write(dest2); //second destination name
  file_ptr->write(reinterpret_cast<const char *>(&transporting),sizeof(bool)); //transportation status
  if (caravan_ptr!=nullptr) //do not write the caravan if there is nothing to write
  {
    write(caravan_ptr);
  }
}

//readRoad
Road* ObjectWriter::readRoad(std::string* dest1, std::string* dest2, std::string* caravan_dest)
{
  bool transporting;
  Caravan* caravan_ptr=nullptr;

  //get file pointer
  std::fstream* file_ptr=getFile();

  //read data
  *dest1=readString(); //first destination
  *dest2=readString(); //second destination
  file_ptr->read(reinterpret_cast<char *>(&transporting), sizeof(bool)); //transportation status
  if (transporting)
  {
    //std::cout<<"reading caravan\n";
    int data_id;
    file_ptr->read(reinterpret_cast<char *>(&data_id), sizeof(int)); //read the caravan id
    if (data_id!=CARAVAN_ID)
    {
      std::cout<<"error reading caravan in readRoad\n";
    }
    else
    {
      caravan_ptr=readCaravan(caravan_dest); 
    }
  } 

  //initialize road
  Road* road_ptr=new Road();
  road_ptr->setTransporting(transporting);
  road_ptr->setCaravan(caravan_ptr);
  return road_ptr;
}
//write (caravan)
void ObjectWriter::write(Caravan* caravan_ptr)
{
/*
Caravans will be written in the following way:
substance [materials or people] (bool)
type (int)
amount (int)
destination (string)
*/

  //get data from caravan
  bool isMaterials=caravan_ptr->isMaterials();
  int contents=caravan_ptr->getContents();
  int amount=caravan_ptr->getAmount();
  std::string destination=caravan_ptr->getDestination()->getName();

  //get the file pointer
  std::fstream* file_ptr=getFile();

  int id=CARAVAN_ID;
  //write the data
  file_ptr->write(reinterpret_cast<const char *>(&id), sizeof(int)); //write the id number
  file_ptr->write(reinterpret_cast<const char *>(&isMaterials), sizeof(bool)); //write the substance
  file_ptr->write(reinterpret_cast<const char *>(&contents), sizeof(int)); //write the contents
  file_ptr->write(reinterpret_cast<const char *>(&amount), sizeof(int)); //write the amount
  write(destination);
}

//readCaravan
Caravan* ObjectWriter::readCaravan(std::string* destination_name)
{
  //get data from caravan
  bool isMaterials;
  int contents;
  int amount;

  //get the file pointer
  std::fstream* file_ptr=getFile();

  int id;
  //read the data
/*
  file_ptr->read(reinterpret_cast<char *>(&id), sizeof(int)); //read the id number
  if (id!=CARAVAN_ID)
  {
    std::cout<<"error in reading caravan: incorrect id number\n";
    return nullptr;
  }
*/

  file_ptr->read(reinterpret_cast<char *>(&isMaterials), sizeof(bool)); //read the substance
  file_ptr->read(reinterpret_cast<char *>(&contents), sizeof(int)); //read the contents
  file_ptr->read(reinterpret_cast<char *>(&amount), sizeof(int)); //read the amount
  *destination_name=readString();  

  Caravan* caravan_ptr=new Caravan();
  caravan_ptr->setIsMaterials(isMaterials);
  caravan_ptr->setContents(contents);
  caravan_ptr->setAmount(amount);
  return caravan_ptr;
}
