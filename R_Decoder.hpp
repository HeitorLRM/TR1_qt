#pragma once

#include <string>

namespace Receiver {

class Decoder {
private:
	Decoder();

public:
	static Decoder* Instance();
	std::string listen_frame();

};

}
