#include "udpclient.h"

#include "nodesocket.h"

UDPClient::UDPClient(bool useLocalHost, QObject *parent) :
    NodeSocket(useLocalHost, parent)
{
    qDebug() << "UDPClient::UDPClient called";
}

UDPClient::~UDPClient()
{
}

void UDPClient::startSocket()
{
    udpSocket = new QUdpSocket(this);

    // Bind to IP and port
    int port = 50001;
    ownNode->setPort(port);
    udpSocket->bind(QHostAddress(ownNode->address()), ownNode->port());
    qDebug() << "Trying to bind this address: " << ownNode->address() << ":" << ownNode->port();

    // Bind other Node port to 50000
    port = 50000;
    otherNode->setPort(port);

    // Make connection to read incomming messages
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    qDebug() << "Socket connected!";

    // Send initial message
    sendSYN();

    qDebug() << "UDPClient binded, trying to start";

    //startClient();
    setConnected(true);
}

void UDPClient::stopSocket()
{
    qDebug() << "UDPClient disconnected";
    udpSocket->close();
    disconnect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    //udpSocket->deleteLater();
    stopAliveMsg();
    setConnected(false);
}

void UDPClient::sendUserCommand(QString command)
{
    QByteArray datagram = "11;" + command.toLatin1() + ";";
    sendDatagram(datagram);
    startTimeOutTimer();

    qDebug() << "User command " << command << " sent to server";
}

void UDPClient::handleDataReply(const QByteArray datagram)
{
    qDebug() << "Entered UDPClient::handleDataReply (child)";
    stopTimeOutTimer();

    QString datastring(datagram);
    emit dataReplyReceived(datastring.section(";",1,1));
}

/*
 *
 * PROTECTED SLOTS
 *
 */
void UDPClient::startClient()
{
    // Bind to IP and port
    int port = 50001;
    ownNode->setPort(port);
    udpSocket->bind(QHostAddress(ownNode->address()), ownNode->port());
    qDebug() << "Trying to bind this address: " << ownNode->address() << ":" << ownNode->port();

    // Bind other Node port to 50000
    port = 50000;
    otherNode->setPort(port);

    // Make connection to read incomming messages
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    qDebug() << "Socket connected!";

    // Send initial message
    sendSYN();
}

/*
 *
 * PROTECTED
 *
 */
void UDPClient::sendSYN()
{
    // Used when connection is initialised by Client
    QString address = ownNode->address();
    quint16 port = ownNode->port();
    QByteArray datagram = "1;" + address.toLatin1() + ";" +  QByteArray::number(port);

    startTimeOutTimer();
    sendDatagram(datagram);

    qDebug() << "SYN Message sent to " << otherNode->address() << ":" << otherNode->port();
}

void UDPClient::sendACK()
{
    // Used when connection is initialised by Client
    QString address = udpSocket->localAddress().toString();
    quint16 port = udpSocket->localPort();
    QByteArray datagram = "3;" + address.toLatin1() + ";" +  QByteArray::number(port);

    startTimeOutTimer();
    sendDatagram(datagram);

    qDebug() << "ACK Message sent";
}

void UDPClient::startAliveMsg()
{
    connect(aliveTimer, SIGNAL(timeout()), this, SLOT(sendAliveMsg()));
    aliveTimer->start(AMSF);

    qDebug() << "Alive message started, sending every " << AMSF << " miliseconds";
}

void UDPClient::sendAliveMsg()
{
    QString address = udpSocket->localAddress().toString();
    quint16 port = udpSocket->localPort();
    QByteArray datagram = "5;" + address.toLatin1() + ";" +  QByteArray::number(port);
    //QByteArray datagram = "5:127.0.0.1:55000";
    sendDatagram(datagram);
}

void UDPClient::stopAliveMsg()
{
    disconnect(aliveTimer, SIGNAL(timeout()), this, SLOT(sendAliveMsg()));
    aliveTimer->stop();

    qDebug() << "Alive message stopped";
}

void UDPClient::handleDataReply(const QString datagram)
{
    qDebug() << "Entered UDPClient::handleDataReply (parent)";

    QString datastring(datagram);
    emit dataReplyReceived(datastring.section(";",1));
}

void UDPClient::handleControllerAction(const QString datagram)
{
    emit controllerAction(datagram.section(';', 1, 1));
}

// All possible cases for incoming client datagrams
void UDPClient::processDatagram(const QByteArray &datagram)
{
    QString datastring(datagram);
    switch (datastring.section(';',0,0).toInt())
    {
    case 2:
        // Client case: reply to SYNACK msg
        stopTimeOutTimer();
        sendACK();

        qDebug() << "processed Datagram case 2";
        break;
    case 4:
        //Client case: sent every x msecs to confirm connection
        setConnected(true);
        emit connectionEstablished();

        stopTimeOutTimer();
        startAliveMsg();

        qDebug() << "processed Datagram case 4";
        break;
    case 10:
        // Client case: receiving reply to the sent request
        stopTimeOutTimer();
        handleDataReply(datagram);

        qDebug() << "processed Datagram case 10";
        break;
    case 20:
        // Client case: receiving controller action
        handleControllerAction(datagram);
        break;
    default:
        // Just break on everything else
        break;
    }
}
