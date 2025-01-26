#include "Media.hpp"
#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>

std::unique_ptr<Medium> Medium::instance(new Medium());

// Get method for singleton instance and defines wether the medium either reads or writes in this process.
Medium* Medium::Instance(Medium::Permission p) {
	if (instance->perm == NOT_SET) 
		instance->Narrow(p);

	else if (p != instance->perm)
		throw std::runtime_error("Attempted to access medium from the wrong READ/WRITE end.");

	return instance.get();
}

Medium::Medium() :
	perm(NOT_SET),
	value(0)
{
	int pipe_err = pipe2(pipe_fd, O_NONBLOCK);
	if (pipe_err)
		throw std::runtime_error("pipe error");
}

Medium::~Medium() {
	switch (perm) {
		case READ:
			close(pipe_fd[READ]);
			break;
		case WRITE:
			close(pipe_fd[WRITE]);
			break;
		case NOT_SET:
			close(pipe_fd[READ]);
			close(pipe_fd[WRITE]);
			break;
	}
}

// Narrows instance permissions to read or write only and closes unused pipe endpoint. 
void Medium::Narrow(Medium::Permission p) {
	perm = p;

	switch (perm) {
		case READ:
			close(pipe_fd[WRITE]);
			break;
		case WRITE:
			close(pipe_fd[READ]);
			break;
		case NOT_SET:
			throw std::invalid_argument("Medium cannot narrow to NOT_SET.");
			break;
	};
}

void Medium::transmit(float f) {
	if (perm != WRITE)
		throw std::invalid_argument("This endpoint does not have writing permissions.");

	value = f;
	auto w_err = write(pipe_fd[WRITE], &value, sizeof(value));
	if (w_err == -1)
		throw std::runtime_error("Error writing to medium.");
}

float Medium::listen() {
	if (perm != READ)
		throw std::invalid_argument("This endpoint does not have reading permissions.");

	while (
		read(pipe_fd[READ], &value, sizeof(value)) != -1
	) {}
	if (errno != EAGAIN) throw std::runtime_error("Error reading medium");
	return value;
}
