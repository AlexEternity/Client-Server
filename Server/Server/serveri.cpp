#include "serveri.h"
#include <iostream>
using namespace std;
#include <stdio.h>

Server::Server()
{
    server_status=0;
}

Server::~Server()
{
    delete tcpServer;
    SClients.clear();
}

void Server::Sstart()
{
    tcpServer = new QTcpServer();

    connect(tcpServer, SIGNAL(newConnection())
            , this, SLOT(newuser()));

    if (!tcpServer->listen(QHostAddress::Any, 34998)
            && server_status==0)
    {
        cout<<"Error";
    }
    else
    {
        server_status=1;
        cout<<"Server is started\n";
    }
}
void Server::newuser()
{
    if(server_status==1)
    {
            cout<<"You have new connection\n";

            QTcpSocket* clientSocket=tcpServer->nextPendingConnection();
            int idusersocs=clientSocket->socketDescriptor();

            SClients[idusersocs]=clientSocket;
            Flags[idusersocs]=0;

            connect(clientSocket, SIGNAL(disconnected()),
                        clientSocket, SLOT(deleteLater())
                       );
            connect(SClients[idusersocs],SIGNAL(readyRead())
                    ,this, SLOT(ReadFromClient()));
    }
}

void Server::deleteLater()
{
    cout<<"Connection was lost\n";
}

void Server::ReadFromClient()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    int idusersocs=clientSocket->socketDescriptor();
    QFile file("File"+QDateTime::currentDateTime().toString("hh-mm-ss")+".txt");

    if(!file.open(QIODevice::ReadWrite))
        cout<<"Error,while open file";

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
        if(str.contains('~'))
            Flags[idusersocs]=1;
    }
    file.remove();
    SendToClient(clientSocket,count,Flags[idusersocs]);

}

void Server::SendToClient(QTcpSocket* pSocket, int count,int flag)
{
    QTextStream out(pSocket);
    int idusersocs=pSocket->socketDescriptor();
    QString s=QString::number(count);

    s+="-1";
    out<<s;
    if(flag==1)
    {
        pSocket->close();
        SClients.remove(idusersocs);
        Flags.remove(idusersocs);
    }
}
