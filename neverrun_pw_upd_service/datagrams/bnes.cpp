#include "bnes.hpp"

#include <QByteArray>
#include <QtEndian>

namespace BNES {

Response parse(const char *data, int size)
{
#define cpy(dest) \
    memcpy(&dest, data + pos, sizeof(dest)); \
    pos += sizeof(dest)

    Response res;
    ptrdiff_t pos = 5;

    cpy(res.port);
    pos += 1;
    uint8_t len = 0;
    cpy(len);
    if(len > 0) {
        res.server_name.resize(len, 0);
        memcpy(&res.server_name[0], data + pos, len);
    }
    return res;

#undef cpy
}

QByteArray build(uint16_t port) {
    QByteArray datagram;
    datagram.append("BNES", 4);

    char portbe[2];
    qToBigEndian(port, (uchar*)&portbe);
    datagram.append(portbe, 2);

    return datagram;
}

} // namespace BNES


