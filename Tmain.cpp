#include "T_App.h"
#include "T_Modulator.hpp"
#include "T_Encoder.hpp"
#include "Sync.hpp"

#include <QApplication>
#include <thread>

QT_USE_NAMESPACE

int Tmain(int argc, char *argv[]) {
    Transmitter::Modulator::Instance()->modulation = Transmitter::Modulator::NRZ_POLAR;
    Transmitter::Encoder::Instance()->send("Hello World!");
    std::this_thread::sleep_until(Sync::next_byte());
    while(true)
        Sync::runT();

    QApplication a(argc, argv);
    TransmitterAPP w;
    w.show();
    return a.exec();
}
