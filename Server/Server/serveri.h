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
    void Sstart();

private slots:
    void SendToClient(QTcpSocket* pSocket, int count,int flag);
    void newuser();
    void ReadFromClient();
    void deleteLater();

private:
    QString Word;
    QTcpServer *tcpServer;
    int server_status;
    QMap<int,QTcpSocket *> SClients;
    QMap<int,int> Flags;
};

#endif // SERVERI_H
