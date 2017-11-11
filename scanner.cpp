#include <cstring>
#include <arpa/inet.h>
#include <cassert>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include "scanner.h"

scanner::scanner() : start(MIN_START), end(MAX_END), delay(0), target_init(false) {
    memset(&this->sa, 0, sizeof(this->sa));
    this->sa.sin_family = AF_INET;
}

bool scanner::set_end(uint16_t p) {
    if (p <= this->MAX_END) {
        this->end = p;
        return true;
    }
    return false;
}

bool scanner::set_start(uint16_t p) {
    if (p >= this->MIN_START) {
        this->start = p;
        return true;
    }
    return false;
}

bool scanner::set_delay(int d) {
    if (d < std::pow(10, 6) && d >= 0) {
        this->delay = d;
        return true;
    }
    return false;
}

bool scanner::set_target(char const * target) {
    struct hostent* host;

    this->target_init = false;
    if (inet_pton(AF_INET, target, &this->sa.sin_addr) == 1) {

    } else {
        if ((host = gethostbyname(target)) == 0) {
            return false;
        }
        strncpy((char*) &this->sa.sin_addr, host->h_addr, sizeof(this->sa.sin_addr));
    }
    return this->target_init = true;
}

ERROR scanner::scan(std::vector<uint16_t>& open_ports) {
    int socket, so_error;
    socklen_t so_error_len;
    fd_set fdset;
    struct timeval tv;
    bool stop;

    if (this->start > this->end) {
        return ERROR::range;
    }
    else if (!this->target_init) {
        return ERROR::target;
    }
    else {
        tv.tv_usec = 1;
        tv.tv_sec = 0;

        so_error_len = sizeof(so_error);

        stop = false;

        for (auto port = this->start; !stop; ++port) {
            this->sa.sin_port = htons(port);
            socket = ::socket(AF_INET, SOCK_STREAM, 0);
            assert(socket >= 0);
            assert(fcntl(socket, F_SETFL, O_NONBLOCK) >= 0);

            connect(socket, (struct sockaddr *) &this->sa, sizeof(this->sa));

            FD_ZERO(&fdset);
            FD_SET(socket, &fdset);

            if (select(socket + 1, nullptr, &fdset, nullptr, &tv) == 1) {
                getsockopt(socket, SOL_SOCKET, SO_ERROR, &so_error, &so_error_len);
                if (so_error == 0)
                    open_ports.push_back(port);
            }

            close(socket);
            stop = port == this->end;
        }
    }
    return ERROR::success;
}