#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QtNetwork>

#include "../nodesocket.h"

class UDPClient : public NodeSocket
{
    Q_OBJECT

public:
    explicit UDPClient(bool useLocalHost, QObject *parent = 0);
    virtual ~UDPClient();

signals:
    void connected(bool status);

public slots:
    void startSocket();
    void sendUserCommand(QString command);

private:
    void handleDataReply(const QByteArray datagram);
};

#endif // UDPCLIENT_H
