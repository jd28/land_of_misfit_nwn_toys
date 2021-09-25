#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

#include "neverrunpwudpserice.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    NeverrunPWUdpSerice service{5230};
    return a.exec();
}
