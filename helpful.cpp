//function definitions for "helpful"
#include <cmath>
#include <ctime>
#include <climits>
#include <limits>

#include "helpful.h"
#include "settings.h"
#include "land.h"
#include "major.h"
#include "minor.h"
#include "village.h"
#include "castle.h"
#include "fort.h"

//newline
std::string newline(int num)
{
  std::string new_string;
  for (int i=0; i<num; i+=1)
  {
    new_string.append("\n");
  }

  return new_string;
}

//clearBuffer
void clearBuffer()
{
  if (std::cin.peek()!=EOF)
  {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
}

//absolute
int absolute(int num)
{
  if (num>=0)
  {
    return static_cast<int>(num); //return number as an integer
  }
  else
  {
    return static_cast<int>(-1*num); //number is negative so multiply by -1 to make it positive
  }
}

//power
int power(int base, int exponent)
{
  if (exponent==0)
  {
    return 1;
  }

  int result=base;
  for (int i=1;i<exponent; i+=1)
  {
    result*=base;
  }

  return result;
}

//int2string
std::string int2string(int num)
{
  std::string result;
  if (num==0)
  {
    return "0";
  }
  else if (num<0) //if the number is negative
  {
    result+="-"; //add a negative sign
  }

  num=absolute(num); //take the absolute value to make things simpler for later

  //approximate the log(num) [this finds how many digits are in the number]
  int exponent=0;
  bool cont=true;
  while (cont) //cont becomes false when 10^ex>num
  {
    int ex=power(10, exponent);
    if (ex>num)
    {
      exponent-=1; //the algorithm used by this loop overshoots the appropriate power by 1
      cont=false;
    }
    else
    {
      exponent+=1;
    }
  }

  for (;exponent>=0;exponent-=1)
  {
    int digit=(num-(num%power(10,exponent)))/(power(10,exponent)); //this should get you the digit
    result+=digit2string(digit); //get the string of the digit
    num-=digit*power(10,exponent); //remove that digit from the number
  }
  return result;
}

//digit2string
std::string digit2string(int digit)
{
  std::string result;
  switch (digit)
  {
    case (1):
    {
      result+="1";
      break;
    }
    case (2):
    {
      result+="2";
      break;
    }
    case (3):
    {
      result+="3";
      break;
    }
    case (4):
    {
      result+="4";
      break;
    }
    case (5):
    {
      result+="5";
      break;
    }
    case (6):
    {
      result+="6";
      break;
    }
    case (7):
    {
      result+="7";
      break;
    }
    case (8):
    {
      result+="8";
      break;
    }
    case (9):
    {
      result+="9";
      break;
    }
    case (0):
    {
      result+="0";
      break;
    }
    default:
    {
      result+="[unkown digit]";
    }
  }
  return result;
}

//string2num
int string2num(std::string str)
{
  int result=0;
  int start=0;
  if (str.at(0)=='-')
  {
    start=1;
  }

  for (int i=start; i<str.length(); i+=1)
  {
    char c=str.at(i);
    int digit=char2digit(c);
    if (digit!=-1)
    {
      result=10*result+digit;
    }
    else
    {
      return 0;
    }
  }
  if (start==1)
  {
    result*=-1;
  }
  return result;
}

//material2string
std::string material2string(Material resource)
{
  switch (resource)
  {
    case (WOOD):
    {
      return "wood";
      break;
    }
    case (STONE):
    {
      return "stone";
      break;
    }
    case (METAL):
    {
      return "metal";
      break;
    }
    case (ANIMALS):
    {
      return "animals";
      break;
    }
    case (PRODUCE):
    {
      return "produce";
      break;
    }
    default:
    {
      return "unkown";
    }
  }
}

//career2string
std::string career2string(Career trade)
{
  switch (trade)
  {
    case (FARMER):
    {
      return "farmer";
    }
    case (LUMBERJACK):
    {
      return "lumberjack";
    }
    case (MINER):
    {
      return "miner";
    }
    case (SHEPHERD):
    {
      return "shepherd";
    }
    case (STONE_MASON):
    {
      return "stone mason";
    }
    case (SOLDIER):
    {
      return "soldier";
    }
    case (TOWNSFOLK):
    {
      return "townsfolk";
    }
    default:
    {
      return "unkown";
    }
  }
}

//fillString
std::string fillString(std::string str, int length)
{
  while (str.length()<length)
  {
    str=str+" ";
  }
  return str;
}

//isElement
bool isElement(char elmt, std::string str)
{
  for (int i=0; i<str.length(); i+=1)
  {
    if (elmt==str.at(i))
    {
      return true;
    }
  }
  return false;
}

//getName
void getName(std::string* name)
{
  if (std::cin.peek()=='\n')
  {
    std::cin.ignore(1, '\n');
  }
  std::getline(std::cin, *name);
}

//getInt
int* getInt()
{
  std::string input;
  std::cin>>input;
  if (isNum(input))
  {
    //std::cout<<"that is a digit\n";
    int* num=new int(string2num(input));
    return num;
  }
  else
  {
    return nullptr;
  }
}

//isNum
bool isNum(std::string str)
{
  if (!isElement(str.at(0),"-1234567890")) //allow for a negative sign
  {
    return false;
  }
  
  for (int i=1; i<str.length(); i+=1) //check the rest of the digits
  {
    if (!isElement(str.at(i),"1234567890"))
    {
      return false;
    }
  }
  return true;
}

//char2digit
int char2digit(char digit)
{
  int result=digit-'0';
  if ((result<=9)&&(result>=0))
  {
    return result;
  }
  else
  {
    return -1;
  }
/*
  if digit=case ("1"):
  {
    return 1;
  }
  case ("2"):
  {
    return 2;
  }
  case ("3"):
  {
    return 3;
  }
  case ("4"):
  {
    return 4;
  }
  case ("5"):
  {
    return 5;
  }
  case ("6"):
  {
    return 6;
  }
  case ("7"):
  {
    return 7;
  }
  case ("8"):
  {
    return 8;
  }
  case ("9"):
  {
    return 9;
  }
  case ("0"):
  {
    return 0;
  }
  default:
  {
    return -1;
  }
*/
}

//pause
void pause(int duration)
{
  time_t start=time(0);
  while ((time(0)-start)<duration);
}

//kingdom_builder functions

//----------------------------------------------------build functions--------------------------------

//canBuildMajor
void buildMajor(Major_Structure kind, Land* tile_ptr, std::vector<Major*>* structure_vector)
{
  Major* closest_ptr=closestStructure(tile_ptr->getX(), tile_ptr->getY(), structure_vector);

  if (!(tile_ptr->getKeep()==nullptr)) //if the land is not empty
  {
    std::cout<<"That tile is already attached to a structure\n";
    return;
  }
  else if (!checkValid(kind, tile_ptr)) //if the land is the right type
  {
    std::cout<<"That tile is not the right type of land\n";
    return;
  }
  else if (!(closest_ptr->dist(tile_ptr->getX(), tile_ptr->getY())<=BUILD_DISTANCE)) //check the distance
  {
    std::cout<<"That tile is too far away\n";
    return;
  }
  else if (!getMaterials(closest_ptr, kind))
  {
    std::cout<<"The closest structure does not have enough material to build there\n";
    return;
  }
  else
  {

    structure_vector->push_back(newMajor(kind));
    structure_vector->at(structure_vector->size()-1)->setBase(tile_ptr);
    //this is only temporary. Remove once transportatio is implemented
    structure_vector->at(structure_vector->size()-1)->setPopulation(TOWNSFOLK, 7);
    closest_ptr->setPopulation(TOWNSFOLK, closest_ptr->getPopulation(TOWNSFOLK)-7);
    //7 townsfolk from the building structure move to the build structure
    structure_vector->at(structure_vector->size()-1)->changeName(); //allow the player to change the name
  }
}

//canBuildMinor
bool canBuildMinor(Minor_Structure kind, Major* keep_ptr)
{

  if (keep_ptr->getSupportSize()>=keep_ptr->getLimit()) //check if the keep has enough space for the minor
  {
    std::cout<<"This "<<keep_ptr->getTypeString()<<" has already used all of its space\n";
    return false;//return false because not enough space will automatically disable a major from adding a support
  }
  else if (!Minor::isValid(kind, keep_ptr)) //check that the keep has a tile of the right type
  {
    std::cout<<"This "<<keep_ptr->getTypeString()<<" does not have the right type of land\n";
    return false;
  }
  else if (!keep_ptr->isOccupied())
  {
    std::cout<<"This "<<keep_ptr->getTypeString()<<" is occupied by a barbarian tribe\n";
    return false;//return false because not enough space will automatically disable a major from adding a support
  }
  else if (!getMaterials(kind, keep_ptr)) //check if the major has enough material
  {
    std::cout<<"The "<<keep_ptr->getTypeString()<<" does not have enough resources\n";
    return false;
  }

  return true;
}

//getMaterials (for a major)
bool getMaterials(Major* major_ptr, Major_Structure kind)
{
  /*getMaterials takes a pointer to a major and a type of major and tries to use the materials from the major pointer
  * to build the designated type of major
  *
  * it returns true if succesful and false otherwise
  */

  //vector with cost of materials for the structure to be built
  std::vector<int> costs={0,0,0,0,0};

  switch (kind)
  {
    case (VILLAGE):
    {
      //cost: 2 animals, 5 wood
      costs={0,2,5,0,0};
      break;
    }
    case (CASTLE):
    {
      //To build a castle costs 5 animals, 10 wood, 20 stone, and 5 metal
      costs={0, 5, 10, 20, 5};
      break;
    }
    case (FORT):
    {
      //cost: 2 animals, 10 wood, 20 stone
      costs={0, 2, 10, 20, 0};
      break;
    }
    default:
    {
      std::cout<<"unrecognized major type in getMaterials\n";
      return false;
    }
  }
  if (build(costs, major_ptr))
  {
    return true;
  }
  else
  {
    return false;
  }
}

//getMaterials (for a minor)
bool getMaterials(Minor_Structure kind, Major* keep_ptr)
{
  /*getMaterials takes a pointer to a major and a type of minor and tries to use the materials from the major pointer
  * to build the designated minor
  *
  * it returns true if succesful and false otherwise
  */

  //vector with cost of materials for the structure to be built
  std::vector<int> costs={0,0,0,0,0};

  switch (kind)
  {
    case (GARDEN):
    {
      //cost: 5 produce, 2 wood
      costs={5,0,2,0,0};
      break;
    }
    case (PASTURE):
    {
      //cost: 5 animals, 5 wood
      costs={0, 5, 5, 0, 0};
      break;
    }
    case (SAWMILL):
    {
      //cost: 7 wood, 3 stone, 2 metal
      costs={0, 0, 7, 3, 2};
      break;
    }
    case (QUARRY):
    {
      //cost: 5 produce, 2 animals, 5 wood, 5 metal
      costs={5, 2, 5, 0, 5};
      break;
    }
    case (MINE):
    {
      //cost: 3 animals, 3 wood, 5 metal
      costs={0, 3, 3, 0, 5};
      break;
    }
    default:
    {
      return false;
    }
  }
  if (build(costs, keep_ptr))
  {
    return true;
  }
  else
  {
    return false;
  }
}

//build
bool build(std::vector<int> costs, Major* major_ptr)
{
  int total_produce=major_ptr->getMaterials(PRODUCE);
  int total_animals=major_ptr->getMaterials(ANIMALS);
  int total_wood=major_ptr->getMaterials(WOOD);
  int total_stone=major_ptr->getMaterials(STONE);
  int total_metal=major_ptr->getMaterials(METAL);

  if (
    (total_produce>=costs.at(PRODUCE))&&
    (total_animals>=costs.at(ANIMALS))&&
    (total_wood>=costs.at(WOOD))&&
    (total_stone>=costs.at(STONE))&&
    (total_metal>=costs.at(METAL))
  )
  {
    major_ptr->setMaterials(PRODUCE, total_produce-costs.at(PRODUCE));
    major_ptr->setMaterials(ANIMALS, total_animals-costs.at(ANIMALS));
    major_ptr->setMaterials(WOOD, total_wood-costs.at(WOOD));
    major_ptr->setMaterials(STONE, total_stone-costs.at(STONE));
    major_ptr->setMaterials(METAL, total_metal-costs.at(METAL));
    return true;
  }
  return false;
}

//checkValid
bool checkValid(Major_Structure kind, Land* tile_ptr)
{
  switch (kind)
  {
    case (VILLAGE):
    {
      return Village::isValid(tile_ptr);
    }
    case (CASTLE):
    {
      return Castle::isValid(tile_ptr);
    }
    case (PORT):
    {
      return false;
    }
    case (FORT):
    {
      return Fort::isValid(tile_ptr);
    }
    default:
    {
      return false;
    }
  }
}

//newMajor
Major* newMajor(Major_Structure kind)
{
  switch (kind)
  {
    case (VILLAGE):
    {
      return new Village();
    }
    case (CASTLE):
    {
      return new Castle();
    }
    case (FORT):
    {
      return new Fort();
    }
    default:
    {
      std::cout<<"unrecognized major type in newMajor\n";
      return nullptr;
    }
  }
}


//---------------------------------------------explore functions-------------------------------------
void explore(std::vector<Land>* land_vector, int starting_index, Direction direction, int level)
{
  if (level==0)
  {
    return;
  }
  int pos_x=index2pos(starting_index, 'x');
  int pos_y=index2pos(starting_index, 'y');
  int add_x=0;
  int add_y=0;
  switch (direction)
  {
    case (NORTH):
    {
      add_y=1;
      break;
    }
    case (EAST):
    {
      add_x=1;
      break;
    }
    case (SOUTH):
    {
      add_y=-1;
      break;
    }
    case (WEST):
    {
      add_x=-1;
      break;
    }
    default:
    {
      std::cout<<"could not recognize direction in explore\n";
      return;
    }
  }
  int explored_index=pos2index(pos_x+add_x, pos_y+add_y);

  Land* tile=&(land_vector->at(starting_index)); //get the starting tile
  Land* explored_ptr=&(land_vector->at(explored_index));//get the explored tile
  if (explored_ptr->getType()!=SPACE)
  {
    return;
  }

  Land_type kind=getExploredType(tile);

  explored_ptr->setType(kind);
  explored_ptr->setX(pos_x+add_x);
  explored_ptr->setY(pos_y+add_y);
  for (int i=NORTH; i<=WEST; i+=1)
  {
    explore(land_vector, explored_index, static_cast<Direction>(i), level-1);
  }
}

//closestStructure
Major* closestStructure(int coord_x, int coord_y, std::vector<Major*>* major_ptr)
{
  double distance=2*MAP_SIZE+1; //this is theoretically the furthest a structure could be
  Major* closest_ptr=major_ptr->at(0); //set the initial value to the first entry

  for (int i=0; i<major_ptr->size(); i+=1) //iterate over all the structures
  {
    double check_distance=(major_ptr->at(i))->dist(coord_x, coord_y); //get the distance

    if ((distance>check_distance)&&(check_distance!=0)) //if the current structure is closer than the previous closest
    //the second condition is so that if the specified coordinates
    //already have a structure that structure is not returned
    {
      closest_ptr=major_ptr->at(i); //set the current structure to be closest
      distance=check_distance;
    }
  }
  return closest_ptr;
}

//getExploredType
Land_type getExploredType(Land* tile_ptr)
{
  int index=pos2index(tile_ptr->getX(), tile_ptr->getY());
  //srand(static_cast<int>(index+time(0)));
  //use the index of the land tile and the time as the seed for the random number generator
  //this is because the function will be called multiple times a second, so if it used only
  //time then all outputs would be the same
  Land_type kind=tile_ptr->getType();
  double change_number=static_cast<double>(rand())/INT_MAX;
  if (change_number>BIOME_CHANGE)
  {
    return kind;
  }

  int add=1;
  double sum=FOREST_GEN+FIELD_GEN+MOUNTAIN_GEN+TUNDRA_GEN+SEA_GEN; // the sum of the generation numbers
  while (kind==tile_ptr->getType())
  {
    //srand(static_cast<int>(index+time(0)+add)); //add is there so that this seed is not the same as above

    double random_number=static_cast<double>(rand())/INT_MAX;
    if (random_number<=(FOREST_GEN/sum))
    {
      kind=FOREST;
    }
    else if (random_number<=((FOREST_GEN+FIELD_GEN)/sum))
    {
      kind=FIELDS;
    }
    else if (random_number<=((FOREST_GEN+FIELD_GEN+MOUNTAIN_GEN)/sum))
    {
      kind=MOUNTAINS;
    }
    else if (random_number<=((FOREST_GEN+FIELD_GEN+MOUNTAIN_GEN+TUNDRA_GEN)/sum))
    {
      kind=TUNDRA;
    }
    else
    {
      kind=SEA;
    }
    add+=1;
  }
  return kind;
}

//------------------------------------------------------map functions-------------------------------------------

//pos2index
int pos2index(int x, int y) // convert coordinates into an index in the land array
{
  return ((MAP_SIZE*2+1)*y+x);
}

//index2pos
int index2pos(int index, char axis)
{
  if (axis=='x')
  {
    return index % ((MAP_SIZE*2+1));
  }
  else if (axis=='y')
  {
    return index / ((MAP_SIZE*2+1));
  }
  std::cout<<"invalid axis entered in index2pos: "<<axis<<" not an axis\n";
  return 0;
}

//displayMap
//note: coordinates entered in display map need to be c_cen, not p_cen
void displayMap(int center_x, int center_y, std::vector<Land>* map)
{
  //display upper border
  displayBorder();

  for (int y=(DISPLAY_SIZE/2); y>-(DISPLAY_SIZE/2); y-=1)
  {
    for (int row=0; row<3; row+=1)
    {
      std::cout<<"|| "; //display the left border
      for (int x=-(DISPLAY_SIZE/2); x<(DISPLAY_SIZE/2); x+=1)
      {
        std::string land_char=" ";
        std::string structure_char=" ";
        land_char=getLandChar((map)->at((pos2index(center_x+x, center_y+y))).getType());

        Major* keep_ptr=(map)->at(pos2index(center_x+x, center_y+y)).getKeep();
        if ((row==1)&&(keep_ptr!=nullptr))
        {
          if (((keep_ptr->getBase())->getX()==center_x+x)&&((keep_ptr->getBase())->getY()==center_y+y))
            //if the keep is actually on that tile
          {
            structure_char=getStructureChar(((map)->at(pos2index(center_x+x,center_y+y)).getKeep())->getType());
          }
          else
          {
            structure_char=land_char;
          }
          std::cout<<land_char<<structure_char<<land_char<<" ";
        }
        else
        {
          std::cout<<land_char<<land_char<<land_char<<" ";
        }
      }
      std::cout<<"||\n";
    }
    std::cout<<"||";
    space(DISPLAY_SIZE*4+1);
    std::cout<<"||\n";
  }

  //display lower border
  displayBorder();

  std::cout<<"LEGEND: forest:+ fields:) mountains:^ tundra:_ sea:~  castle:H village:h fort:d port:N\n";
}

//getLandChar
std::string getLandChar(Land_type tile_type)
{
  switch (tile_type)
  {
    case (FOREST):
    {
      return "\033[38;5;28m+\033[0m";
      //return '+';
    }
    case (FIELDS):
    {
      return "\033[38;5;220m)\033[0m";
      //return ")";
    }
    case (MOUNTAINS):
    {
      return "\033[38;5;8m^\033[0m";
      //return "\033[34m^\033[0m";
      //return "^";
    }
    case (TUNDRA):
    {
      return "\033[38;5;143m_\033[0m";
      //return "_";
    }
    case (SEA):
    {
      return "\033[36m~\033[0m";
      //return "~";
    }
    case (SPACE):
    {
      return " ";
    }
    default:
    {
      return "*";
    }
  }
  return "*";
}

//getStructureChar
std::string getStructureChar(Major_Structure keep_kind)
{
  switch (keep_kind)
  {
    case (CASTLE):
    {
      return "H";
    }
    case (VILLAGE):
    {
      return "h";
    }
    case (FORT):
    {
      return "d";
    }
    case (PORT):
    {
      return "N";
    }
    default:
    {
    }
  }
  return " ";
}

//displayBorder
void displayBorder()
{
  std::cout<<" ";
  for (int i=0; i<(DISPLAY_SIZE*4+3); i+=1)
  {
    std::cout<<"=";
  }
  std::cout<<" \n";
}

//-----------------------------------------------------name functions------------------------------------------------
//getStructure
Major* getStructure(std::string name, std::vector<Major*>* list_ptr)
{
  for (int i=0; i<list_ptr->size(); i+=1) //iterate over all the majors in the list
  {
    Major* major_ptr=list_ptr->at(i); //get the current major
    if (major_ptr->getName()==name) //compare the names
    {
      return major_ptr;
    }
  }
  //std::cout<<"There is no structure with the name "<<name<<"\n";
  return nullptr;
}

//listStructures
std::vector<std::string> listStructures(std::vector<Major*>* list_ptr)
{
  std::vector<std::string> names;
  for (int i=0; i<list_ptr->size(); i+=1)
  {
    Major* major_ptr=list_ptr->at(i);
    names.push_back(major_ptr->getName());
  }
  return names;
}

//-----------------------------------------------------combat functions-------------------------------------------

//getHits
int getHits(int warriors, double defense)
{
  int hits=0;
  for (int i=0; i<warriors; i+=1)
  {
    int num=rand()%100; //random number between 0-99
    if ((num/100.0)<(KILL_CHANCE-defense))//if the percentage is less than the chance of a kill (adjusted for defense)
    {
      hits+=1;
    }
  }
  return hits;
}

int getHits(int warriors)
{
  return getHits(warriors, 0);
}

//furthestStructure
Major* furthestStructure(std::vector<Major*>* major_ptr)
{
  double current_distance=0; //this is theoretically the furthest a structure could be
  Major* furthest_ptr=major_ptr->at(0); //set the initial value to the first entry
  int coord_x=furthest_ptr->getX();
  int coord_y=furthest_ptr->getY();

  for (int i=0; i<major_ptr->size(); i+=1) //iterate over all the structures
  {
    double check_distance=(major_ptr->at(i))->dist(coord_x, coord_y); //get the distance

    if (current_distance<check_distance) //if the current structure is further than the previous closest
    {
      furthest_ptr=major_ptr->at(i); //set the current structure to be closest
      current_distance=check_distance;
    }
  }
  return furthest_ptr;
}

//--------------------------------------------------------other functions--------------------------------------------

//space
void space(int num)
{
  for (int i=0; i<num; i+=1)
  {
    std::cout<<" ";
  }
}

void getCoords(int* x_coord, int* y_coord)
//get player entered coordinates and output said coordinates in c_cen
{
/*
  //get the player input
  std::string input;
  getName(&input); //get name is used because it is a reliable way to set a string
  //It probably should be called getString but I am not going to refactor all of its uses

  std::vector<std::string> input_vec; //initialize vector
  parseString(&input_vec, &input); //parse input (separate into space separated values)
  if (input_vec.size()<2)
  {
    *x_coord=nullptr;
    *y_coord=nullptr;
    return;
  }
*/
  int* p_cen_x=nullptr; //p_cen x coordinate
  int* p_cen_y=nullptr; //p_cen y coordinate
  
  //std::cin>>p_cen_x>>p_cen_y; //take in the coordinates (p_cen)

  while ((p_cen_x==nullptr)||(p_cen_y==nullptr))
  {
    p_cen_x=getInt(); //p_cen x coordinate
    p_cen_y=getInt(); //p_cen y coordinate

    if ((p_cen_x==nullptr)||(p_cen_y==nullptr))
    {
      std::cout<<"please enter valid coordinates\n";
    }
  }

  *x_coord=static_cast<int> (*p_cen_x+MAP_SIZE); //convert x coordinate to c_cen
  *y_coord=static_cast<int> (*p_cen_y+MAP_SIZE); //convert y coordinate to c_cen
}

std::string displayCoords(int x_coord, int y_coord)
{
  return "("+int2string(static_cast<int>(x_coord-MAP_SIZE))+", "+int2string(static_cast<int>(y_coord-MAP_SIZE))+")";
}

//dist
double dist(int x1, int y1, int x2, int y2)
{
  return sqrt(power(x1-x2, 2)+power(y1-y2, 2));
}

double dist(Major* major1, Major* major2)
{
  int x1=major1->getX();
  int y1=major1->getY();

  int x2=major2->getX();
  int y2=major2->getY();
  return dist(x1,y1,x2,y2);
}

double dist(Land* land1, Land* land2)
{
  int x1=land1->getX();
  int y1=land1->getY();

  int x2=land2->getX();
  int y2=land2->getY();
  return dist(x1,y1,x2,y2);
}


//template instantiations
//template Major* closestStructure<Village>(int, int, std::vector <Village>*); //explicit instantiation

