#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTcpSocket>
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
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void onSokReadyRead();

private:
    Ui::MainWindow *ui;
    int count;
    QString Word;
    QString OFile;
    QTcpSocket *_socket;
    quint16 _blockSize;
    QString _client;
    int sbPort=34998;
};

#endif // MAINWINDOW_H
