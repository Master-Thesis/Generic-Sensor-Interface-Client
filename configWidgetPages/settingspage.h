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
    void configFileSelected(const QString filepath);

public slots:
    void addToViewer(const QString addText);
    void adjustToConnection(bool isConnected);

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void sendButtonClicked();
    void connectButtonClicked();
    void disconnectButtonClicked();
    void launchFileDialog();
    void applyButtonClicked();

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

    // File Input Section
    QGroupBox *fileGroup;
    QLineEdit *filepathField;
    QPushButton *browseButton;
    QPushButton *applyButton;
    QHBoxLayout *fileLayout;

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
