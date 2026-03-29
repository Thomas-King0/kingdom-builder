/*Object file for Village class
 */

#include <iostream>

#include "village.h"

#include "minor.h"
#include "land.h"
#include "helpful.h"
#include "settings.h"

//constructor
Village::Village(): Major()
{
  Major::setLimit(VILLAGE_LIMIT); //adjust the limit
  setVacancy(VILLAGE_CAPACITY); //set the vacancy of the village, it starts out empty
  Major::setType(VILLAGE); //set the type of the structure
  for (int resource=WOOD; resource<=PRODUCE; resource+=1)
  {
    materials.push_back(0); //initialize the list of materials, the village starts out with none
  }
}

//isValid
bool Village::isValid(Land* base)
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
void Village::setVacancy(int num)
{
  if (num<=VILLAGE_CAPACITY)
  {
    Major::setVacancy(num);
  }
  else
  {
    std::cout<<"Tried setting the vacancy of a village to a number ["<<num
      <<"greater than the village capacity ["<<VILLAGE_CAPACITY<<"\n";
  }
}

//setMaterials
void Village::setMaterials(Material resource, int num)
{ //this function might need significant altering after further consideration of how materials will work
  if (num>=0)
  {
    materials.at(resource)=num;
  }
}

//getMaterials
int Village::getMaterials(Material resource)
{
  return materials.at(resource);
}

int Village::getMaterials(int resource)
{
  return materials.at(static_cast<Material>(resource));
}

//getDefense
double Village::getDefense()
{
  return VILLAGE_DEFENSE;
}

//getData
std::string Village::getData()
{
  std::string data="";
  //------------------------Major structure data--------------------------
  data+=this->Major::getData(); //call the base class function first

  //materials data
  data+="\033[38;5;200m  ********Inventory********\033[0m\n";
  for (int i=WOOD; i<=PRODUCE; i+=1)
  {
    data+=("  "+fillString(material2string(static_cast <Material>(i))+":",20)+int2string(getMaterials(i))+"\n");
  }

  return data;
}
