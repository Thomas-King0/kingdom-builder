/*Class Pasture object file
 */

#include <iostream>

#include "pasture.h"
#include "major.h"
#include "helpful.h"
#include "settings.h"


//constructor
Pasture::Pasture(): Minor()
{
  setType(PASTURE);
}

//endProduction
void Pasture::endProduction()
{
  if (Minor::getProducing()==true) //if the pasture is in fact producing
  {
    if (getKeep()->isOccupied())
    {
      setProducing(false);
      std::cout<<"production ended at "<<getKeep()->getName()<<" due to invasion\n";
      return;
    }
    time_t now=time(0); //get the current time
    if (now-getTime()>PASTURE_PRODUCTION) //if enough time has elapsed
    {
      setProducing(false); //stop production
      int inventory=(getKeep())->getMaterials(ANIMALS); //get the number of materials the keep currently has
      int tradesmen=(getKeep())->getPopulation(SHEPHERD); //get the population of miners
      (getKeep())->setMaterials(ANIMALS, inventory+4*tradesmen);
      std::cout<<"Finished production in a pasture attached to "<<getKeep()->getName()
        <<", added "<<4*tradesmen<<" animals to inventory\n";
      //for animals, the number of materials produced is equal to 4 times the number of tradesmen
    }
  }
}
