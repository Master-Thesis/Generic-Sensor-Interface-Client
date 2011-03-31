#ifndef NETWORKPAGE_H
#define NETWORKPAGE_H

#include <QtGui>
#include <QWidget>

class NetworkPage : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkPage(QWidget *parent = 0);
    void init(const QString address, const int port);

signals:
    void ipChanged(const QString newIP);
    void portChanged(const int newPort);
    void confirmed();
    void cancelled();

public slots:
    void setIp(QString ipString);
    void setPort(QString portString);
    void adjustToConnection(bool isConnected);

private slots:
    void toggleOtherRadio(bool toggle);
    void confirmButtonClicked();
    void cancelButtonClicked();

private:
    // Protocol Section
    QGroupBox *protocolGroup;
    QLabel *protocolLabel;
    QComboBox *protocolCombo;
    QHBoxLayout *protocolLayout;
    QVBoxLayout *protocolVertLayout;

    // IP Section
    QGroupBox *ipGroup;
    QRadioButton *localHostRadio;
    QRadioButton *otherRadio;
    QLineEdit *otherEdit;
    QHBoxLayout *otherLayout;
    QVBoxLayout *ipVertLayout;

    // Port Section
    QGroupBox *portGroup;
    QLabel *portLabel;
    QLineEdit *portEdit;
    QHBoxLayout *portLayout;

    // Speed Section
    QGroupBox *speedGroup;
    QRadioButton *immediateRadio;
    QRadioButton *msecRadio;
    QLineEdit *msecEdit;
    QHBoxLayout *msecLayout;
    QVBoxLayout *speedVertLayout;

    // Buttons
    QPushButton *confirmButton;
    QPushButton *cancelButton;
    QHBoxLayout *buttonLayout;

    // mainLayout
    QVBoxLayout *mainLayout;
};

#endif // NETWORKPAGE_H
