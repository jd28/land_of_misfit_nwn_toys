#ifndef NWDATAGRAM_HPP
#define NWDATAGRAM_HPP

#include "datagrams/bnds.hpp"
#include "datagrams/bnes.hpp"
#include "datagrams/bnxi.hpp"

#include <cstdint>

class NWDatagram {
    uint16_t port_;

    enum class Datagrams {
        BNDS,
        BNES,
        BNXI
    };

public:
    NWDatagram(uint16_t port)
        : port_{port}
    {}

    bool handle_datagram(const char* datagram, int size);
};

#endif // NWDATAGRAM_HPP
