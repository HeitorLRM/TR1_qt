#include "T_App.h"
#include "T_WorkerThread.hpp"

#include <QApplication>
#include <QObject>

QT_USE_NAMESPACE

int Tmain(int argc, char *argv[]) {
    T_Worker worker;
    worker.start();

    QApplication a(argc, argv);
    TransmitterAPP t;

    QObject::connect(&t, &TransmitterAPP::send_message, &worker, &T_Worker::transmit_request);

    t.show();
    return a.exec();
}
