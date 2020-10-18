#ifndef TABTENNO_H_H
#define TABTENNO_H_H

#include<string>

using std::string;

class TableTemmosPlayer
{
private:
    string firstname;
    string lastname;
    bool   hasTable;
public:
    TableTemmosPlayer (const string &fn = "none",\
                       const string &in = "none",\
                       bool ht = false);

    void Name() const;
    bool HasTable() const { return hasTable; };
    void ResetTable(bool v) { hasTable = v; };
};

#endif
