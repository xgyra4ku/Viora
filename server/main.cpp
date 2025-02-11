#include "inc/server.h"

int main(const int argc, char* argv[]) {
    Server server(argc, argv);
    server.run();
    return 0;
}
