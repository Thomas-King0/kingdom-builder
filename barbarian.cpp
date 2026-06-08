/*Barbarian class file
 */

#include <iostream>
#include <ctime>

#include "barbarian.h"
#include "major.h"
#include "land.h"
#include "settings.h"

int Barbarian::tribe_count=0; //initialize barbarian counter
double Barbarian::spread_chance=0.05; //initialize chance of spreading

//constructor
Barbarian::Barbarian()
{
  setPopulation(0);
  setKeep(nullptr);
  Barbarian::tribe_count+=1; //increment the tribe counter
}

Barbarian::Barbarian(Major* major_ptr): Barbarian()
{
  setKeep(major_ptr);
}

//setKeep
void Barbarian::setKeep(Major* keep_ptr)
{
  //always allow for a keep to be set to the nullptr
  if (keep_ptr==nullptr) //if the program is trying to set the keep to the nullptr
  {
    keep=nullptr;
    return;
  }

  /*check to make sure that the keep is the nullptr. If it is not the nullptr then the tribe already has a keep and
   * resetting the keep would mean that this tribe would have switched keeps
   */
  //if (keep==nullptr)
  //{
  keep=keep_ptr;
  keep->setOccupants(this);
  keep->endProduction();
  /*}
  else
  {
    std::cout<<"tried setting a keep for a barbarian tribe that already has a keep\n";
    std::cout<<"\tkeep is "<<keep<<"\n";
  }*/
  //the lines above are commented out because barbarians need to be able to switch keeps for when they are first spawned
}

//getKeep
Major* Barbarian::getKeep()
{
  return keep;
}

//getBase
Land* Barbarian::getBase()
{
  return getKeep()->getBase();
}

//setPopulation
void Barbarian::setPopulation(int num)
{
  if (num>=0)
  {
    population=num;
  }
  else
  {
    std::cout<<"tried setting population to a negative number ["<<num<<"] in Barbarian::setPopulation\n";
    std::cout<<"\tsetting population to 0\n";
    population=0;
  }
}

//getPopulation
int Barbarian::getPopulation()
{
  return population;
}

//increasePopulation
void Barbarian::increasePopulation()
{
  Major* major_ptr=getKeep(); //get the keep of the tribe

  int random_number=rand()%100; //generate a random number from 0-99
  if (random_number<(2*BIRTH_FREQUENCY)) //give any one tribe a BIRTH_FREQUENCY% chance of increase
  //BIRTH_FREQUENCY is a setting found in helpful.h
  {
    int current_population=getPopulation(); //get the current population
    setPopulation(current_population*(1+BIRTH_RATE)+1); //increase the population by 16%
    std::cout<<"The barbarian tribe at "<<major_ptr->getName()<<" added "<<(current_population*BIRTH_RATE+1)
      <<" people to their tribe\n";
    //the +1 is so that at least 1 barbarian is added each time
  }
}

//pillage
void Barbarian::pillage()
{
  std::cout<<"The barbarian tribe at "<<getKeep()->getName()<<" has taken:\n";
  //decrease materials
  for (int i=WOOD; i<=PRODUCE; i+=1)
  {
    Major* major_ptr=getKeep(); //get the keep of the tribe

    int random_number=rand()%100; //generate a random number from 0-99
    if (random_number<PILLAGE_FREQUENCY)
    {
      //seedRandom(major_ptr, i+100);
      int amount_taken=rand()%MAX_PILLAGE+1; //get the amount taken
      int current_materials=major_ptr->getMaterials(static_cast<Material>(i));
      major_ptr->setMaterials(static_cast<Material>(i), current_materials-amount_taken);
      std::cout<<amount_taken<<" "<<material2string(static_cast<Material>(i))<<"\n";
    }
  }
  //decrease population
  for (int i=FARMER; i<=TOWNSFOLK; i+=1)
  {
    Major* major_ptr=getKeep(); //get the keep of the tribe

    int random_number=rand()%100; //generate a random number from 0-99
    if (random_number<PILLAGE_FREQUENCY)
    {
      //seedRandom(major_ptr, i+300);
      int amount_taken=rand()%MAX_PILLAGE+1; //get the amount taken
      int current_population=major_ptr->getPopulation(static_cast<Career>(i));
      if (amount_taken<current_population)
      {
        major_ptr->setPopulation(static_cast<Career>(i), current_population-amount_taken);
        std::cout<<amount_taken<<" "<<career2string(static_cast<Career>(i))<<"s\n";
      }
      else
      {
        major_ptr->setPopulation(static_cast<Career>(i), 0);
        std::cout<<current_population<<" "<<career2string(static_cast<Career>(i))<<"s\n";
      }
    }
  }
}

//getTarget
Major* Barbarian::getTarget()
{
  //get the list of connected structures
  Major* keep_ptr=getKeep(); //get the keep

  if (keep_ptr->getHighwaySize()==0)
  {
    return nullptr;
  }
  else if (keep_ptr->isSurrounded()==true)
  {
    return nullptr;
  }
  else
  {
    while (true)
    {
      int random_number=rand()%keep_ptr->getHighwaySize(); //get a random number between 0 and the highway size
      if (keep_ptr->getSister(random_number)->isOccupied()==false)
      {
        return keep_ptr->getSister(random_number); //get the connected structure at that point
      }
    }
  }
  return nullptr; //this is so that there is a return statement at the end of the function
}

//increaseSpreadChance
void Barbarian::increaseSpreadChance()
{
  Barbarian::spread_chance*=(1+SPREAD_INCREASE_RATE); //increase the chance of spread by 25%
}
