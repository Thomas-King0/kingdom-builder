/*Class Sawmill object file
 */

#include <iostream>

#include "sawmill.h"
#include "major.h"
#include "helpful.h"
#include "settings.h"


//constructor
Sawmill::Sawmill(): Minor()
{
  setType(SAWMILL);
}

//endProduction
void Sawmill::endProduction()
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
    if (now-getTime()>SAWMILL_PRODUCTION) //if enough time has elapsed
    {
      setProducing(false); //stop production
      int inventory=(getKeep())->getMaterials(WOOD); //get the number of materials the keep currently has
      int tradesmen=(getKeep())->getPopulation(LUMBERJACK); //get the population of miners
      (getKeep())->setMaterials(WOOD, inventory+SAWMILL_MULTIPLIER*tradesmen);
      std::cout<<"Finished production in a sawmill attached to "<<getKeep()->getName()
        <<", added "<<SAWMILL_MULTIPLIER*tradesmen<<" wood to inventory\n";
      //for metal, the number of materials produced is equal to the number of tradesmen
    }
  }
}
