#include "xmlserver.h"

#include <QDomDocument>
#include <QFile>

#include <QAxObject>
#include <QAxWidget>

#include <QApplication>
#include <QDir>
#include <QDebug>

XmlServer::XmlServer()
{
    m_iRecordCount = 0;
    m_iCurrentCount = 0;
    mode = Default;
}

bool XmlServer::getXmlObject(QString filePath)
{
    currentXmlFile.setFileName(filePath);
    if(mode == DataToExcel) {
        if(!currentXmlFile.open(QIODevice::ReadWrite))
        {
            qDebug()<<"getXmlObject error\tmode = "<<mode;
            currentXmlFile.close();
            return false;
        }

        if(!m_domDocument.setContent(&currentXmlFile))
        {
            qDebug()<<"m_domDocument.setContent error\tmode = "<<mode;
            currentXmlFile.close();
            return false;
        }
        //currentXmlFile.close();
    }
    else if(mode == DataToTS) {
        if(!currentXmlFile.open(QIODevice::ReadWrite)) {
            qDebug()<<"getXmlObject error\tmode = "<<mode;
            currentXmlFile.close();
            return false;
        }
        xmlReader.setDevice(&currentXmlFile);
        qDebug()<<"Open "<<filePath<<" success";
    }
    mode = Default;
    return true;
}

void XmlServer::setDataToExcel(QString strTsFilePath, QString strExcelFilePath)
{
    mode = DataToExcel;
    QString filePath = strTsFilePath;
    m_iRecordCount = 0;
    if(!getXmlObject(filePath))
        return;
    currentXmlFile.close(); //Add
    QDomElement root = m_domDocument.documentElement();
    QDomNodeList contextNodeList = root.elementsByTagName("context");

    QDomNode contextDomNode, messageDomNode;
    QDomElement contextDomElement, messageDomElement;
    QString strDomName, strSource, strTranslation;

    QList< QList<QVariant> > m_datas;
    //    QScopedPointer<ExcelBase> m_xls;
    //    if(m_xls.isNull()) {
    //        m_xls.reset(new ExcelBase);
    //    }
    ExcelBase *m_xls = new ExcelBase;

    m_xls->create(/*QApplication::applicationDirPath() + "\\" + */strExcelFilePath);

    for(int k=0; k<contextNodeList.size(); k++)
    {
        //context 节点
        contextDomNode = contextNodeList.at(k);
        contextDomElement = contextDomNode.toElement();

        //context 节点 name 子节点
        strDomName = contextDomElement.elementsByTagName("name").at(0).toElement().text();

        QDomNodeList messageNodeList = contextDomElement.elementsByTagName("message");
        for(int i=0; i<messageNodeList.size(); i++)
        {
            //message NODE 节点
            messageDomNode = messageNodeList.at(i);
            messageDomElement = messageDomNode.toElement();

            strSource = messageDomElement.elementsByTagName("source").at(0).toElement().text();
            strTranslation = messageDomElement.elementsByTagName("translation").at(0).toElement().text();

            //qDebug()<<"k =\t"<<k<<"\t"<<"i =\t"<<i;
            //qDebug()<<"strSource\t"<<strSource<<"\t"<<"strTranslation\t"<<strTranslation;
            QList<QVariant> rows;
            rows.append(strDomName);
            rows.append(strSource);
            rows.append(strTranslation);
            m_datas.append(rows);
        }
    }
    m_xls->setCurrentSheet(1);
    m_xls->writeCurrentSheet(m_datas);
    m_xls->save();
    m_xls->close();

    delete m_xls;
}

bool XmlServer::getExcelObject(QString filePath)
{
    qDebug()<<"Excel filePath"<<filePath;
    if(!m_excelBase.open(filePath))
    {
        m_excelBase.close();
        qDebug()<<"getExcelObject Error...";
        return false;
    }

    return true;
}

void XmlServer::setDataToXml(QString strExcelFilePath, QString strTsFilePath, QString setOutTsFile)
{
    //QString filePath = strExcelFilePath;
    mode = DataToTS;
    if(!getExcelObject(strExcelFilePath)) //m_excelBase
    {
        return;
    }
    m_excelBase.setCurrentSheet(1);
    m_iRecordCount = 0;
    if(!getXmlObject(strTsFilePath))
    {
        return;
    }

    QFile outXmlFile;
    outXmlFile.setFileName(setOutTsFile);
    if(!outXmlFile.open(QIODevice::WriteOnly)) {
        qDebug()<<"open out xml file error";
        return;
    }
    QXmlStreamWriter xmlWriter;
    xmlWriter.setDevice(&outXmlFile);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeDTD("<!DOCTYPE TS>");

    //Read >>>
    while(!xmlReader.atEnd()) {
        xmlReader.readNext();
        if(xmlReader.isStartElement()) {
            QXmlStreamAttributes attrs = xmlReader.attributes();
            xmlWriter.writeStartElement(xmlReader.name().toString());
            if(attrs.size()>0) {
                xmlWriter.writeAttributes(attrs);
            }
            //continue;

            QString strElementName = xmlReader.name().toString();
            //qDebug()<<"start element\t"<<strElementName;
            /*            if(QString::compare(strElementName, "context") == 0) {
                qDebug()<<"==========Start Element <context>==========";
            }
            else */
            if(QString::compare(strElementName, "message") == 0) {
                xmlReader.readNext();
                if(xmlReader.isCharacters()) {
                    //qDebug()<<"message:\t\t"<<xmlReader.text().toString();
                    //xmlWriter.writeCharacters("\n\t\t");
                    xmlWriter.writeCharacters(xmlReader.text().toString());
                }
            }
            else if(QString::compare(strElementName, "location") == 0) {
                xmlReader.readNext();
                if(xmlReader.isCharacters()) {
                    //qDebug()<<"location:\t\t"<<xmlReader.text().toString();
                    xmlWriter.writeCharacters(xmlReader.text().toString());
                }
            }
            else if(QString::compare(strElementName, "source") == 0) {
                xmlReader.readNext();
                if(xmlReader.text().toString() != m_excelBase.read(m_iRecordCount + 1, 2).toString()) {
                    qDebug()<<"Validate error at col"<<m_iRecordCount+1<<" string is"<<xmlReader.text().toString();
                    continue;
                }
                if(xmlReader.isCharacters()) {
                    qDebug()<<"source:\t\t"<<xmlReader.text().toString();
                    xmlWriter.writeCharacters(xmlReader.text().toString());
                }
            }
            else if(QString::compare(strElementName, "translation") == 0) {
                xmlReader.readNext();
                //xmlWriter.writeStartElement(xmlReader.name().toString());
                if(xmlReader.isEndElement()) { //add for empty text
                    m_iRecordCount++;
                    //xmlWriter.writeCharacters("");
                    xmlWriter.writeCharacters(m_excelBase.read(m_iRecordCount, 3).toString());
                    xmlWriter.writeEndElement();
                    xmlReader.readNext();
                }
                else if(xmlReader.isCharacters()) {
                    m_iRecordCount++;
                    xmlWriter.writeCharacters(m_excelBase.read(m_iRecordCount, 3).toString());
                }
                qDebug()<<"translation\t:"<<m_excelBase.read(m_iRecordCount, 3).toString();
            }
            else{
                xmlReader.readNext();
                xmlWriter.writeCharacters(xmlReader.text().toString());
            }
        }

        if(xmlReader.isEndElement()) {
            //qDebug()<<"end   element\t"<<xmlReader.name().toString();
            xmlWriter.writeEndElement();
        }
    }
    qDebug()<<"m_iRecordCount"<<m_iRecordCount;
    xmlWriter.writeEndDocument();
    outXmlFile.close();
    currentXmlFile.close();
    m_excelBase.close();
    return;
}
