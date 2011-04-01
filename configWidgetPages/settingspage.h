#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QtGui>
#include <QWidget>

class SettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPage(QWidget *parent = 0);

signals:
    void connectClicked();
    void disconnectClicked();
    void sendClicked(const QString command);

public slots:
    void addToViewer(const QString addText);
    void adjustToConnection(bool isConnected);

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void sendButtonClicked();
    void connectButtonClicked();
    void disconnectButtonClicked();

private:
    void makeConnections();
    void addCommand(QString command);
    void toggleCommands(bool isUp);

    // Stream Textfield & ClearButton Section
    QGroupBox *settingsGroup;
    QTextBrowser *settingsViewer;
    QPushButton *clearButton;
    QLineEdit *inputField;
    QPushButton *sendButton;
    QHBoxLayout *inputLayout;
    QVBoxLayout *settingsVertLayout;

    // Buttons Section
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QHBoxLayout *buttonLayout;

    // Main Layout
    QVBoxLayout *mainLayout;

    // Command List
    QStringList *commands;
    int i_commands;
};

#endif // SETTINGSPAGE_H
