#include "CamadaEnlace.hpp"
#include "R_Demodulator.hpp"

#include <cstdint>
#include <iostream>

std::string ENCODER::encode_msg(const std::string& message, FRAMING framing, ERROR error_mode) {
    // Mensagem precisa caber no quadro
    if (message.size() > max_bytes) {
        std::string first(message.begin(), message.begin() + max_bytes);
        std::string rest(message.begin() + max_bytes, message.end());
        return encode_msg(first, framing, error_mode) + encode_msg(rest, framing, error_mode);
    }

    // Codificamos a mensagem
    std::string err_encoded;
    switch (error_mode) {
    case ERROR::NONE:         err_encoded = message;            break;
    case ERROR::PARITY_BIT:   err_encoded = parity(message);    break;
    case ERROR::CRC:          err_encoded = CRC(message);       break;
    case ERROR::HAMMING:      err_encoded = Hamming(message);   break;
    default:break;
    }

    // Depois enquadramos
    std::string framed;
    switch (framing) {
    case FRAMING::BYTE_COUNT:    framed = count_bytes(err_encoded);  break;
    case FRAMING::BYTE_INSERION: framed = insert_bytes(err_encoded); break;
    default:break;
    }

    return framed;
}

std::string ENCODER::count_bytes(const std::string& message) {
    unsigned char count = message.size(); // Byte de contagem
    return std::string{char(count)} + message;
}

std::string ENCODER::insert_bytes(const std::string & message) {
    // Fazemos o enquadramento somente depois de lidar com as flags falsas
    return frame_flags(fix_false_flags(message));
}

std::string ENCODER::fix_false_flags(std::string message) {
    // Inserimos caractere de escape antes de uma flag coincidente na mensagem
    for (auto it = message.begin(); it != message.end(); it++) {
        char c = *it;
        if (c == flag || c == esc)
            it = message.insert(it, esc) + 1;
    }

    return message;
}

std::string ENCODER::frame_flags(const std::string& data) {
    std::string result = "";
    result += flag;
    result += data;
    result += flag;
    return result;
}

bool _parity(const std::string& data) {
    int _1_count = 0;
    for (auto c : data)
        for (int i = 7; i >= 0; i--) { // bit por bit
            char bit = (c >> i) & 0x1;
            if (bit) _1_count++; // contar os 1s
        }
    return _1_count % 2;
}

std::string ENCODER::parity(const std::string& message) {
    // Concatenar o bit de paridade ao final
    std::string new_message = message;
    new_message += char(_parity(message)); // Por questoes de projeto isso requer um byte inteiro
    return new_message;
}

// Lê 32 bits de uma posição da string
uint32_t fetch_32(const std::string& s, unsigned bit_index) {
    uint32_t result = 0;
    for (unsigned i = 0; i < 32; i++) {
        unsigned bit = i + bit_index;
        unsigned byte = bit/8;
        unsigned bit_off = bit%8;
        unsigned c = static_cast<unsigned>(s[byte]);
        c >>= 7-bit_off;
        c &= 0x1;
        result |= c << (31-i);
    }
    return result;
}

// Escreve 32 bits numa string
void write_32(std::string& s, unsigned bit_index, uint32_t val) {
    for (unsigned i = 0; i < 32; i++) {
        bool set = val & 1 << (31-i);
        unsigned bit = i + bit_index;
        unsigned byte = bit/8;
        unsigned bit_off = bit%8;
        char mask = 1 << (7-bit_off);
        s[byte] &= ~mask;
        if (set) s[byte] |= mask;
    }
}

uint32_t crc32(std::string message) {
    // polinomio crc32, de acordo com IEEE
    // x32 + x26 + x23 + x22 + x16 + ...
    const uint32_t poly = 0x04C11DB7;

    // crc32 requer ao menos mensagem com 32 bits
    message = std::string{0x00, 0x00, 0x00, 0x00} + message;

    // mais 31 bits no final para o resto
    message += std::string{0x00, 0x00, 0x00, 0x00}; // Aqui são 32, corrigiremos depois
    unsigned message_size_bits = message.size()*8; // Lembrar do tamanho original

    uint32_t divisor = poly;

    // Percorrer a mensagem fazendo divisao polinomial
    for (int i = 0; i < message_size_bits-32; i++) {
        uint32_t dividend = fetch_32(message, i);
        if (!(dividend & 0x80000000)) continue; // Os primeiros 1s significativos do dividendo e do divisor devem alinhar

        // Calcula o resto
        uint32_t remainder = dividend ^ divisor;
        // Desce os outros bits do dividendo, que é igual a subir os do resto
        write_32(message, i, remainder);
    }

    // Os bits de detecção de erro estarão nos ultimos 31 bits
    uint32_t crc = fetch_32(message, message_size_bits-32);
    crc >>= 1; // Correção por serem 32
    return crc;
}

std::string ENCODER::CRC(const std::string& message) {
    auto crc = crc32(message);
    char c0 = crc >> 24;
    char c1 = crc >> 16;
    char c2 = crc >> 8;
    char c3 = crc >> 0;
    return message + std::string{c0,c1,c2,c3};
}

std::string ENCODER::Hamming(const std::string& message) {
    return message;
}


std::string DECODER::deframe_count() {
    // read byte pede ao desmodulador que ele leia um byte do sinal
    unsigned char head = static_cast<char>(Receiver::Demodulator::Instance()->read_byte());
    std::string message = "";
    while(head--) {
        message += Receiver::Demodulator::Instance()->read_byte();
    }
    return message;
}

std::string DECODER::deframe_insert() {
    char head = Receiver::Demodulator::Instance()->read_byte();
    if (head != ENCODER::flag) // flag denota o inicio do quadro
        return "";

    std::string result = "";
    while (true) {
        char c = Receiver::Demodulator::Instance()->read_byte();
        if (c == ENCODER::flag) // flag denota o fim do quadro
            break;
        if (c == ENCODER::esc) // ESC ignora o proximo caractere.
            c = Receiver::Demodulator::Instance()->read_byte();
        result += c;
    }
    return result;
}

std::string DECODER::detect_parity(const std::string& frame) {
    if (_parity(frame)) // Paridade, quando incluso bit de paridade, deve ser 0.
        return "\nQUADRO COM ERRO";

    return std::string(frame.begin(), frame.end() -1);
}

std::string DECODER::detect_crc(const std::string& frame) {
    std::string message(frame.begin(), frame.end()-4);
    std::string crc_str(frame.end()-4, frame.end());

    uint32_t crc1 = crc32(message);
    uint32_t crc2 = 0;
    crc2 <<= 8; crc2 += static_cast<unsigned char>(crc_str[0]);
    crc2 <<= 8; crc2 += static_cast<unsigned char>(crc_str[1]);
    crc2 <<= 8; crc2 += static_cast<unsigned char>(crc_str[2]);
    crc2 <<= 8; crc2 += static_cast<unsigned char>(crc_str[3]);
    if (crc2 != crc1) return ("\nQUADRO COM ERRO");

    return message;
}
