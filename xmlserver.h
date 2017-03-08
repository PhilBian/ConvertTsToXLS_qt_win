#ifndef XMLSERVER_H
#define XMLSERVER_H
#include <QString>
#include <QDomDocument>
#include "ExcelBase.h"
#include <QFile>
#include <qxmlstream.h>

class XmlServer
{
private:
    QDomDocument m_domDocument;
    ExcelBase m_excelBase;
public:
    XmlServer();
    enum Mode {
        DataToExcel,
        DataToTS,
        Default = 65536
    };

    void setDataToExcel(QString strTsFilePath, QString strExcelFilePath);
    void setDataToXml(QString strExcelFilePath, QString strTsFilePath, QString setOutTsFile);

private:
    bool getXmlObject(QString filePath);
    bool getExcelObject(QString filePath);

public:
    int m_iRecordCount;
    int m_iCurrentCount;
    QFile currentXmlFile;
    QXmlStreamReader xmlReader;
    QXmlStreamWriter xmlWriter;
    Mode mode;
};

#endif // XMLSERVER_H
