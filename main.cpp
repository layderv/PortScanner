#include <iostream>
#include <getopt.h>
#include <cassert>
#include "scanner.h"

void usage(const char*);

int main(int argc, char* argv[]) {
    scanner s;

    int option;
    // s:start, e:end, t:target (ip address, hostname), m:ms timeout
    char const* short_opts = "s:e:t:d:";
    if (argc <= 1)
        usage(argv[0]);
    while ((option = getopt(argc, argv, short_opts)) != -1) {
        switch( option ) {
            case 's':
                assert(s.set_start(std::stoi(optarg)));
                break;
            case 'e':
                assert(s.set_end(std::stoi(optarg)));
                break;
            case 't':
                assert(s.set_target(optarg));
                break;
            case 'd':
                assert(s.set_delay(std::stoi(optarg)));
                break;
            default:
                usage(argv[0]);
        }
    }

    std::vector<uint16_t> ports;
    switch (s.scan(ports)) {
        case ERROR::range:
            std::cerr << "bad range" << std::endl;
            break;
        case ERROR::target:
            std::cerr << "bad target" << std::endl;
            break;
        case ERROR::success:
        default:
            if (!ports.empty()) {
                for (auto port : ports) {
                    std::cout << port << "\tis open" << std::endl;
                }
            } else {
                std::cout << "No open port found" << std::endl;
            }

            break;
    }

    return 0;
}

void usage(char const* prog) {
    std::cout << prog << " -[sed]t" << std::endl;
    std::cout << R"(
-s: set starting port (1); min 1
-e: set ending port (2^16-1); max 2^16-1
-t: target hostname or ip address
-d: delay in ms to connect (0); [0,10^6)
scans t:[s, e]
)" << std::endl;
    exit(EXIT_FAILURE);
}