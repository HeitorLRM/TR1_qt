#ifndef CAMADAENLACE_HPP
#define CAMADAENLACE_HPP

#include <string>

enum FRAMING {BYTE_COUNT, BYTE_INSERION};
enum ERROR {NONE, PARITY_BIT, CRC, HAMMING};

namespace ENCODER {
const unsigned max_bytes = 16;
const char flag = 0x7E;
const char esc = 0x7D;

std::string encode_msg(const std::string&, FRAMING, ERROR);

std::string count_bytes(const std::string&);

std::string insert_bytes(const std::string&);

std::string fix_false_flags(std::string);

std::string frame_flags(const std::string&);

std::string parity(const std::string&);

std::string CRC(const std::string&);

std::string Hamming(const std::string&);
}

namespace DECODER {

std::string deframe_count();

std::string deframe_insert();

std::string detect_parity(const std::string&);

std::string detect_crc(const std::string&);

}

#endif // CAMADAENLACE_HPP
