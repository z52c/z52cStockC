#ifndef STRATEGYMACD1_H
#define STRATEGYMACD1_H
#include "config.h"
#include "datastruct.h"
#include "stockdata.h"
#include "util.h"

#include <iostream>

/**
 * 1、该级别金叉且开口向上，2、孙子级别即将金叉(0轴下方)，3、孙子级别的子级别底背离且放倍量(只筛选前两个条件)
 * 级别：1分、5分、15分、60分、日、周、月、季、年 （4的倍数）
 */

class strategyMACD1{
private:
    stockdata *highLevelData;
    stockdata *lowLevelData;
    char stockCode[7];
public:
    strategyMACD1(const char* _stockCode,MARKET _market,DATAKIND _highLevelDataKind,DATAKIND _lowLevelDataKind);
    ~strategyMACD1();
    bool ifOK();
};

#endif