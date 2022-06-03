数据处理暂时只有从通达信盘后数据读取数据，进行复权处理，还有macd和均线的内容。
选股策略只有一种多级别macd结合的选股策略，说明在strategymacd1.h中
## 股票列表
股票列表从证监会的行业分类中获取：http://www.csrc.gov.cn/csrc/c100103/common_list.shtml  
简单整理就能得到一个csv表格，只要股票代码一个字段就可以了，保存到stockList.csv中

## 数据的复权处理
网上很多使用涨跌幅复权因子处理数据的，个人觉得不是很准确  
这里使用跟通达信一样的处理方式  
$前复权价格=\frac{复权前价格-每股分红现金+配股比例*配股价格}{1+配股比例+送股比例}$  

## 分红配股数据来源
分红送转股记录从凤凰网获取，例：https://app.finance.ifeng.com/data/stock/tab_fhpxjl.php?symbol=002555  
凤凰网数据请求频繁会限制，没有测试具体频率，使用过程直接用了1秒的间隔


配股数据从东方财富获取：`https://datacenter-web.eastmoney.com/api/data/v1/get?sortColumns=EQUITY_RECORD_DATE&sortTypes=-1&pageSize=500&pageNumber=1&reportName=RPT_IPO_ALLOTMENT&columns=ALL&quoteColumns=f2~01~SECURITY_CODE~NEW_PRICE&source=WEB&client=WEB`
每一页最多只能获取500条数据,一共就一千多条，直接抓取好像有限制，这里就直接复制下来扔进pd.txt文件了

## 分红配股数据获取以及存储
代码在fhpg目录中的fhpg.py中实现
分红配股数据保存在fhpg/fhpg.db的表fhpg中  
#### 表字段  
|  名字   | 类型  | 说明  |
|  ----  | ----  | ----  |
| stockid  | text | 股票代码，如002555 |
| date  | int | 股权登记日，如20220501 |
| fenhong  | real | 每股分红 |
| songzhuan  | real | 每股送转股数 |
| peigu  | real | 每股配股数量 |
| peigujia  | real | 配股价格 |
| type  | int | 1表示这一条是分红送转数据，2表示这一条是配股数据 |

#### 数据更新
fhpg.db中记录了20220530之前的分红配股数据  
后面的分红数据更新，在stockList = []中填入要更新的股票代码  
有新公司分红可以在东方财富查看，https://data.eastmoney.com/yjfp/  （股权登记日大于当天的不要录入--筛选已实施方案）  
后面的配股数据更新，在pg.txt中放入东方财富链接的内容(列表)  
运行fhpg.py更新

## 股票数据
数据用通达信软件下载盘后数据得到  
上海和深圳数据分别保存在  vipdoc/sh   vipdoc/sz  
日线数据文件 lday/sh600001.day  
1分钟数据文件 minline/sh600001.lc1  
5分钟数据文件 fzline/sh600001.lc5  

#### 通达信数据文件格式
文件内容均为2进制保存
分钟文件格式  

 * *.lc5   *.lc1
 * 每32个字节为一个5分钟数据，每字段内低字节在前 时间日期合成4字节
 * 00 ~ 01 字节：日期，整型，设其值为date，则日期计算方法为：
 *  year : date/2048+2004  
 *  month : date%2048/100
 *  day : date%2048%100
 * 02 ~ 03 字节： 从0点开始至目前的分钟数，整型
 * 04 ~ 07 字节：开盘价，float型
 * 08 ~ 11 字节：最高价，float型
 * 12 ~ 15 字节：最低价，float型
 * 16 ~ 19 字节：收盘价，float型
 * 20 ~ 23 字节：成交额，float型
 * 24 ~ 27 字节：成交量（股），整型
 * 28 ~ 31 字节：（保留）

`typedef struct _tdxMinuteData{
    unsigned short date;
    unsigned short time;
    float open;
    float high;
    float low;
    float close;
    float amount;
    unsigned int vol;
    unsigned int tmp;
}tdxMinuteData;`
   
日线文件格式  
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

 `typedef struct _tdxDayData{
    unsigned int date;
    unsigned int open100;
    unsigned int high100;
    unsigned int low100;
    unsigned int close100;
    float amount;
    unsigned int vol;
    unsigned int tmp;
}tdxDayData;`

**在本项目中为了方便统一格式，在使用过程中都以下面的格式**  

 * *综合分钟数据以及日线数据后统一的数据结构
 * 00 ~ 03 字节：日期，整型
 * 04 ~ 07 字节：开盘价，float型
 * 08 ~ 11 字节：最高价，float型
 * 12 ~ 15 字节：最低价，float型
 * 16 ~ 19 字节：收盘价，float型
 * 20 ~ 23 字节：成交额，float型
 * 24 ~ 27 字节：成交量（股），整型
 * 28 ~ 31 字节：time (分钟数据中保存时间，日线数据中留空)
   
`typedef struct _tdxRawData{
    unsigned int date;
    float open;
    float high;
    float low;
    float close;
    float amount;
    unsigned int vol;
    unsigned int time;
}tdxRawData;`
   


## 文件内容
* config.h   指定通达信目录以及一些文件位置
* datastruct.h  数据结构内容
* util.h  通达信数据之外的一些常用函数
* tdx.h tdx.cpp 通达信相关的数据处理,暂时只有前复权跟不复权的，因为我用不到后复权，里面的函数只为stockdata服务
* db.h db.cpp 分红送股配股数据库处理，只负责读取，更新在fhpg中的python文件
* stockdata.h stockdata.cpp 数据的处理在stockdata类中完成，暂时只有k线数据以及macd和均线
* strategymacd1.h strategymacd1.cpp 不同级别macd结合的一种选股策略
