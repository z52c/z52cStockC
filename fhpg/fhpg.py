import requests
from lxml import etree
import sqlite3
import time


'''
数据库文件格式
stockid：002555 text
date：20220501 整数
fenhong： 每股分红 小数
songzhuan： 每股送转股 小数
peigu：每股配股数量   小数
peigujia：小数
type：1分红数据，2配股数据
'''


stockListFile = 'D:\\project\\z52cStockC\\stockList.csv'
urlHead = 'http://app.finance.ifeng.com/data/stock/tab_fhpxjl.php?symbol='
dbFile = './fhpg.db'

headers = {
    'user-agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/102.0.5005.61 Safari/537.36'
}

class sqliteConn:
    def __init__(self):
        self.conn = sqlite3.connect(dbFile)
        self.c = self.conn.cursor()
    
    def exit(self):
        self.conn.close()


    def getStockFHDate(self,stockid):
        rtn = []
        sqlStr = "select * from fhpg where stockid = '" + stockid + "' and type = 1 order by date asc"
        cursor = self.c.execute(sqlStr)
        if cursor.rowcount == 0:
            return rtn
        for i in cursor:
            rtn.append(i[1])
        return rtn
    
    def getStockPGDate(self,stockid):
        rtn = []
        sqlStr = "select * from fhpg where stockid = '" + stockid + "' and type = 2 order by date asc"
        cursor = self.c.execute(sqlStr)
        if cursor.rowcount == 0:
            return rtn
        for i in cursor:
            rtn.append(i[1])
        return rtn
    '''
    插入数据二维列表，列表内容与数据库表完全一致
    '''
    def pushStockFHData(self,dataList):
        for data in dataList:
            data.append(1)
            sqlStr = "insert into fhpg (stockid,date,fenhong,songzhuan,peigu,peigujia,type) values ('%s',%d,%f,%f,%f,%f,%d)" %tuple(data)
            self.conn.execute(sqlStr)
        self.conn.commit()

    def pushStockPGData(self,dataList):
        for data in dataList:
            data.append(2)
            sqlStr = "insert into fhpg (stockid,date,fenhong,songzhuan,peigu,peigujia,type) values ('%s',%d,%f,%f,%f,%f,%d)" %tuple(data)
            self.conn.execute(sqlStr)
        self.conn.commit()



def getHtml(stockid):
    url = urlHead+stockid
    try:
        r = requests.get(url,headers=headers)
        return r.text
    except:
        print('retry  ' + url)
        time.sleep(20)
        getHtml(stockid)

def getPG():
    with open('pg.txt','r',encoding='utf-8') as f:
        text = f.read()
    text = text.replace('null',"'null'")
    data = eval(text)
    rtn = []
    for i in data:
        tmp = []
        tmp.append(i['SECURITY_CODE'])
        tmp.append(int(i['EQUITY_RECORD_DATE'].replace(' 00:00:00','').replace('-','')))
        tmp.append(0.0)
        tmp.append(0.0)
        tmp.append(i['PLACING_RATIO']/10.0)
        tmp.append(i['ISSUE_PRICE'])
        rtn.append(tmp)
    return rtn




#  10股派现   .//tbody/tr[3]/td[2]
#  10股送股   .//tbody/tr[4]/td[2]
#  10股转增   .//tbody/tr[4]/td[4]
#  股权登记日   .//tbody/tr[5]/td[2]
def getFH(stockid):
    rtn = []
    rs = getHtml(stockid)
    _element = etree.HTML(rs)
    try:
        div = _element.xpath('/html/body/div[9]/div[2]')
        tables = div[0].xpath('.//table')
        for i in tables:
            line = []
            line.append(stockid)
            line.append(int(i.xpath('.//tbody/tr[5]/td[2]/text()')[0].replace('-','')))
            line.append(float(i.xpath('.//tbody/tr[3]/td[2]/text()')[0].replace('元',''))/10)
            line.append((float(i.xpath('.//tbody/tr[4]/td[2]/text()')[0].replace('股',''))+float(i.xpath('.//tbody/tr[4]/td[4]/text()')[0].replace('股','')))/10)
            line.append(0.0)
            line.append(0.0)
            rtn.append(line)
        return rtn
    except:
        return rtn



def getStockList():
    rtn = []
    with open(stockListFile,'r') as f:
        for i in f.readlines():
            rtn.append(i.replace('\n',''))
    return rtn



if __name__ == '__main__':
    stockList = []
    a = sqliteConn()
    for i in stockList:
        dateInDB = a.getStockFHDate(i)
        dataTmp = getFH(i)
        data = []
        for j in dataTmp:
            if j[1] not in dateInDB:
                data.append(j)
        if len(data) != 0:
            a.pushStockFHData(data)
        time.sleep(1)
    pgData = getPG()
    data = []
    for i in pgData:
        dateInDB = a.getStockPGDate(i[0])
        if i[1] not in dateInDB:
            data.append(i)
    a.pushStockPGData(data)
    a.exit()
