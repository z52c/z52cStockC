#include "../include/tdx.h"
#include "../include/db.h"
#include "../include/stockdata.h"
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

ofstream logFile;
ofstream outFile;
fhpgDB *sqliteDB;

int main()
{
    sqliteDB = new fhpgDB();
    logFile.open("error.log",ios::out | ios::trunc);
    outFile.open("rst.txt",ios::out | ios::trunc);
    stockdata a = stockdata(DAY,SZ,"002555",FOREADJUST);
    a.calcMACD();
    for(int i=0;i<a.dataNum;i++)
    {
        cout<<a.rawData[i].date<<"-dea:"<<a.pMACD[i].dea<<"-dif:"<<a.pMACD[i].dif<<"-macd:"<<a.pMACD[i].macd<<"  ;";
    }
    outFile.close();
    logFile.close();
}
