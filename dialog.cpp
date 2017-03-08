#include "dialog.h"
#include "ui_dialog.h"

#include "xmlserver.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QTimer>

#include <QDebug>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    m_pXmlServer = new XmlServer;

}

Dialog::~Dialog()
{
    if(m_pXmlServer)
        delete m_pXmlServer;

    delete ui;
}




void Dialog::on_btnWriteExcel_clicked()
{
    QString tsFilePath = this->ui->leFilePath->text();
    QString excelFileName = this->ui->leExcelPath->text();

    QString errStr;
    if(tsFilePath.isEmpty() || excelFileName.isEmpty())
    {
        errStr = tr("����ѡ��TS�ļ���");

    }

    if(tsFilePath.isEmpty() || excelFileName.isEmpty())
    {
        errStr = tr("������дExcel�ļ�����");

    }

    if(!errStr.isEmpty())
    {
        QMessageBox::information(this, tr("����"), errStr, QMessageBox::Yes);
        return;
    }

    QString excelFilePath = ui->leFilePath->text();
    int index = excelFilePath.lastIndexOf("/");
    excelFilePath.resize(index);
    excelFilePath = excelFilePath + "/" + excelFileName;
    qDebug()<<"tsFile"<<tsFilePath;


    ui->lbProcess->setText(tr("����д��Excel����..."));
    qApp->processEvents();
    qDebug()<<"excelFilePath"<<excelFilePath;

    m_pXmlServer->setDataToExcel(tsFilePath, excelFilePath);
    ui->lbProcess->setText(tr("д������!"));
    ui->leExcelPath->setText(excelFilePath);
}

void Dialog::on_btnChooseFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("ѡ��TS�ļ�"),
                                                    QApplication::applicationDirPath(),
                                                     tr("TS (*.ts);;�����ļ�(*.*)"));
    if(!fileName.isEmpty())
    {
        ui->leFilePath->setText(fileName);
    }
}

void Dialog::on_btnWriteCancel_clicked()
{
    qApp->exit(0);
}

void Dialog::on_btnExcelSource_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("ѡ��Excel�ļ�"),QApplication::applicationDirPath(),
                                                    tr("Excel (*.xlsx *.xls);;�����ļ�(*.*)"));
    if(!fileName.isEmpty())
    {
        ui->leInExcel->setText(fileName);
    }
}

void Dialog::on_btnCancel_clicked()
{
    qApp->exit(0);
}

void Dialog::on_btnTsOut_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("ѡ��TS�ļ�"),
                                                    QApplication::applicationDirPath(),
                                                     tr("TS (*.ts);;�����ļ�(*.*)"));
    if(!fileName.isEmpty())
    {
        ui->leInTs->setText(fileName);
    }
}


void Dialog::on_btnWriteTs_clicked()
{
    QString inExcel = ui->leInExcel->text();
    QString inTs = ui->leInTs->text();
    QString outTs = ui->leOutTsName->text();

    if(inExcel.isEmpty() || inTs.isEmpty() || outTs.isEmpty())
    {
        qDebug()<<"Some of these files is empty";
        ui->lblGetProcess->setText("Some of these files is empty");
        return; //test --
    }


    QString outTsPath(inTs);
    int index = outTsPath.lastIndexOf("/");
    outTsPath.resize(index + 1);
    outTs.insert(0, outTsPath);

//    QFile::copy(inTs, outTs);
//    inExcel = "C:/Users/Biantf/Desktop/123.xls"; //test ++
//    outTs = "C:/Users/Biantf/Desktop/test.ts"; // test ++
    qDebug()<<inExcel<<inTs<<outTs;
    m_pXmlServer->setDataToXml(inExcel, inTs, outTs);

}
