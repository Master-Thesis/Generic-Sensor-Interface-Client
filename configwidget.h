#ifndef CONFIGWIDGET_H
#define CONFIGWIDGET_H

#include <QWidget>

#include "udpclient.h"
#include "configWidgetPages/networkpage.h"
#include "configWidgetPages/settingspage.h"
#include "configWidgetPages/consolepage.h"
#include "tetrixWidget/tetrixwindow.h"

QT_BEGIN_NAMESPACE
class QListWidget;
class QListWidgetItem;
class QStackedWidget;
QT_END_NAMESPACE

class ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    ConfigWidget();
    virtual ~ConfigWidget();

public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
    void startConsoleStream();
    void stopConsoleStream();

private:
    void createIcons();
    void makeConnections();

    QListWidget *contentsWidget;
    QStackedWidget *pagesWidget;
    NetworkPage *networkPage;
    SettingsPage *settingsPage;
    ConsolePage *consolePage;
    TetrixWindow *tetrixPage;

    UDPClient *udpClient;
};

#endif // CONFIGWIDGET_H
