#pragma once

#include <string>
#include <memory>
#include <set>

// exception on failure to load dictionary file
class NameParser_InvalidFile
{
};

class NameParser
{
public:
    NameParser(){};
    void load();
    std::set<std::string> get_list();
    void add(const std::string &);
    bool check(const std::string&) const;
    std::string names_to_string();
    size_t size(void) const;

private:
    std::set<std::string> dict;
};
