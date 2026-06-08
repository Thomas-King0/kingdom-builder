/*Class Major object code
 */

#include <iostream>
#include <cmath>
#include "major.h"
#include "land.h"
#include "minor.h"
#include "barbarian.h"
#include "road.h"
#include "caravan.h"
#include "helpful.h"
#include "settings.h"
#include "menu.h"

//constructor
Major::Major() //default constructor
{
  //changeName();
  setName("Unnamed structure");
  setBase(nullptr);
  setOccupants(nullptr);
  setLimit(0); //0 will be the default tile limit for now
  for (int career=FARMER; career<=TOWNSFOLK; career+=1)
  {
    population.push_back(0);
  }
  for (int resource=PRODUCE; resource<VACUUM; resource+=1)
  {
    materials.push_back(0); //initialize the list of materials, the village starts out with none
  }
}

//copy
void Major::copy(Major* major_ptr)
{
  setName(major_ptr->getName());
  Land* base_ptr=major_ptr->getBase();
  base_ptr->setKeep(nullptr);
  setBase(base_ptr);
  //setOccupants(major_ptr->getOccupants());
  setType(major_ptr->getType());
  //for (int i=0; i<major_ptr->getHighwaySize(); i+=1)
  //{
    //addRoad(major_ptr->getRoad(i));
  //}

  for (int career=FARMER; career<=TOWNSFOLK; career+=1)
  {
    setPopulation(static_cast<Career>(career), major_ptr->getPopulation(static_cast<Career>(career)));
  }
  for (int resource=PRODUCE; resource<VACUUM; resource+=1)
  {
    setMaterials(static_cast<Material>(resource), major_ptr->getMaterials(static_cast<Material>(resource)));
  }
}

//setName
void Major::setName(std::string givenName)
{
  name=givenName;
}

//getName
std::string Major::getName()
{
  return name;
}

//changeName
void Major::changeName()
{
  std::string givenName;
  std::cout<<"What would you like to name this: ";
  if (std::cin.peek()=='\n')
  {
    std::cin.ignore(1, '\n');
  }
  std::getline(std::cin, givenName);
  setName(givenName);
}

//setMaterials
void Major::setMaterials(Material resource, int num)
{ //this function might need significant altering after further consideration of how materials will work
  if (num>=0)
  {
    materials.at(resource)=num;
  }
}

//getMaterials
int Major::getMaterials(Material resource)
{
  return materials.at(resource);
}

int Major::getMaterials(int resource)
{
  return materials.at(static_cast<Material>(resource));
}

//setType
void Major::setType(Major_Structure kind)
{
  major_type=kind;
  //no data validation happens. To see the reason for this, refer to the comment in function setType in minor.cpp
}

//getType
Major_Structure Major::getType()
{
  return major_type;
}

//getTypeString
std::string Major::getTypeString()
{
  switch (getType())
  {
    case (CASTLE):
    {
      return "castle";
      break;
    }
    case (VILLAGE):
    {
      return "village";
      break;
    }
    case (FORT):
    {
      return "fort";
      break;
    }
    case (PORT):
    {
      return "port";
      break;
    }
    default:
    {
      return "unkown";
      break;
    }
  }
}

//setBase
void Major::setBase(Land* tile) //set the base tile
{
  if (tile==nullptr)
  {
    land=tile; //set the base tile to the null pointer
  }
  else if (tile->getKeep()==nullptr)
  {
    land=tile;
    tile->setKeep(this); //set the structure as the keep for the base tile
  }
  else
  {
    std::cout<<"tried setting a land tile as a base that already has a keep\n";
  }
}

//getBase
Land* Major::getBase() //get the base tile
{
  return land;
}

//getX
int Major::getX()
{
  return getBase()->getX();
}

//getY
int Major::getY()
{
  return getBase()->getY();
}

//dist
double Major::dist(int x, int y)
{
  return sqrt(((getX()-x)*(getX()-x))+((getY()-y)*(getY()-y)));
}

//addClaim
void Major::addClaim(Land* claim)
{
  if (canClaim(claim)) //if the land can be claimed
  {
    claimed.push_back(claim); //add the claim to the major's list
    claim->setKeep(this); //set the major as the keep of the tile
  }
}

//getClaim
Land* Major::getClaim(int num)
{
  if ((num<static_cast<int>(claimed.size()))&&(num>=0))
  {
    return claimed[num];
  }
  else
  {
    std::cout<<"Tried to index the vector 'claimed' at an index["<<num<<"] greater than it's size"
      <<"["<<claimed.size()<<"]: returned null"<<" pointer\n";
    return nullptr;
  }
}

//getClaimSize
int Major::getClaimSize()
{
  return claimed.size();
}

//canClaim
bool Major::canClaim(Land* potential)
{
  //check that the land and the major can both claim each other
  if (getClaimSize()>=getLimit()) //if the land limit has already been reached
  {
    std::cout<<"Can't add a claim to a structure that is already full\n";
    return false;
  }
  else if (potential->getKeep()!=nullptr) //if the tile already has a keep
  {
    std::cout<<"Can't add a claim to a tile that already has a keep\n";
    std::cout<<"keep is "<<(potential->getKeep())->getName()<<"\n";
    return false;
  }
  else if (potential->getType()==SPACE) //if the tile has not been explored
  {
    std::cout<<"That tile has not been explored yet\n";
    return false;
  }

  //check that the tile is adjacent to another tile already claimed
  if ((getBase())->isAdjacent(potential)==true) //if the potential claim is adjacent to the base
  {
    return true; //the major can claim it
  }

  for (int i=0; i<getClaimSize(); i+=1)
  {
    if ((getClaim(i))->isAdjacent(potential)==true) //if the potential claim is adjacent to another claim
    {
      return true;
    }
  }

  std::cout<<"Tile is too far away\n";
  return false;
}

void Major::forceClaim(Land* land_ptr)
{
  claimed.push_back(land_ptr); //add the claim to the major's list
  land_ptr->setKeep(this); //set the major as the keep of the tile
}

//setLimit
void Major::setLimit(int num) //set the limit of the structure
{
  land_limit=num; //there is no data validation in this function because the data validation will happen
          //in the derived classes
}


//getLimit
int Major::getLimit()
{
  return land_limit;
}

//setPopulation
void Major::setPopulation(Career trade, int num)
{
  if (trade<=TOWNSFOLK)
  {
    population[static_cast<int>(trade)]=num;
  }
  else
  {
    std::cout<<"tried to access population out of bounds in setPopulation, index "<<trade<<" is out of bounds\n";
  }
}

//getPopluation
int Major::getPopulation(Career trade)
{
  if (trade<=TOWNSFOLK)
  {
    return population[static_cast<int>(trade)];
  }
  else
  {
    std::cout<<"tried to access population out of bounds in getPopluation, index "<<trade<<" is out of bounds\n";
    return 0;
  }
}

int Major::getPopluation(Career trade)
{
  std::cout<<"YOU WROTE THE WRONG THING FOR THE POPULATION FUNCTION AGAIN!!!!!!\n";
  return getPopulation(trade);
}


//trainTownsfolk
void Major::trainTownsfolk()
{
  Menu trade_menu=Menu("Choose a trade to train them in");
  trade_menu.addOption({"farmer", "lumberjack", "miner", "shepherd", "stone mason"});
  trade_menu.display();

  int* input_ptr=trade_menu.getChoice();
  if (input_ptr==nullptr)
  {
    std::cout<<"exiting...\n";
    return;
  }
  int input=*input_ptr;

  if ((input>0)&&(input<=5))
  {
    std::cout<<"This "<<getTypeString()<<" has "<<getPopulation(TOWNSFOLK)<<" townsfolk\n"
      <<"how many would you like to train: ";
    int* num_ptr=getInt();
    if (num_ptr==nullptr)
    {
      std::cout<<"that is not a valid input\n";
      return;
    }

    int num_to_train=*num_ptr;
    if (num_to_train>getPopulation(TOWNSFOLK))
    {
      std::cout<<"This "<<getTypeString()<<" does not have that many townsfolk\n";
    }
    else if (num_to_train>0)
    {
      Career profession=static_cast<Career>(input-1);
      setPopulation(profession, getPopulation(profession)+num_to_train);
      setPopulation(TOWNSFOLK, getPopulation(TOWNSFOLK)-num_to_train);
    }
    else
    {
    std::cout<<"Number must be positive\n";
    }
  }
  else
  {
      std::cout<<"Please input a valid trade\n";
  }
}

//addTownsfolk
void Major::addTownsfolk()
{
  //double birth_rate=0.16; //birth rate of townsfolk
  int number=static_cast<int>(getPopulation(TOWNSFOLK)*(BIRTH_RATE))+1; //get the number of townfolk added(automatically add at least 1)
  if (number>getVacancy()) //if the population would be more than the structure could handle
  {
    number=getVacancy(); //only add as many people as there is space left
    setVacancy(0); //vacancy is now zero
  }
  else
  {
    setVacancy(getVacancy()-number); //set the vacancy
  }
  int new_population=getPopulation(TOWNSFOLK)+number;
  setPopulation(TOWNSFOLK, static_cast<int>(new_population));
  std::cout<<"new population of "<<getName()<<": "<<getPopulation(TOWNSFOLK)<<"\n";
}

//setVacancy
void Major::setVacancy(int num)
{
  if (num<0) //if vacancy would be set to less than 0
  {
    std::cout<<"in setVacancy, num less than 0, vacancy set to 0\n";
    vacancy=0;
  }
  else //upper limit set in an overriden function
  {
    vacancy=num; //set the vacancy
  }
}

//getVacancy
int Major::getVacancy()
{
  return vacancy;
}

//addSupport (VIRTUAL)
void Major::addSupport(Minor* support)
{
  if (support->getKeep()==this)
  {
    minor_structures.push_back(support);
    return;
  }
  else if (support->getKeep()!=nullptr) //if the support already has a keep that is not this
  {
    std::cout<<"tried to add a support that already has a keep\n";
    return;
  }
  else if (static_cast<int>(minor_structures.size())>=getLimit()) //if the major is already full
  {
    std::cout<<"tried to add a support to a major that is already full\n";
    return;
  }
  else
  {
    minor_structures.push_back(support);
    support->setKeep(this); //add the major as the keep for the minor
  }
}

//getSupport
Minor* Major::getSupport(int num)
{
  if (num<0)
  {
    std::cout<<"tried to index a vector at a negative number in getSupport\n";
    return nullptr;
  }
  else if (num>=static_cast<int>(minor_structures.size()))
  {
    std::cout<<"tried to index a vector out of bounds in getSupport\n";
    return nullptr;
  }
  else
  {
    return minor_structures.at(num);
  }
}

//getSupportSize
int Major::getSupportSize()
{
  return minor_structures.size();
}


//canProduce
bool Major::canProduce()
{
  for (int i=0; i<getSupportSize(); i+=1)
  {
    if ((getSupport(i))->getProducing()==false) //if there is a support that is not producing
    {
      return true; //the major can produce
    }
  }
  return false; //if the for loop runs its course then the major cannot produce
}

//startProduction
void Major::startProduction()
{
  if (canProduce()==true)
  {
    for (int i=0; i<getSupportSize(); i+=1)
    {
      getSupport(i)->startProduction(); //try to start production at each minor structure
    }
  }
}

//endProduction
void Major::endProduction()
{
  for (int i=0; i<getSupportSize(); i+=1)
  {
    getSupport(i)->endProduction();
  }
}

//************************************************************road functions******************************************************
//addRoad
void Major::addRoad(Road* road)
{
  //make sure that this structure is one of the roads destinations
  if ((road->getDestination(1)==this)||(road->getDestination(2)==this))
  {
    highways.push_back(road); //add the road
  }
  else
  {
    std::cout<<"This "<<getTypeString()<<" is not connected to that road\n"; //output error message
  }
}

//getRoad
Road* Major::getRoad(int num)
{
  if (num<getHighwaySize()) //if the index is not greater than the number of roads
  {
    return highways[num]; //get the road pointer
  }
  else
  {
    std::cout<<"tried to index getRoad at an out of bounds number["<<num<<"\n";
    return nullptr;
  }
}

Road* Major::getRoad(std::string name)
{
  for (int i=0; i<getHighwaySize(); i+=1) //iterate over all the roads in the road vector
  {
    if (getSister(i)->getName()==name) //if the names match
    {
      return getRoad(i); //return the pointer to the road
    }
  }
  std::cout<<"there is no road to a sister structure with that name\n";
  return nullptr; //return the nullptr if no road goes between this major and the given major
}

//getRoadNum
int Major::getHighwaySize()
{
  return highways.size(); //return the size of the highways vector
}

//getSister
Major* Major::getSister(int index)
{
  Road* road=getRoad(index); //get the road pointer at the index
  if (road==nullptr) //if that road does not exist then the road variable will be the nullptr
  {
    std::cout<<"attempt to get sister structure failed\n";
    return nullptr;
  }

  if (road->getDestination(1)==this) //if this structure is destination1
  {
    return road->getDestination(2); //return the other destination
  }
  else
  {
    return road->getDestination(1); //otherwise return destination1
  }
}

Major* Major::getSister(std::string name)
{
  for (int i=0; i<getHighwaySize(); i+=1) //iterate over all roads in the road vector
  {
    if (getSister(i)->getName()==name) //if the names match
    {
      return getSister(i); //return the pointer to that major
    }
  }
  std::cout<<"there is no sister structure with that name\n";
  return nullptr; //return the nullptr if no sister structure matches the given name
}

//getSisterList
std::string Major::getSisterList()
{
  std::string list; //initialize list
  for (int i=0; i<getHighwaySize(); i+=1) //for all roads in the highway system
  {
    list+=getSister(i)->getName()+"\n"; //add the name of the sister structure attached to that road
  }
  return list;
}

//isConnected
bool Major::isConnected(std::string name)
{
  for (int i=0; i<getHighwaySize(); i+=1) //for all the roads in the highway system
  {
    Major* major_ptr=getSister(i); //get the sister from the ith road
    if (major_ptr->getName()==name) //if the name of that sister is the name of the structure we want
    {
      return true;
    }
  }
  return false; //return false by default
}

bool Major::isConnected(Major* major_ptr)
{
  for (int i=0; i<getHighwaySize(); i+=1) //for all the roads in the highway system
  {
    Major* sister_ptr=getSister(i); //get the sister from that road
    if (major_ptr==sister_ptr) //if the two pointers point to the same address
    {
      return true;
    }
  }
  return false; //return false by default
}

//load
void Major::load()
{
  //get the name of the destination
  Menu load_menu=Menu("Choose location to send caravan");
  for (int i=0; i<getHighwaySize(); i+=1)
  {
    load_menu.addOption(getSister(i)->getName());
  }
  //load_menu.addOption("exit");
  load_menu.display();
  int choice;
  int* choice_ptr=load_menu.getChoice();
  if (choice_ptr==nullptr)
  {
    std::cout<<"that's a no go for load\n";
    return;
  }
  choice=*(choice_ptr)-1;
/*
  std::cout<<"connected structures\n{\n"<<getSisterList()<<"}\nwhere are you sending this caravan: ";
  std::string name;

  getString(&name);
   */

  //Check that the destination is valid
  Major* destination_ptr=getSister(choice); //get the pointer to the destination
  if (destination_ptr==nullptr) //if there is no sister structure with the given name
  {
    std::cout<<"unable to load caravan at "<<getName()<<"\n";
    std::cout<<"no valid destination specified\n";
    return;
  }

  //Check that the road is not in use
  Road* road_ptr=getRoad(choice); //get the road between the structures
  if (road_ptr->isTransporting()==true)
  {
    std::cout<<"the road between those two destinations is already in use\n";
    return;
  }

  //check that the sister city is not occupied
  if (destination_ptr->isOccupied())
  {
    std::cout<<"that location is overrun by barbarians\n";
    return;
  }

  road_ptr->setCaravan(new Caravan(destination_ptr)); //initialize the caravan
  Caravan* haul=road_ptr->getCaravan(); //get the caravan pointer
  if (packageCaravan(haul)) //take the contents from the major and add them to the caravan
  {
    road_ptr->startJourney(); //start the journey
  }
  else
  {
    road_ptr->setCaravan(nullptr);
  }
}

//packageCaravan
bool Major::packageCaravan(Caravan* haul)
{
  if (haul->getDestination()==nullptr) //make sure that the caravan was successfully set up
  {
    std::cout<<"failed to package caravan because it had no destination\n";
    delete haul; //delete the pointer
    return false;
  }

  if (haul->isMaterials()) //if the caravan is carrying materials
  {
    Material kind=static_cast<Material> (haul->getContents()); //get the kind of materials
    int current_amount=getMaterials(kind); //get the current amount of that material the sending structure has
    if (current_amount<haul->getAmount()) //if the player is trying to send more material than the structure has
    {
      std::cout<<getName()<<" does not have that much "<<material2string(kind)<<"\n";
      delete haul; //delete the pointer so that there are no memory leaks
      return false;
    }
    //if the program makes it through the above statement then the structure can send the materials
    setMaterials(kind, current_amount-haul->getAmount()); //set the amount of materials to the appropriate number
  }
  else
  {
    Career kind=static_cast<Career> (haul->getContents()); //get the career
    int current_amount=getPopulation(kind); //get the current population of that trade
    if (current_amount<haul->getAmount()) //if the player is trying to send more of that trade then the structure has
    {
      std::cout<<getName()<<" does not have that many "<<career2string(kind)<<"s\n";
      delete haul; //delete the pointer
      return false;
    }
    setPopulation(kind, current_amount-haul->getAmount()); //set the population to the appropriate number
  }
  return true;
}
//unload
void Major::unload(Caravan* haul)
{
  if (haul->isMaterials())
  {
    Material kind=static_cast<Material> (haul->getContents());
    int current_amount=getMaterials(kind);
    setMaterials(kind, current_amount+haul->getAmount());
  }
  else
  {
    Career kind=static_cast<Career> (haul->getContents());
    int current_amount=getPopulation(kind);
    setPopulation(kind, current_amount+haul->getAmount());
  }
  delete haul;
}

//setOccupants
void Major::setOccupants(Barbarian* tribe)
{
  if (getOccupants()==nullptr)
  {
    occupants=tribe;
  }
  else if (tribe==nullptr)
  {
    occupants=nullptr;
  }
  else
  {
    std::cout<<"cannot set tribe to occupy major["<<getName()<<"]: major is probably already occupied\n";
  }
}

//getOccupants
Barbarian* Major::getOccupants()
{
  return occupants;
}

//isSurrounded
bool Major::isSurrounded()
{
  for(int i=0; i<getHighwaySize(); i+=1)
  {
    if(getSister(i)->getOccupants()==nullptr)
    {
      return false;
    }
  }
  return true;
}

//isOccupied
bool Major::isOccupied()
{
  if (getOccupants()==nullptr)
  {
    return false;
  }
  else
  {
    return true;
  }
}

//*************************attacking functions*****************************
//attack
int Major::attack(Major* target_ptr)
{
  int attackers=getWarriors();
  int defenders=target_ptr->getWarriors();
  bool retreat=false;

  double defense=target_ptr->getDefense();

  while((attackers>0)&&(defenders>0)) //battle loop
  {
    int attacking_hits=getHits(attackers, defense); //get the number of hits that the attackers produce
    int defending_hits=getHits(defenders);
    attackers-=defending_hits;
    defenders-=attacking_hits;
    std::cout<<"attackers: "<<attackers<<"\n";
    std::cout<<"defenders: "<<defenders<<"\n\n";
    if (!isOccupied())
    {
      std::cout<<"What do you wish to do:\n";
      std::cout<<"\t1. continue\n\t2. retreat\n";
      int* action_ptr=nullptr;
      while (action_ptr==nullptr)
      {
        action_ptr=getInt();
        if (action_ptr==nullptr)
        {
          std::cout<<"please enter 1 or 2\n";
        }
      } 
      int action=*action_ptr;
      if (action==2)
      {
        retreat=true;
        break;
      }
    }
    else
    {
      pause(1);
    }
  }
  if (retreat)
  {
    return -1;
  }
  else if (attackers<=0)
  {
    setAftermath(0);
    target_ptr->setAftermath(defenders);
    return 0;
  }
  else if (defenders<=0)
  {
    setAftermath(attackers);
    target_ptr->setAftermath(0);
    return 1;
  }
  else
  {
    std::cout<<"something weird happened in attack\n";
    return 0;
  }
}

//getWarriors
int Major::getWarriors()
{
  if (isOccupied()==false) //if the structure is not occupied
  {
    return getPopulation(SOLDIER); //return the soldiers in the keep
  }
  else
  {
    return getOccupants()->getPopulation(); //return the barbarians occupying the structure
  }
}

//getDefense
double Major::getDefense()
{
  return 0;
}

//setAftermath
void Major::setAftermath(int num)
{
  /*
  Major* keep_ptr=nullptr;
  if (getKeep()==nullptr)
  {
    std::cout<<"tried to set the soldiers/barbarians for a land tile with no keep\n";
  }
  else
  {
    keep_ptr=getKeep(); //get the keep of the tile
  }
  */

  if (isOccupied()==false) //if the keep is not occupied
  {
    setPopulation(SOLDIER, num); //set the soldiers in the keep
  }
  else
  {
    getOccupants()->setPopulation(num); //set the barbarians occupying the structure
  }
}

//getData
std::string Major::getData()
{
  std::string data="";
  //------------------------Major structure data--------------------------
  //upper border
  std::string color="\033[38;5;105m";
  data+=(color+"=============\033[0m");
  for (int i=0; i<getName().length(); i+=1)
  {
    data+="\033[38;5;105m=\033[0m";
  }
  data+=("\033[38;5;105m====\033[0m\n");

  //middle section
  data+=("<\033[38;5;105m   Data for "+getName()+"   >\033[0m\n");

  //lower border
  data+=("=\033[38;5;105m============\033[0m");
  for (int i=0; i<getName().length(); i+=1)
  {
    data+="\033[38;5;105m=\033[0m";
  }
  data+=("\033[38;5;105m====\033[0m\n\n");

  //type
  data+=("  "+fillString("Type:",20)+getTypeString()+"\n\n");

  //occupation status
  if (isOccupied())
  {
    data+=("  "+fillString("Currently occupied:", 20)+"\033[38;5;196mtrue\033[0m\n");
    data+=("  "+fillString("Population of tribe:",20)+int2string(getOccupants()->getPopulation())+"\n\n");
  }
  else
  {
    data+=("  "+fillString("Currently occupied:", 20)+"\033[38;5;46mfalse\033[0m\n\n");
  }

  //population data
  data+=("\033[38;5;129m  ******Population data*****\033[0m\n");
  for (int i=FARMER; i<TOWNSFOLK; i+=1)
  {
    Career career=static_cast<Career>(i);
    data+="  "+fillString((career2string(career))+"s:",20)+int2string(getPopulation(career))+"\n";
  }
  data+="  "+fillString((career2string(TOWNSFOLK))+":",20)+int2string(getPopulation(TOWNSFOLK))+"\n";
  data+="\n";

  //support data
  data+="\033[38;5;208m  *******Support data******\033[0m\n";
  //number of supports
  data+=("  "+fillString("Number of supports:",20)+int2string(getSupportSize())+"\n");
  for (int i=0;i<getSupportSize();i+=1)
  {
    data+=("\n"+getSupport(i)->getData()+"\n");
  }
  data+="\n";

  //land data
  data+="\033[38;5;34m  ********Land data********\033[0m\n";
  //amount of land
  data+=("  "+fillString("Amount of land claimed:",20)+int2string(getClaimSize())+"\n");
  for (int i=0; i<getClaimSize(); i+=1)
  {
    data+=("\n"+getClaim(i)->getData()+"\n");
  }
  data+="\n";

  //road data
  data+="\033[38;5;59m  ********Road data********\033[0m\n";
  if (getHighwaySize()==0)
  {
    data+="  this "+getTypeString()+" has no roads\n";
  }
  
  for (int i=0; i<getHighwaySize(); i+=1)
  {
    data+=("\n  Road to "+getSister(i)->getName()+"\n"+getRoad(i)->getData()+"\n");
  }
  data+="\n";

  return data;
}


//destructor
Major::~Major()
{}
