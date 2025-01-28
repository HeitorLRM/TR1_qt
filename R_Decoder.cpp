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
    switch (Sync::GetRSettings().framing) {
    case FRAMING::BYTE_INSERION: return DECODER::deframe_insert();
    case FRAMING::BYTE_COUNT:    return DECODER::deframe_count();
    }
}

