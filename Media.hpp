#pragma once

#include <memory>

// Medium is a singleton class responsible for inter-process comunication and represents guided or non-guided mediums of telecomunication.
// After the program is forked, there will be one singleton instance per process.
// Each process must either read or write to the medium

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
