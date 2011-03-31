#include "node.h"

Node::Node(QObject *parent) :
    QObject(parent)
{
    init();
}

Node::Node(QString &n_address, int n_port, QObject *parent)
    : QObject(parent)
{
    init();
    setAddress(n_address);
    setPort(n_port);
}

Node::~Node()
{
    _address->~QHostAddress();
}

//Node::Node(QHostAddress &n_address, int n_port, QObject *parent)
//    : QObject(parent)
//{
//    init();
//    setAddress(n_address);
//    setPort(n_port);
//}

void Node::init()
{
    qDebug() << "Entered init() of Node";
    _address = new QHostAddress(QHostAddress::LocalHost);
    _port = 50000;
}

QString Node::address() const
{
    return _address->toString();
}

int Node::port() const
{
    return _port;
}

void Node::setAddress(const QString &n_address)
{
    if(_address->toString() == n_address)
        return;
    _address->setAddress(n_address);
    emit addressChanged(_address->toString());
}

//void Node::setAddress(QHostAddress &n_address)
//{
//    if(_address == n_address)
//        return;
//    _address = n_address;
//}

void Node::setPort(const int &n_port)
{
    if(_port == n_port)
        return;
    _port = n_port;
    emit portChanged(_port);
}
