#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QtNetwork>

#include "nodesocket.h"

class UDPClient : public NodeSocket
{
    Q_OBJECT

public:
    explicit UDPClient(bool useLocalHost, QObject *parent = 0);
    virtual ~UDPClient();

signals:
    void dataReplyReceived(QString);
    void controllerAction(QString);
    void connected(bool status);

public slots:
    void startSocket();
    void stopSocket();
    void sendUserCommand(QString command);

protected slots:
    void startClient();
    void sendAliveMsg();
    void stopAliveMsg();

private:
    void handleDataReply(const QByteArray datagram);
    void sendSYN();
    void sendACK();
    void startAliveMsg();
    void handleControllerAction(const QString datagram);
    void handleDataReply(const QString datagram);

    void processDatagram(const QByteArray &datagram);

    // Alive Message Send Frequency: client constant
    static const int AMSF = 750;
};

#endif // UDPCLIENT_H
