#ifndef NETWORKSOCKET_H
#define NETWORKSOCKET_H

#include "networkdata.h"
#include <QtNetwork/QTcpSocket>

class NetworkSocket : public QObject {
    Q_OBJECT
public:
    explicit NetworkSocket(QTcpSocket* socket, QObject* parent = nullptr);
    void send(NetworkData);
    void hello(const QString& host, quint16 port);
    QTcpSocket* base() const;

private:
    QTcpSocket* socket;

signals:
    void receive(NetworkData);
    void parseError(const InvalidMessage&);

public slots:
    void bye();

private slots:
    void receiveData();
};

#endif // NETWORKSOCKET_H
