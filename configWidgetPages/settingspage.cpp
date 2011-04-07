#include <QtGui>

#include "settingspage.h"

SettingsPage::SettingsPage(QWidget *parent)
    : QWidget(parent)
{
    // initialise commands QStrinList
    commands = new QStringList();
    commands->reserve(10);
    i_commands = -1;

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

    // initialise file input Section
    fileGroup = new QGroupBox("Quick Configuration File Selection");

    filepathField = new QLineEdit;
    filepathField->setEnabled(false);
    browseButton = new QPushButton(tr("Browse..."));
    applyButton = new QPushButton(tr("Apply"));

    fileLayout = new QHBoxLayout;
    fileLayout->addWidget(filepathField);
    fileLayout->addWidget(browseButton);
    fileLayout->addSpacing(75);
    fileLayout->addWidget(applyButton);

    fileGroup->setLayout(fileLayout);
    fileGroup->setEnabled(false);

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
    mainLayout->addWidget(fileGroup);
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
    case Qt::Key_Up:
        toggleCommands(true);
        break;
    case Qt::Key_Down:
        toggleCommands(false);
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
    connect(browseButton, SIGNAL(clicked()), this, SLOT(launchFileDialog()));
    connect(filepathField, SIGNAL(selectionChanged()), this, SLOT(launchFileDialog()));
    connect(filepathField, SIGNAL(textEdited(QString)), this, SLOT(launchFileDialog()));
    connect(applyButton, SIGNAL(clicked()), this, SLOT(applyButtonClicked()));
}

void SettingsPage::sendButtonClicked()
{
    if (inputField->text().isEmpty())
    {
        return;
    }
    settingsViewer->append("you-> " + inputField->text());
    emit sendClicked(inputField->text());
    addCommand(inputField->text());
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

void SettingsPage::launchFileDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Quick Configuration File"), "C:/", tr("Quick Configuration Files (*.qcf)"));
    filepathField->setText(fileName);
    qDebug() << "File Name: " << fileName;
}

void SettingsPage::applyButtonClicked()
{
    QFile file(filepathField->text());
    if (!file.exists() || !file.open(QIODevice::ReadOnly))
    {
        QMessageBox msgBox(QMessageBox::Warning, "Warning: invalid file", "The file you selected was not a valid .qcf file!");
        msgBox.exec();
        QApplication::processEvents();
        return;
    }
    emit configFileSelected(filepathField->text());
    filepathField->setText("");
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
    fileGroup->setEnabled(isConnected);
}

void SettingsPage::addCommand(const QString command)
{
    i_commands = -1;
    commands->prepend(command);
    if(commands->size() > 10)
        commands->pop_back();
}

void SettingsPage::toggleCommands(bool isUp)
{
    if(isUp)
    {
        ++i_commands;

        if(i_commands >= commands->size() || i_commands >= 9)
            i_commands = commands->size()-1;

        inputField->setText(commands->at(i_commands));
    }
    else
    {
        if(i_commands == -1)
            return;

        --i_commands;

        if(i_commands < 0)
            i_commands = 0;

        inputField->setText(commands->at(i_commands));
    }
}

