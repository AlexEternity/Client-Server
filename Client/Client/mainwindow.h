#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QString>
#include <QHostAddress>
#include <QTime>
#include <QPushButton>
#include <QRegExp>
#include <QFileSystemModel>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onFileButtonClicked();

    void onSendButtonClicked();

    void onSokReadyRead();

private:
    Ui::MainWindow *ui_;
    int count_;
    QString word_;
    QString oFile_;
    QTcpSocket *socket_;
    quint16 blockSize_;
    QString client_;
    int SB_PORT=34998;
};

#endif // MAINWINDOW_H
