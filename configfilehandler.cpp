 #include "configfilehandler.h"

#include <QFile>
#include <QTextStream>

#include <QMutex>
static QMutex mutex;
ConfigFileHandler* ConfigFileHandler::m_confighandler = 0;


ConfigFileHandler::ConfigFileHandler(QObject *parent) :
    QObject(parent)
{
}

ConfigFileHandler::~ConfigFileHandler()
{
}

ConfigFileHandler* ConfigFileHandler::instance()
{
    if(!m_confighandler)
    {
        mutex.lock();

        if(!m_confighandler)
            m_confighandler = new ConfigFileHandler();

        mutex.unlock();
    }

    return m_confighandler;
}

void ConfigFileHandler::drop()
{
    mutex.lock();
    delete m_confighandler;
    m_confighandler = 0;
    mutex.unlock();
}


void ConfigFileHandler::handleFile(const QString filepath)
{
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        emit commandFromFile(line);
    }
}
