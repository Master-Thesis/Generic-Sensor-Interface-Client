#include <QtGui>

#include "settingspage.h"

SettingsPage::SettingsPage(QWidget *parent)
    : QWidget(parent)
{
    // Stream Textfield & ClearButton Section
    settingsGroup = new QGroupBox(tr("Settings Display Window"));

    settingsViewer = new QTextBrowser;
    settingsViewer->setMinimumHeight(400);
    clearButton = new QPushButton(tr("Clear Window"));

    inputField = new QLineEdit;
    inputField->setEnabled(false);
    sendButton = new QPushButton(tr("Send"));
    sendButton->setEnabled(false);
    inputLayout = new QHBoxLayout;
    inputLayout->addWidget(inputField);
    inputLayout->addWidget(sendButton);

    settingsVertLayout = new QVBoxLayout;
    settingsVertLayout->addWidget(clearButton);
    settingsVertLayout->addWidget(settingsViewer);
    settingsVertLayout->addLayout(inputLayout);

    settingsGroup->setLayout(settingsVertLayout);

    // Buttons Section
    connectButton = new QPushButton(tr("Connect"));
    disconnectButton = new QPushButton(tr("Disconnect"));
    disconnectButton->setEnabled(false);

    buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(connectButton);
    buttonLayout->addWidget(disconnectButton);

    // Buttons Connections
    makeConnections();

    // Main Layout
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(settingsGroup);
    mainLayout->addStretch(1);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

void SettingsPage::keyPressEvent(QKeyEvent *event)
{
    if (!inputField->hasFocus())
        return;
    switch (event->key()) {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        sendButtonClicked();
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void SettingsPage::makeConnections()
{
    connect(clearButton, SIGNAL(clicked()), settingsViewer, SLOT(clear()));
    connect(sendButton, SIGNAL(clicked()), this, SLOT(sendButtonClicked()));
    connect(connectButton, SIGNAL(clicked()), this, SLOT(connectButtonClicked()));
    connect(disconnectButton, SIGNAL(clicked()), this, SLOT(disconnectButtonClicked()));
}

void SettingsPage::sendButtonClicked()
{
    if (inputField->text().isEmpty())
    {
        return;
    }
    settingsViewer->append("you-> " + inputField->text());
    emit sendClicked(inputField->text());
    inputField->setText("");
    repaint();
}

void SettingsPage::connectButtonClicked()
{
    connectButton->setEnabled(false);
    emit connectClicked();
}

void SettingsPage::disconnectButtonClicked()
{
    disconnectButton->setEnabled(false);
    emit disconnectClicked();
}

void SettingsPage::addToViewer(QString addText)
{
    if (addText.isEmpty())
        return;
    QString text("");
    text.append("server-> ");
    text.append(addText);
    settingsViewer->append(text);
}

void SettingsPage::adjustToConnection(bool isConnected)
{
    disconnectButton->setEnabled(isConnected);
    connectButton->setEnabled(!isConnected);

    inputField->setEnabled(isConnected);
    sendButton->setEnabled(isConnected);
}
