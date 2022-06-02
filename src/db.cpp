#include "../include/db.h"
extern std::ofstream logFile;

fhpgDB::fhpgDB()
{
    int result;
    result = sqlite3_open(FHPGDBFILE, &db );
    if(result != SQLITE_OK)
    {
        std::cout<<"db open failed\n";
        logFile<<"数据库打开失败\n";
        return;
    }
}

fhpgDB::~fhpgDB()
{
    sqlite3_close(db);
}


vector<struct fhpg> fhpgDB::getFHPGData(const char *stockid)
{
    vector<struct fhpg> rtn;
    struct fhpg tmpNode;
    int result;
    char * errmsg = NULL;
    char **dbResult; 
    int nRow, nColumn;
    int index;
    char sqlStr[100];
    sprintf(sqlStr,"select * from fhpg where stockid = '%s' order by date asc" ,stockid);
    result = sqlite3_get_table( db,sqlStr, &dbResult, &nRow, &nColumn, &errmsg );
    if(SQLITE_OK != result)
    {
        logFile<<stockid<<" get fhpg date failed ,"<<sqlStr<<" , "<<errmsg<<endl;
        return rtn;
    }
    index = nColumn;
    for(int i=1;i<=nRow;i++)
    {
        tmpNode.date = atoi(dbResult[i*nColumn+1]);
        tmpNode.fenhong = atof(dbResult[i*nColumn+2]);
        tmpNode.songzhuan = atof(dbResult[i*nColumn+3]);
        tmpNode.peigu = atof(dbResult[i*nColumn+4]);
        tmpNode.peigujia = atof(dbResult[i*nColumn+5]);
        rtn.push_back(tmpNode);
    }
    return rtn;
}
