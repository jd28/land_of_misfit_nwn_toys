#include "bnds.hpp"

#include <QByteArray>
#include <QtEndian>

namespace BNDS {

Response parse(const char *data, int size)
{
#define cpy(dest) \
    memcpy(&dest, data + pos, sizeof(dest)); \
    pos += sizeof(dest)

    Response res;
    ptrdiff_t pos = 4;

    cpy(res.port);
    uint32_t len = 0;
    cpy(len);
    if(len > 0) {
        res.server_description.resize(len, 0);
        memcpy(&res.server_description[0], data + pos, len);
    }
    pos += len;

    cpy(len);
    if(len > 0) {
        res.module_description.resize(len, 0);
        memcpy(&res.module_description[0], data + pos, len);
    }
    pos += len;

    cpy(len);
    if(len > 0) {
        res.build_number.resize(len, 0);
        memcpy(&res.build_number[0], data + pos, len);
    }
    pos += len;

    cpy(res.game_type);

    return res;

#undef cpy
}

QByteArray build(uint16_t port) {
    QByteArray datagram;
    datagram.append("BNDS", 4);

    char portbe[2];
    qToBigEndian(port, (uchar*)&portbe);
    datagram.append(portbe, 2);

    return datagram;
}

} // namespace BNDS

