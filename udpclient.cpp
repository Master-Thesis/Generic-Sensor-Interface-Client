#include "udpclient.h"

#include "../nodesocket.h"

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
    // Bind to ip and port

    qDebug() << "UDPClient binded, trying to start";

    startClient();
    emit connected(true);
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
