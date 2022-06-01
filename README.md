# z52cStockC
股票数据分析/编程不太会，会多少写多少，基础数据来源通达信，复权数据来源凤凰网
  
## 股票列表
股票列表从证监会的行业分类中获取：http://www.csrc.gov.cn/csrc/c100103/common_list.shtml  
简单整理就能得到一个csv表格，只要股票代码一个字段就可以了，保存到stockList.csv中

## 数据的复权处理
网上很多使用涨跌幅复权因子处理数据的，个人觉得不是很准确  
这里使用跟通达信一样的处理方式  
$前复权价格=(复权前价格-每股分红现金+配股比例*配股价格)/(1+配股比例+送股比例)$  

## 分红配股数据来源
分红派息记录从凤凰网获取，例：https://app.finance.ifeng.com/data/stock/tab_fhpxjl.php?symbol=002555  
凤凰网数据请求频繁会限制，没有测试具体频率，使用过程直接用了1秒的间隔


配股数据从东方财富获取：`https://datacenter-web.eastmoney.com/api/data/v1/get?sortColumns=EQUITY_RECORD_DATE&sortTypes=-1&pageSize=500&pageNumber=1&reportName=RPT_IPO_ALLOTMENT&columns=ALL&quoteColumns=f2~01~SECURITY_CODE~NEW_PRICE&source=WEB&client=WEB`
每一页最多只能获取500条数据

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
后面的配股数据更新，在pg.txt中放入东方财富链接的内容(列表)  
运行fhpg.py更新
