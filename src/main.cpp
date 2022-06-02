#include "../include/tdx.h"
#include "../include/db.h"
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
    /*
    vector<struct stockListNode> stockList;
    stockList=z_get_stock_list();
    vector<struct fhpg> b;
    b=sqliteDB->getFHPGData("002555");
    for(int i=0;i<b.size();i++)
    {
        cout<<b[i].date<<"  "<<b[i].fenhong<<"  "<<b[i].songzhuan<<"  "<<b[i].peigu<<"  "<<b[i].peigujia<<endl;
    }
    */
    unsigned int dataNum;
    tdxRawData* pTmp = tdx_get_week_data(SZ,"002555",&dataNum,FOREADJUST);
    for(int i=0;i<dataNum;i++)
    {
        cout<<pTmp[i].date<<"  "<<pTmp[i].close<<"  ";
    }
    outFile.close();
    logFile.close();
}