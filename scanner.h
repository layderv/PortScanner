#ifndef PORTSCANNER_SCANNER_H
#define PORTSCANNER_SCANNER_H


#include <cstdint>
#include <string>
#include <cmath>
#include <sys/socket.h>
#include <netdb.h>
#include <vector>
#include "errors.h"

class scanner {
public:
    scanner();

    bool set_target(char const*);
    bool set_start(uint16_t);
    bool set_end(uint16_t);
    bool set_delay(int);

    ERROR scan(std::vector<uint16_t>&);

private:
    static const uint16_t MIN_START = 1;
    static const uint16_t MAX_END = (std::pow(2, 16) - 1);

    uint16_t start;
    uint16_t end;
    struct sockaddr_in sa;
    bool target_init;
    int delay;
};


#endif //PORTSCANNER_SCANNER_H
