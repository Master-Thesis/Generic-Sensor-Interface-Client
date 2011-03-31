#ifndef CONSOLEPAGE_H
#define CONSOLEPAGE_H

#include <QtGui>
#include <QWidget>

class ConsolePage : public QWidget
{
    Q_OBJECT

public:
    explicit ConsolePage(QWidget *parent = 0);

signals:
    void startClicked();
    void stopClicked();

public slots:
    void addToViewer(QString appendText);
    void adjustToConnection(bool isConnected);

private slots:
    void startButtonClicked();
    void stopButtonClicked();

private:
    void makeConnections();

    QGroupBox *streamGroup;
    QTextBrowser *streamViewer;
    QPushButton *clearButton;
    QVBoxLayout *streamVertLayout;
    QPushButton *startButton;
    QPushButton *stopButton;
    QHBoxLayout *buttonLayout;
    QVBoxLayout *mainLayout;
};

#endif // CONSOLEPAGE_H
