#include "CamadaEnlace.hpp"
#include "R_Demodulator.hpp"

std::string ENCODER::encode_msg(const std::string& message, FRAMING framing, ERROR error_mode) {
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

std::string ENCODER::CRC(const std::string& message) {
    return message;
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
