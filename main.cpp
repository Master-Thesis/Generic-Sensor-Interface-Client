#include <QApplication>
#include <QDebug>

#include "udpclient.h"
#include "configwidget.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ConfigWidget configWidget;
    configWidget.show();

    return app.exec();
}
