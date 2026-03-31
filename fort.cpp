/*Object file for Fort class
 */

#include <iostream>

#include "fort.h"

#include "minor.h"
#include "land.h"
#include "helpful.h"
#include "settings.h"
#include "menu.h"

//constructor
Fort::Fort(): Major()
{
  Major::setLimit(FORT_LIMIT); //adjust the land limit
  setVacancy(FORT_CAPACITY); //set the vacancy of the village, it starts out empty
  Major::setType(FORT); //set the type of the structure
  for (int resource=WOOD; resource<=PRODUCE; resource+=1)
  {
    materials.push_back(0); //initialize the list of materials, the village starts out with none
  }
}

//isValid
bool Fort::isValid(Land* base)
{
  switch (base->getType())
  {
    case SEA:
    case SPACE:
      return false;
    default:
      return true;
  }
}

//setVacancy
void Fort::setVacancy(int num)
{
  if (num<=FORT_CAPACITY)
  {
    Major::setVacancy(num);
  }
  else
  {
    std::cout<<"Tried setting the vacancy of a fort to a number ["<<num
      <<"greater than the fort capacity ["<<FORT_CAPACITY<<"\n";
  }
}

//setMaterials
void Fort::setMaterials(Material resource, int num)
{ //this function might need significant altering after further consideration of how materials will work
  if (num>=0)
  {
    materials.at(resource)=num;
  }
}

//getMaterials
int Fort::getMaterials(Material resource)
{
  return materials.at(resource);
}

int Fort::getMaterials(int resource)
{
  return materials.at(static_cast<Material>(resource));
}

//trainTownsfolk
void Fort::trainTownsfolk()
{
  Menu train_menu=Menu("Choose trade to train townsfolk in");
  train_menu.addOption({"farmer", "lumberjack", "miner", "shepherd", "stone mason", "soldier"});
  train_menu.display();

  int input=*train_menu.getChoice();
  if ((input>0)&&(input<=6)) //input needs to be between 0 and 6 because there are 6 trades
  {
    std::cout<<"This "<<getTypeString()<<" has "<<getPopulation(TOWNSFOLK)<<" townsfolk\n"
      <<"how many would you like to train: "; //get the player input

    int* num_ptr=getInt();
    if (num_ptr==nullptr) //make sure the player entered a valid input
    {
      std::cout<<"\033[31mthat is not a valid input\033[0m\n";
      return;
    }

    int num_to_train=*num_ptr;
    if (num_to_train<0) //make sure the player entered a positive number
    {
      std::cout<<"\033[31mNumber must be positive\033[0m\n";
    }
    else if (num_to_train>getPopulation(TOWNSFOLK)) //make sure that the castle has that many townsfolk
    {
      std::cout<<"\033[31mThis "<<getTypeString()<<" does not have that many townsfolk\033[0m\n";
    }
    else
    {
      Career profession=static_cast<Career>(input-1); //cast the input integer to a Career enumerated type
      setPopulation(profession, getPopulation(profession)+num_to_train); //increase the population of tradesmen
      setPopulation(TOWNSFOLK, getPopulation(TOWNSFOLK)-num_to_train); //decrease the population of townsfolk
    }
  }
  else
  {
    std::cout<<"\033[31mPlease input a valid trade\033[0m\n";
  }
}

//getDefense
double Fort::getDefense()
{
  return FORT_DEFENSE;
}

//getData
std::string Fort::getData()
{
  std::string data="";
  //------------------------Major structure data--------------------------
  data+=this->Major::getData(); //call the base class function first
  return data;
}
