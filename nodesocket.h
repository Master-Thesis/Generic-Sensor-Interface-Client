#ifndef NODESOCKET_H
#define NODESOCKET_H

#include <QObject>
#include <QtNetwork>

#include "node.h"

class NodeSocket : public QObject
{
    Q_OBJECT

public:
    NodeSocket(bool useLocalhost, QObject *parent = 0);
    virtual ~NodeSocket();

    QString address() const;
    int port() const;
    QString otherNodeAddress() const;
    int otherNodePort() const;

    bool isConnected() const;

signals:
    // Both
    void started();
    void dataSent(QByteArray);
    void destAddressInvalid();
    void connectionEstablished();
    void connectionLost();
    void connectionChanged(bool);
    // Server Specific
    void dataRequestReceived(QString);
    // Client Specific
    void dataReplyReceived(QString);
    void controllerAction(QString);

public slots:
    // Both
    virtual void startSocket() = 0;
    void setAddress(const QString ip);
    void setPort(const int portNo);
    void setOtherNodeAddress(const QString ip);
    void setOtherNodePort(const int portNo);
    // Server specific
    // Client specific

protected slots:
    // Both
    void processPendingDatagrams();
    void nodeLost();
    void noResponse();
    // Server Specific
    void startServer();
    // Client Specific
    void startClient();
    void sendAliveMsg();

protected:
    // Both
    void setConnected(const bool connectionBool);
    void processDatagram(const QByteArray &datagram);
    void sendDatagram(const QByteArray &message);
    void startTimeOutTimer();
    void stopTimeOutTimer();
    QString findIpAddress() const;
    // Server Specific
    void sendSYNACK();
    void sendAME();
    void startAliveTimer();
    void resetAliveTimer();
    void stopAliveTimer();
    virtual void handleDataRequest(const QString datagram);
    // Client Specific
    void sendSYN();
    void sendACK();
    void startAliveMsg();
    void handleControllerAction(const QString datagram);
    virtual void handleDataReply(const QString datagram);

    // Used structures
    QUdpSocket *udpSocket;
    QTimer *aliveTimer;
    Node *ownNode;
    Node *otherNode;

private:
    bool connected;
    QTimer *timeOutTimer;

    // Alive Message Time Out: server constant
    static const int AMTO = 2000;
    // Alive Message Send Frequency: client constant
    static const int AMSF = 750;

    // Time Out Timer Value: used to check for response
    static const int TOTV = 2000;

};

#endif // NODESOCKET_H
