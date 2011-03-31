#include <QtGui>

#include "networkpage.h"
#include "ip4validator.h"

NetworkPage::NetworkPage(QWidget *parent)
    : QWidget(parent)
{
    // Protocol Section
    protocolGroup = new QGroupBox(tr("Protocol Selection"));

    protocolLabel = new QLabel(tr("Server/Client Protocol:"));

    protocolCombo = new QComboBox;
    protocolCombo->addItem(tr("UDP Protocol"));
    protocolCombo->addItem(tr("TCP Protocol"));
    protocolCombo->setMaximumWidth(200);

    protocolLayout = new QHBoxLayout;
    protocolLayout->addWidget(protocolLabel);
    protocolLayout->addWidget(protocolCombo);

    protocolVertLayout = new QVBoxLayout;
    protocolVertLayout->addLayout(protocolLayout);

    protocolGroup->setLayout(protocolVertLayout);
    protocolGroup->setEnabled(false);

    // IP Section
    ipGroup = new QGroupBox(tr("Server IP-Address"));

    localHostRadio = new QRadioButton(tr("LocalHost"));
    localHostRadio->setChecked(true);

    otherRadio = new QRadioButton(tr("Other"));
    otherEdit = new QLineEdit;
    IP4Validator *otherValidator = new IP4Validator();
    otherEdit->setMaximumWidth(200);
    otherEdit->setEnabled(false);
    otherEdit->setInputMask("000.000.000.000;");
    otherEdit->setValidator(otherValidator);
    //otherEdit->setInputMask("999.999.999.999;0");

    otherLayout = new QHBoxLayout;
    otherLayout->addWidget(otherRadio);
    otherLayout->addStretch();
    otherLayout->addWidget(otherEdit);

    ipVertLayout = new QVBoxLayout;
    ipVertLayout->addWidget(localHostRadio);
    ipVertLayout->addLayout(otherLayout);

    ipGroup->setLayout(ipVertLayout);

    // IP Section connections
    connect(otherRadio, SIGNAL(toggled(bool)), this, SLOT(toggleOtherRadio(bool)));

    // Port Section
    portGroup = new QGroupBox(tr("Server Port"));

    portLabel = new QLabel(tr("Port (between 49152 and 65535):"));
    portEdit = new QLineEdit();
    portEdit->setMaximumWidth(200);
    //portEdit->setInputMask("99999;0");

    portLayout = new QHBoxLayout;
    portLayout->addWidget(portLabel);
    portLayout->addStretch();
    portLayout->addWidget(portEdit);

    portGroup->setLayout(portLayout);

    // Speed Section
    speedGroup = new QGroupBox(tr("Throughput Speed"));

    immediateRadio = new QRadioButton(tr("Immediate (no delay)"));

    msecRadio = new QRadioButton(tr("Every ... msecs"));
    msecEdit = new QLineEdit(msecRadio);
    msecEdit->isEnabledTo(msecRadio);
    msecEdit->setMaximumWidth(200);

    msecLayout = new QHBoxLayout;
    msecLayout->addWidget(msecRadio);
    msecLayout->addStretch();
    msecLayout->addWidget(msecEdit);

    speedVertLayout = new QVBoxLayout;
    speedVertLayout->addWidget(immediateRadio);
    speedVertLayout->addLayout(msecLayout);

    speedGroup->setLayout(speedVertLayout);
    speedGroup->setEnabled(false);

    // Buttons
    confirmButton = new QPushButton(tr("Confirm"));
    cancelButton = new QPushButton(tr("Cancel"));

    buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(confirmButton);
    buttonLayout->addWidget(cancelButton);

    // Buttons connections
    connect(confirmButton, SIGNAL(clicked()), this, SLOT(confirmButtonClicked()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));

    // Add all sections to mainLayout
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(protocolGroup);
    mainLayout->addWidget(ipGroup);
    mainLayout->addWidget(portGroup);
    mainLayout->addWidget(speedGroup);
    mainLayout->addStretch(1);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

void NetworkPage::init(const QString address, const int port)
{
    otherEdit->setText(address);
    portEdit->setText(QString::number(port));
}

void NetworkPage::toggleOtherRadio(bool toggle)
{
    otherEdit->setEnabled(toggle);
}

void NetworkPage::confirmButtonClicked()
{
    bool portOk;
    int port = portEdit->text().toInt(&portOk);
    if (!(port >= 49152 && port <= 65535) || !portOk)
    {
        QMessageBox msgBox(QMessageBox::Warning, "Warning: invallid input", "The port value should be between 49152 and 65535!");
        msgBox.exec();
        QApplication::processEvents();
        return;
    }
    QString ip;
    localHostRadio->isChecked() ? ip = "127.0.0.1" : ip = otherEdit->text();
    emit ipChanged(ip);
    emit portChanged(port);
    emit confirmed();
}

void NetworkPage::cancelButtonClicked()
{
    emit cancelled();
}

void NetworkPage::setIp(QString ipString)
{
    otherEdit->setText(ipString);
}

void NetworkPage::setPort(QString portString)
{
    portEdit->setText(portString);
}

void NetworkPage::adjustToConnection(bool isConnected)
{
    //protocolGroup->setEnabled(!isConnected);
    ipGroup->setEnabled(!isConnected);
    portGroup->setEnabled(!isConnected);
    //speedGroup->setEnabled(!isConnected);

    confirmButton->setEnabled(!isConnected);
    cancelButton->setEnabled(!isConnected);
}
