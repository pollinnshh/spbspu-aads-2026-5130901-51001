#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "bstree.hpp"

namespace shevchenko
{
using Dictionary = BSTree< int, std::string >;
using Dictionaries = BSTree< std::string, Dictionary >;

void readDict();

void doPrint();
void doUnion();
void doIntersect(Dictionaries& dictionaries, std::istream& input);
void doComplement(Dictionaries& dictionaries, std::istream& input);

}

#endif
