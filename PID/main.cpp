#include "gui.h"
#include "pid.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GUI w;
    PID pid(&w);

    return a.exec();
}
