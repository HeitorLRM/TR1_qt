#include "R_App.h"
#include "R_Decoder.hpp"
#include "R_Demodulator.hpp"
#include "Sync.hpp"

#include <QApplication>
#include <iostream>
#include <thread>

QT_USE_NAMESPACE

int Rmain(int argc, char *argv[]) {
    Receiver::Demodulator::Instance()->modulation = Receiver::Demodulator::NRZ_POLAR;
    std::this_thread::sleep_until(Sync::next_byte());

    while (true) {
        std::string message = Receiver::Decoder::Instance()->listen_frame();
        if (!message.empty())
            std::cout << "Read: " << message << std::endl;
    }

    QApplication a(argc, argv);
    ReceiverAPP w;
    w.show();
    return a.exec();
}
