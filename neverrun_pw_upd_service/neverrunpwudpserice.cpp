#include "neverrunpwudpserice.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTimer>
#include <QtNetwork>

const QString NeverrunPWUdpSerice::hostname{"localhost"};

#ifdef NEVERRUN_PW_PRODUCTION
const QString NeverrunPWUdpSerice::db_name{"neverrun_pw_production"};

#elif NEVERRUN_PW_DEVELOPMENT
const QString NeverrunPWUdpSerice::dbname{"neverrun_pw_development"};
const QString NeverrunPWUdpSerice::username{"jmd"};
const QString NeverrunPWUdpSerice::password{"password1"};
#endif

NeverrunPWUdpSerice::NeverrunPWUdpSerice(uint16_t port)
    : port_{port}
    , socket_{new QUdpSocket(this)}
    , db_{QSqlDatabase::addDatabase("QPSQL")}
    , dg_{port}
    , desc_timer_{new QTimer(this)}
    , player_timer_{new QTimer(this)}
    , sweep_timer_{new QTimer(this)}
{
    socket_->bind(port_);
    connect(socket_, &QUdpSocket::readyRead,
            this, &NeverrunPWUdpSerice::handle_datagrams);

    db_.setHostName(hostname);
    db_.setDatabaseName(dbname);
    db_.setUserName(username);
    db_.setPassword("xxxxx");

    if(!db_.open()) {
        throw std::runtime_error("Unable to connect to database");
    }

    connect(desc_timer_, &QTimer::timeout,
            this, &NeverrunPWUdpSerice::request_descriptions);
    desc_timer_->start(10 * 60 * 1000);

    connect(player_timer_, &QTimer::timeout,
            this, &NeverrunPWUdpSerice::request_player_counts);
    player_timer_->start(60*1000);

    connect(sweep_timer_, &QTimer::timeout,
            this, &NeverrunPWUdpSerice::db_sweep_offline);
    sweep_timer_->start(2 * 60 * 1000);
}

bool NeverrunPWUdpSerice::handle_datagram(const QHostAddress& sender, quint16 port,
                                          const char *datagram, int size) {
    if (size < 1) { return false; }

    QDateTime now = QDateTime::currentDateTimeUtc();
    QString addy{QHostAddress(sender.toIPv4Address()).toString()};

    switch (datagram[0]) {
    case 'B':
        if (size < 2) { return false; }
        switch(datagram[1]) {
        case 'N':
            if (size < 3) { return false; }
            switch(datagram[2]) {
            case 'D': {
                auto response = BNDS::parse(datagram, size);
                qDebug() << "Response from: " << QHostAddress(sender.toIPv4Address()).toString() << ":" << port;
                db_update_description(addy, port, now, response);
                return true;
            }
            case 'X': {
                auto response = BNXI::parse(datagram, size);
                qDebug() << "Response from: " << addy << ":" << port << response.players;
                int server_id = db_update_players(addy, port, now, response.players);
                db_insert_players(server_id, now, response.players);
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

void NeverrunPWUdpSerice::request_player_counts() {
    QSqlQuery query{"select address, port from servers", db_};
    while(query.next()) {
        QString addy = query.value(0).toString();
        int port = query.value(1).toInt();
        qDebug() << addy << " " << port;
        socket_->writeDatagram(BNXI::build(port_), QHostAddress(addy), port);
    }
}

void NeverrunPWUdpSerice::request_descriptions() {
    QSqlQuery query{"select address, port from servers", db_};
    while(query.next()) {
        QString addy = query.value(0).toString();
        int port = query.value(1).toInt();
        qDebug() << addy << " " << port;
        socket_->writeDatagram(BNDS::build(port_), QHostAddress(addy), port);
    }
}

void NeverrunPWUdpSerice::db_insert_players(int server_id, const QDateTime &now, int players) {
    static const QString INSERT_PLAYERS{
        "INSERT INTO player_counts (count, timestamp, server_id)\n"
        "VALUES(?, ?, ?)"
    };

    if (server_id != -1) {
        QSqlQuery q{db_};
        q.prepare(INSERT_PLAYERS);
        q.bindValue(0, players);
        q.bindValue(1, now);
        q.bindValue(2, server_id);

        db_.transaction();
        if (!q.exec()) { qDebug() << "player_counts:insert" << q.lastError(); }
        db_.commit();
    }

}

void NeverrunPWUdpSerice::db_sweep_offline()
{
    QString Q = { "UPDATE servers\n"
                  "set online = false, players = 0\n"
                  "where (NOW() - updated_at) > interval '5 minutes'" };
    QSqlQuery q{db_};
    q.prepare(Q);
    db_.transaction();
    if (!q.exec()) { qDebug() << "db_sweep_offline" << q.lastError(); }
    db_.commit();
}

void NeverrunPWUdpSerice::db_update_description(const QString& address, uint16_t port,
                                                const QDateTime &now, const BNDS::Response &data) {
    static const QString UPDATE_DESC{
        "UPDATE servers\n"
        "SET server_description = ?, module_description = ?, updated_at = ?"
        "WHERE address = ? AND port = ?"
    };

    QSqlQuery q{db_};
    db_.transaction();
    q.prepare(UPDATE_DESC);
    q.bindValue(0, QString::fromStdString(data.server_description));
    q.bindValue(1, QString::fromStdString(data.module_description));
    q.bindValue(2, now);
    q.bindValue(3, address);
    q.bindValue(4, port);
    if (!q.exec()) { qDebug() << "servers:descriptions" << q.lastError(); }
    db_.commit();
}

int NeverrunPWUdpSerice::db_update_players(const QString& address, uint16_t port, const QDateTime &now, int players) {
    static const QString UPDATE_PLAYERS{
        "UPDATE servers\n"
        "SET players = ?, updated_at = ?, online = true\n"
        "WHERE address = ? AND port = ?\n"
        "RETURNING id"
    };

    QSqlQuery q;
    q.prepare(UPDATE_PLAYERS);
    q.bindValue(0, players);
    q.bindValue(1, now);
    q.bindValue(2, address);
    q.bindValue(3, port);

    db_.transaction();
    if (!q.exec()) { qDebug() << "servers:players" << q.lastError(); }
    int server_id = -1;
    while(q.next()) {
        server_id = q.value(0).toInt();
    }
    db_.commit();
    return server_id;
}

void NeverrunPWUdpSerice::handle_datagrams() {
    while (socket_->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket_->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;

        socket_->readDatagram(datagram.data(), datagram.size(),
                              &sender, &senderPort);

        if(!handle_datagram(sender, senderPort, datagram.data(), datagram.size())) {
            throw std::runtime_error("Unknown datagrams");
        }
    }
}
