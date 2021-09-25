#ifndef NWBNXI_HPP
#define NWBNXI_HPP

class QByteArray;

#include <cstdint>
#include <string>

namespace BNXI {

struct Response {
    // std::string datagram; // BNXR
    uint16_t port; // 4
    // uint8_t  unknown;
    uint8_t  password; // 6
    uint8_t  min_level; // 7
    uint8_t  max_level; // 8
    uint8_t  players; // 9
    uint8_t  max_players; // 10
    uint8_t  local_vault; // 11
    uint8_t  pvp; // 12
    // uint8_t  unknown2; // 13
    uint8_t  one_party; // 14
    uint8_t  elc; // 15
    uint8_t  ilr; // 16
    // uint8_t  unknown3; // 17
    // uint8_t  mod_name_size; // 18
    std::string module_name; // 19
};

static const std::string response_header{"BNXR"};

QByteArray build(uint16_t port);
Response parse(const char* data, int size);

}

#endif // NWBNXI_HPP
