#ifndef DB_H
#define DB_H

extern "C"
{
#include "../include/sqlite3.h"
};
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "../include/config.h"
#include "../include/datastruct.h"

using namespace std;

class fhpgDB{
private:
    sqlite3 * db = NULL;
public:
    fhpgDB();
    ~fhpgDB();
    vector<struct fhpg> getFHPGData(const char * stockid);
};

#endif
