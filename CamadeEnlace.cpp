#include "CamadaEnlace.hpp"
#include "R_Demodulator.hpp"

std::string ENCODER::encode_msg(const std::string& message, FRAMING framing, ERROR error_mode) {
    std::string err_encoded;
    switch (error_mode) {
    case ERROR::NONE:         err_encoded = message;            break;
    case ERROR::PARITY_BIT:   err_encoded = parity(message);    break;
    case ERROR::CRC:          err_encoded = CRC(message);       break;
    case ERROR::HAMMING:      err_encoded = Hamming(message);   break;
    default:break;
    }

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

    unsigned max_bytes = frame_max_bytes - 1;
    if (message.size() > max_bytes) {
        // Split in two
        auto split = message.begin() + max_bytes;
        std::string first(message.begin(),split);
        std::string rest(split, message.end());

        result += count_bytes(first);
        result += count_bytes(rest);
        return result;
    }

    unsigned char count = message.size();
    result = std::string{char(count)} + message;
    return result;
}

std::string ENCODER::insert_bytes(const std::string & message) {
    return frame_flags(fix_false_flags(message));
}

std::string ENCODER::fix_false_flags(std::string message) {
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
        if (i >= frame_max_bytes - 2) {
            result += flag;
            std::string remainder(data.begin() + i, data.end());
            return result + frame_flags(remainder);
        }

        if (data[i] == esc) {
            if (i < frame_max_bytes - 3) {
                result += data[i++];
                result += data[i];
                continue;
            }
            else {
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

std::string ENCODER::parity(const std::string& message) {
    return message;
}

std::string ENCODER::CRC(const std::string& message) {
    return message;
}

std::string ENCODER::Hamming(const std::string& message) {
    return message;
}
