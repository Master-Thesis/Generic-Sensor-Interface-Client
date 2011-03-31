#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QtNetwork>

class Node : public QObject
{
    Q_OBJECT

public:
    explicit Node(QObject *parent = 0);
    virtual ~Node();
    Node(QString &n_address, int n_port, QObject *parent = 0);
    //Node(QHostAddress &n_address, int n_port, QObject *parent = 0);

    QString address() const;
    int port() const;

signals:
    void addressChanged(QString address);
    void portChanged(int port);

public slots:
    void setAddress(const QString &n_address);
    //void setAddress(QHostAddress &n_address);
    void setPort(const int &n_port);

private:
    void init();

    QHostAddress *_address;
    int _port;
};

#endif // NODE_H
