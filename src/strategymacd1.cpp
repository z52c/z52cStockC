#include "../include/strategymacd1.h"
#include <fstream>

extern std::ofstream logFile;

strategyMACD1::strategyMACD1(const char* _stockCode,MARKET _market,DATAKIND _highLevelDataKind,DATAKIND _lowLevelDataKind)
{
    strcpy(stockCode,_stockCode);
    highLevelData = new stockdata(_highLevelDataKind,_market,_stockCode,FOREADJUST);
    lowLevelData = new stockdata(_lowLevelDataKind,_market,_stockCode,FOREADJUST);
    highLevelData->calcMACD();
    lowLevelData->calcMACD();
}

bool strategyMACD1::ifOK()
{
    if(!highLevelData->rawData || !lowLevelData->rawData)
        return false;
    return(highLevelData->pMACD[highLevelData->dataNum-1].dif>highLevelData->pMACD[highLevelData->dataNum-1].dea+0.1 && 
        highLevelData->pMACD[highLevelData->dataNum-1].dif>highLevelData->pMACD[highLevelData->dataNum-2].dif &&
        lowLevelData->pMACD[lowLevelData->dataNum-1].dif<0 &&
        lowLevelData->pMACD[lowLevelData->dataNum-1].dif<lowLevelData->pMACD[lowLevelData->dataNum-1].dea && 
        lowLevelData->pMACD[lowLevelData->dataNum-1].dif>lowLevelData->pMACD[lowLevelData->dataNum-2].dif 
        );
}

strategyMACD1::~strategyMACD1()
{
    if(highLevelData)
        delete highLevelData;
    if(lowLevelData)
        delete lowLevelData;
}