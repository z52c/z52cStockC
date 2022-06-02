#ifndef UTIL__H
#define UTIL__H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "datastruct.h"
#include "config.h"
#include <vector>


  /**
  * @brief 从文件中获取股票列表
  * @return 返回std::vector<struct stockListNode>
  */
std::vector<struct stockListNode> z_get_stock_list();

  /**
  * @brief 返回多个小数中最大的数
  * @param argc: 数字的数量
  * @return 返回最大的小数
  */
double biggestNumber(const int argc,...);

  /**
  * @brief 返回多个小数中最小的数
  * @param argc: 数字的数量
  * @return 返回最小的小数
  */
double smallestNumber(const int argc,...);


  /**
  * @brief 计算给定日期是周几
  * @param _date: 指定的日期
  * @return 返回周几（0-6）
  */
int calcWeekDay(unsigned int _date);

  /**
  * @brief 计算给定日期之间的周数，用于malloc，比实际大
  * @param date1: 小的日期
  * @param date2: 大的日期
  * @return 返回周数
  */
int calcWeeksBetweenDate(unsigned int date1,unsigned int date2);

  /**
  * @brief 计算给定日期之间的月数
  * @param date1: 小的日期
  * @param date2: 大的日期
  * @return 返回月数
  */
int calcMonthsBetweenDate(unsigned int date1,unsigned int date2);

#endif