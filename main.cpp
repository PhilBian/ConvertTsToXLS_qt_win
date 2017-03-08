#include <QtGui>
#include <QString>

#include "dialog.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    QTextCodec *tc = QTextCodec::codecForName("gb2312");
    QTextCodec::setCodecForTr(tc);
    QTextCodec::setCodecForCStrings(tc);
    QTextCodec::setCodecForLocale(tc);
    Dialog dlg;
    dlg.show();

    return a.exec();
}
