#ifndef STOCKDATA_H
#define STOCKDATA_H

#include <iostream>
#include <fstream>
#include "tdx.h"
#include "datastruct.h"
#include "util.h"
#include <string.h>

class stockdata{
private:
    enum ADJUSTFACTOR adjustFactor; 
    struct ma* pMa = NULL;
    bool getData();
public:
    enum DATAKIND dataKind;
    enum MARKET market;
    char stockCode[7];
    unsigned int dataNum;  //K线数据数量
    struct macd* pMACD = NULL;
    tdxRawData *rawData = NULL;
    stockdata(enum DATAKIND _dataKind,enum MARKET _market,const char* _stockCode,enum ADJUSTFACTOR _adjustFactor);
    ~stockdata();
    bool calcMACD();
    bool calcMa();
};

#endif