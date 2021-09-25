#include "bnxi.hpp"

#include <QByteArray>
#include <QtEndian>

namespace BNXI {

Response parse(const char *data, int size) {
#define cpy(dest) \
    memcpy(&dest, data + pos, sizeof(dest)); \
    pos += sizeof(dest)

    Response res;
    ptrdiff_t pos = 4;

    cpy(res.port); // 4
    pos += 1;
    cpy(res.password); // 6
    cpy(res.min_level); // 7
    cpy(res.max_level); // 8
    cpy(res.players); // 9
    cpy(res.max_players); // 10
    cpy(res.local_vault); // 11
    cpy(res.pvp); // 12
    pos += 1;
    cpy(res.one_party); // 14
    cpy(res.elc); // 15
    cpy(res.ilr); // 16
    pos += 1;
    // uint8_t  unknown3; // 17
    // uint8_t  mod_name_size; // 18
    uint8_t len = 0;
    cpy(len);
    if(len > 0) {
        res.module_name.resize(len, 0);
        memcpy(&res.module_name[0], data + pos, len);
    }
    return res;

#undef cpy
}

QByteArray build(uint16_t port) {
    QByteArray datagram;
    datagram.append("BNXI");

    char portbe[2];
    qToBigEndian(port, (uchar*)&portbe);
    datagram.append(portbe, 2);

    return datagram;
}

} // namespace BNXI
