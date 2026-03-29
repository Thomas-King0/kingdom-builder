
//helpful header file
#ifndef HELPFUL_H
#define HELPFUL_H

//settings
#define MAP_SIZE 1000 //the max x or y coordinate
#define DISPLAY_SIZE 20 //the size of the display when displaying the map
#define BUILD_DISTANCE 7 //a tile must be within this distance to a structure in order to build on it
#define ROAD_COEFF 3 //how many seconds each unit of distance adds to transportaion time
#define EXPLORE_LIMIT 5
#define BIRTH_RATE 0.16
#define BIRTH_FREQUENCY 10 //number out of 100 that causes an increase of townsfolk

//land settings
#define FOREST_GEN 5.0 //forest generation number
#define FIELD_GEN 4.0 //fields generation number
#define MOUNTAIN_GEN 3.5 //mountains generation number
#define TUNDRA_GEN 3.0 //tundra generation number
#define SEA_GEN 1.0 //sea generation number

#define BIOME_CHANGE 0.08 //chance to change biomes

//combat settings
#define KILL_CHANCE 0.10 //the chance of a kill in battle

//minor structure constants
#define MINE_PRODUCTION 60 //How long it takes a mine to produce
#define QUARRY_PRODUCTION 45 //how long it takes for a quarry to produce
#define SAWMILL_PRODUCTION 30 //how many seconds it takes a sawmill to produce
#define PASTURE_PRODUCTION 15 //how long it takes a pasture to produce
#define GARDEN_PRODUCTION 5 //how long it takes a garden to produce

//village constants
#define VILLAGE_CAPACITY 50 //this is the total capacity of the village
#define VILLAGE_LIMIT 3 //this is the land limit for a village
#define VILLAGE_DEFENSE 0.0 //village defense ability

//castle constants
#define CASTLE_CAPACITY 300
#define CASTLE_LIMIT 11
#define CASTLE_DEFENSE 0.04

//fort constants
#define FORT_CAPACITY 50
#define FORT_LIMIT 0
#define FORT_DEFENSE 0.02

//barbarian constants
#define PILLAGE_FREQUENCY 4 //5% chance of a tribe pillaging (0-4: pillages 5-99 doesn't)
#define MAX_PILLAGE 20 //the most that a tribe can take when pillaging
#define SPREAD_INCREASE_RATE 0.25 //the amount the the chance of spreading increases
#define BARBARIAN_THRESHOLD 10 //the minimum number of people needed for a barbarian tribe to attack

//calculated constants

#include <iostream>
#include <string>
#include <array>
#include <vector>


class Land; //forward declaration
class Major;
//class Village;

enum Career
{
  FARMER,
  LUMBERJACK,
  MINER,
  SHEPHERD,
  STONE_MASON,
  SOLDIER,
  TOWNSFOLK
};

enum Material
{
  PRODUCE,
  ANIMALS,
  WOOD,
  STONE,
  METAL,
  VACUUM
};

enum Major_Structure
{
  CASTLE,
  VILLAGE,
  FORT,
  PORT
};

enum Minor_Structure
{
  MINE,
  QUARRY,
  SAWMILL,
  PASTURE,
  GARDEN
};

enum Land_type
{
  FOREST, //30%
  FIELDS, //25%
  MOUNTAINS, //25%
  TUNDRA, //15%
  SEA, //5%
  SPACE //this is essentially a "null" land type
};

enum Direction
{
  NORTH,
  EAST,
  SOUTH,
  WEST
};

std::string newline(int num); //newline
void clearBuffer(); //clear the buffer (doesn't work)
int absolute(int num); //return the aboslute value of the input
int power(int base, int exponent); //return the value base^exponent
std::string int2string(int num); //convert an integer to a string
//std::string dec2string(double num); //convert a decimal number to a string
std::string digit2string(int digit); //convert a single digit to a string
std::string material2string(Material resource); //convert material to string
std::string career2string(Career trade);
std::string fillString(std::string str, int length); //fill a string to be the required length
bool isElement(char elmt, std::string str); //check if the input character is in the input string
void getName(std::string* name); //get the name of a structure
int* getInt(); //more robust way to get an integer
bool isNum(std::string str); //check if a string is a number
int char2digit(char str); //convert a string to a digit
int string2num(std::string str); //convert a string to a number
void pause(int duration); //pause for the number of seconds indicated

//kingdom_builder functions

//build functions
void buildMajor(Major_Structure kind, Land* tile_ptr, std::vector<Major*>* structure_vector);
//check that the associated major can be built at that location
bool canBuildMinor(Minor_Structure kind, Major* keep_ptr); //check that the associated minor can be built at that location
bool getMaterials(Major* major_ptr, Major_Structure kind);
bool getMaterials(Minor_Structure kind, Major* keep_ptr);
bool build(std::vector<int> costs, Major* major_ptr);
bool checkValid(Major_Structure kind, Land* tile_ptr);
Major* newMajor(Major_Structure kind);

//explore functions
void explore(std::vector<Land>* land_vector, int index, Direction direction, int level);
//input the address of a land tile and its index in the land array
Major* closestStructure(int coord_x, int coord_y, std::vector<Major*>* major_ptr);
Land_type getExploredType(Land* tile_ptr);

//map functions
int pos2index(int x, int y); //take c_cen coordinates as inputs and output the index in the land array
int index2pos(int index, char axis);
void displayMap(int center_x, int center_y, std::vector< Land>* map);
std::string getLandChar(Land_type tile_type); //get the map character for a land tile
std::string getStructureChar(Major_Structure keep_kind); //get the map character for a major structure
void displayBorder(); //display the upper and lower map borders

//name functions
Major* getStructure(std::string name, std::vector<Major*>* major_ptr); //get a structure based on its name
std::vector<std::string> listStructures(std::vector<Major*>* major_ptr); //get the list of structure names

//combat and barbarian functions
int getHits(int warriors, double defense); //get the number of hits given a defense level
int getHits(int warriors); //get the number of hits with no defense level
Major* furthestStructure(std::vector<Major*>* major_ptr); //get the furthest structure from the center castle

//other functions (for now)
void space(int num); //input a number of spaces to display
void getCoords(int* x_coord, int* y_coord);
std::string displayCoords(int x_coord, int y_coord);
double dist(int x1, int y1, int x2, int y2);
double dist(Major* major1, Major* major2);
double dist(Land* land1, Land* land2);
//void seedRandom(Major* major_ptr1, Major* major_ptr2, int add); //seed the random number generator with an add value
//void seedRandom(Major* major_ptr1, Major* major_ptr2); //seed the random number generator with two majors
//void seedRandom(Major* major_ptr, int add); //seed the random number generator with an add value
//void seedRandom(Major* major_ptr); //seed the random number generator with one major

#endif
