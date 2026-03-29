/*Class Mine object file
 */

#include <iostream>

#include "mine.h"
#include "major.h"
#include "helpful.h"


//constructor
Mine::Mine(): Minor()
{
  setType(MINE);
}

//endProduction
void Mine::endProduction()
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
    if (now-getTime()>MINE_PRODUCTION) //if enough time has elapsed
    {
      setProducing(false); //stop production
      int inventory=(getKeep())->getMaterials(METAL); //get the number of materials the keep currently has
      int tradesmen=(getKeep())->getPopulation(MINER); //get the population of miners
      (getKeep())->setMaterials(METAL, inventory+tradesmen);
      std::cout<<"Finished production in a mine attached to "<<getKeep()->getName()
        <<", added "<<tradesmen<<" metal to inventory\n";
      //for metal, the number of materials produced is equal to the number of tradesmen
    }
  }
}
