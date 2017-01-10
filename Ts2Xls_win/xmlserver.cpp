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
}

bool XmlServer::getXmlObject(QString filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadWrite))
    {
        file.close();
        return false;
    }

    if(!m_domDocument.setContent(&file))
    {
        file.close();
        return false;
    }

    file.close();
    return true;
}

void XmlServer::setDataToExcel(QString strTsFilePath, QString strExcelFilePath)
{
    QString filePath = strTsFilePath;
    m_iRecordCount = 0;
    if(!getXmlObject(filePath))
        return;

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

    m_xls->create(QApplication::applicationDirPath() + "\\" + strExcelFilePath);

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

            qDebug()<<"strSource\t"<<strSource<<"\t"<<"strTranslation\t"<<strTranslation;
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
