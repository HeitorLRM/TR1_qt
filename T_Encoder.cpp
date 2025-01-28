#include "T_Encoder.hpp"
#include "T_Modulator.hpp"
#include "CamadaEnlace.hpp"
#include "Sync.hpp"

#include <memory>
#include <string>
#include <iostream>

using Transmitter::Encoder;
using std::string;

Encoder* Encoder::Instance() {
	static std::unique_ptr<Encoder> instance(new Encoder);
	return instance.get();
}

Encoder::Encoder() {
}

void Encoder::send(const string& message) {
    FRAMING framing = Sync::GetTSettings().framing;
    ERROR err_handl = Sync::GetTSettings().err_handling;
    auto framed = ENCODER::encode_msg(message, framing, err_handl);
    Modulator::Instance()->out() << framed;
}
