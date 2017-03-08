#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include <QTimer>
namespace Ui {
    class Dialog;
}

class XmlServer;
class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;

    XmlServer *m_pXmlServer;


private slots:
    void on_btnWriteCancel_clicked();
    void on_btnChooseFile_clicked();
    void on_btnWriteExcel_clicked();

    void on_btnExcelSource_clicked();
    void on_btnCancel_clicked();
    void on_btnTsOut_clicked();
    void on_btnWriteTs_clicked();
};

#endif // DIALOG_H
