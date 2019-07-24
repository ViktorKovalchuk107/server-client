#include "names.h"
#include <fstream>
#include <iostream>
#include <algorithm>

const char* name_list = "../list.txt";


void NameParser::load() 
{ 
    std::ifstream file(name_list); 
    if (file.fail())
         throw NameParser_InvalidFile();
     std::string tmp;
    while (getline(file, tmp))
    {
        dict.insert(tmp);
    }
}

std::set<std::string>NameParser::get_list() 
{  
    return dict;
}

bool NameParser::check(const std::string& word) const
{
    return (std::find(dict.begin(), dict.end(), word) != dict.end());
}

void NameParser::add(const std::string& word)
{
    if (!check(word))
    {
        std::ofstream file(name_list, std::ios_base::app);
        if (file.fail())
            throw NameParser_InvalidFile();
        file << std::endl << word;
        file.close();
        dict.insert(word);
    }
}
std::string NameParser::names_to_string()
{
    std::string rez;
    for (auto it = dict.begin(); it != dict.end(); it++)
    {
        rez += *it + '\n';
    }
    return rez;
}
size_t NameParser::size(void) const
{
  return dict.size();
}



