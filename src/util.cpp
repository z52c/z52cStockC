#include "../include/util.h"
#include <iostream>
#include <fstream>

extern std::ofstream logFile;

  /**
  * @brief 从文件中获取股票列表
  * @return 返回std::vector<struct stockListNode>
  */
std::vector<struct stockListNode> z_get_stock_list()
{
    struct stockListNode tmpNode;
    std::vector<struct stockListNode> stockListVector;
    FILE *fp = NULL;
    fp = fopen(STOCKLISTFILE,"r");
    while(!feof(fp))
    {
        fscanf(fp,"%s",tmpNode.stockCode);
        if(tmpNode.stockCode[0] == '6')
            tmpNode.market = SH;
        else 
            tmpNode.market = SZ;
        stockListVector.push_back(tmpNode);
    }
    return stockListVector;
}

  /**
  * @brief 返回多个小数中最大的数
  * @param argc: 数字的数量
  * @return 返回最大的小数
  */
double biggestNumber(const int argc,...)
{
    int i = 1;
    double nowNum;
    va_list argptr;
    va_start(argptr,argc);
    double biggest = va_arg(argptr,double);
    for(i=1;i<argc;i++)
    {
        nowNum = va_arg(argptr,double);
        if(nowNum>biggest)
            biggest = nowNum;
    }
    va_end(argptr);
    return biggest;
}

  /**
  * @brief 返回多个小数中最小的数
  * @param argc: 数字的数量
  * @return 返回最小的小数
  */
double smallestNumber(const int argc,...)
{
    int i = 1;
    double nowNum;
    va_list argptr;
    va_start(argptr,argc);
    double smallest = va_arg(argptr,double);
    for(i=1;i<argc;i++)
    {
        nowNum = va_arg(argptr,double);
        if(nowNum<smallest)
            smallest = nowNum;
    }
    va_end(argptr);
    return smallest;
}


int calcWeekDay(unsigned int _date)
{
    int y=_date/10000;
    int m=_date%10000/100;
    int d=_date%100;
    if(m==1 || m==2)
    {
        m+=12;
        y--;
    }
    return((d+2*m+3*(m+1)/5+y+y/4-y/100+y/400)%7);
}

int calcWeeksBetweenDate(unsigned int date1,unsigned int date2)
{
    int y1 = date1/10000;
    int y2 = date2/10000;
    int m1 = date1%10000/100;
    int m2 = date2%10000/100;
    return(((y2-y1)*12+m2-m1+1)*5);
}

int calcMonthsBetweenDate(unsigned int date1,unsigned int date2)
{
    int y1 = date1/10000;
    int y2 = date2/10000;
    int m1 = date1%10000/100;
    int m2 = date2%10000/100;
    return((y2-y1)*12+m2-m1+1);
}