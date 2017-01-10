#ifndef XMLSERVER_H
#define XMLSERVER_H
#include <QString>
#include <QDomDocument>
#include "ExcelBase.h"

class XmlServer
{
private:
    QDomDocument m_domDocument;
public:
    XmlServer();

    void setDataToExcel(QString strTsFilePath, QString strExcelFilePath);

private:
    bool getXmlObject(QString filePath);

public:
    int m_iRecordCount;
    int m_iCurrentCount;
};

#endif // XMLSERVER_H
