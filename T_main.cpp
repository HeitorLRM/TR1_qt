#include "T_App.hpp"
#include "T_WorkerThread.hpp"

#include <QApplication>
#include <QObject>

QT_USE_NAMESPACE

int Tmain(int argc, char *argv[]) {
    T_Worker* worker = T_Worker::Instance();
    worker->start();

    QApplication a(argc, argv);
    TransmitterAPP t;

    QObject::connect(&t, &TransmitterAPP::send_message, worker, &T_Worker::transmit_request);
    QObject::connect(&t, &TransmitterAPP::settings_changed, worker, &T_Worker::set_settings);
    QObject::connect(worker, &T_Worker::sent_bit, &t, &TransmitterAPP::on_send_bit);
    QObject::connect(worker, &T_Worker::sent_energy, &t, &TransmitterAPP::on_send_signal);

    t.set_settings(T_Settings{10, 10, 0, T_Settings::MODS::NRZ_POLAR});
    t.show();
    return a.exec();
}
