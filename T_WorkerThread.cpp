#include "T_WorkerThread.hpp"
#include "T_Modulator.hpp"
#include "T_Encoder.hpp"
#include "Sync.hpp"
#include "T_App.hpp"

#include <thread>
#include <memory>
#include <iostream>

T_Worker* T_Worker::Instance() {
    static std::unique_ptr<T_Worker> instance(new T_Worker);
    return instance.get();
}

void T_Worker::emit_bit(bool bit) {
    emit sent_bit(bit);
}

void T_Worker::emit_energy(float f) {
    emit sent_energy(f);
}

void T_Worker::run() {
    std::this_thread::sleep_until(Sync::next_byte());
    while(true)
        Sync::runT();
}

void T_Worker::transmit_request(std::string message) {
    Transmitter::Encoder::Instance()->send(message);
}

void T_Worker::set_settings(T_Settings settings) {
    Sync::SetTSettings(settings);
}
