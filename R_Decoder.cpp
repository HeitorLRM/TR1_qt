#include "R_Decoder.hpp"
#include "R_Demodulator.hpp"
#include <memory>
#include <string>

using Receiver::Decoder;


Decoder* Decoder::Instance() {
	static std::unique_ptr<Decoder> instance(new Decoder);
	return instance.get();
}

Decoder::Decoder(){
}

std::string Decoder::listen_frame() {
	return listen_count_bytes();
}

std::string Decoder::listen_count_bytes() {
	unsigned char head = static_cast<char>(Demodulator::Instance()->read_byte());
	std::string message = "";
	while(head--) {
		message += Demodulator::Instance()->read_byte();
	}
	return message;
}
