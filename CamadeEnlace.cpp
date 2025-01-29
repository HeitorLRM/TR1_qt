#include "CamadaEnlace.hpp"
#include "R_Demodulator.hpp"

std::string ENCODER::encode_msg(const std::string& message, FRAMING framing, ERROR error_mode) {
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
    std::string result = "";

    unsigned max_bytes = frame_max_bytes - 1; // Consideramos o byte de contagem
    if (message.size() > max_bytes) {
        // Fazemos enquadramento dos que couberem e passamos o resto para outro quadro
        auto split = message.begin() + max_bytes;
        std::string first(message.begin(),split);
        std::string rest(split, message.end());

        result += count_bytes(first);
        result += count_bytes(rest);
        return result;
    }

    unsigned char count = message.size(); // Byte de contagem
    result = std::string{char(count)} + message;
    return result;
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

    for (unsigned i = 0; i < data.size(); i++) {
        if (i >= frame_max_bytes - 2) { // Dividimos a mensagem em mais quadros quando não couber
            result += flag;
            std::string remainder(data.begin() + i, data.end());
            return result + frame_flags(remainder);
        }

        if (data[i] == esc) {
            if (i < frame_max_bytes - 3) { // A flag de escape não pode ser separada do que cancela.
                result += data[i++];
                result += data[i];
                continue;
            }
            else { // Caso caibam no mesmo quadro
                result += flag;
                std::string remainder(data.begin() + i, data.end());
                return result + frame_flags(remainder);
            }
        }

        result += data[i];
    }

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
        return "QUADRO COM ERRO";

    return std::string(frame.begin(), frame.end() -1);
}
