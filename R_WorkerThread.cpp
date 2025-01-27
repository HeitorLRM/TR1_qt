#include "R_WorkerThread.hpp"
#include "R_Decoder.hpp"
#include "R_Demodulator.hpp"
#include "R_WorkerThread.hpp"
#include "Sync.hpp"

#include <thread>

void R_Worker::run() {
    Receiver::Demodulator::Instance()->modulation = Receiver::Demodulator::NRZ_POLAR;
    std::this_thread::sleep_until(Sync::next_byte());

    while (should_run) {
        std::string message = Receiver::Decoder::Instance()->listen_frame();
        if (!message.empty()) {
            emit got_message(message);
        }
    }
}
