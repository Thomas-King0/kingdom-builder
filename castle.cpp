/*Object file for Castle class
 */

#include <iostream>

#include "castle.h"

#include "minor.h"
#include "land.h"
#include "helpful.h"
#include "settings.h"
#include "menu.h"

//constructor
Castle::Castle(): Major()
{
  Major::setLimit(CASTLE_LIMIT); //adjust the land limit
  setVacancy(CASTLE_CAPACITY); //set the vacancy of the village, it starts out empty
  Major::setType(CASTLE); //set the type of the structure
}

//isValid
bool Castle::isValid(Land* base) //return whether the input land is a valid base
{
  switch (base->getType())
  {
    case FOREST: //castles can go on forest and fields
    case FIELDS:
      return true;
    default:
      return false;
  }
}

//setVacancy
void Castle::setVacancy(int num) //set the amount of space the castle has left
{
  if (num<=CASTLE_CAPACITY)
  {
    Major::setVacancy(num);
  }
  else
  {
    std::cout<<"Tried setting the vacancy of a castle to a number ["<<num
      <<"greater than the castle capacity ["<<CASTLE_CAPACITY<<"\n";
  }
}

//trainTownsfolk
void Castle::trainTownsfolk()
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
double Castle::getDefense()
{
  return CASTLE_DEFENSE;
}

//getData
std::string Castle::getData()
{
  std::string data="";
  //------------------------Major structure data--------------------------
  data+=this->Major::getData(); //call the base class function first

  //materials data
  data+="\033[38;5;200m  ********Inventory********\033[0m\n";
  for (int i=PRODUCE; i<VACUUM; i+=1)
  {
    data+=("  "+fillString(material2string(static_cast <Material>(i))+":",20)+int2string(getMaterials(i))+"\n");
  }

  return data;
}
