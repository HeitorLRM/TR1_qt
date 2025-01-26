#include "Transmitter_App.h"

#include <QApplication>

QT_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TransmitterAPP w;
    w.show();
    return a.exec();
}
