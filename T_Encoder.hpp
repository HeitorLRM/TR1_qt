#pragma once

#include <string>
namespace Transmitter {

class Encoder {
private:
	Encoder();

public:
	static Encoder* Instance();

	void send(const std::string&);

private:
	std::string frame_msg(const std::string&);

	std::string count_bytes(const std::string&);
	unsigned char calc_useful();

	const unsigned char frame_max = 10;

};

}