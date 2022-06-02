#include "../include/stockdata.h"
extern std::ofstream logFile;


stockdata::stockdata(enum DATAKIND _dataKind,enum MARKET _market,const char* _stockCode,enum ADJUSTFACTOR _adjustFactor)
{
    dataKind = _dataKind;
    market = _market;
    adjustFactor = _adjustFactor;
    strcpy(stockCode,_stockCode);
    if(!getData())
    {
        printf("%s get Data failed\n",stockCode);
        return;
    }
}

stockdata::~stockdata()
{
    if(rawData)
        free(rawData);
    if(pMACD)
        free(pMACD);
    if(pMa)
        free(pMa);
}

bool stockdata::getData()
{
    switch (dataKind)
    {
    case MIN1:
        rawData = tdx_get_min1_data(market,stockCode,&dataNum,adjustFactor);
        if(NULL ==  rawData)
            return false;
        break;
    case MIN5:
        rawData = tdx_get_min5_data(market,stockCode,&dataNum,adjustFactor);
        if(NULL ==  rawData)
            return false;
        break;
    case MIN15:
        rawData = tdx_get_min5_times_data(market,stockCode,3,&dataNum,adjustFactor);
        if(NULL ==  rawData)
            return false;
        break;
    case MIN30:
        rawData = tdx_get_min5_times_data(market,stockCode,6,&dataNum,adjustFactor);
        if(NULL ==  rawData)
            return false;
        break;
    case MIN60:
        rawData = tdx_get_min5_times_data(market,stockCode,12,&dataNum,adjustFactor);
        if(NULL ==  rawData)
            return false;
        break;
    case DAY:
        rawData = tdx_get_day_data(market,stockCode,&dataNum,adjustFactor);
        if(NULL ==  rawData)
            return false;
        break;
    case WEEK:
        rawData = tdx_get_week_data(market,stockCode,&dataNum,adjustFactor);
        if(NULL ==  rawData)
            return false;
        break;
    case MOONTH:
        rawData = tdx_get_month_data(market,stockCode,&dataNum,adjustFactor);
        if(NULL ==  rawData)
            return false;
        break;
    }
    return true;
}

  /**
  * @brief 计算MACD，不调用不计算
  */
bool stockdata::calcMACD()
{
    if(!rawData)
        return false;
    pMACD = (struct macd*)malloc(dataNum*sizeof(struct macd));
    if(!pMACD)
    {
        logFile<<stockCode<<" macd malloc failed\n";
        return false;
    }
    pMACD[0].emaShort = rawData[0].close*2/13.0;
    pMACD[0].emaLong = rawData[0].close*2/27.0;
    pMACD[0].dif = pMACD[0].emaShort - pMACD[0].emaLong;
    pMACD[0].dea = pMACD[0].dif*2/10.0;
    pMACD[0].macd = (pMACD[0].dif-pMACD[0].dea)*2;
    for(unsigned int i=1;i<dataNum;i++)
    {
        pMACD[i].emaShort = pMACD[i-1].emaShort*11/13.0 + rawData[i].close*2/13.0;
        pMACD[i].emaLong = pMACD[i-1].emaLong*25/27.0 + rawData[i].close*2/27.0;
        pMACD[i].dif = pMACD[i].emaShort - pMACD[i].emaLong;
        pMACD[i].dea = pMACD[i-1].dea*0.8 + pMACD[i].dif*0.2;
        pMACD[i].macd = (pMACD[i].dif-pMACD[i].dea)*2;
    }
    return true;
}


  /**
  * @brief 计算均线，不调用不计算
  */
bool stockdata::calcMa()
{
    if(!rawData)
        return false;
    if(dataNum<100)
        return false;
    pMa = (struct ma*)malloc(dataNum*sizeof(struct ma));
    if(!pMa)
    {
        logFile<<stockCode<<" ma malloc failed\n";
        return false;
    }
    float sum5 = 0;
    float sum10 = 0;
    for(int i=0;i<5;i++)
    {
        sum5 += rawData[i].close;
        pMa[i].ma5 = sum5/(i+1);
    }
    for(unsigned int i=5;i<dataNum;i++)
    {
        sum5 = sum5 + rawData[i].close - rawData[i-5].close;
        pMa[i].ma5 = sum5/5;
    }
    for(int i=0;i<10;i++)
    {
        sum10 += rawData[i].close;
        pMa[i].ma10 = sum10/(i+1);
    }
    for(unsigned int i=10;i<dataNum;i++)
    {
        sum10 = sum10 + rawData[i].close - rawData[i-10].close;
        pMa[i].ma10 = sum10/10;
    }
    return true;
}