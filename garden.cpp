/*Class Garden object file
 */

#include <iostream>

#include "garden.h"
#include "major.h"
#include "helpful.h"
#include "settings.h"


//constructor
Garden::Garden(): Minor()
{
  setType(GARDEN);
}

//endProduction
void Garden::endProduction()
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
    if (now-getTime()>GARDEN_PRODUCTION) //if enough time has elapsed
    {
      setProducing(false); //stop production
      int inventory=(getKeep())->getMaterials(PRODUCE); //get the number of materials the keep currently has
      int tradesmen=(getKeep())->getPopulation(FARMER); //get the population of miners
      (getKeep())->setMaterials(PRODUCE, inventory+GARDEN_MULTIPLIER*tradesmen);
      std::cout<<"Finished production in a garden attached to "<<getKeep()->getName()
        <<", added "<<GARDEN_MULTIPLIER*tradesmen<<" produce to inventory\n";
      //for metal, the number of materials produced is equal to the number of tradesmen
    }
  }
}
