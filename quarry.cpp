/*Class quarry object file
 */

#include <iostream>

#include "quarry.h"
#include "major.h"
#include "helpful.h"
#include "settings.h"


//constructor
Quarry::Quarry(): Minor()
{
  setType(QUARRY);
}

//endProduction
void Quarry::endProduction()
{
  if (Minor::getProducing()==true) //if the mine is in fact producing
  {

    if (getKeep()->isOccupied())
    {
      setProducing(false);
      std::cout<<"production ended at "<<getKeep()->getName()<<" due to invasion\n";
      return;
    }
    time_t now=time(0); //get the current time
    if (now-getTime()>QUARRY_PRODUCTION) //if enough time has elapsed
    {
      setProducing(false); //stop production
      int inventory=(getKeep())->getMaterials(STONE); //get the number of materials the keep currently has
      int tradesmen=(getKeep())->getPopulation(STONE_MASON); //get the population of miners
      (getKeep())->setMaterials(STONE, inventory+QUARRY_MULTIPLIER*tradesmen);
      std::cout<<"Finished production in a quarry attached to "<<getKeep()->getName()
        <<", added "<<QUARRY_MULTIPLIER*tradesmen<<" stone to inventory\n";
      //for metal, the number of materials produced is equal to the number of tradesmen
    }
  }
}
