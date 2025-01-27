#include <unistd.h>
#include <stdexcept>


int Tmain(int argc, char *argv[]);
int Rmain(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    int pid = fork();

    switch (pid) {
    case -1:
        throw std::runtime_error("fork error");
        return -1;
    case 0:
        // Child process
        Rmain(argc, argv);
        break;
    default:
        // Parent process
        Tmain(argc, argv);
        break;
    }

    return 0;
}
