#ifndef NEVERRUNPWUDPSERICE_H
#define NEVERRUNPWUDPSERICE_H

#include "nwdatagram.hpp"

#include <QHostAddress>
#include <QObject>
#include <QSqlDatabase>

#include <cstdint>

class QUdpSocket;
class QTimer;

class NeverrunPWUdpSerice : public QObject {
    Q_OBJECT

    static const QString hostname;
    static const QString dbname;
    static const QString username;
    static const QString password;

    uint16_t port_;
    QUdpSocket *socket_;
    QSqlDatabase db_;
    NWDatagram dg_;
    QTimer *desc_timer_;
    QTimer *player_timer_;
    QTimer *sweep_timer_;

public:
    NeverrunPWUdpSerice(uint16_t port);

    bool handle_datagram(const QHostAddress& sender, quint16 port, const char *datagram, int size);
    void handle_datagrams();
    void request_player_counts();
    void request_descriptions();

    void db_insert_players(int server_id, const QDateTime& now, int players);
    void db_sweep_offline();
    void db_update_description(const QString& address, uint16_t port, const QDateTime& now, const BNDS::Response& data);
    int db_update_players(const QString& address, uint16_t port, const QDateTime& now, int players);
};

#endif // NEVERRUNPWUDPSERICE_H
