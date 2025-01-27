#include "R_App.h"
#include "R_WorkerThread.hpp"

#include <iostream>
#include <thread>

#include <QApplication>

QT_USE_NAMESPACE

int Rmain(int argc, char *argv[]) {
    R_Worker worker;
    worker.start();

    QApplication a(argc, argv);
    ReceiverAPP w;

    QObject::connect(&worker, &R_Worker::got_message, &w, &ReceiverAPP::on_receive_message);

    w.show();
    return a.exec();
}
