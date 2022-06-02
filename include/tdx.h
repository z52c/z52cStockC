#ifndef TDX_H
#define TDX_H

#include "config.h"
#include "datastruct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "db.h"
#include<io.h>
#include <string>
#include <vector>

  /**
  * @brief 从通达信的下载文件中读取1分钟数据
  * @param _market: 指定市场，SH SZ
  * @param stockCode：股票代码字符串
  * @param dataNum：用于保存数据数量（条数）的指针
  * @param aj: 复权类型，暂时就写了前复权，处理后数据结果跟通达信一致
  * @return 返回tdxRawData结构体指针，需要手动free，出错则返回NULL
  */
tdxRawData * tdx_get_min1_data(enum MARKET _market,const char* stockCode,unsigned int *dataNum,enum ADJUSTFACTOR aj);

tdxRawData * tdx_get_min5_data(enum MARKET _market,const char* stockCode,unsigned int *dataNum,enum ADJUSTFACTOR aj);

tdxRawData * tdx_get_day_data(enum MARKET _market,const char* stockCode,unsigned int *dataNum,enum ADJUSTFACTOR aj);

  /**
  * @brief 从通达信的下载文件中读取5的倍数分钟数据
  * @param _market: 指定市场，SH SZ
  * @param stockCode：股票代码字符串
  * @param times: 倍数
  * @param dataNum：用于保存数据数量（条数）的指针
  * @param aj: 复权类型
  * @return 返回tdxMinuteData结构体指针，需要手动free，出错则返回NULL
  */
tdxRawData *tdx_get_min5_times_data(enum MARKET _market,const char* stockCode,int times,unsigned int *dataNum,enum ADJUSTFACTOR aj);

  /**
  * @brief 从通达信的下载文件中读取日的倍数数据
  * @param _market: 指定市场，SH SZ
  * @param stockCode：股票代码字符串
  * @param times: 倍数
  * @param dataNum：用于保存数据数量（条数）的指针
  * @param aj: 复权类型
  * @return 返回tdxDayData结构体指针，需要手动free，出错则返回NULL
  */
tdxRawData *tdx_get_day_times_data(enum MARKET _market,const char* stockCode,int times,unsigned int *dataNum,enum ADJUSTFACTOR aj);


  /**
  * @brief 从通达信的下载文件中读取周线数据
  * @param _market: 指定市场，SH SZ
  * @param stockCode：股票代码字符串
  * @param dataNum：用于保存数据数量（条数）的指针
  * @param aj: 复权类型
  * @return 返回tdxRawData结构体指针，需要手动free，出错则返回NULL
  */
tdxRawData *tdx_get_week_data(enum MARKET _market,const char* stockCode,unsigned int *dataNum,enum ADJUSTFACTOR aj);


  /**
  * @brief 从通达信的下载文件中读取月线数据
  * @param _market: 指定市场，SH SZ
  * @param stockCode：股票代码字符串
  * @param dataNum：用于保存数据数量（条数）的指针
  * @param aj: 复权类型
  * @return 返回tdxRawData结构体指针，需要手动free，出错则返回NULL
  */
tdxRawData *tdx_get_month_data(enum MARKET _market,const char* stockCode,unsigned int *dataNum,enum ADJUSTFACTOR aj);

  /**
  * @brief 将通达信文件中分钟数据读取出来的日期转换成unsigned int 日期如20210701
  * @param date: 分钟文件读取出来的日期
  * @return 返回unsigned int 日期如20210701
  */
unsigned int tdx_lcfile_date_to_unsigned_int(const unsigned short date);




#endif