#include "T_Encoder.hpp"
#include "T_Modulator.hpp"
#include "CamadaEnlace.hpp"

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
    auto framed = ENCODER::encode_msg(message, FRAMING::BYTE_COUNT, ERROR::NONE);
    Modulator::Instance()->out() << framed;
}
