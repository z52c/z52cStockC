#ifndef DATASTRUCT_H
#define DATASTRUCT_H

/**
 * *.lc5   *.lc1
 * 每32个字节为一个5分钟数据，每字段内低字节在前 时间日期合成4字节
 * 00 ~ 01 字节：日期，整型，设其值为date，则日期计算方法为：
 *     year : date/2048+2004  
 *     month : date%2048/100
 *     day : date%2048%100
 * 02 ~ 03 字节： 从0点开始至目前的分钟数，整型
 * 04 ~ 07 字节：开盘价，float型
 * 08 ~ 11 字节：最高价，float型
 * 12 ~ 15 字节：最低价，float型
 * 16 ~ 19 字节：收盘价，float型
 * 20 ~ 23 字节：成交额，float型
 * 24 ~ 27 字节：成交量（股），整型
 * 28 ~ 31 字节：（保留）
 */
typedef struct _tdxMinuteData{
    unsigned short date;
    unsigned short time;
    float open;
    float high;
    float low;
    float close;
    float amount;
    unsigned int vol;
    unsigned int tmp;
}tdxMinuteData;


/**
 * *.day 文件结构
 * 日线文件中，每32字节（32bytes）为一天的记录。每4个字节为一项数据：
 * 00 ~ 03 字节：日期，整型
 * 04 ~ 07 字节：开盘价*100，整型
 * 08 ~ 11 字节：最高价*100，整型
 * 12 ~ 15 字节：最低价*100，整型
 * 16 ~ 19 字节：收盘价*100，整型
 * 20 ~ 23 字节：成交额，float型
 * 24 ~ 27 字节：成交量（股），整型
 * 28 ~ 31 字节：（保留）
 */
typedef struct _tdxDayData{
    unsigned int date;
    unsigned int open100;
    unsigned int high100;
    unsigned int low100;
    unsigned int close100;
    float amount;
    unsigned int vol;
    unsigned int tmp;
}tdxDayData;

/**
 * *综合分钟数据以及日线数据后统一的数据结构
 * 00 ~ 03 字节：日期，整型
 * 04 ~ 07 字节：开盘价，float型
 * 08 ~ 11 字节：最高价，float型
 * 12 ~ 15 字节：最低价，float型
 * 16 ~ 19 字节：收盘价，float型
 * 20 ~ 23 字节：成交额，float型
 * 24 ~ 27 字节：成交量（股），整型
 * 28 ~ 31 字节：time
 */

typedef struct _tdxRawData{
    unsigned int date;
    float open;
    float high;
    float low;
    float close;
    float amount;
    unsigned int vol;
    unsigned int time;
}tdxRawData;



  /**
  * @brief 股票市场，SH表示上海证券交易所, SZ表示深圳交易所
  */
enum MARKET{
    SH = 1,SZ
};


  /**
  * @brief 分红配股数据结构
  */
struct fhpg{
  int date;
  float fenhong;
  float songzhuan;
  float peigu;
  float peigujia;
};


  /**
  * @brief 复权类型
  */
enum ADJUSTFACTOR{
  NOADJUST=1,FOREADJUST,BACKADJUST
};

  /**
  * @brief 数据种类，MIN1表示1分钟数据, MIN5表示5分钟数据，DAY表示日线数据
  */
enum DATAKIND{
    MIN1 = 1 ,MIN5,MIN15,MIN30,MIN60,DAY,WEEK,MOONTH
};


  /**
  * @brief MACD数据结构
  */
struct macd{
  float emaShort;
  float emaLong;
  float dif;
  float dea;
  float macd; 
};


  /**
  * @brief 均线数据结构
  */
struct ma
{
  float ma5;
  float ma10;
};

  /**
  * @brief 股票列表节点数据结构
  */
struct stockListNode{
  MARKET market;
  char stockCode[7];
};

#endif