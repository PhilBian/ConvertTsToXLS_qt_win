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
        errStr = tr("One or more files do not exist");

    }

    if(!errStr.isEmpty())
    {
        QMessageBox::information(this, tr("One or more files do not exist"), errStr, QMessageBox::Yes);
        return;
    }

    QString excelFilePath = ui->leFilePath->text();
    int index = excelFilePath.lastIndexOf("/");
    excelFilePath.resize(index);
    excelFilePath = excelFilePath + "/" + excelFileName;
    qDebug()<<"tsFile"<<tsFilePath;


    ui->lbProcess->setText(tr("Start setting data to excel"));
    qApp->processEvents();
    qDebug()<<"excelFilePath"<<excelFilePath;

    m_pXmlServer->setDataToExcel(tsFilePath, excelFilePath);
    ui->lbProcess->setText(tr("Data set finished"));
    ui->leExcelPath->setText(excelFilePath);
}

void Dialog::on_btnChooseFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select TS file"),
                                                    QApplication::applicationDirPath(),
                                                     tr("TS (*.ts);;All files(*.*)"));
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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select Excel file"),QApplication::applicationDirPath(),
                                                    tr("Excel (*.xlsx *.xls);;All files(*.*)"));
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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select TS file"),
                                                    QApplication::applicationDirPath(),
                                                     tr("TS (*.ts);;All files(*.*)"));
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
    ui->lblGetProcess->setText("Start setting data to TS");
    qDebug()<<inExcel<<inTs<<outTs;
    m_pXmlServer->setDataToXml(inExcel, inTs, outTs);
    ui->lblGetProcess->setText("Data set finished");
    ui->leOutTsName->setText(outTs);
}
