/*kingdom_builder
 * this is the kingdom builder main file
 */

#include <iostream>
#include <fstream>
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
#include "objectWriter.h"

using namespace std;

//int Barbarian::tribeCount=0; //initialize barbarian counter

int main(void)
{
  /*
  Game information:
  save name
  wins (number of times the barbarians have been expelled
  */
  string save_name; //the name of the current save
  //seed the random number generation
  srand(time(0));

  //initialize data vectors
  vector <Land> test_land;
  for (int i=0; i<power((2*MAP_SIZE+1),2) ;i+=1)
  {
    test_land.push_back(Land());
  }
  vector <Major*> structure_list;
  vector <Road*> highway;
  vector <Minor*> minor_list;
  vector<Barbarian*> tribes;
  bool barbarian_invasion=false; //store whether there are currently any barbarians in the kingdom
  //Village steppes=Village(); //give the barbarians a beginning structure tile to start from
  //this is needed to that they can attack since attacking passes pointers to the base tile
  bool start_attack=false;
  bool spread=false;
  int wins=0;

  //start menu
  Menu start_menu=Menu("Welcome to LTKingdom!");
  start_menu.addOption("new game");
  start_menu.addOption("load game");
  int* option;
  start_menu.display();
  option=start_menu.getChoice();
  if (option==nullptr)
  {
    cout<<"exiting...\n";
    return 0;
  }

  switch (*option)
  {
    case (1): //new game
    {
      cout<<"What do you want to name this save: ";
      getString(&save_name);
      fstream file;
      file.open((save_name+".bin"), ios::out | ios::binary);
      if (!file)
      {
        cout<<"error in creating "<<save_name<<".bin\n";
        return 1;
      }
      file.close();

      //Land objects
      //int land_found=1; //initilaize to 1, because 1 tile starts out explored
    
      test_land[pos2index(MAP_SIZE,MAP_SIZE)].setType(FOREST);
      test_land[pos2index(MAP_SIZE, MAP_SIZE)].setX(MAP_SIZE); //MAP_SIZE:c_cen=0:p_cen
      test_land[pos2index(MAP_SIZE, MAP_SIZE)].setY(MAP_SIZE); //see above
    
      //structure list
      std::cout<<"first you need to choose a name for your starting castle\n";
      structure_list.push_back(new Castle());

      //roads

      //Mines objects

      //barbarian tribes

      //set values of starting castle
      structure_list[0]->setBase(&test_land[pos2index(MAP_SIZE,MAP_SIZE)]);
      structure_list[0]->setPopulation(TOWNSFOLK,50); 
      structure_list[0]->setMaterials(WOOD, 15);
      structure_list[0]->setMaterials(ANIMALS, 20);
      structure_list[0]->setMaterials(METAL, 15);
      structure_list[0]->setMaterials(STONE, 15);
      structure_list[0]->setMaterials(PRODUCE, 20);
      structure_list[0]->changeName();
    
      //structure_list[1]->setBase(&test_land[pos2index(MAP_SIZE+3, MAP_SIZE)]);
      break;
    }
    case (2): //load game
    {
      cout<<"Please enter name of save: ";
      getString(&save_name);
      fstream file;
      string file_name=(save_name+".bin");
      cout<<"opening "<<file_name<<"\n";
      file.open(file_name, ios::in | ios::binary);
      if (!file)
      {
        cout<<"error in opening "<<save_name<<".bin\n";
        return 1;
      }
      ObjectWriter writer=ObjectWriter(&file); //initialize the object writer

      //object reading loop
      file.seekg(0, ios::end); //put the pointer at the end
      streampos file_size=file.tellg(); //read the file size
      file.seekg(0, ios::beg); //put the pointer back to the beginning

      vector<string> land_keeps; //store the name of each land tile's keep
      vector<int> land_id; //store the index of that tile

      file.read(reinterpret_cast<char *>(&wins), sizeof(int)); //read the number of wins
      while (file.tellg()<file_size)
      {
        int id_num; //initialize the id number
        file.read(reinterpret_cast<char *>(&id_num), sizeof(int)); //read the id number
        switch (id_num)
        {
          case (LAND_ID): //land
          {
            string keep_name;
            Land* land_ptr=writer.readLand(&keep_name);
            //copy all data from the pointer to the appropriate land tile
            int index=pos2index(land_ptr->getX(), land_ptr->getY()); //get the index of the tile
            //test_land.at(index).setExplored(true);
            test_land.at(index).setX(land_ptr->getX());
            test_land.at(index).setY(land_ptr->getY());
            test_land.at(index).setType(land_ptr->getType()); //set the type
            //cout<<"index of "<<keep_name<<": "<<land_keeps.size()<<"\n";
            if (keep_name!="")
            {
              land_keeps.push_back(keep_name); //store the keep name
              land_id.push_back(index); //store the index of the land
              //the index needs to be stored so that the program can find the right land tile later
            }
            break;
          }
          case (MAJOR_ID): //major
          {
            Land* base_ptr=new Land();
            Major* major_ptr=writer.readMajor(base_ptr); //read the major
            //structure_list.push_back(writer.readMajor(base_ptr)); //read the major
            
            int index=pos2index(base_ptr->getX(), base_ptr->getY()); //get the index of the base
            major_ptr->setBase(&test_land.at(index)); //set the base tile
            //set land limit
            /*
            switch (major_ptr->getType())
            {
              case (CASTLE):
              {
                major_ptr->setLimit(CASTLE_LIMIT);
                break;
              }
              case (VILLAGE):
              {
                major_ptr->setLimit(VILLAGE_LIMIT);
                break;
              }
              case (FORT):
              {
                major_ptr->setLimit(FORT_LIMIT);
                break;
              }
              default:
              {
                major_ptr->setLimit(0);
                break;
              }
            }
            */
            structure_list.push_back(major_ptr); //push the pointer into the vector
            delete base_ptr; //delete the extra land pointer
            break;
          }
          case (MINOR_ID):
          {
            string keep_name;
            Minor* minor_ptr=writer.readMinor(&keep_name);
            Major* major_ptr=getStructure(keep_name, &structure_list); //get the major structure
            minor_ptr->setKeep(major_ptr); //set the keep for the minor
            major_ptr->addSupport(minor_ptr); //add the support to the major
            //the minor's keep is set first so that the major adds the support with no checks
            minor_list.push_back(minor_ptr); //add the minor to the list
            break;
          }
          case (ROAD_ID):
          {
            string dest1;
            string dest2;
            string caravan_dest;
            //caravan_ptr=obj_writ.readRoad(&dest1, &dest2, &caravan_dest);
            //cout<<caravan_ptr->getData()<<"\n";
            //cout<<"destination: "<<caravan_dest<<"\n";
            Road saved_road;
            Road* road_ptr=writer.readRoad(&dest1, &dest2, &caravan_dest);
            Caravan* caravan_ptr=road_ptr->getCaravan(); //get the caravan
            //set the caravan destination
            Major* destination_ptr=getStructure(caravan_dest, &structure_list);
            if (destination_ptr!=nullptr)
            {
              caravan_ptr->setDestination(destination_ptr);
            }

            //set the road endpoints
            Major* endpoint1=getStructure(dest1, &structure_list);
            if (endpoint1!=nullptr)
            {
              road_ptr->setDestination(1,endpoint1);
            }
            endpoint1->addRoad(road_ptr);

            Major* endpoint2=getStructure(dest2, &structure_list);
            if (endpoint2!=nullptr)
            {
              road_ptr->setDestination(2,endpoint2);
            }
            endpoint2->addRoad(road_ptr);

            highway.push_back(road_ptr);
            break;
          }
          case (BARBARIAN_ID):
          {
            string keep_name;
            Barbarian* barbarian_ptr=writer.readBarbarian(&keep_name);
            Major* major_ptr=getStructure(keep_name, &structure_list); //get the major structure
            barbarian_ptr->setKeep(major_ptr); //set the keep for the minor
            tribes.push_back(barbarian_ptr);
            break;
          }
          default:
          {
            cout<<"unkown id number while reading saved data\n";
            cout<<"number read: "<<id_num<<"\n";
            break;
          }
        }
      }
      //link up the land tiles to their respective keeps
      for (int i=0; i<land_keeps.size(); i+=1)
      {
        int index=land_id.at(i); //get the index
        Land* land_ptr=&(test_land.at(index));
        if (land_ptr->getKeep()==nullptr)
        {
          string name=land_keeps.at(i); //get the name
          Major* major_ptr=getStructure(name, &structure_list); //get the major structure
          major_ptr->forceClaim(land_ptr);
        }
      }
      //display all data
      //cout<<"displaying majors data\n";
      /*
      for (int i=0; i<structure_list.size(); i+=1)
      {
        cout<<structure_list.at(i)->getData()<<"\n";
      }
      cout<<"displaying land data\n";
      for (int i=0; i<test_land.size(); i+=1)
      {
        if (test_land.at(i).getType()!=SPACE)
        {
          cout<<test_land.at(i).getData()<<"\n";
        }
      }
      */
      break;
    }
    default:
    {
      cout<<"error when trying to start game\n";
      return 0;
    }
  }


  //*****************************************************************menus*****************************************************************

  //action menu
  vector<string> action_options={"produce", "build", "train townsfolk", "display data", "display map", "explore", "claim land", "list structure",
    "transport cargo", "attack", "rename a structure", "wait",  "save", "quit"};
  Menu action_menu=Menu("Choose an action", action_options);
  action_menu.setColor(105);
  string action_help="this is the main menu. Here you choose what action you would like to take this turn.\n";
  action_help+="Produce is how you acquire resources. \nBuild is how you make more villages, castles, mines, and the like. \n";
  action_help+="Train townsfolk is how you turn your townsfolk into various tradesmen. \n";
  action_help+="Display data displays various information about a specific location. \n";
  action_help+="Display map shows you a map of explored territory. \n";
  action_help+="Explore allows you to venture out an discover new land tiles. \n";
  action_help+="Claim land is how you get more land to build structures on.\n";
  action_help+="List structure provides a list of all major structures.\n";
  action_help+="Transport cargo is how you move people and materials between locations.\n";
  action_help+="Attack is how you fight off invading barbarian tribes.\n";
  action_help+="Rename structure allows you to change the name of an existing structure.\n";
  action_help+="Wait allows you to essentially pass and check if any ongoing processes have finished.\n";
  action_help+="Save saves the current game state.\n";
  action_help+="Quit exits the game.\n";
  
  action_menu.setHelp(action_help);

  //build menu
  vector<string> build_options={"garden", "pasture", "sawmill", "quarry", "mine", "castle", "fort", "village", "road"}; //options for what the user can build
  Menu build_menu=Menu("What are you going to build", build_options);
  build_menu.setColor(105);
  string build_help="\033[1;38;5;81mBuild\033[0m\n\n";
  build_help+="This is the build menu. You can use this menu to build more structures in your kingdom.\n";
  build_help+="To see the cost of building a structure select it in this menu\n";
  build_help+="Major structures are large structures that house people, store materials, and can have certain other structures attached.\n";
  build_help+="Minor structures are smaller structures used to produce resources. They have to be built in a major structure.\n\n";
  build_help+="\033[1;38;5;140mMinor Structures:\033[0m\n";
  build_help+="Gardens are used to produce produce. They require farmers.Gardens can only be built in tundra.\n";
  build_help+="Pastures are used to produce animals. They require shephers. Pastures can be built in fields or tundra.\n";
  build_help+="Sawmills are used to produce wood. They require lumberjacks. Sawmills can only be built in forests.\n";
  build_help+="Quarries are used to produce stone. They require stone masons. Quarries can be built in forests, mountains, or tundra.\n";
  build_help+="Mines are used to produce metal. They require miners. Mines can be built in forests, fields, or mountains.\n\n";
  build_help+="\033[1;38;5;118mMajor Structures:\033[0m\n";
  build_help+="Castles are large fortified structures. They can claim up to 11 land tiles, have room for 300 people,\n";
  build_help+="  and also have defenses that make it harder for attacking tribes to defeat them.\n";
  build_help+="  Castles can also train townsfolk into soldiers.\n";
  build_help+="  Castles can be built in a forest or fields.\n";
  build_help+="Villages are quick cheap structures for housing people and materials. They can claim up to 3 land tiles, can house up to 50 people,\n";
  build_help+="  but have no defenses to attackers except any soldiers stationed there.\n";
  build_help+="  Villages can be built on any tile except a sea.\n";
  build_help+="Forts are structures designed for defense. They cannot claim any land,\n";
  build_help+="  but can house up to 80 people and have good defenses against barbarian tribes. Forts can train townsfolk into soldiers.\n";
  build_help+="  Forts can be built on any tile except a sea.\n\n";
  build_help+="\033[1;38;5;122mOther structures\033[0m\n";
  build_help+="Roads are structures that connect two major structures. They can be used to transport people or materials,\n";
  build_help+="  however a road connected to an occupied location cannot be used. The time a journey takes is dependent on the length of the road.\n";
  build_help+="  The cost of a road is also dependent on its length.\n\n\n";
  build_menu.setHelp(build_help);


  //production menu (help page)
  string production_help="\033[1;38;5;39mProduction\033[0m\n\n";
  production_help+="Production is how you acquire more resources. When you use\n";
  production_help+="production it is activated for an entire location at once.\n";
  production_help+="Any structures currently producing will continue, and any\n";
  production_help+="structures not already producing will begin if possible.\n";
  production_help+="If a location is overrun with barbarians production stops\n";
  production_help+="and any materials in the process of production are lost\n\n";
  production_help+="\033[1;38;5;41mGardens\033[0m:\n\tTime: "+int2string(GARDEN_PRODUCTION)+" seconds\n";
  production_help+="\tAmount produced per worker: "+int2string(GARDEN_MULTIPLIER)+"\n\n";
  production_help+="\033[1;38;5;166mPastures\033[0m:\n\tTime: "+int2string(PASTURE_PRODUCTION)+" seconds\n";
  production_help+="\tAmount produced per worker: "+int2string(PASTURE_MULTIPLIER)+"\n\n";
  production_help+="\033[1;38;5;184mSawmills\033[0m:\n\tTime: "+int2string(SAWMILL_PRODUCTION)+" seconds\n";
  production_help+="\tAmount produced per worker: "+int2string(SAWMILL_MULTIPLIER)+"\n\n";
  production_help+="\033[1;38;5;197mQuarries\033[0m:\n\tTime: "+int2string(QUARRY_PRODUCTION)+" seconds\n";
  production_help+="\tAmount produced per worker: "+int2string(QUARRY_MULTIPLIER)+"\n\n";
  production_help+="\033[1;38;5;196mMines\033[0m:\n\tTime: "+int2string(MINE_PRODUCTION)+" seconds\n";
  production_help+="\tAmount produced per worker: "+int2string(MINE_MULTIPLIER)+"\n\n";

  //train menu (help page)
  string train_help="\033[1;38;5;226mTrain Townsfolk\033[0m\n\n";
  train_help+="training townsfolk in a trade is how you get more tradesmen.\n";
  train_help+="Most trades can be trained in any location, soldiers are\n";
  train_help+="the only career type that must be trained at a castle or fort.\n";
  train_help+="Townsfolk will occasionally multiply, but only townfolk can multiply.\n";

  //data menu (help page)
  string data_help="\033[1;38;5;93mDisplay Data\033[0m\n\n";
  data_help+="Choose a location to display the data for. Any\n";
  data_help+="data relating to that location(such as population\n";
  data_help+="data, inventory, available land, etc) will be shown.\n";

  //map menu (help page)
  string map_help="\033[1;38;5;82mDisplay Map\033[0m\n\n";
  map_help+="This action displays a map of your kingdom.\n";
  map_help+="Because your kingdom may become too big to\n";
  map_help+="fit in the map, you must choose a location\n";
  map_help+="to be the center of the map. There is a legend\n";
  map_help+="at the bottom of the map.\n";

  //claim land menu (help page)
  string claim_help="\033[1;38;5;51mClaim Land\033[0m\n\n";
  claim_help+="Any location must have land if it is going\n";
  claim_help+="to have supporting structures. First choose\n";
  claim_help+="which location you would like to claim land\n";
  claim_help+="for, then enter the coordinates of the land\n";
  claim_help+="you would like to claim. The game will then\n";
  claim_help+="check if the land can be claimed. In order\n";
  claim_help+="for a land to be claimable it must be close\n";
  claim_help+="enough to the location trying to claim it,\n";
  claim_help+="it must be adjacent to land already part of\n";
  claim_help+="the location, and the location must have\n";
  claim_help+="enough room for growth. Castles can claim "+int2string(CASTLE_LIMIT)+"\n";
  claim_help+="land tiles. Villages can claim "+int2string(VILLAGE_LIMIT)+" land tiles.\n";
  claim_help+="Forts cannot claim any land.\n\n";

  //transport menu (help page)
  string transport_help="\033[1;38;5;135mTransport\033[0m\n\n";
  transport_help+="Transportation is how you move people and\n";
  transport_help+="materials between locations. First select\n";
  transport_help+="the location that is sending the caravan\n";
  transport_help+="(the load being transported), then select\n";
  transport_help+="from the connected locations where you\n";
  transport_help+="like to send that caravan. Only one caravan\n";
  transport_help+="can be on a road at a given time. Caravans\n";
  transport_help+="cannot be sent to or from locations that\n";
  transport_help+="are occupied by a barbarian tribe. If a\n";
  transport_help+="caravan is sent to a location that gets\n";
  transport_help+="attacked while the caravan is en route, then\n";
  transport_help+="that caravan and everything in it will be\n";
  transport_help+="lost. So be careful.\n";

  //attack menu (help page)
  string attack_help="\033[1;38;5;32mAttack\033[0m\n\n";
  attack_help+="When a barbarian tribe takes over one of your\n";
  attack_help+="structures, you will need to attack them to get\n";
  attack_help+="rid of them. First choose where you are going\n";
  attack_help+="base your attack from. Then choose the target\n";
  attack_help+="of your attack. You cannot attack a tribe with\n";
  attack_help+="soldiers from an occupied city.\n";
  //attack_help+="

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


        production_menu.setHelp(production_help); //add the help menu

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
          cout<<"\033[31mThat structure cannot produce\033[0m\n";
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
          case (5): //mine
          {
            cout<<"To build a mine costs 3 animals, 3 wood, and 5 metal\n"
              <<"do you want to continue\n"<<"1. continue\n2. exit\n";
            int* cont_ptr=getInt();
            if (cont_ptr==nullptr)
            {
              std::cout<<"\033[31mthat is not a valid input\033[0m\n";
              break;
            }
            int cont=*cont_ptr;
            if (cont==2)
            {
              break;
            }
            else if (cont!=1)
            {
              cout<<"\033[31m"<<cont<<" is not a valid input\033[0m\n";
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
              cout<<"\033[32mbuilt a mine at "<<major_ptr->getName()<<"\033[0m\n";
            }
            else
            {
              cout<<"\033[31mcannot build a mine there\033[0m\n";
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
              cout<<"\033[31mthat is not a valid input\033[0m\n";
              break;
            }
            int cont=*cont_ptr;
            if (cont==2)
            {
              break;
            }
            else if (cont!=1)
            {
              cout<<"\033[31m"<<cont<<" is not a valid input\033[0m\n";
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
            else
            {
              break;
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
              cout<<"\033[32mBuilt a pasture at "<<major_ptr->getName()<<"\033[0m\n";
            }
            else
            {
              cout<<"\033[31mCannot build a pasture there\033[0m\n";
            }
            break;
          }
          case (4): //quarry
          {
            cout<<"To build a quarry costs 5 produce, 2 animals, 5 wood, and 5 metal\n"
              <<"do you want to continue\n"<<"1. continue\n2. exit\n";
            int* cont_ptr=getInt();
            if (cont_ptr==nullptr)
            {
              std::cout<<"\033[31mthat is not a valid input\033[0m\n";
              break;
            }
            int cont=*cont_ptr;
            if (cont==2)
            {
              break;
            }
            else if (cont!=1)
            {
              cout<<"\033[31m"<<cont<<" is not a valid input\033[0m\n";
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
            else
            {
              break;
            }
            Major* major_ptr=structure_list.at(index-1); 

            if (canBuildMinor(QUARRY, major_ptr)) //if the mine can be built
            {
              minor_list.push_back(new Quarry());
              major_ptr->addSupport(minor_list.at(minor_list.size()-1));
              //add the mine as a support to the keep
              cout<<"\033[32mbuilt a quarry at "<<major_ptr->getName()<<"\033[0m\n";
            }
            else
            {
              cout<<"\033[31mcannot build a quarry there\033[0m\n";
            }
            break;
          }
          case (3): //sawmill
          {
            cout<<"To build a sawmill costs 7 wood, 3 stone, and 2 metal\n"
              <<"do you want to continue\n"<<"1. continue\n2. exit\n";
            int* cont_ptr=getInt();
            if (cont_ptr==nullptr)
            {
              std::cout<<"\033[31mthat is not a valid input\033[0m\n";
              break;
            }
            int cont=*cont_ptr;
            if (cont==2)
            {
              break;
            }
            else if (cont!=1)
            {
              cout<<"\033[31m"<<cont<<" is not a valid input\033[0m\n";
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
            else
            {
              break;
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
              cout<<"\033[32mbuilt a sawmill at "<<major_ptr->getName()<<"\033[0m\n";
            }
            else
            {
              cout<<"\033[31mcannot build a sawmill there\033[0m\n";
            }
            break;
          }
          case (1): //garden
          {
            cout<<"To build a garden costs 5 produce, 2 wood\n"
              <<"do you want to continue\n"<<"1. continue\n2. exit\n";
            int* cont_ptr=getInt();
            if (cont_ptr==nullptr)
            {
              std::cout<<"\033[31mthat is not a valid input\033[0m\n";
              break;
            }
            int cont=*cont_ptr;
            if (cont==2)
            {
              break;
            }
            else if (cont!=1)
            {
              cout<<"\033[31m"<<cont<<" is not a valid input\033[0m\n";
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
            else
            {
              break;
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
              cout<<"\033[32mbuilt a garden at "<<major_ptr->getName()<<"\033[0m\n";
            }
            else
            {
              cout<<"\033[31mcannot build a garden there\033[0m\n";
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
              std::cout<<"\033[31mthat is not a valid input\033[0m\n";
              break;
            }
            int cont=*cont_ptr;
            if (cont==2)
            {
              break;
            }
            else if (cont!=1)
            {
              cout<<"\033[31m"<<cont<<" is not a valid input\033[0m\n";
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
              std::cout<<"\033[31mthat is not a valid input\033[0m\n";
              break;
            }
            int cont=*cont_ptr;
            if (cont==2)
            {
              break;
            }
            else if (cont!=1)
            {
              cout<<"\033[31m"<<cont<<" is not a valid input\033[0m\n";
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
              std::cout<<"\033[31mthat is not a valid input\033[0m\n";
              break;
            }
            int cont=*cont_ptr;
            if (cont==2)
            {
              break;
            }
            else if (cont!=1)
            {
              cout<<"\033[0m"<<cont<<" is not a valid input\033[0m\n";
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
              cout<<"\033[31mroad endpoints cannot be the same location\033[0m\n";
              break;
            }

            int cost=3*dist(start_ptr, end_ptr); //get the cost for the road (3 stone per unit length)

            if ((start_ptr==nullptr)||(end_ptr==nullptr))
            // if one of the names was invalid then the corresponding pointer will be the nullptr
            {
              cout<<"\033[31mEntered an invalid name\033[0m\n";
              break;
            }
            else if ((start_ptr->isOccupied())||(end_ptr->isOccupied()))
            {
              cout<<"\033[31mcannot build a road to a structure that is occupied\033[0m\n";
              break;
            }
            else if (start_ptr->getMaterials(STONE)<cost)
            {
              cout<<"\033[31mthat "<<start_ptr->getTypeString()<<" does not have enough resources for a road that long\033[0m\n";
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
              cout<<"\033[32mbuilt a road between "<<start_ptr->getName()<<" and "
                <<end_ptr->getName()<<"\033[0m\n";
            }
            break;
          }
          default:
          {
            cout<<"\033[31m"<<action_2<<" does not designate a valid structure"<<"\033[0m\n";
            break;
          }
        }
        break;
      }
      case (3): //train townsfolk
      {
        Menu train_menu=Menu("Where would you like to train townsfolk");
        train_menu.addOption(listStructures(&structure_list));
        train_menu.setHelp(train_help);
        train_menu.display();

        int index;
        int* choice_ptr=train_menu.getChoice();
        if (choice_ptr!=nullptr)
        {
          index=*choice_ptr;
        }
        else
        {
          break;
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
        data_menu.setHelp(data_help);
        data_menu.display();

        int index;
        int* choice_ptr=data_menu.getChoice();
        if (choice_ptr!=nullptr)
        {
          index=*choice_ptr;
        }
        else
        {
          break;
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
        map_menu.setHelp(map_help);
        map_menu.display();
        
        int index;
        int* choice_ptr=map_menu.getChoice();
        if (choice_ptr!=nullptr)
        {
          index=*choice_ptr;
        }
        else
        {
          break;
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
          cout<<"\033[31mThat tile is too far away\033[0m\n";
          break;
        }
        else if (major_ptr->isOccupied())
        {
          cout<<"\033[31mcannot explore from an occupied structure\033[0m\n";
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
        else
        {
          break;
        }
        int index=pos2index(start_x, start_y); //convert the player entered coordinates to an index
        if (test_land[index].getType()!=SPACE) //as long as the starting tile is explored
        {
          explore(&(test_land), index, static_cast<Direction>(direction-1), EXPLORE_LIMIT);
        }
        else
        {
          cout<<"\033[31mthe land at "<<displayCoords(start_x, start_y)<<" has not been explored yet\033[0m\n";
        }
        break;
      } //end explore
      case(7): //claim land 
      {
        Menu claim_menu=Menu("Choose structure to claim land for");
        claim_menu.addOption(listStructures(&structure_list));
        claim_menu.setHelp(claim_help);
        claim_menu.display();

        int index;
        int* choice_ptr=claim_menu.getChoice();
        if (choice_ptr!=nullptr)
        {
          index=*choice_ptr;
        }
        else
        {
          break;
        }
        Major* major_ptr=structure_list.at(index-1); //getStructure(name, &structure_list); //get the structure

        if (major_ptr==nullptr) //check for a valid name
        {
          break;
        }
        else if (major_ptr->isOccupied())
        {
          cout<<"\033[31mAn occupied structure cannot claim land\033[0m\n";
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
          cout<<"\033[31mThat "<<major_ptr->getTypeString()<<" has already maxed out its claims\033[0m\n";
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
        transport_menu.setHelp(transport_help);
        transport_menu.display();

        int index;
        int* choice_ptr=transport_menu.getChoice();
        if (choice_ptr!=nullptr)
        {
          index=*choice_ptr;
        }
        else
        {
          break;
        }
        Major* sender_ptr=structure_list.at(index-1); //getStructure(sender, &structure_list);

        if (sender_ptr!=nullptr) //if the chosen structure exists
        {
          if (sender_ptr->isOccupied()) //if the chosen structure is not occupied
          {
            cout<<"\033[31mcannot send a caravan from an occupied structure\033[0m\n";
          }
          else
          {
            sender_ptr->load();
          }
        }
        else
        {
          cout<<"\033[31mthere is no structure with that name\033[0m\n";
        }
        break;
      }
      case (10): //attack a tribe
      {
        //**********************************get the location of the attackers***********************************
        Menu attack_menu=Menu("Choose location to attack with");
        attack_menu.addOption(listStructures(&structure_list));
        attack_menu.display();

        int index;
        int* choice_ptr=attack_menu.getChoice();
        if (choice_ptr!=nullptr)
        {
          index=*choice_ptr;
        }
        else
        {
          break;
        }
        Major* attacker_ptr=structure_list.at(index-1); //getStructure(target_name, &structure_list);
        if (attacker_ptr==nullptr)
        {
          cout<<"\033[31mthere is no structure with that name\033[0m\n";
          break;
        }
        else if (attacker_ptr->isOccupied())
        {
          cout<<"\033[31mcannot attack from an occupied structure\033[0m\n";
          break;
        }


        //*********************************get the location of the defenders**********************************
        Menu target_menu=Menu("Choose location to attack");
        target_menu.addOption(listStructures(&structure_list));
        target_menu.display();

        int index2;
        choice_ptr=target_menu.getChoice();
        if (choice_ptr!=nullptr)
        {
          index2=*choice_ptr;
        }
        else
        {
          break;
        }
        Major* target_ptr=structure_list.at(index2-1); //getStructure(attacker_name, &structure_list);
        if (target_ptr==nullptr)
        {
          cout<<"\033[31mthere is no structure with that name\033[0m\n";
        }

        //************************************************attack***********************************************
        if (target_ptr->isOccupied()) //ensure that there is no friendly fire
        {
          int result=attacker_ptr->attack(target_ptr);
          if (result==1)
          {
            cout<<"\033[32myou defeated the barbarians at "<<target_ptr->getName()<<"\033[0m\n";
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
              cout<<"remove all tribes \033[38;5;220m"<<3-wins<<"\033[0m more times to win\n";
            }
            target_ptr->setOccupants(nullptr);
          }
          else if (result==0)
          {
            cout<<"\033[31mthe barbarians won\033[0m\n";
          }
        }
        else
        {
          cout<<"\033[31mthat structure is not occupied by a barbarian tribe\033[0m\n";
        }
        break;
      }
      case (11): //change name
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
        else
        {
          break;
        }
        Major* major_ptr=structure_list.at(index-1); //getStructure(name, &structure_list);
        if (major_ptr==nullptr)
        {
          cout<<"\033[31mThere is no structure with that name\033[0m\n";
          break;
        }

        cout<<"What do you want to rename it: ";
        string new_name;
        getString(&new_name);
        if (getStructure(new_name, &structure_list)==nullptr)
        {
          major_ptr->setName(new_name);
        }
        else
        {
          cout<<"\033[31mthere is already a structure with that name\033[0m\n";
        }
        break;
      }
      case (12): //wait
      {
        break;
      }
      case (13): //save
      {
        //open file
        fstream file_writer;
        file_writer.open((save_name+".bin"), ios::out | ios::binary);
        ObjectWriter writer=ObjectWriter(&file_writer);

        //write game information
        file_writer.write(reinterpret_cast<const char*>(&wins), sizeof(int));

        //write land data
        for (int i=0; i<test_land.size(); i+=1)
        {
          Land current=test_land.at(i);
          if (current.getType()!=SPACE) //only write a land tile if it has been explored, this reduces the size of the save file
          {
            writer.write(&current);
          }
        }

        //write major structures
        for (int i=0; i<structure_list.size(); i+=1)
        {
          writer.write(structure_list.at(i));
        }

        //write minor structures
        for (int i=0; i<minor_list.size(); i+=1)
        {
          writer.write(minor_list.at(i));
        }

        //write roads
        for (int i=0; i<highway.size(); i+=1)
        {
          writer.write(highway.at(i));
        }

        //write barbarians
        for (int i=0; i<tribes.size(); i+=1)
        {
          cout<<"writing barbarian\n";
          writer.write(tribes.at(i));
        }
        
        file_writer.close();
        cout<<"saved game\n";
        break;
      }
      case (14): //quit
      {
        cout<<"If you quit, all unsaved progress will be lost\nAre you sure you want to quit?\n";
        Menu quit_menu=Menu("          ");
        quit_menu.addOption("yes");
        quit_menu.addOption("no");
        quit_menu.display();
        int* answer=quit_menu.getChoice();
        if (answer==nullptr)
        {
          break;
        }
        else if (*answer==1)
        {
          cont=false;
          break;
        }
      }
      /*
      case (15): //this is only for testing
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
        getString(&sender);

        Major* sender_ptr=getStructure(sender, &structure_list);
        int num;
        cout<<"how many soldiers would you like it to have: ";
        cin>>num;
        sender_ptr->setPopulation(SOLDIER, num);
        break;
      }*/
      default:
      {
        cout<<"\033[31m"<<action<<" not a valid action: please choose a valid action\033[0m\n";
        break;
      }
    }



//**************************check for end of production and increase population of townsfolk****************************
    if (cont)
    {
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
          if  (random_number<3) //||(start_attack))//check if the barbarians spawn
          {
            Major* major_ptr=furthestStructure(&structure_list); //get the furthest structure
            cout<<"\033[1;31mA barbarian tribe is attacking "<<major_ptr->getName()<<"\033[0m\n"; //tell the player about the attack
            pause(1);
            Village steppes=Village(); //give the barbarians a beginning structure tile to start from
            Barbarian* tribe_ptr=new Barbarian(&steppes);
            tribe_ptr->setPopulation(25);
            if (tribe_ptr->getKeep()->attack(major_ptr))
            {
              cout<<"\033[31mthe barbarians won\033[0m\n";
              tribe_ptr->setKeep(major_ptr); //set the major as the keep
              tribes.push_back(tribe_ptr); //initialize the barbarians
              barbarian_invasion=true;
            }
            else
            {
              cout<<"\033[32myou fended off the barbarians\033[0m\n";
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
                cout<<"\033[31mthe barbarians won\033[0m\n";
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
                cout<<"\033[32myou fended off the barbarians\033[0m\n";
              }
            }
          }
          //spread=false;
        }
        //********************************************check for end of game**************************************************
        if (wins==5)
        {
          cout<<"\033[1;32mCONGRATULATIONS! YOU WON!\033[0m You fended off the barbarian hordes three times, showing that your"
            <<"kingdom is strong enough to protect itself.\n Thanks for playing\n";
          cont=false;
        }
    
        bool all_occupied=true;
        for (int i=0; i<structure_list.size(); i+=1)
        {
          if (!structure_list.at(i)->isOccupied()) //if there is an unoccupied structure
          {
            all_occupied=false;
            break;
          }
        }
    
        if (all_occupied)
        {
          cout<<"\033[1;31m\n\nGAME OVER\n\nYour Kingdom has fallen!\033[0m"
              <<"All of your villages, castles, and forts are taken by barbarian tribes.Better luck next time!\n";
          cont=false;
        }
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
