#include <QtGui>

#include "consolepage.h"

ConsolePage::ConsolePage(QWidget *parent)
    : QWidget(parent)
{
    // Stream Textfield & Button Section
    streamGroup = new QGroupBox(tr("Controller Stream View"));

    streamViewer = new QTextBrowser;
    streamViewer->setMinimumHeight(400);
    clearButton = new QPushButton(tr("Clear Window"));

    streamVertLayout = new QVBoxLayout;
    streamVertLayout->addWidget(clearButton);
    streamVertLayout->addWidget(streamViewer);

    streamGroup->setLayout(streamVertLayout);

    // Buttons
    startButton = new QPushButton(tr("Start Stream"));
    startButton->setEnabled(false);
    stopButton = new QPushButton(tr("Stop Stream"));
    stopButton->setEnabled(false);

    buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(stopButton);

    // Make all the necessary connections
    makeConnections();

    // MainLayout
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(streamGroup);
    mainLayout->addStretch(1);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

void ConsolePage::makeConnections()
{
    connect(clearButton, SIGNAL(clicked()), streamViewer, SLOT(clear()));
    connect(startButton, SIGNAL(clicked()), this, SLOT(startButtonClicked()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(stopButtonClicked()));
}

void ConsolePage::startButtonClicked()
{
    startButton->setEnabled(false);
    stopButton->setEnabled(true);
    emit startClicked();
}

void ConsolePage::stopButtonClicked()
{
    stopButton->setEnabled(false);
    startButton->setEnabled(true);
    emit stopClicked();
}

void ConsolePage::adjustToConnection(bool isConnected)
{
    startButton->setEnabled(isConnected);
    if(!isConnected)
        stopButton->setEnabled(isConnected);
}

void ConsolePage::addToViewer(QString appendText)
{
    if (appendText.isEmpty())
        return;
    streamViewer->append(appendText);
}
