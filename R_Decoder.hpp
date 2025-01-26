#pragma once

#include <string>

namespace Receiver {

class Decoder {
private:
	Decoder();

public:
	static Decoder* Instance();
	std::string listen_frame();

private:
	std::string listen_count_bytes();
};

}