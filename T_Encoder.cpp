#include "T_Encoder.hpp"
#include "T_Modulator.hpp"

#include <memory>
#include <string>

using Transmitter::Encoder;
using std::string;

Encoder* Encoder::Instance() {
	static std::unique_ptr<Encoder> instance(new Encoder);
	return instance.get();
}

Encoder::Encoder() {
}

void Encoder::send(const string& message) {
	if (message.size() > calc_useful()) {
		auto split = message.begin() + calc_useful();
		string first(message.begin(),split);
		string rest(split, message.end());
		send(first);
		send(rest);
		return;
	}

	string frame = frame_msg(message);

	Modulator::Instance()->out() << frame;
}

string Encoder::frame_msg(const string& message) {
	return count_bytes(message);
}

unsigned char Encoder::calc_useful() {
	return frame_max-2;
}

string Encoder::count_bytes(const string& message) {
	unsigned char count = message.size();
	string head = string{static_cast<char>(count)};
	head.insert(head.end(), message.begin(), message.end());
	return head;
}
