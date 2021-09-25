#ifndef NWBNDS_HPP
#define NWBNDS_HPP

class QByteArray;

#include <cstdint>
#include <string>

namespace BNDS {

struct Response {
    //char datagram[4]; // BNDR
    uint16_t port;
    //uint32_t :server_desc_length
    std::string server_description;
    //uint32 :mod_desc_length
    std::string module_description;
    //uint32 :build_number_len
    std::string build_number;
    uint16_t game_type;
};

static const std::string response_header{"BNDR"};

QByteArray build(uint16_t port);
Response parse(const char* data, int size);

} // namespace BNDS

#endif // NWBNDS_HPP
