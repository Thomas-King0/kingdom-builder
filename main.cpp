/*kingdom_builder
 * this is the kingdom builder main file
 */

#include <iostream>
#include <vector>
#include <string>
#include <ctime>

#include "land.h"
#include "major.h"
#include "minor.h"
#include "castle.h"
#include "village.h"
#include "mine.h"
#include "pasture.h"
#include "quarry.h"
#include "sawmill.h"
#include "garden.h"
#include "road.h"
#include "caravan.h"
#include "barbarian.h"
#include "helpful.h"
#include "settings.h"
#include "menu.h"

using namespace std;

//int Barbarian::tribeCount=0; //initialize barbarian counter

int main(void)
{
  //seed the random number generation
  srand(37);

  //Land objects
  vector <Land> test_land;
  for (int i=0; i<power((2*MAP_SIZE+1),2) ;i+=1)
  {
    test_land.push_back(Land());
  }
  //int land_found=1; //initilaize to 1, because 1 tile starts out explored

  test_land[pos2index(MAP_SIZE,MAP_SIZE)].setType(FOREST);
  test_land[pos2index(MAP_SIZE, MAP_SIZE)].setX(MAP_SIZE); //MAP_SIZE:c_cen=0:p_cen
  test_land[pos2index(MAP_SIZE, MAP_SIZE)].setY(MAP_SIZE); //see above

  //structure list
  std::cout<<"first you need to choose a name for your starting castle\n";
  vector <Major*> structure_list;
  structure_list.push_back(new Castle());

  //roads
  vector <Road*> highway;

  //Mines objects
  vector <Minor*> minor_list;

  //barbarian tribes
  vector<Barbarian*> tribes;
  bool barbarian_invasion=false; //store whether there are currently any barbarians in the kingdom
  //Village steppes=Village(); //give the barbarians a beginning structure tile to start from
  //this is needed to that they can attack since attacking passes pointers to the base tile
  bool start_attack=false;
  bool spread=false;
  int wins=0;


  structure_list[0]->setBase(&test_land[pos2index(MAP_SIZE,MAP_SIZE)]);
  structure_list[0]->setPopulation(TOWNSFOLK,VILLAGE_CAPACITY*2 ); //50); this is just a test
  structure_list[0]->setMaterials(WOOD, 100);
  structure_list[0]->setMaterials(ANIMALS, 100);
  structure_list[0]->setMaterials(METAL, 50);
  structure_list[0]->setMaterials(STONE, 100);
  structure_list[0]->setMaterials(PRODUCE, 200);
  structure_list[0]->changeName();

  //structure_list[1]->setBase(&test_land[pos2index(MAP_SIZE+3, MAP_SIZE)]);

  //menus
  vector<string> action_options={"produce", "build", "train townsfolk", "display data", "display map", "explore", "claim land", "list structure",
    "transport cargo", "wait", "attack", "rename a structure", "quit"};
  Menu action_menu=Menu("Choose an action", action_options);
  action_menu.setColor(105);

  vector<string> build_options={"mine", "pasture", "quarry", "sawmill", "garden", "castle", "fort", "village", "road"}; //options for what the user can build
  Menu build_menu=Menu("What are you going to build", build_options);
  build_menu.setColor(105);

  bool cont=true;
  while (cont==true)
  {
    //get player action
    /*
    cout<<"what would you like to do:\n\t1. produce\n\t2. build\n\t3. train townsfolk\n\t4. display data\n\t"
      <<"5. display map\n\t6. explore\n\t7. claim land\n\t8. list structure\n\t9. transport cargo\n\t"
      <<"10. wait\n\t11. attack\n\t12. rename a structure\n\t13. quit\n";
    */
    action_menu.display();
    int action=0;
    int* action_ptr=action_menu.getChoice();
    if (action_ptr!=nullptr)
    {
      action=*action_ptr;
    }
    else
    {
      continue;
    }

    switch (action)
    {
      case (1): //produce //READY FOR INITIAL PLAYTEST
      {
        //cout<<"where would you like to produce:\n";
        Menu production_menu=Menu("Choose a structure to start production");
        production_menu.setColor(208);

        production_menu.addOption(listStructures(&structure_list)); //add the list of structures to the production menu options
        production_menu.display();

        int index;
        int* choice_ptr=production_menu.getChoice();
        if (choice_ptr!=nullptr)
        {
          index=*choice_ptr;
        }
        else
        {
          break;
        }

        Major* keep_ptr=structure_list.at(index-1); //getStructure(name, &structure_list);
        if (keep_ptr->canProduce()==false) //if the structure cannot produce
        {
          cout<<"That structure cannot produce\n";
          break;
        }
        keep_ptr->startProduction(); //start production in structure_list
        break;
      }
      case (2): //build
      {
        build_menu.display();
        int action_2=0;
        int* choice_ptr=build_menu.getChoice();
        if (choice_ptr!=nullptr)
        {
          action_2=*choice_ptr;
        }
        else
        {
          break;
        }

        switch (action_2)
        {
          case (1): //mine
          {
            cout<<"To build a mine costs 3 animals, 3 wood, and 5 metal\n"
              <<"do you want to continue\n"<<"1. continue\n2. exit\n";
            int* cont_ptr=getInt();
            if (cont_ptr==nullptr)
            {
              std::cout<<"that is not a valid input\n";
              break;
            }
            int cont=*cont_ptr;
            if (cont==2)
            {
              break;
            }
            else if (cont!=1)
            {
              cout<<cont<<" is not a valid input\n";
              break;
            }
            Menu location_menu=Menu("Where would you like to build this mine");
            location_menu.addOption(listStructures(&structure_list)); //add the list of structures to the production menu options
            location_menu.display();

            int index;
            int* choice_ptr=location_menu.getChoice();
            if (choice_ptr!=nullptr)
            {
              index=*choice_ptr;
            }
            else
            {
              break;
            }

            Major* major_ptr=structure_list.at(index-1); 

            if (canBuildMinor(MINE, major_ptr)) //if the mine can be built
            {
              minor_list.push_back(new Mine());
              major_ptr->addSupport(minor_list.at(minor_list.size()-1));
              //add the mine as a support to the keep
              cout<<"built a mine at "<<major_ptr->getName()<<"\n";
            }
            else
            {
              cout<<"cannot build a mine there\n";
            }
            break;
          }
          case (2): //pasture
          {
            cout<<"To build a pasture costs 5 animals and 5 wood\n"
              <<"do you want to continue\n"<<"1. continue\n2. exit\n";
            int* cont_ptr=getInt();
            if (cont_ptr==nullptr)
            {
              std::cout<<"that is not a valid input\n";
              break;
            }
            int cont=*cont_ptr;
            if (cont==2)
            {
              break;
            }
            else if (cont!=1)
            {
              cout<<cont<<" is not a valid input\n";
              break;
            }
            Menu location_menu=Menu("Where would you like to build this pasture");
            location_menu.addOption(listStructures(&structure_list)); //add the list of structures to the production menu options
            location_menu.display();

            int index;
            int* choice_ptr=location_menu.getChoice();
            if (choice_ptr!=nullptr)
            {
                index=*choice_ptr;
            }
            Major* major_ptr=structure_list.at(index-1); 
            if (major_ptr==nullptr) //if there is no structure with that name
            {
              break;
            }

            if (canBuildMinor(PASTURE, major_ptr)) //if the mine can be built
            {
              minor_list.push_back(new Pasture());
              major_ptr->addSupport(minor_list.at(minor_list.size()-1));
              //add the mine as a support to the keep
              cout<<"built a pasture at "<<major_ptr->getName()<<"\n";
            }
            else
            {
              cout<<"cannot build a pasture there\n";
            }
            break;
          }
          case (3): //quarry
          {
            cout<<"To build a quarry costs 5 produce, 2 animals, 5 wood, and 5 metal\n"
              <<"do you want to continue\n"<<"1. continue\n2. exit\n";
            int* cont_ptr=getInt();
            if (cont_ptr==nullptr)
            {
              std::cout<<"that is not a valid input\n";
              break;
            }
            int cont=*cont_ptr;
            if (cont==2)
            {
              break;
            }
            else if (cont!=1)
            {
              cout<<cont<<" is not a valid input\n";
              break;
            }
            Menu location_menu=Menu("Where would you like to build this quarry");
            location_menu.addOption(listStructures(&structure_list)); //add the list of structures to the production menu options
            location_menu.display();

            int index;
            int* choice_ptr=location_menu.getChoice();
            if (choice_ptr!=nullptr)
            {
              index=*choice_ptr;
            }
            Major* major_ptr=structure_list.at(index-1); 

            if (canBuildMinor(QUARRY, major_ptr)) //if the mine can be built
            {
              minor_list.push_back(new Quarry());
              major_ptr->addSupport(minor_list.at(minor_list.size()-1));
              //add the mine as a support to the keep
              cout<<"built a quarry at "<<major_ptr->getName()<<"\n";
            }
            else
            {
              cout<<"cannot build a quarry there\n";
            }
            break;
          }
          case (4): //sawmill
          {
            cout<<"To build a sawmill costs 7 wood, 3 stone, and 2 metal\n"
              <<"do you want to continue\n"<<"1. continue\n2. exit\n";
            int* cont_ptr=getInt();
            if (cont_ptr==nullptr)
            {
              std::cout<<"that is not a valid input\n";
              break;
            }
            int cont=*cont_ptr;
            if (cont==2)
            {
              break;
            }
            else if (cont!=1)
            {
              cout<<cont<<" is not a valid input\n";
              break;
            }
            Menu location_menu=Menu("Where would you like to build this sawmill");
            location_menu.addOption(listStructures(&structure_list)); //add the list of structures to the production menu options
            location_menu.display();

            int index;
            int* choice_ptr=location_menu.getChoice();
            if (choice_ptr!=nullptr)
            {
              index=*choice_ptr;
            }
            Major* major_ptr=structure_list.at(index-1); 

            if (major_ptr==nullptr) //if there is no structure with that name
            {
              break;
            }

            if (canBuildMinor(SAWMILL, major_ptr)) //if the mine can be built
            {
              minor_list.push_back(new Sawmill());
              major_ptr->addSupport(minor_list.at(minor_list.size()-1));
              //add the mine as a support to the keep
              cout<<"built a sawmill at "<<major_ptr->getName()<<"\n";
            }
            else
            {
              cout<<"cannot build a sawmill there\n";
            }
            break;
          }
          case (5): //garden
          {
            cout<<"To build a garden costs 5 produce, 2 wood\n"
              <<"do you want to continue\n"<<"1. continue\n2. exit\n";
            int* cont_ptr=getInt();
            if (cont_ptr==nullptr)
            {
              std::cout<<"that is not a valid input\n";
              break;
            }
            int cont=*cont_ptr;
            if (cont==2)
            {
              break;
            }
            else if (cont!=1)
            {
              cout<<cont<<" is not a valid input\n";
              break;
            }
            Menu location_menu=Menu("Where would you like to build this garden");
            location_menu.addOption(listStructures(&structure_list)); //add the list of structures to the production menu options
            location_menu.display();

            int index;
            int* choice_ptr=location_menu.getChoice();
            if (choice_ptr!=nullptr)
            {
              index=*choice_ptr;
            }
            Major* major_ptr=structure_list.at(index-1); 
            if (major_ptr==nullptr) //if there is no structure with that name
            {
              break;
            }

            if (canBuildMinor(GARDEN, major_ptr)) //if the mine can be built
            {
              minor_list.push_back(new Garden());
              major_ptr->addSupport(minor_list.at(minor_list.size()-1));
              //add the mine as a support to the keep
              cout<<"built a garden at "<<major_ptr->getName()<<"\n";
            }
            else
            {
              cout<<"cannot build a garden there\n";
            }
            break;
          }
          case (6): //castle
          {
            cout<<"To build a castle costs 20 stone, 10 wood, 5 metal, and 5 animals\n"
              <<"do you want to continue\n"<<"1. continue\n2. exit\n";
            int* cont_ptr=getInt();
            if (cont_ptr==nullptr)
            {
              std::cout<<"that is not a valid input\n";
              break;
            }
            int cont=*cont_ptr;
            if (cont==2)
            {
              break;
            }
            else if (cont!=1)
            {
              cout<<cont<<" is not a valid input\n";
              break;
            }
            cout<<"Enter coordinates for castle: ";
            int coord_x;
            int coord_y;
            getCoords(&coord_x, &coord_y);
            buildMajor(CASTLE, &test_land[pos2index(coord_x, coord_y)],&structure_list);
            break;
          }
          case (7): //fort
          {
            cout<<"To build a fort costs 2 animals, 10 wood, and 20 stone\n"
              <<"do you want to continue\n"<<"1. continue\n2. exit\n";
            int* cont_ptr=getInt();
            if (cont_ptr==nullptr)
            {
              std::cout<<"that is not a valid input\n";
              break;
            }
            int cont=*cont_ptr;
            if (cont==2)
            {
              break;
            }
            else if (cont!=1)
            {
              cout<<cont<<" is not a valid input\n";
              break;
            }
            cout<<"Enter coordinates for fort: ";
            int coord_x;
            int coord_y;
            getCoords(&coord_x, &coord_y);
            buildMajor(FORT, &test_land[pos2index(coord_x, coord_y)],&structure_list);
            break;
          }
          case (8): //village
          {
            cout<<"To build a village costs 5 wood and 2 animals\ndo you want to continue\n"
              <<"1. continue\n2. exit\n";
            int* cont_ptr=getInt();
            if (cont_ptr==nullptr)
            {
              std::cout<<"that is not a valid input\n";
              break;
            }
            int cont=*cont_ptr;
            if (cont==2)
            {
              break;
            }
            else if (cont!=1)
            {
              cout<<cont<<" is not a valid input\n";
            }
            cout<<"Enter coordinates for village: ";
            int coord_x;
            int coord_y;
            getCoords(&coord_x, &coord_y);
            buildMajor(VILLAGE, &test_land[pos2index(coord_x, coord_y)],&structure_list);
            break;
          }
          case (9): //road
          {
            Menu location_menu=Menu("Choose a starting and ending location");
            location_menu.addOption(listStructures(&structure_list));
            location_menu.display();
            int location1=0;
            int* choice_ptr=location_menu.getChoice();
            if (choice_ptr!=nullptr)
            {
              location1=*choice_ptr;
            }
            else
            {
              break;
            }
            int location2=0;
            choice_ptr=location_menu.getChoice();
            if (choice_ptr!=nullptr)
            {
              location2=*choice_ptr;
            }
            else
            {
              break;
            }

            Major* start_ptr=nullptr;
            Major* end_ptr=nullptr;
            if (location1!=location2) //make sure that the endpoints are different
            {
              start_ptr=structure_list.at(location1-1); //getStructure(location1, &structure_list); //get the pointer to the start
              end_ptr=structure_list.at(location2-1); //getStructure(location2, &structure_list); //get the pointer to the end
            }
            else
            {
              cout<<"road endpoints cannot be the same location\n";
              break;
            }

            int cost=3*dist(start_ptr, end_ptr); //get the cost for the road (3 stone per unit length)

            if ((start_ptr==nullptr)||(end_ptr==nullptr))
            // if one of the names was invalid then the corresponding pointer will be the nullptr
            {
              cout<<"Entered an invalid name\n";
              break;
            }
            else if ((start_ptr->isOccupied())||(end_ptr->isOccupied()))
            {
              cout<<"cannot build a road to a structure that is occupied\n";
              break;
            }
            else if (start_ptr->getMaterials(STONE)<cost)
            {
              cout<<"that "<<start_ptr->getTypeString()<<" does not have enough resources for a road that long\n";
              break;
            }
            else
            {
              cout<<"this road will cost "<<cost<<" stone to build\n 1. continue\n 2. exit\n";
              int* choice_ptr=getInt();
              int choice;
              if (choice_ptr==nullptr)
              {
                break;
              }
              else
              {
                choice=*choice_ptr; 
              }

              if (choice!=1)
              {
                break;
              }

              int current_stone=start_ptr->getMaterials(STONE);
              start_ptr->setMaterials(STONE, current_stone-cost);
              highway.push_back(new Road(start_ptr, end_ptr)); //push back the new road
              cout<<"built a road between "<<start_ptr->getName()<<" and "
                <<end_ptr->getName()<<"\n";
            }
            break;
          }
          default:
          {
            cout<<action_2<<" does not designate a valid structure"<<"\n";
            break;
          }
        }
        break;
      }
      case (3): //train townsfolk
      {
        Menu train_menu=Menu("Where would you like to train townsfolk");
        train_menu.addOption(listStructures(&structure_list));
        train_menu.display();
/*
        cout<<"where are the townsfolk that you want to train:\n";
        string name;
        getName(&name); //get the name of the stucture
*/
        int index;
        int* choice_ptr=train_menu.getChoice();
        if (choice_ptr!=nullptr)
        {
          index=*choice_ptr;
        }
        Major* major_ptr=structure_list.at(index-1); //getStructure(name, &structure_list); //get the pointer to the major
        if ((major_ptr!=nullptr)&&(!major_ptr->isOccupied())) //make sure the najor exists and is not occupied
        {
          major_ptr->trainTownsfolk();
        }
        break;
      }
      case (4): //display data
      {
        Menu data_menu=Menu("Choose structure to display data");
        data_menu.addOption(listStructures(&structure_list));
        data_menu.display();
/*
        cout<<"What structure would you like to display data for: ";
        string name;
        getName(&name);
*/

        int index;
        int* choice_ptr=data_menu.getChoice();
        if (choice_ptr!=nullptr)
        {
          index=*choice_ptr;
        }
        Major* major_ptr=structure_list.at(index-1); //getStructure(name, &structure_list); //get a pointer to the structure
        if (major_ptr!=nullptr)
        {
          cout<<major_ptr->getData()<<"\n"; //display the data
        }
        break;
      }
      case (5): //display map //READY FOR INITIAL PLAYTEST
      {
        Menu map_menu=Menu("Choose structure to center on");
        map_menu.addOption(listStructures(&structure_list));
        map_menu.display();
/*
        cout<<"enter name of structure to center on\n";
        string name;
        getName(&name);
*/
        
        int index;
        int* choice_ptr=map_menu.getChoice();
        if (choice_ptr!=nullptr)
        {
          index=*choice_ptr;
        }
        Major* major_ptr=structure_list.at(index-1); //getStructure(name, &structure_list);
        if (major_ptr==nullptr)
        {
          break;
        }
        int coord_x=major_ptr->getX();
        int coord_y=major_ptr->getY();

        displayMap(coord_x,coord_y, &(test_land));
        break;
      }
      case (6): //explore
      {
        cout<<"enter coordinates for tile to start exploring from in\n";
        int start_x=0;
        int start_y=0;
        getCoords(&start_x, &start_y);


        //check distance to starting point
        Major* major_ptr=closestStructure( start_x, start_y, &structure_list);

        if (major_ptr->dist(start_x, start_y)>BUILD_DISTANCE)
        {
          cout<<"That tile is too far away\n";
          break;
        }
        else if (major_ptr->isOccupied())
        {
          cout<<"cannot explore from an occupied structure\n";
          break;
        }

        //get direction
        Menu direction_menu=Menu("Choose direction to explore");
        direction_menu.addOption({"north", "east", "south", "west"});
        direction_menu.display();

        int direction;
        int* choice_ptr=direction_menu.getChoice();
        if (choice_ptr!=nullptr)
        {
          direction=*choice_ptr;
        }
        int index=pos2index(start_x, start_y); //convert the player entered coordinates to an index
        if (test_land[index].getType()!=SPACE) //as long as the starting tile is explored
        {
          explore(&(test_land), index, static_cast<Direction>(direction-1), EXPLORE_LIMIT);
        }
        else
        {
          cout<<"the land at "<<displayCoords(start_x, start_y)<<" has not been explored yet\n";
        }
        break;
      } //end explore
      case(7): //claim land //READY FOR INITIAL PLAYTEST
      {
        Menu claim_menu=Menu("Choose structure to claim land for");
        claim_menu.addOption(listStructures(&structure_list));
        claim_menu.display();
/*
        cout<<"What structure are you claiming the land for: ";
        string name;
        getName(&name);
*/
        int index;
        int* choice_ptr=claim_menu.getChoice();
        if (choice_ptr!=nullptr)
        {
          index=*choice_ptr;
        }
        Major* major_ptr=structure_list.at(index-1); //getStructure(name, &structure_list); //get the structure

        if (major_ptr==nullptr) //check for a valid name
        {
          break;
        }
        else if (major_ptr->isOccupied())
        {
          cout<<"an occupied structure cannot claim land\n";
          break;
        }

        //coordinates of potential claim
        int land_x;
        int land_y;


        if (major_ptr->getClaimSize()<major_ptr->getLimit())
        //make sure that the major has enough space to make the claim
        {
          cout<<"Enter coordinates of land you would like to claim: ";
          getCoords(&land_x, &land_y);
          Land* claim_ptr=&test_land[pos2index(land_x, land_y)];
          if (major_ptr->canClaim(claim_ptr)) //if the major can claim it
          {
            major_ptr->addClaim(claim_ptr);
          }
        }
        else
        {
          cout<<"That "<<major_ptr->getTypeString()<<" has already maxed out its claims\n";
        }
        break;
      }
      case (8): //list structures
      {
        Menu list_menu=Menu("List of Structures");
        for (int i=0; i<structure_list.size(); i+=1)
        {
          Major* major_ptr=structure_list.at(i);
          list_menu.addOption(fillString(major_ptr->getName()+": ", 20)+major_ptr->getTypeString());
        }
        list_menu.display();
        break;
      }
      case (9): //transport
      {
        Menu transport_menu=Menu("Select location to send caravan from");
        transport_menu.addOption(listStructures(&structure_list));
        transport_menu.display();
/*
        cout<<"where is this caravan coming from: ";
        string sender;
        getName(&sender);
*/
        int index;
        int* choice_ptr=transport_menu.getChoice();
        if (choice_ptr!=nullptr)
        {
          index=*choice_ptr;
        }
        Major* sender_ptr=structure_list.at(index-1); //getStructure(sender, &structure_list);

        if (sender_ptr!=nullptr) //if the chosen structure exists
        {
          if (sender_ptr->isOccupied()) //if the chosen structure is not occupied
          {
            cout<<"cannot send a caravan from an occupied structure\n";
          }
          else
          {
            sender_ptr->load();
          }
        }
        else
        {
          cout<<"there is no structure with that name\n";
        }
        break;
      }
      case (10): //wait
      {
        break;
      }
      case (11): //attack a tribe
      {
        //**********************************get the location of the defenders***********************************
        Menu attack_menu=Menu("Choose location to attack with");
        attack_menu.addOption(listStructures(&structure_list));
        attack_menu.display();
/*
        cout<<"Where are you attacking:\n";
        string target_name;
        getName(&target_name);
*/
        int index;
        int* choice_ptr=attack_menu.getChoice();
        if (choice_ptr!=nullptr)
        {
          index=*choice_ptr;
        }
        Major* attacker_ptr=structure_list.at(index-1); //getStructure(target_name, &structure_list);
        if (attacker_ptr==nullptr)
        {
          cout<<"there is no structure with that name\n";
          break;
        }
        else if (attacker_ptr->isOccupied())
        {
          cout<<"cannot attack from an occupied structure\n";
          break;
        }


        //*********************************get the location of the attackers**********************************
        Menu target_menu=Menu("Choose location to attack");
        target_menu.addOption(listStructures(&structure_list));
        target_menu.display();
/*
        cout<<"Where are you attacking from:\n";
        string attacker_name;
        getName(&attacker_name);
*/
        int index2;
        choice_ptr=target_menu.getChoice();
        if (choice_ptr!=nullptr)
        {
          index2=*choice_ptr;
        }
        Major* target_ptr=structure_list.at(index2-1); //getStructure(attacker_name, &structure_list);
        if (target_ptr==nullptr)
        {
          cout<<"there is no structure with that name\n";
        }

        //************************************************attack***********************************************
        if (target_ptr->isOccupied()) //ensure that there is no friendly fire
        {
          int result=attacker_ptr->attack(target_ptr);
          if (result==1)
          {
            cout<<"you defeated the barbarians at "<<target_ptr->getName()<<"\n";
            for (int i=0; i<Barbarian::tribe_count; i+=1) //remove the pointer from the barbarian vector
            {
              if (tribes.at(i)->getKeep()==target_ptr) //if the current tribe is the one that was defeated
              {
                Barbarian* tribe_ptr=tribes.at(i); //make an alias for the tribe at that index
                tribes.erase(tribes.begin()+i); //remove it from the vector
                delete tribe_ptr; //delete the pointer
                break;
              }
            }
            Barbarian::tribe_count-=1; //decrement the tribe counter
            if (Barbarian::tribe_count==0) //if there are now no tribes in the kingdom
            {
              wins+=1;
              cout<<"you removed all the barbarian tribes from your kingdom\n";
              cout<<"remove all tribes "<<3-wins<<"more times to win\n";
            }
            target_ptr->setOccupants(nullptr);
          }
          else if (result==0)
          {
            cout<<"the barbarians won\n";
          }
        }
        else
        {
          cout<<"that structure is not occupied by a barbarian tribe\n";
        }
        break;
      }
      case (12): //change name
      {
        Menu name_menu=Menu("Choose structure to rename");
        name_menu.addOption(listStructures(&structure_list));
        name_menu.display();

        int index;
        int* choice_ptr=name_menu.getChoice();
        if (choice_ptr!=nullptr)
        {
          index=*choice_ptr;
        }
        Major* major_ptr=structure_list.at(index-1); //getStructure(name, &structure_list);
        if (major_ptr==nullptr)
        {
          cout<<"There is no structure with that name\n";
          break;
        }

        cout<<"What do you want to rename it: ";
        string new_name;
        getName(&new_name);
        if (getStructure(new_name, &structure_list)==nullptr)
        {
          major_ptr->setName(new_name);
        }
        else
        {
          cout<<"there is already a structure with that name\n";
        }
        break;
      }
      case (13): //quit
      {
        cont=false;
        break;
      }
      /*
      case (14): //this is only for testing
      {
        cout<<"Debug: set start_attack=true;\n";
        start_attack=true;
        break;
      }/*
      case (100): //this is only for testing
      {
        cout<<"Debug: set spread=true;\n";
        spread=true;
        break;
      }
      case (200): //testing again
      {
        cout<<"where would you like to add soldiers: ";
        string sender;
        getName(&sender);

        Major* sender_ptr=getStructure(sender, &structure_list);
        int num;
        cout<<"how many soldiers would you like it to have: ";
        cin>>num;
        sender_ptr->setPopulation(SOLDIER, num);
        break;
      }*/
      default:
      {
        cout<<action<<" not a valid action: please choose a valid action\n";
        break;
      }
    }



//**************************check for end of production and increase population of townsfolk****************************
    for (int i=0;i<structure_list.size();i+=1)
    {
      if (structure_list[i]->isOccupied()==false)
      {
        //check for end of production
        structure_list[i]->endProduction();

        //increase population

        int random_number=rand()%100; //generate a random number from 0-99
        if (random_number<BIRTH_FREQUENCY) //give any one village a 5% chance of increase
        {
          structure_list[i]->addTownsfolk();
          cout<<"added townsfolk to "<<structure_list[i]->getName()<<"\n";
        }
      }
      else
      {
        cout<<structure_list[i]->getName()<<" is occupied\n";
      }
    }



//***********************************check for end of journey****************************************
    for (int i=0; i<highway.size(); i+=1)
    {
      highway[i]->endJourney(); //try to end the journey
    }



//***************************************check barbarians********************************************
    if (Barbarian::tribe_count==0) //if there are not currently barbarians
    {
      //cout<<"Checking for barbarian spawn\n";
      int random_number=rand()%100; //generate random number
      //cout<<"random number is "<<random_number<<"\n";
      if  (random_number<5) //||(start_attack))//check if the barbarians spawn
      {
        Major* major_ptr=furthestStructure(&structure_list); //get the furthest structure
        cout<<"A barbarian tribe is attacking "<<major_ptr->getName()<<"\n"; //tell the player about the attack
        pause(1);
        Village steppes=Village(); //give the barbarians a beginning structure tile to start from
        Barbarian* tribe_ptr=new Barbarian(&steppes);
        tribe_ptr->setPopulation(25);
        if (tribe_ptr->getKeep()->attack(major_ptr))
        {
          cout<<"the barbarians won\n";
          tribe_ptr->setKeep(major_ptr); //set the major as the keep
          tribes.push_back(tribe_ptr); //initialize the barbarians
          barbarian_invasion=true;
        }
        else
        {
          cout<<"you fended off the barbarians\n";
          Barbarian::tribe_count-=1; //decrement the tribe counter
        }
        start_attack=false;
      }
    }
    else //if there are barbarians
    {
      int current_tribe_count=Barbarian::tribe_count;
      //this variable is so that if any of the tribes spread, the new tribes are also not iterated over
      for (int i=0; i<current_tribe_count; i+=1)
      {
        //increase population
        tribes.at(i)->increasePopulation(); //try to increase the population of all tribes

        //pillage
        tribes.at(i)->pillage(); //try to pillage with all tribes

        //spread
        double chance=(rand()%100+1)/100.0; //get a double between 0 and 1
        if ((chance<Barbarian::spread_chance)&&(tribes.at(i)->getPopulation()>BARBARIAN_THRESHOLD))
        //if the number is low enough so the barbarians spread
        //if the barbarian tribe has enough people to spread
        {
          cout<<"the barbarians are spreading\n";
          Major* target_ptr=tribes.at(i)->getTarget(); //get the next target for the tribe
          if (target_ptr==nullptr)
          {
            Major* current_ptr=tribes.at(i)->getKeep(); //get the current keep
            int coord_x=current_ptr->getX();
            int coord_y=current_ptr->getY();
            target_ptr=closestStructure(coord_x, coord_y, &structure_list);
            if (target_ptr->isOccupied()) //if the target already has a tribe
            {
              cout<<"The barbarians cannot spread because all the structures around them are occupied\n";
              continue;
            }
          }
          cout<<"the target for the tribe at "<<tribes.at(i)->getKeep()->getName()<<" is "
            <<target_ptr->getName()<<"\n";

          if (tribes.at(i)->getKeep()->attack(target_ptr))
          {
            cout<<"the barbarians won\n";
            Barbarian* tribe_ptr=new Barbarian(target_ptr); //initialize the new tribe
            int roamers=tribes.at(i)->getPopulation()*0.1; //the number of barbarians moving to the new keep
            tribe_ptr->setPopulation(roamers); //set the population of the new tribe
            tribes.at(i)->setPopulation(tribes.at(i)->getPopulation()-roamers);
            //set new population of the old tribe
            cout<<roamers<<" barbarians have moved to "<<target_ptr->getName()<<"\n";
            tribes.push_back(tribe_ptr);
          }
          else
          {
            cout<<"you fended off the barbarians\n";
          }
        }
      }
      spread=false;
    }
//********************************************check for end of game**************************************************
    if (wins==3)
    {
      cout<<"Congratulations! You won! You fended off the barbarian hordes three times, showing that your"
        <<"kingdom is strong enough to protect itself.\n Thanks for playing\n";
      cont=false;
    }
  }
  //delete majors
  for (int i=structure_list.size()-1; i>=0; i-=1)
  {
    delete structure_list.at(i);
  }

  //delete minors
  for (int i=minor_list.size()-1; i>=0; i-=1)
  {
    delete minor_list.at(i);
  }

  //delete roads
  for (int i=highway.size()-1; i>=0; i-=1)
  {
    delete highway.at(i);
  }

  //delete barbarians
  for (int i=Barbarian::tribe_count-1; i>=0; i-=1)
  {
    delete tribes.at(i);
  }

} //end of main
