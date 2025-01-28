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
};

}
