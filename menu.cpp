/*menu class file
*/

#include <iostream>
#include <string>
#include <vector>

#include "menu.h"
#include "helpful.h"
#include "settings.h"

//Constructor
Menu::Menu(std::string input_prompt)
{
  setPrompt(input_prompt);
  std::vector<std::string> options;
  setColor(255);
  setHelp("there is no help page for this menu");
}

Menu::Menu(std::string input_prompt, std::vector<std::string> input_options)
{
  setPrompt(input_prompt);
  std::vector<std::string> options;
  for (int i=0; i<input_options.size(); i+=1)
  {
    addOption(input_options.at(i));
  }
  setHelp("there is no help page for this menu");
}

//setPrompt
void Menu::setPrompt(std::string input_prompt)
{
  prompt=input_prompt;
}

//getPrompt
std::string Menu::getPrompt()
{
  return prompt;
}

//addOption
void Menu::addOption(std::string input_option)
{
  options.push_back(input_option);
}

void Menu::addOption(std::vector<std::string> input_list)
{
  for (int i=0; i<input_list.size(); i+=1)
  {
    addOption(input_list.at(i));
  }
}

//getOption
std::string Menu::getOption(int index)
{
  if ((index>=0)&&(index<=options.size()))
  {
    return options.at(index);
  }
  else
  {
    std::cout<<"error: invalid index in Menu::getOption --"<<index<<" out of bounds\n";
    return "";
  }
}

//clearOptions
void Menu::clearOptions()
{
  while (options.size()>0)
  {
    auto index=options.end();
    options.erase(index);
  }
}

//setColor
void Menu::setColor(int ansi)
{
  if ((ansi>=0)&&(ansi<256))
  {
    color=int2string(ansi);
  }
  else
  {
    color="255"; //the default color
  }
}

//getColor
std::string Menu::getColor()
{
  return color;
}

//setHelp
void Menu::setHelp(std::string message)
{
  help_page=message;
}

//help
void Menu::help()
{
  std::cout<<"\n\n"<<help_page<<"\n\n";
}

//display
void Menu::display()
{
  if (options.size()==0) //if there are no options in the list
  {
    std::cout<<"there are no options for this menu\n";
    return;
  }

  //upper border
  std::cout<<("\033[38;5;"+color+"m****");
  for (int i=0; i<getPrompt().size(); i+=1)
  {
    std::cout<<"*";
  }
  std::cout<<"****\033[0m\n";

  std::cout<<"\033[38;5;"+color+"m*   \033[0m"<<getPrompt()<<"\033[38;5;"+color+"m   *\033[0m\n";

  //lower border
  std::cout<<("\033[38;5;"+color+"m****");
  for (int i=0; i<getPrompt().size(); i+=1)
  {
    std::cout<<"*";
  }
  std::cout<<"****\033[0m\n\n";
  
  //menu options
  int j=0;
  for (; j<options.size(); j+=1)
  {
    std::cout<<"  "<<(j+1)<<". "<<getOption(j)<<"\n";
  }
  std::cout<<"  "<<(j+1)<<". help\n"; //automatically add a help option
  std::cout<<"\n";

  //bottom of menu
  std::cout<<"\033[38;5;"+color+"m~~~~";
  for (int i=0; i<getPrompt().size(); i+=1)
  {
    std::cout<<"~";
  }
  std::cout<<"~~~~\033[0m\n\n";
}

//getChoice
int* Menu::getChoice()
{
  if (options.size()==0)
  {
    return nullptr;
  }
  int choice=0;
  while ((choice<1)||(choice>options.size()))
  {
    int* num=getInt();
    if (num!=nullptr)
    {
      choice=*num;
    }

    if (choice==options.size()+1) //the help option
    {
      help();
    }
    else if ((choice<1)||(choice>(options.size()+1)))
    {
      std::cout<<"not a valid choice: pick again\n";
    }
  }
  return new int(choice);
}

