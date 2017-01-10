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
        errStr = tr("请先选择TS文件！");

    }

    if(tsFilePath.isEmpty() || excelFilePath.isEmpty())
    {
        errStr = tr("请先填写Excel文件名！");

    }

    if(!errStr.isEmpty())
    {
        QMessageBox::information(this, tr("错误"), errStr, QMessageBox::Yes);
        return;
    }

    ui->lbProcess->setText(tr("正在写入Excel数据..."));
    qApp->processEvents();
    qDebug()<<"excelFilePath"<<excelFilePath;
    m_pXmlServer->setDataToExcel(tsFilePath, excelFilePath);
    ui->lbProcess->setText(tr("写入完成!"));
}

void Dialog::on_btnChooseFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择TS文件"),
                                                    QApplication::applicationDirPath(),
                                                     tr("TS (*.ts);;所有文件(*.*)"));
    if(!fileName.isEmpty())
    {
        ui->leFilePath->setText(fileName);
    }
}

void Dialog::on_btnWriteCancel_clicked()
{
    qApp->exit(0);
}
