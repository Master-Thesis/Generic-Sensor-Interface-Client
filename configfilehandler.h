#ifndef CONFIGFILEHANDLER_H
#define CONFIGFILEHANDLER_H

#include <QObject>

class ConfigFileHandler : public QObject
{
    Q_OBJECT
public:
    static ConfigFileHandler* instance();
    static void drop();

signals:
    void commandFromFile(const QString command);

public slots:
    void handleFile(const QString filepath);

private:
    explicit ConfigFileHandler(QObject *parent = 0);
    virtual ~ConfigFileHandler();

    static ConfigFileHandler* m_confighandler;
};

#endif // CONFIGFILEHANDLER_H
