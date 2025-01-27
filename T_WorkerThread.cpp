#include "T_WorkerThread.hpp"
#include "T_Modulator.hpp"
#include "T_Encoder.hpp"
#include "Sync.hpp"

#include <thread>

void T_Worker::run() {
    Transmitter::Modulator::Instance()->modulation = Transmitter::Modulator::NRZ_POLAR;
    Transmitter::Encoder::Instance()->send("Hello World!");
    std::this_thread::sleep_until(Sync::next_byte());
    while(true)
        Sync::runT();
}

void T_Worker::transmit_request(std::string message) {
    Transmitter::Encoder::Instance()->send(message);
}
