#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include "bstree.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <ctime>

namespace shevchenko
{

struct Translation
{
  std::string russian;
  std::string pos;
};

struct User
{
  std::string name;
  std::string assignedDict;
};

class LearningSystem
{
public:
  LearningSystem()
  {
    srand(static_cast<unsigned int>(time(nullptr)));
  }

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

  void addTranslation(const std::string& dictName, const std::string& eng,
                      const std::string& rus, const std::string& pos)
  {
    if (!dictionaries_.contains(dictName))
    {
      std::cout << "<ERROR: Dictionary '" << dictName << "' not found>\n";
      return;
    }

    BSTree<std::string, Translation>& dict = dictionaries_.at(dictName);

    if (!dict.contains(eng))
    {
      std::cout << "<ERROR: Word '" << eng << "' not found>\n";
      return;
    }

    Translation trans;
    trans.russian = rus;
    trans.pos = pos;
    dict.insert(eng, trans);
    std::cout << "<ADDED TRANSLATION: " << eng << " (" << pos << ") - " << rus << ">\n";
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

  void addUser(const std::string& username)
  {
    if (users_.contains(username))
    {
      std::cout << "<INVALID COMMAND: User '" << username << "' already exists>\n";
      return;
    }

    User newUser;
    newUser.name = username;
    newUser.assignedDict = "";
    users_.insert(username, newUser);
    std::cout << "<USER CREATED: " << username << ">\n";
  }

  void login(const std::string& username)
  {
    if (!users_.contains(username))
    {
      std::cout << "<ERROR: User '" << username << "' not found>\n";
      return;
    }

    currentUser_ = username;
    std::cout << "<WELCOME BACK, " << username << ">\n";

    User user = users_.at(username);
    if (user.assignedDict.empty())
    {
      std::cout << "Assigned dictionary: none\n";
    }
    else
    {
      std::cout << "Assigned dictionary: " << user.assignedDict << "\n";
    }
  }

  void logout()
  {
    if (currentUser_.empty())
    {
      std::cout << "<ERROR: No user logged in>\n";
      return;
    }

    std::cout << "<USER LOGGED OUT: " << currentUser_ << ">\n";
    currentUser_ = "";
  }

  void assignDict(const std::string& username, const std::string& dictName)
  {
    if (!users_.contains(username))
    {
      std::cout << "<ERROR: User '" << username << "' not found>\n";
      return;
    }

    if (!dictionaries_.contains(dictName))
    {
      std::cout << "<ERROR: Dictionary '" << dictName << "' not found>\n";
      return;
    }

    User user = users_.at(username);
    user.assignedDict = dictName;
    users_.insert(username, user);
    std::cout << "<DICTIONARY ASSIGNED: " << dictName << " to " << username << ">\n";
  }

private:
  BSTree<std::string, BSTree<std::string, Translation>> dictionaries_;
  BSTree<std::string, User> users_;
  std::string currentUser_;
};

}
#endif
