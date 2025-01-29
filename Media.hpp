#pragma once

#include <memory>

class Medium {
public:
	enum Permission {
		NOT_SET = -1,
		READ = 0, 
		WRITE = 1, 
	};

	static Medium* Instance(Permission);
	~Medium();

	void transmit(float);
	float listen();

private:
	Medium();
	Medium(const Medium&) = default;
	Medium(Medium&&) = default;

	void Narrow(Permission);

	static std::unique_ptr<Medium> instance;
	Permission perm;
	int pipe_fd[2];
	float value;
};
