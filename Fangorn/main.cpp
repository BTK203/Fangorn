#include "fangornmainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FangornMainWindow w;
    w.show();
    return a.exec();
}
