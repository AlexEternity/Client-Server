#include "serveri.h"
#include <iostream>
using namespace std;
#include <stdio.h>

Server::Server()
{
    server_status=0;
}

void Server::Sstart()
{
    tcpServer = new QTcpServer();

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newuser()));

    if (!tcpServer->listen(QHostAddress::Any, 34998) && server_status==0)
    {
        qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(tcpServer->errorString());
        std::cout<<"Error";
    }
    else
    {
        server_status=1;
        qDebug() << tcpServer->isListening() << "TCPSocket listen on port";
        std::cout<<"Server is started";
    }
}
void Server::newuser()
{
    if(server_status==1)
    {
            std::cout<<"You have new connection\n";

            QTcpSocket* clientSocket=tcpServer->nextPendingConnection();
            int idusersocs=clientSocket->socketDescriptor();

            SClients[idusersocs]=clientSocket;

            connect(SClients[idusersocs],SIGNAL(readyRead()),this, SLOT(ReadFromClient()));
    }
}

void Server::ReadFromClient()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();

    QFile file("File"+QDateTime::currentDateTime().toString("hh-mm-ss")+".txt");
    if(!file.open(QIODevice::ReadWrite))
    {
        qDebug() << "Ошибк  открытия";
    }
    QTextStream in(clientSocket);
    char a[1024];
    while(in.atEnd()!=true)
    {
        int nBlockSize=in.device()->read(a,sizeof(a));
        file.write(a, nBlockSize);
    }

    QString line;
    QString word;
    int count=0;
    file.seek(0);
    while(!file.atEnd())
    {
        QByteArray line = file.readLine();
        QString str = line.data();
        if(str.contains('#'))
        {
            QStringList list=str.split('#');
            Word=list[0];
            count+=list[1].count(Word);
            continue;
        }
    count+=str.count(Word);
    }
    file.remove();

    SendToClient(clientSocket,count);

}

void Server::SendToClient(QTcpSocket* pSocket, int count)
{
    QTextStream out(pSocket);
    QString s=QString::number(count);

    s+="-1";
    out<<s;
}
