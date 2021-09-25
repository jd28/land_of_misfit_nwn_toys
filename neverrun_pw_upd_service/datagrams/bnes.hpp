#ifndef NWBNES_H
#define NWBNES_H

class QByteArray;

#include <cstdint>
#include <string>

namespace BNES {

struct Response {
    //std::string datagram; // BNERU
    uint16_t    port;
    //uint8_t     unknown;
    //uint8_t     server_name_length;
    std::string server_name;
};

static const std::string response_header{"BNERU"};

QByteArray build(uint16_t port);
Response parse(const char* data, int size);

}

#endif // NWBNES_H
