#include "commands.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace shevchenko
{
void readDict(std::istream& input, Dictionaries& dictionaries)
{
  std::string line;
  
  while (std::getline(input, line))
  {
    if (line.empty())
    {
      continue;
    }
    
    std::istringstream iss(line);
    std::string name;
    iss >> name;
    
    Dictionary dict;
    int key = 0;
    std::string value;
    
    while (iss >> key >> value)
    {
      dict.push(key, value);
    }
    
    dictionaries.push(name, dict);
  }
}

void doPrint(Dictionaries& dictionaries, std::istream& input, std::ostream& output)
{
  std::string name;
  input >> name;
  
  try
  {
    const Dictionary& dict = dictionaries.get(name);
    
    if (dict.empty())
    {
      output << "<EMPTY>\n";
      return;
    }
    
    output << name;
    for (auto it = dict.cbegin(); it != dict.cend(); ++it)
    {
      output << ' ' << it->first << ' ' << it->second;
    }
    output << '\n';
  }
  catch (const std::out_of_range&)
  {
    output << "<INVALID COMMAND>\n";
  }
}
}
