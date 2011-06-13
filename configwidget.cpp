#include <QtGui>

#include "configwidget.h"
#include "udpclient.h"
#include "configfilehandler.h"
#include "configWidgetPages/networkpage.h"
#include "configWidgetPages/settingspage.h"
#include "configWidgetPages/consolepage.h"
#include "tetrixWidget/tetrixwindow.h"

ConfigWidget::ConfigWidget()
{
    //udpClient = new UDPClient(true);
    udpClient = new UDPClient(false);
    udpClient->setPort(50001);

    contentsWidget = new QListWidget;
    contentsWidget->setViewMode(QListView::IconMode);
    contentsWidget->setIconSize(QSize(64, 64));
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setMaximumWidth(128);
    contentsWidget->setSpacing(12);

    pagesWidget = new QStackedWidget;
    pagesWidget->setMinimumSize(QSize(800,700));

    networkPage = new NetworkPage;
    networkPage->init(udpClient->otherNodeAddress(), udpClient->otherNodePort());
    settingsPage = new SettingsPage;
    consolePage = new ConsolePage;
    tetrixPage = new TetrixWindow;
    pagesWidget->addWidget(networkPage);
    pagesWidget->addWidget(settingsPage);
    pagesWidget->addWidget(consolePage);
    pagesWidget->addWidget(tetrixPage);

    QPushButton *closeButton = new QPushButton(tr("Close"));

    createIcons();
    contentsWidget->setCurrentRow(0);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    makeConnections();

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(contentsWidget);
    horizontalLayout->addWidget(pagesWidget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(5);
    //mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Generic Sensor Interface Client"));
}

ConfigWidget::~ConfigWidget()
{
    networkPage->~QObject();
    settingsPage->~QObject();
    consolePage->~QObject();
    pagesWidget->~QObject();
    contentsWidget->~QObject();

    udpClient->~NodeSocket();
}

void ConfigWidget::createIcons()
{
    QListWidgetItem *configButton = new QListWidgetItem(contentsWidget);
    configButton->setIcon(QIcon(":/images/network.png"));
    configButton->setText(tr("Network Settings"));
    configButton->setTextAlignment(Qt::AlignHCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *queryButton = new QListWidgetItem(contentsWidget);
    queryButton->setIcon(QIcon(":/images/settings.png"));
    queryButton->setText(tr("Controller Settings"));
    queryButton->setTextAlignment(Qt::AlignHCenter);
    queryButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *consoleButton = new QListWidgetItem(contentsWidget);
    consoleButton->setIcon(QIcon(":/images/console.png"));
    consoleButton->setText(tr("Console / Ouptut"));
    consoleButton->setTextAlignment(Qt::AlignHCenter);
    consoleButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *gameButton = new QListWidgetItem(contentsWidget);
    gameButton->setIcon(QIcon(":/images/game.png"));
    gameButton->setText(tr("Game: Tetrix"));
    gameButton->setTextAlignment(Qt::AlignHCenter);
    gameButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);


    connect(contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}

void ConfigWidget::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    pagesWidget->setCurrentIndex(contentsWidget->row(current));
}

void ConfigWidget::startConsoleStream()
{
    connect(udpClient, SIGNAL(controllerAction(QString)), consolePage, SLOT(addToViewer(QString)));
    connect(udpClient, SIGNAL(controllerAction(QString)), tetrixPage, SLOT(handleCommand(QString)));
}

void ConfigWidget::stopConsoleStream()
{
    disconnect(udpClient, SIGNAL(controllerAction(QString)), consolePage, SLOT(addToViewer(QString)));
    disconnect(udpClient, SIGNAL(controllerAction(QString)), tetrixPage, SLOT(handleCommand(QString)));
}

void ConfigWidget::makeConnections()
{
    // networkPage Connections
    connect(networkPage, SIGNAL(ipChanged(QString)), udpClient, SLOT(setOtherNodeAddress(QString)));
    connect(networkPage, SIGNAL(portChanged(int)), udpClient, SLOT(setOtherNodePort(int)));

    // settingsPage Connections
    connect(settingsPage, SIGNAL(connectClicked()), udpClient, SLOT(startSocket()));
    connect(settingsPage, SIGNAL(sendClicked(QString)), udpClient, SLOT(sendUserCommand(QString)));
    connect(settingsPage, SIGNAL(disconnectClicked()), udpClient, SLOT(stopSocket()));
    connect(settingsPage, SIGNAL(configFileSelected(QString)), ConfigFileHandler::instance(), SLOT(handleFile(QString)));

    // consolePage Connections
    connect(consolePage, SIGNAL(startClicked()), this, SLOT(startConsoleStream()));
    connect(consolePage, SIGNAL(stopClicked()), this, SLOT(stopConsoleStream()));

    // ConfigFileHandler Connections
    connect(ConfigFileHandler::instance(), SIGNAL(commandFromFile(QString)), udpClient, SLOT(sendUserCommand(QString)));

    // udpClient Connection
    connect(udpClient, SIGNAL(dataReplyReceived(QString)), settingsPage, SLOT(addToViewer(QString)));
    connect(udpClient, SIGNAL(connectionChanged(bool)), networkPage, SLOT(adjustToConnection(bool)));
    connect(udpClient, SIGNAL(connectionChanged(bool)), settingsPage, SLOT(adjustToConnection(bool)));
    connect(udpClient, SIGNAL(connectionChanged(bool)), consolePage, SLOT(adjustToConnection(bool)));
}
