/****************************************************************************
** * Master's Thesis
** * Generic Sensor Interface
** * Ken Foncé & Joris Renckens
** * Promotor: Stef Desmet
** * June 2011, Groep T - Internationale Hogeschool Leuven, Leuven, Belgium
**
** Purpose of this class is to implement the basic functionality needed to
** make a server or client for the "Generic Sensor Interface" project.
**
** This class makes sure server and client communicate in the appropriate
** way. Secondly there are some virtual functions that should be overloaded
** by the server or client class implemented in the real application.
**
** Target application used as client has to inherrit from this class.
** See header file for virtual methods that should be overloaded.
**
****************************************************************************/

#include "nodesocket.h"

/*
 *
 * PUBLIC
 *
 */
// TODO: make server/client independant
NodeSocket::NodeSocket(bool useLocalhost, QObject *parent) :
    QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    timeOutTimer = new QTimer(this);
    aliveTimer = new QTimer(this);

    ownNode = new Node();
    otherNode = new Node();

    connected = false;

    // Set IP-Address
    QString ownIP;
    useLocalhost ? ownIP = "127.0.0.1" : ownIP = findIpAddress();
    ownNode->setAddress(ownIP);

    // Timer Connections
    connect(timeOutTimer, SIGNAL(timeout()), this, SLOT(noResponse()));

    qDebug() << "NodeSocket::NodeSocket (with bool) called";
    qDebug() << "NodeSocket (Server/Client) Object Made and IP set";
}

NodeSocket::~NodeSocket()
{
    udpSocket->close();
    udpSocket->~QUdpSocket();

    aliveTimer->~QTimer();
    timeOutTimer->~QTimer();

    ownNode->~Node();
    otherNode->~Node();

    qDebug() << "NodeSocket(Client/Server) Object Destroyed";
}

QString NodeSocket::address() const
{
    return ownNode->address();
}

int NodeSocket::port() const
{
    return ownNode->port();
}

QString NodeSocket::otherNodeAddress() const
{
    return otherNode->address();
}

int NodeSocket::otherNodePort() const
{
    return otherNode->port();
}

bool NodeSocket::isConnected() const
{
    return connected;
}


/*
 *
 * PUBLIC SLOTS
 *
 */

void NodeSocket::setAddress(QString ip)
{
    ownNode->setAddress(ip);
}

void NodeSocket::setPort(int portNo)
{
    ownNode->setPort(portNo);
}

void NodeSocket::setOtherNodeAddress(QString ip)
{
    otherNode->setAddress(ip);
}

void NodeSocket::setOtherNodePort(int portNo)
{
    otherNode->setPort(portNo);
}


/*
 *
 * PROTECTED SLOTS
 *
 */
void NodeSocket::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        qDebug() << "--Datagram received: " << datagram.data();

        processDatagram(datagram);
        qDebug() << "//Datagram processed: " << datagram.data();
    }
}

// TODO: make server independant, usable for both client and server
void NodeSocket::nodeLost()
{
    stopAliveTimer();
    setConnected(false);

    qDebug() << "aliveTimer stopped, alive signal not received in time, connection to other node (client) is lost";
}


void NodeSocket::noResponse()
{
    stopTimeOutTimer();
    setConnected(false);

    qDebug() << "TimeOut stopped, expected response not received";
}

/*
 *
 * PROTECTED SLOTS (SERVER SPECIFIC)
 *
 */
void NodeSocket::startServer()
{
    // Bind to IP and port
    int port = 50000;
    ownNode->setPort(port);
    udpSocket->bind(QHostAddress(ownNode->address()), ownNode->port());
    qDebug() << "Trying to bind this address: " << ownNode->address() << ":" << ownNode->port();

    // Bind other Node port to 50001
    port = 50001;
    otherNode->setPort(port);

    // Make connection to read incomming messages
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    // Make connection with aliveTimer to call nodeLost
    connect(aliveTimer, SIGNAL(timeout()), this, SLOT(nodeLost()));
    qDebug() << "Socket connected!";
}

/*
 *
 * PROTECTED SLOTS (CLIENT SPECIFIC)
 *
 */
void NodeSocket::startClient()
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

// Used when class is used as client
void NodeSocket::sendAliveMsg()
{
    QString address = udpSocket->localAddress().toString();
    quint16 port = udpSocket->localPort();
    QByteArray datagram = "5;" + address.toLatin1() + ";" +  QByteArray::number(port);
    //QByteArray datagram = "5:127.0.0.1:55000";
    sendDatagram(datagram);
}


/*
 *
 * PROTECTED
 *
 */
void NodeSocket::setConnected(const bool connectionBool)
{
//    if (connected == connectionBool)
//            return;

    connected = connectionBool;
    connected ? emit connectionEstablished() : emit connectionLost();
    emit connectionChanged(connected);
}

// All possible cases for incoming datagrams, either server or client
// Odd no's = Server case
// Even no's = Client case
void NodeSocket::processDatagram(const QByteArray &datagram)
{
    QString datastring(datagram);
    switch (datastring.section(';',0,0).toInt())
    {
    case 1:
        // Server case: reply to SYN msg
        sendSYNACK();

        qDebug() << "processed Datagram case 1";
        break;
    case 2:
        // Client case: reply to SYNACK msg
        stopTimeOutTimer();
        sendACK();

        qDebug() << "processed Datagram case 2";
        break;
    case 3:
        // Server case: reply to ACK msg
        stopTimeOutTimer();
        sendAME();

        qDebug() << "processed Datagram case 3: Handshake complete";
        break;
    case 4:
        //Client case: sent every x msecs to confirm connection
        setConnected(true);
        emit connectionEstablished();

        stopTimeOutTimer();
        startAliveMsg();

        qDebug() << "processed Datagram case 4";
        break;
    case 5:
        // Server case: intern reply to the 'alive' msg of the client
        if (!connected)
            setConnected(true);

        resetAliveTimer();

        qDebug() << "processed Datagram case 5";
        break;
    case 10:
        // Client case: receiving reply to the sent request
        stopTimeOutTimer();
        handleDataReply(datagram);

        qDebug() << "processed Datagram case 10";
        break;
    case 11:
        // Server case: incomming request for data from server
        handleDataRequest(datagram);

        qDebug() << "processed Datagram case 11";
        break;
    case 20:
        // Client case: receiving controller action
        handleControllerAction(datagram);
    default:
        // Just break on everything else
        break;
    }
}

void NodeSocket::sendDatagram(const QByteArray &message)
{
    if (QHostAddress(otherNode->address()) == QHostAddress::Any || otherNode->port() == -1)
    {
        emit destAddressInvalid();
        qWarning() << "Trying to send datagram to invalid HostAddress or on an invalid port!";
        return;
    }
    udpSocket->writeDatagram(message.data(), message.size(), QHostAddress(otherNode->address()), otherNode->port());
    emit dataSent(message);
}

void NodeSocket::startTimeOutTimer()
{
    timeOutTimer->start(TOTV);
}

void NodeSocket::stopTimeOutTimer()
{
    timeOutTimer->stop();
}

QString NodeSocket::findIpAddress() const
{
    // Check all available IPv4 addresses of the machine and exclude personal IP's and Localhost
    // Under the assumption the machine is incorporated in a network
    // Use the first found as IP-Address
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    QHostAddress cAddress;
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i).toIPv4Address() &&
            ipAddressesList.at(i) != QHostAddress::LocalHost &&
            !ipAddressesList.at(i).toString().startsWith("169"))
        {
            cAddress = ipAddressesList.at(i);
            break;
        }
        //end if
        qDebug() << "IP Found: " <<  cAddress.toString();
        break;
    }
    //end for
    return cAddress.toString();
}

/*
 *
 * PROTECTED (SERVER SPECIFIC)
 *
 */

// Used when class is used as server
void NodeSocket::sendSYNACK()
{
    QString address = udpSocket->localAddress().toString();
    quint16 port = udpSocket->localPort();
    QByteArray datagram = "2;" + address.toLatin1() + ";" +  QByteArray::number(port);

    startTimeOutTimer();
    sendDatagram(datagram);

    qDebug() << "SYN-ACK Message sent";
}

// Used when class is used as server
// AME = Alive Message Expected
void NodeSocket::sendAME()
{
    QString address = udpSocket->localAddress().toString();
    quint16 port = udpSocket->localPort();
    QByteArray datagram = "4;" + address.toLatin1() + ";" +  QByteArray::number(port);

    startAliveTimer();
    sendDatagram(datagram);

    qDebug() << "AME Message sent";
}

// Used when class is used as server
void NodeSocket::startAliveTimer()
{
    aliveTimer->start(AMTO);

    qDebug() << "aliveTimer started, expecting alive signal at least every " << AMTO << " miliseconds";
}

// Used when class is used as server
void NodeSocket::resetAliveTimer()
{
    aliveTimer->start(AMTO);

    qDebug() << "Trying to reset aliveTimer";
}

void NodeSocket::stopAliveTimer()
{
    aliveTimer->stop();

    qDebug() << "aliveTimer stopped";
}

// Used when class is used as server
void NodeSocket::handleDataRequest(const QString datagram)
{
    qDebug() << "NodeSocket::handleDataRequest: " << datagram;
    QString datastring(datagram);
    emit dataRequestReceived(datastring.section(";",1));
    qDebug() << "Data emitted: " << datastring.section(";",1);
}

/*
 *
 * PROTECTED (CLIENT SPECIFIC)
 *
 */

// Used when class is used as client
void NodeSocket::sendSYN()
{
    // Used when connection is initialised by Client
    QString address = ownNode->address();
    quint16 port = ownNode->port();
    QByteArray datagram = "1;" + address.toLatin1() + ";" +  QByteArray::number(port);

    startTimeOutTimer();
    sendDatagram(datagram);

    qDebug() << "SYN Message sent to " << otherNode->address() << ":" << otherNode->port();
}

// Used when class is used as client
void NodeSocket::sendACK()
{
    // Used when connection is initialised by Client
    QString address = udpSocket->localAddress().toString();
    quint16 port = udpSocket->localPort();
    QByteArray datagram = "3;" + address.toLatin1() + ";" +  QByteArray::number(port);

    startTimeOutTimer();
    sendDatagram(datagram);

    qDebug() << "ACK Message sent";
}

// Used when class is used as Client
void NodeSocket::startAliveMsg()
{
    connect(aliveTimer, SIGNAL(timeout()), this, SLOT(sendAliveMsg()));
    aliveTimer->start(AMSF);

    qDebug() << "Alive message started, sending every " << AMSF << " miliseconds";
}

// Used when class is used as Client
void NodeSocket::handleDataReply(const QString datagram)
{
    qDebug() << "Entered NodeSocket::handleDataReply (parent)";

    QString datastring(datagram);
    emit dataReplyReceived(datastring.section(";",1));
}

void NodeSocket::handleControllerAction(const QString datagram)
{
    emit controllerAction(datagram.section(';', 1, 1));
}
