#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include "bstree.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace shevchenko
{

struct Translation
{
  std::string russian;
  std::string pos;
};

class DictionarySystem
{
public:
  void makeDictionary(const std::string& name)
  {
    if (dictionaries_.contains(name))
    {
      std::cout << "<INVALID COMMAND: Dictionary '" << name << "' already exists>\n";
      return;
    }
    dictionaries_.insert(name, BSTree<std::string, Translation>());
    std::cout << "<DICTIONARY CREATED: " << name << ">\n";
  }

  void dropDictionary(const std::string& name)
  {
    if (!dictionaries_.contains(name))
    {
      std::cout << "<ERROR: Dictionary '" << name << "' not found>\n";
      return;
    }
    dictionaries_.erase(name);
    std::cout << "<DICTIONARY DELETED: " << name << ">\n";
  }

  void showDictionary(const std::string& name)
  {
    if (!dictionaries_.contains(name))
    {
      std::cout << "<ERROR: Dictionary '" << name << "' not found>\n";
      return;
    }

    BSTree<std::string, Translation>& dict = dictionaries_.at(name);
    std::cout << "<DICT: " << name << ", WORDS: " << dict.size() << ">\n";

    std::vector<std::string> keys = dict.getKeys();
    for (size_t i = 0; i < keys.size(); ++i)
    {
      Translation trans = dict.at(keys[i]);
      std::cout << "  " << keys[i] << " (" << trans.pos << ") - " << trans.russian << "\n";
    }
  }

  void addWord(const std::string& dictName, const std::string& eng,
               const std::string& rus, const std::string& pos)
  {
    if (!dictionaries_.contains(dictName))
    {
      std::cout << "<ERROR: Dictionary '" << dictName << "' not found>\n";
      return;
    }

    BSTree<std::string, Translation>& dict = dictionaries_.at(dictName);

    if (dict.contains(eng))
    {
      std::cout << "<INVALID COMMAND: Word already exists>\n";
      return;
    }

    Translation trans;
    trans.russian = rus;
    trans.pos = pos;
    dict.insert(eng, trans);
    std::cout << "<ADDED: " << eng << " (" << pos << ") - " << rus << ">\n";
  }

  void lookup(const std::string& dictName, const std::string& eng)
  {
    if (!dictionaries_.contains(dictName))
    {
      std::cout << "<ERROR: Dictionary '" << dictName << "' not found>\n";
      return;
    }

    BSTree<std::string, Translation>& dict = dictionaries_.at(dictName);

    if (!dict.contains(eng))
    {
      std::cout << "<WORD NOT FOUND: " << eng << ">\n";
      return;
    }

    Translation trans = dict.at(eng);
    std::cout << eng << ": (" << trans.pos << ") " << trans.russian << "\n";
  }

private:
  BSTree<std::string, BSTree<std::string, Translation>> dictionaries_;
};

}
#endif
