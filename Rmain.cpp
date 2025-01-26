#include "Receiver_App.h"

#include <QApplication>

QT_USE_NAMESPACE

int Rmain(int argc, char *argv[]) {
    QApplication a(argc, argv);
    ReceiverAPP w;
    w.show();
    return a.exec();
}
