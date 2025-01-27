#include "R_App.hpp"
#include "R_WorkerThread.hpp"

#include <iostream>
#include <thread>

#include <QApplication>

QT_USE_NAMESPACE

int Rmain(int argc, char *argv[]) {
    R_Worker* worker = R_Worker::Instance();
    worker->start();

    QApplication a(argc, argv);
    ReceiverAPP w;

    QObject::connect(&w, &ReceiverAPP::settings_changed, worker, &R_Worker::set_settings);
    QObject::connect(worker, &R_Worker::got_message, &w, &ReceiverAPP::on_receive_message);
    QObject::connect(worker, &R_Worker::got_bit, &w, &ReceiverAPP::on_receive_bit);

    w.set_settings(R_Settings());
    w.show();
    return a.exec();
}
