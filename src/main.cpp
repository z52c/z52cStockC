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
    vector<struct stockListNode> stockList = z_get_stock_list();
    strategyMACD1* b;
    for(int i=0;i<stockList.size();i++)
    {
        b = new strategyMACD1(stockList[i].stockCode,stockList[i].market,WEEK,MIN60);
        logFile<<stockList[i].stockCode<<endl;
        if(b->ifOK())
        {
            cout<<stockList[i].stockCode<<endl;
            outFile<<stockList[i].stockCode<<endl;
        }
        delete b;
    }
    outFile.close();
    logFile.close();
}
