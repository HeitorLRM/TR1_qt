#include "CamadaEnlace.hpp"
#include "R_Demodulator.hpp"

#include <cstdint>
#include <iostream>
#include <vector>
#include <algorithm>

// Codigo para obter um bit de uma sequencia de bytes em string
bool str_get_bit(const std::string& s, unsigned index) {
    unsigned byte = index/8;
    unsigned b_off = index%8;
    return s[byte] & (0x1 << (7-b_off));
}

// Codigo para definir um bit de uma sequencia de bytes em string
void str_set_bit(std::string& s, unsigned index, bool bit) {
    unsigned byte = index/8;
    unsigned b_off = index%8;
    char& c = s[byte];
    unsigned char mask = 0x1 << (7-b_off);
    c &= ~mask;
    if (bit) c |= mask;
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
    case ERROR::HAMMING:      err_encoded = hamming(message);   break;
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

unsigned bin_digit_count(unsigned number) {
    // Numero de digitos de um numero binario
    return log2(number) + 1;
}

unsigned hamming_pbits_count(unsigned mbit_count) {
    // Numero de bits de paridade necessarios para uma mensagem com m bits
    unsigned pbits = bin_digit_count(mbit_count);
    mbit_count += pbits;
    return bin_digit_count(mbit_count);
}

unsigned hamming_total_bytes(unsigned message_size) {
    //
    unsigned mbits = message_size * 8;
    unsigned pbits = hamming_pbits_count(mbits);
    unsigned tbits = mbits + pbits;
    unsigned bytes = tbits / 8;
    if (tbits % 8) bytes++;
    return bytes;
}

bool hamming_parity(const std::string& s, unsigned pbit) {
    unsigned _1_count = 0;
    unsigned sbit_count = s.size() * 8;

    // Começando do p-1, pega p, pula p.
    for (unsigned i = pbit-1; true; i += pbit) {
        // Anda p indices
        for (unsigned step_end = i+pbit; i < step_end; i++) {
            if (i > sbit_count) return _1_count % 2;

            // calcula a paridade
            if (str_get_bit(s, i))
                _1_count++;
        }
        // Pula p indices
    }
}

std::string ENCODER::hamming(const std::string& message) {
    std::string result(hamming_total_bytes(message.size()),0x0); // Tamanho final, preenchida com zeros

    unsigned mbit_count = message.size() * 8; // bits de mensagem
    unsigned pbit_count = hamming_pbits_count(mbit_count); // bits de paridade

    // Transferir bits de mensagem para seus novos indices
    for (unsigned i = 0; i < mbit_count; i++) {
        bool mbit = str_get_bit(message, i);
        unsigned pos = i + hamming_pbits_count(i+1);
        str_set_bit(result, pos, mbit);
    }

    // Calcular os bits de paridade
    for (unsigned i = 0; i < pbit_count; i++) {
        // Do maior para o menor
        unsigned p = pbit_count - i;
        // Pbits sao potencias de 2
        p = 0x1 << (p-1);
        bool pbit_parity = hamming_parity(result, p);
        str_set_bit(result, p-1, pbit_parity);
    }

    return result;
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
        return "\nERROR DETECTED";

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
    if (crc2 != crc1) return ("\nERROR DETECTED");

    return message;
}


std::string DECODER::detect_hamming(std::string data) {
    unsigned data_bit_count = data.size()*8;

    // Calculo dos indices de pbits
    std::vector<unsigned> indexes;
    for (
        unsigned pow = 0, pos = 0;
        pos < data_bit_count;
        pos = (0x1 << ++pow) - 1 // off by one
        )
        indexes.push_back(pos);

    // 0btem os bits de paridade recebidos
    unsigned inc_pbits = 0;
    for (auto it = indexes.rbegin(); it != indexes.rend(); it++) {
        unsigned pos = *it;
        inc_pbits <<= 1;
        if (str_get_bit(data, pos))
            inc_pbits |= 1;
    }

    // Calcula quantos bytes tinha a mensagem original
    unsigned msg_byte_count = (data_bit_count - indexes.size()) / 8;
    // Reconstroi a mensagem
    std::string message(msg_byte_count, 0x0);
    for (
        unsigned data_pos = 0, message_pos = 0;
        message_pos < msg_byte_count*8;
        data_pos++, message_pos++
        ) {
        // Pula se for bit de paridade
        if (std::find(indexes.begin(), indexes.end(), data_pos) != indexes.end()) {
            message_pos--;
            continue;
        }

        str_set_bit(message, message_pos, str_get_bit(data, data_pos));
    }

    // Verifica os bits de paridade esperados
    std::string control = ENCODER::hamming(message);
    unsigned exp_pbits = 0;
    for (auto it = indexes.rbegin(); it != indexes.rend(); it++) {
        unsigned pos = *it;
        exp_pbits <<= 1;
        if (str_get_bit(control, pos))
            exp_pbits |= 1;
    }

    unsigned err = exp_pbits ^ inc_pbits;
    if (!err) return message;

    // Erro no EDC não altera a mensagem
    if (log2(err) == int(log2(err))) // log2 redondo => potencia de 2
        return message;

    // Corrige a mensagem
    unsigned pbit_count = int(log2(err)) + 1;
    unsigned err_pos = err - pbit_count -1;
    str_set_bit(message, !str_get_bit(message, err_pos), err_pos);

    return message;
}

