/*Caravan class file
 */

#include "caravan.h"
//#include "road.h"
#include "major.h"
#include "menu.h"
#include <string>

//constructor
Caravan::Caravan()
{
  setDestination(nullptr);
  setContents(1);
  setAmount(0);
  setIsMaterials(false);
}

Caravan::Caravan(Major* destination_ptr)
{
  setDestination(destination_ptr);
  //std::cout<<"What is this caravan taking:\n\t1. people\n\t2. materials\n";
  Menu contents_menu=Menu("Choose contentes of caravan");
  //contents_menu.addOption({"people", "materials"});
  contents_menu.addOption("people");
  contents_menu.addOption("materials");
  contents_menu.display();
  int* action=contents_menu.getChoice();
  if (action==nullptr)
  {
    std::cout<<"exiting...\n";
    setDestination(nullptr);
    return;
  }
  else if (*action==1)
  {
    Menu cargo_menu=Menu("Choose what tradesmen to put in this caravan");
    for (int i=0;i<=static_cast<int>(TOWNSFOLK); i+=1)
    {
      //std::cout<<"\n\t"<<(i+1)<<". "<<career2string(static_cast<Career>(i));
      cargo_menu.addOption(career2string(static_cast<Career>(i)));
    }
    cargo_menu.display();

    int* cargo_ptr=cargo_menu.getChoice();
    if (cargo_ptr==nullptr)
    {
      std::cout<<"exiting...\n";
      setDestination(nullptr);
      return;
    }
    int cargo=*cargo_ptr;
    if ((cargo<(static_cast<int>(FARMER)+1)||(cargo>(static_cast<int>(TOWNSFOLK)+1))))
    {
      std::cout<<"\033[31m["<<cargo<<"] does not specify a valid trade\033[0m\n";
      setDestination(nullptr);
      return;
    }
    else
    {
      setIsMaterials(false);
      setContents(cargo-1);
    }
  }
  else if (*action==2)
  {
    Menu cargo_menu=Menu("Choose material to send");
    for (int i=0; i<static_cast<int>(VACUUM); i+=1)
    {
      //std::cout<<"\n\t"<<(i+1)<<". "<<material2string(static_cast<Material>(i));
      cargo_menu.addOption(material2string(static_cast<Material>(i)));
    }
    cargo_menu.display();

    int* cargo_ptr=cargo_menu.getChoice();
    if (cargo_ptr==nullptr)
    {
      std::cout<<"exiting...\n";
      setDestination(nullptr);
      return;
    }
    int cargo=*cargo_ptr;
    if ((cargo<static_cast<int>(PRODUCE)+1)||(cargo>(static_cast<int>(VACUUM))))
    {
      std::cout<<"\033[31m["<<cargo<<"] does not specify a valid type of materials\033[0m\n";
      setDestination(nullptr);
      return;
    }
    else
    {
      setIsMaterials(true);
      setContents(cargo-1);
    }
  }
  else
  {
    std::cout<<"\033[31m["<<action<<"] does not specify a valid cargo\033[0m\n";
    setDestination(nullptr);
    return;
  }

  std::cout<<"How much are being transported: ";
  int num=*getInt();
  //std::cin>>num;
  if (num<0)
  {
    std::cout<<"\033[31mcannot transport a negative amount\033[0m\n";
    setDestination(nullptr);
    return;
  }
  else if ((num>destination_ptr->getVacancy())&&(!isMaterials()))
  {
    std::cout<<"\033[31m"<<destination_ptr->getName()<<" does not have the space for that many people\033[0m\n";
    setDestination(nullptr);
    return;
  }
  setAmount(num);
}

//isMaterials
bool Caravan::isMaterials()
{
  return is_materials;
}

//setIsMaterials
void Caravan::setIsMaterials(bool tf)
{
  is_materials=tf;
}

//getDestination
Major* Caravan::getDestination()
{
  return destination;
}

//setTowardsDestination1
void Caravan::setDestination(Major* major_ptr)
{
  destination=major_ptr;
}

//getContents
int Caravan::getContents()
{
  return contents;
}

//setContents
void Caravan::setContents(int num)
{
  /*set max according to if the caravan is carrying people or materials
  */
  int max;
  if (isMaterials()) //ensure that the index referring to the contents is a valid index
  {
    max=static_cast<int>(VACUUM)-1; //the minus 1 is because vacuum is not a material the player can actually have
  }
  else
  {
    max=static_cast<int>(TOWNSFOLK); //here no minus 1 is needed because a townsfolk is a valid type of person
  }

  if (num<0) //make sure num is not less than 0
  {
    std::cout<<"tried to set an amount that is less than 0 in setContents\n";
  }
  else if (num>max) //make sure it is less than the max
  {
    std::cout<<"tried to set an amount["<<num<<"] that is greater than max["<<max<<"] in Caravan::setContents\n";
  }
  else
  {
    contents=num; //set contents
  }
}

//getAmount
int Caravan::getAmount()
{
  return amount;
}

//setAmount
void Caravan::setAmount(int num)
{
  if (num>=0)
  {
    amount=num;
  }
  else
  {
    std::cout<<"tried setting amount to a negative number in setAmount\n";
  }
}

//getData
std::string Caravan::getData()
{
  
  std::string data="\033[38;5;226m  ********Caravan data********\033[0m\n";

  //contents
  if (isMaterials())
  {
    data+=("  "+fillString("Contents:",20)+material2string(static_cast<Material>(getContents()))+"\n");
  }
  else
  {
    data+=("  "+fillString("Contents:",20)+career2string(static_cast<Career>(getContents()))+"\n");
  }

  //amount
  data+=("  "+fillString("Amount:",20)+int2string(getAmount())+"\n");

  return data;
}
