#include "../include/tdx.h"
#include <iostream>

extern fhpgDB *sqliteDB;


  /**
  * @brief 从通达信的下载文件中读取1分钟、5分钟以及日数据
  * @param _market: 指定市场，SH SZ
  * @param stockCode：股票代码字符串
  * @param fileSize：用于保存文件大小的指针
  * @return 返回void结构体指针，需要手动free，出错则返回NULL
  */
void* tdx_get_TDX_file_data(enum MARKET _market,const char* stockCode,enum DATAKIND _data_kind,unsigned int *fileSize)
{
    char fileName[100]; 
    const char *market_str;
    void *rtn = NULL;
    size_t file_size = 0;
    if(_market == SH)
        market_str="sh";
    else
        market_str="sz";
    switch(_data_kind)
    {
        case MIN1:
            if(_market == SH)
                strcpy(fileName,TDXSHMIN1DIR);
            else 
                strcpy(fileName,TDXSZMIN1DIR);
            strcat(fileName,market_str);
            strcat(fileName,stockCode);
            strcat(fileName,".lc1");
            break;
        case MIN5:
            if(_market == SH)
                strcpy(fileName,TDXSHMIN5DIR);
            else
                strcpy(fileName,TDXSZMIN5DIR);
            strcat(fileName,market_str);
            strcat(fileName,stockCode);
            strcat(fileName,".lc5");
            break;
        case DAY:
            if(_market == SH)
                strcpy(fileName,TDXSHDAYDIR);
            else
                strcpy(fileName,TDXSZDAYDIR);
            strcat(fileName,market_str);
            strcat(fileName,stockCode);
            strcat(fileName,".day");
            break;
    }    

    FILE *fp = NULL;
    if(!(fp=fopen(fileName,"rb")))
    {
        printf("file %s open failed!\n",fileName);
        return NULL;
    }
    fseek(fp,0,SEEK_END);
    file_size = ftell(fp);
    fseek(fp,0,SEEK_SET);
    rtn = malloc(file_size);
    if(!rtn)
    {
        printf("%s malloc failed!\n",stockCode);
        fclose(fp);
        return rtn;
    }
    if(fread(rtn,1,file_size,fp) != file_size)
    {
        printf("file %s read error!",fileName);
        free(rtn);
        return NULL;
    }
    fclose(fp);
    *fileSize = file_size;
    return rtn;
}


  /**
  * @brief 将通达信文件中分钟数据读取出来的日期转换成unsigned int 日期如20210701
  * @param date: 分钟文件读取出来的日期
  * @return 返回unsigned int 日期如20210701
  */
unsigned int tdx_lcfile_date_to_unsigned_int(const unsigned short date)
{
    unsigned int  year = date/2048 + 2004;
    unsigned int  month = date%2048/100;
    unsigned int  day = date%2048%100;
    unsigned int rtn = year*10000 + month*100 + day;
    return rtn;
}


  /**
  * @brief 将分钟数据的内容修改为统一格式的数据类型
  * @param minuteData: 数据内容的指针，直接从文件中读取出来的内容
  * @param dataNum：数据数量
  */
void minuteData2RawData(void* minuteData, unsigned int dataNum)
{
    tdxMinuteData *pM = (tdxMinuteData*)minuteData;
    tdxRawData *pR = (tdxRawData*)minuteData;
    for(unsigned int i=0;i<dataNum;i++)
    {
        pR[i].time = (unsigned int)pM[i].time/60*100 + (unsigned int)pM[i].time%60;
        pR[i].date = tdx_lcfile_date_to_unsigned_int(pM[i].date);
    }
}

  /**
  * @brief 将日线数据的内容修改为统一格式的数据类型
  * @param dayData: 数据内容的指针，直接从文件中读取出来的内容
  * @param dataNum：数据数量
  */
void dayData2RawData(void* dayData, unsigned int dataNum)
{
    tdxDayData *pD = (tdxDayData*)dayData;
    tdxRawData *pR = (tdxRawData*)dayData;
    for(unsigned int i=0;i<dataNum;i++)
    {
        pR[i].open = (float)pD[i].open100/100.0;
        pR[i].close = (float)pD[i].close100/100.0;
        pR[i].high = (float)pD[i].high100/100.0;
        pR[i].low = (float)pD[i].low100/100.0;
    }
}

  /**
  * @brief 从通达信的下载文件中读取1分钟数据,带复权类型，暂时只有前复权
  * @param _market: 指定市场，SH SZ
  * @param stockCode：股票代码字符串
  * @param dataNum：用于保存数据数量（条数）的指针
  * @param aj：复权类型
  * @return 返回tdxRawData结构体指针，需要手动free，出错则返回NULL
  */
tdxRawData * tdx_get_min1_data(enum MARKET _market,const char* stockCode,unsigned int *dataNum,enum ADJUSTFACTOR aj)
{
    void * data = NULL;
    unsigned int fileSize = 0;
    *dataNum = 0;
    if(!(data = tdx_get_TDX_file_data(_market,stockCode,MIN1,&fileSize)))
        return NULL;
    *dataNum = fileSize/sizeof(tdxMinuteData);
    minuteData2RawData(data,*dataNum);
    tdxRawData* pRtn = (tdxRawData *)data;
    vector<struct fhpg> fhpgData = sqliteDB->getFHPGData(stockCode);
    if(aj == FOREADJUST)
    {
        for(unsigned int i=0;i<*dataNum;i++)
        {
            for(int j=0;j<fhpgData.size();j++)
            {
                if(pRtn[i].date<=fhpgData[j].date)
                {
                    pRtn[i].open = (pRtn[i].open-fhpgData[j].fenhong+fhpgData[j].peigu*fhpgData[j].peigujia)/(1+fhpgData[j].peigu+fhpgData[j].songzhuan);
                    pRtn[i].close = (pRtn[i].close-fhpgData[j].fenhong+fhpgData[j].peigu*fhpgData[j].peigujia)/(1+fhpgData[j].peigu+fhpgData[j].songzhuan);
                    pRtn[i].high = (pRtn[i].high-fhpgData[j].fenhong+fhpgData[j].peigu*fhpgData[j].peigujia)/(1+fhpgData[j].peigu+fhpgData[j].songzhuan);
                    pRtn[i].low = (pRtn[i].low-fhpgData[j].fenhong+fhpgData[j].peigu*fhpgData[j].peigujia)/(1+fhpgData[j].peigu+fhpgData[j].songzhuan);
                }
            }
            pRtn[i].open = ((float)((int)((pRtn[i].open+0.005)*100)))/100;
            pRtn[i].close = ((float)((int)((pRtn[i].close+0.005)*100)))/100;
            pRtn[i].high = ((float)((int)((pRtn[i].high+0.005)*100)))/100;
            pRtn[i].low = ((float)((int)((pRtn[i].low+0.005)*100)))/100;
        }
    }
    return (tdxRawData *)data;
}

tdxRawData * tdx_get_min5_data(enum MARKET _market,const char* stockCode,unsigned int *dataNum,enum ADJUSTFACTOR aj)
{
    void * data = NULL;
    unsigned int fileSize = 0;
    *dataNum = 0;
    if(!(data = tdx_get_TDX_file_data(_market,stockCode,MIN5,&fileSize)))
        return NULL;
    *dataNum = fileSize/sizeof(tdxMinuteData);
    minuteData2RawData(data,*dataNum);
    tdxRawData* pRtn = (tdxRawData *)data;
    vector<struct fhpg> fhpgData = sqliteDB->getFHPGData(stockCode);
    if(aj == FOREADJUST)
    {
        for(unsigned int i=0;i<*dataNum;i++)
        {
            for(int j=0;j<fhpgData.size();j++)
            {
                if(pRtn[i].date<=fhpgData[j].date)
                {
                    pRtn[i].open = (pRtn[i].open-fhpgData[j].fenhong+fhpgData[j].peigu*fhpgData[j].peigujia)/(1+fhpgData[j].peigu+fhpgData[j].songzhuan);
                    pRtn[i].close = (pRtn[i].close-fhpgData[j].fenhong+fhpgData[j].peigu*fhpgData[j].peigujia)/(1+fhpgData[j].peigu+fhpgData[j].songzhuan);
                    pRtn[i].high = (pRtn[i].high-fhpgData[j].fenhong+fhpgData[j].peigu*fhpgData[j].peigujia)/(1+fhpgData[j].peigu+fhpgData[j].songzhuan);
                    pRtn[i].low = (pRtn[i].low-fhpgData[j].fenhong+fhpgData[j].peigu*fhpgData[j].peigujia)/(1+fhpgData[j].peigu+fhpgData[j].songzhuan);
                }
            }
            pRtn[i].open = ((float)((int)((pRtn[i].open+0.005)*100)))/100;
            pRtn[i].close = ((float)((int)((pRtn[i].close+0.005)*100)))/100;
            pRtn[i].high = ((float)((int)((pRtn[i].high+0.005)*100)))/100;
            pRtn[i].low = ((float)((int)((pRtn[i].low+0.005)*100)))/100;
        }
    }
    return (tdxRawData *)data;
}

tdxRawData * tdx_get_day_data(enum MARKET _market,const char* stockCode,unsigned int *dataNum,enum ADJUSTFACTOR aj)
{
    void * data = NULL;
    unsigned int fileSize = 0;
    *dataNum = 0;
    if(!(data = tdx_get_TDX_file_data(_market,stockCode,DAY,&fileSize)))
        return NULL;
    *dataNum = fileSize/sizeof(tdxDayData);
    dayData2RawData(data,*dataNum);
    tdxRawData* pRtn = (tdxRawData *)data;
    vector<struct fhpg> fhpgData = sqliteDB->getFHPGData(stockCode);
    if(aj == FOREADJUST)
    {
        for(unsigned int i=0;i<*dataNum;i++)
        {
            for(int j=0;j<fhpgData.size();j++)
            {
                if(pRtn[i].date<=fhpgData[j].date)
                {
                    pRtn[i].open = (pRtn[i].open-fhpgData[j].fenhong+fhpgData[j].peigu*fhpgData[j].peigujia)/(1+fhpgData[j].peigu+fhpgData[j].songzhuan);
                    pRtn[i].close = (pRtn[i].close-fhpgData[j].fenhong+fhpgData[j].peigu*fhpgData[j].peigujia)/(1+fhpgData[j].peigu+fhpgData[j].songzhuan);
                    pRtn[i].high = (pRtn[i].high-fhpgData[j].fenhong+fhpgData[j].peigu*fhpgData[j].peigujia)/(1+fhpgData[j].peigu+fhpgData[j].songzhuan);
                    pRtn[i].low = (pRtn[i].low-fhpgData[j].fenhong+fhpgData[j].peigu*fhpgData[j].peigujia)/(1+fhpgData[j].peigu+fhpgData[j].songzhuan);
                }
            }
            pRtn[i].open = ((float)((int)((pRtn[i].open+0.005)*100)))/100;
            pRtn[i].close = ((float)((int)((pRtn[i].close+0.005)*100)))/100;
            pRtn[i].high = ((float)((int)((pRtn[i].high+0.005)*100)))/100;
            pRtn[i].low = ((float)((int)((pRtn[i].low+0.005)*100)))/100;
        }
    }
    return (tdxRawData *)data;
}

  /**
  * @brief 从通达信的下载文件中读取5的倍数分钟数据
  * @param _market: 指定市场，SH SZ
  * @param stockCode：股票代码字符串
  * @param times: 倍数
  * @param dataNum：用于保存数据数量（条数）的指针
  * @return 返回tdxRawData结构体指针，需要手动free，出错则返回NULL
  */
tdxRawData *tdx_get_min5_times_data(enum MARKET _market,const char* stockCode,int times,unsigned int *dataNum,enum ADJUSTFACTOR aj)
{
    tdxRawData * dataMin5 = NULL;
    tdxRawData *data = NULL;
    dataMin5 = tdx_get_min5_data(_market,stockCode,dataNum,aj);
    if(NULL == dataMin5)
        return data;
    data = (tdxRawData*)malloc(*dataNum/times*sizeof(tdxRawData));
    if(NULL == data)
    {
        *dataNum = 0;
        printf("tdx_get_min5_times_data %s - 5*%d malloc failed",stockCode,times);
        free(dataMin5);
        return data;
    }
    tdxRawData *pStart = dataMin5;
    dataMin5 = dataMin5 + *dataNum%times;
    for(unsigned int i=0;i<*dataNum/times;i++)
    {
        data[i].date = dataMin5[times*i].date;
        data[i].time = dataMin5[times*i+times-1].time;
        data[i].open = dataMin5[times*i].open;
        data[i].close = dataMin5[times*i+times-1].close;
        data[i].high = dataMin5[times*i].high;
        data[i].low = dataMin5[times*i].low;
        data[i].amount = dataMin5[times*i].amount;
        data[i].vol = dataMin5[times*i].vol;
        for(int j=1;j<times;j++)
        {
            if(dataMin5[times*i+j].high>data[i].high)
                data[i].high = dataMin5[times*i+j].high;
            if(dataMin5[times*i+j].low<data[i].low)
                data[i].low = dataMin5[times*i+j].low;
            data[i].amount += dataMin5[times*i+j].amount;
            data[i].vol += dataMin5[times*i+j].vol;
        }
    }
    *dataNum = *dataNum/times;
    free(pStart);
    return data;
}

  /**
  * @brief 从通达信的下载文件中读取日的倍数数据
  * @param _market: 指定市场，SH SZ
  * @param stockCode：股票代码字符串
  * @param times: 倍数
  * @param dataNum：用于保存数据数量（条数）的指针
  * @return 返回tdxRawData结构体指针，需要手动free，出错则返回NULL
  */
tdxRawData *tdx_get_day_times_data(enum MARKET _market,const char* stockCode,int times,unsigned int *dataNum,enum ADJUSTFACTOR aj)
{
    tdxRawData * dataDay = NULL;
    tdxRawData *data = NULL;
    dataDay = tdx_get_day_data(_market,stockCode,dataNum,aj);
    if(NULL == dataDay)
        return data;
    data = (tdxRawData*)malloc(*dataNum/times*sizeof(tdxRawData));
    if(NULL == data)
    {
        *dataNum = 0;
        printf("tdx_get_day_times_data %s - 5*%d malloc failed",stockCode,times);
        free(dataDay);
        return data;
    }
    for(unsigned int i=0;i<*dataNum/times;i++)
    {
        data[i].date = dataDay[times*i].date;
        data[i].open = dataDay[times*i].open;
        data[i].high = dataDay[times*i+times-1].close;
        data[i].high = dataDay[times*i].high;
        data[i].low = dataDay[times*i].low;
        data[i].amount = dataDay[times*i].amount;
        data[i].vol = dataDay[times*i].vol;
        for(int j=1;j<times;j++)
        {
            if(dataDay[times*i+j].high>data[i].high)
                data[i].high = dataDay[times*i+j].high;
            if(dataDay[times*i+j].low>data[i].low)
                data[i].low = dataDay[times*i+j].low;
            data[i].amount += dataDay[times*i+j].amount;
            data[i].vol += dataDay[times*i+j].vol;
        }
    }
    *dataNum = *dataNum/times;
    free(dataDay);
    return data;
}


  /**
  * @brief 从通达信的下载文件中读取周线数据
  * @param _market: 指定市场，SH SZ
  * @param stockCode：股票代码字符串
  * @param dataNum：用于保存数据数量（条数）的指针
  * @return 返回tdxRawData结构体指针，需要手动free，出错则返回NULL
  */
tdxRawData *tdx_get_week_data(enum MARKET _market,const char* stockCode,unsigned int *dataNum,enum ADJUSTFACTOR aj)
{
    tdxRawData * dataDay = NULL;
    tdxRawData *data = NULL;
    unsigned int weekNum=0;
    dataDay = tdx_get_day_data(_market,stockCode,dataNum,aj);
    if(NULL == dataDay)
        return data;
    data = (tdxRawData*)malloc(calcWeeksBetweenDate(dataDay[0].date,dataDay[*dataNum-1].date)*sizeof(tdxRawData));
    if(NULL == data)
    {
        *dataNum = 0;
        printf("tdx_get_week_data %smalloc failed",stockCode);
        free(dataDay);
        return data;
    }
    int nowWeekDay = -1;
    int lastWeekDay = -1;
    data[weekNum].date = dataDay[0].date;
    data[weekNum].open = dataDay[0].open;
    data[weekNum].low=dataDay[0].low;
    data[weekNum].high=dataDay[0].high;
    data[weekNum].amount = dataDay[0].amount;
    data[weekNum].vol = dataDay[0].vol;
    for(unsigned int i=1;i<*dataNum;i++)
    {
        if((nowWeekDay=calcWeekDay(dataDay[i].date))>lastWeekDay) //还是在当前周
        {
            if(data[weekNum].high<dataDay[i].high)
                data[weekNum].high=dataDay[i].high;
            if(data[weekNum].low>dataDay[i].low)
                data[weekNum].low=dataDay[i].low;
            data[weekNum].amount += dataDay[i].amount;
            data[weekNum].vol += dataDay[i].vol;
        }else{                                   //新的一周
            data[weekNum].close = dataDay[i-1].close;
            weekNum++;
            data[weekNum].date = dataDay[i].date;
            data[weekNum].open = dataDay[i].open;
            data[weekNum].low=dataDay[i].low;
            data[weekNum].high=dataDay[i].high;
            data[weekNum].amount = dataDay[i].amount;
            data[weekNum].vol = dataDay[i].vol;
        }
        lastWeekDay = nowWeekDay;
    }
    data[weekNum].close = dataDay[*dataNum-1].close;
    *dataNum = weekNum+1;
    free(dataDay);
    return data;
}

  /**
  * @brief 从通达信的下载文件中读取月线数据
  * @param _market: 指定市场，SH SZ
  * @param stockCode：股票代码字符串
  * @param dataNum：用于保存数据数量（条数）的指针
  * @return 返回tdxRawData结构体指针，需要手动free，出错则返回NULL
  */
tdxRawData *tdx_get_month_data(enum MARKET _market,const char* stockCode,unsigned int *dataNum,enum ADJUSTFACTOR aj)
{
    tdxRawData * dataDay = NULL;
    tdxRawData *data = NULL;
    unsigned int monthNum=0;
    dataDay = tdx_get_day_data(_market,stockCode,dataNum,aj);
    if(NULL == dataDay)
        return data;
    data = (tdxRawData*)malloc(calcMonthsBetweenDate(dataDay[0].date,dataDay[*dataNum-1].date)*sizeof(tdxRawData));
    if(NULL == data)
    {
        *dataNum = 0;
        printf("tdx_get_month_data %smalloc failed",stockCode);
        free(dataDay);
        return data;
    }
    int lastMonthDay = -1;
    data[monthNum].date = dataDay[0].date;
    data[monthNum].open = dataDay[0].open;
    data[monthNum].low=dataDay[0].low;
    data[monthNum].high=dataDay[0].high;
    data[monthNum].amount=dataDay[0].amount;
    data[monthNum].vol=dataDay[0].vol;
    for(unsigned int i=1;i<*dataNum;i++)
    {
        if(dataDay[i].date%100>lastMonthDay) //还是在当前yue
        {
            if(data[monthNum].high<dataDay[i].high)
                data[monthNum].high=dataDay[i].high;
            if(data[monthNum].low>dataDay[i].low)
                data[monthNum].low=dataDay[i].low;
            data[monthNum].amount += dataDay[i].amount;
            data[monthNum].vol += dataDay[i].vol;
        }else{                                   //新的一月
            data[monthNum].close = dataDay[i-1].close;
            monthNum++;
            data[monthNum].date = dataDay[i].date;
            data[monthNum].open = dataDay[i].open;
            data[monthNum].low=dataDay[i].low;
            data[monthNum].high=dataDay[i].high;
            data[monthNum].amount=dataDay[i].amount;
            data[monthNum].vol=dataDay[i].vol;
        }
        lastMonthDay = dataDay[i].date%100;
    }
    data[monthNum].close = dataDay[*dataNum-1].close;
    *dataNum = monthNum+1;
    free(dataDay);
    return data;
}
