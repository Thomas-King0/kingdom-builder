/*menu class header file
*/

#include <iostream>
#include <string>
#include <vector>

class Menu
{
private:
  std::string prompt;
  std::vector<std::string> options;  
  std::string color;
public:
  Menu(std::string prompt); //constructor (no options)
  Menu(std::string prompt, std::vector<std::string> options); //constructor (with optioins)
  
  void setPrompt(std::string); //set the prompt of the menu
  std::string getPrompt(); //return the prompt
  void addOption(std::string); //add an option to the menu
  void addOption(std::vector<std::string>); //add a list of options
  std::string getOption(int index); //get the option at the specified index
  void clearOptions(); //clear all options
  void setColor(int ansi); //set the ansi color code
  std::string getColor(); //get the color code

  void display(); //display the menu
  int* getChoice(); //get the choice
  //std::string getChoice(); //get the choice and return the option of that choice
};
