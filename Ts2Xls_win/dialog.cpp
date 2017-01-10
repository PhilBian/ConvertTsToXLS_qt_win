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
    QString excelFilePath = this->ui->leExcelPath->text();

    QString errStr;
    if(tsFilePath.isEmpty() || excelFilePath.isEmpty())
    {
        errStr = tr("����ѡ��TS�ļ���");

    }

    if(tsFilePath.isEmpty() || excelFilePath.isEmpty())
    {
        errStr = tr("������дExcel�ļ�����");

    }

    if(!errStr.isEmpty())
    {
        QMessageBox::information(this, tr("����"), errStr, QMessageBox::Yes);
        return;
    }

    ui->lbProcess->setText(tr("����д��Excel����..."));
    qApp->processEvents();
    qDebug()<<"excelFilePath"<<excelFilePath;
    m_pXmlServer->setDataToExcel(tsFilePath, excelFilePath);
    ui->lbProcess->setText(tr("д�����!"));
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
