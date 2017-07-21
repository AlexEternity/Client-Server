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
    void Sstart();

private slots:
    void SendToClient(QTcpSocket* pSocket, int count);
    void newuser();
    void ReadFromClient();


private:
    QString Word;
    QTcpServer *tcpServer;
    int server_status;
    QMap<int,QTcpSocket *> SClients;
};

#endif // SERVERI_H
