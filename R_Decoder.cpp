#include "R_Decoder.hpp"
#include "R_Demodulator.hpp"
#include "Sync.hpp"
#include "CamadaEnlace.hpp"
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
    std::string data = "";

    switch (Sync::GetRSettings().framing) {
    case FRAMING::BYTE_INSERION: data = DECODER::deframe_insert(); break;
    case FRAMING::BYTE_COUNT:    data =  DECODER::deframe_count(); break;
    default: break;
    }

    if (data == "") return "";

    switch (Sync::GetRSettings().err_handling) {
    case ERROR::NONE: return data;
    case ERROR::PARITY_BIT: return DECODER::detect_parity(data);
    case ERROR::CRC: return DECODER::detect_crc(data);
    case ERROR::HAMMING: return DECODER::detect_hamming(data);
    default: break;
    }

    return "";
}

