#ifndef SERVERI_H
#define SERVERI_H
#include <QtNetwork>
#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <QFile>

class QTcpServer;

class Server:QObject
{
     Q_OBJECT
public:
    Server();
    ~Server();

    void serverStart();

private slots:
    void sendToClient(QTcpSocket* pSocket, int count,int flag);
    void newUser();
    void readFromClient();
    void deleteLater();

private:
    QString word_;
    QTcpServer *tcpServer_;
    int serverStatus_;
    QMap<int,QTcpSocket *> sClients_;
    QMap<int,int> flags_;
};

#endif // SERVERI_H
