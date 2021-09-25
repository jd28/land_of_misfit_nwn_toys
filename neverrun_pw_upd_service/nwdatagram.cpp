#include "nwdatagram.hpp"

#include <QDebug>

bool NWDatagram::handle_datagram(const char *datagram, int size) {
    if (size < 1) { return false; }

    switch (datagram[0]) {
    case 'B':
        if (size < 2) { return false; }
        switch(datagram[1]) {
        case 'N':
            if (size < 3) { return false; }
            switch(datagram[2]) {
            case 'D': {
                auto response = BNDS::parse(datagram, size);
                qDebug() << QString::fromStdString(response.server_description);
                return true;
            }
            case 'X': {
                auto response = BNXI::parse(datagram, size);
                qDebug() << response.players;
                return true;
            }
            case 'E': {
                auto response = BNES::parse(datagram, size);
                qDebug() << QString::fromStdString(response.server_name);
                return true;
            }
            default:
                return false;
            }
        default:
            return false;
        }
        break;
    default:
        return false;
    }

    return false;
}
