#include <QtGui/QApplication>
#include "AssemblyMainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AssemblyMainWindow w;
    w.show();

    return a.exec();
}
