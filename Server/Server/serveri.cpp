#include "serveri.h"
#include <iostream>
using namespace std;
#include <stdio.h>

Server::Server()
{
    serverStatus_=0;
}

Server::~Server()
{
    delete tcpServer_;
    sClients_.clear();
}

void Server::serverStart()
{
    tcpServer_ = new QTcpServer();

    connect(tcpServer_, SIGNAL(newConnection())
            , this, SLOT(newUser()));

    if (!tcpServer_->listen(QHostAddress::Any, 34998)
            && serverStatus_==0) {
        cout<<"Error";
    }
    else {
        serverStatus_=1;
        cout<<"Server is started\n";
    }
}
void Server::newUser()
{
    if (serverStatus_==1) {
            cout<<"You have new connection\n";

            QTcpSocket* clientSocket=tcpServer_->nextPendingConnection();
            int iduserSocs=clientSocket->socketDescriptor();

            sClients_[iduserSocs]=clientSocket;
            flags_[iduserSocs]=0;

            connect(clientSocket, SIGNAL(disconnected()),
                        clientSocket, SLOT(deleteLater())
                       );
            connect(sClients_[iduserSocs],SIGNAL(readyRead())
                    ,this, SLOT(readFromClient()));
    }
}

void Server::deleteLater()
{
    cout<<"Connection was lost\n";
}

void Server::readFromClient()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    int iduserSocs=clientSocket->socketDescriptor();
    QFile file("File"+QDateTime::currentDateTime().toString("hh-mm-ss")+".txt");

    if(!file.open(QIODevice::ReadWrite)) {
        cout<<"Error,while open file";
    }
    QTextStream in(clientSocket);
    char buf[1024];
    while (in.atEnd()!=true) {
        int nBlockSize=in.device()->read(buf,sizeof(buf));
        file.write(buf, nBlockSize);
    }
    QString line;
    int count=0;
    file.seek(0);
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QString str = line.data();
        if (str.contains('#')) {
            QStringList list=str.split('#');
            word_=list[0];
            count+=list[1].count(word_);
            continue;
        }
        count+=str.count(word_);
        if (str.contains('~'))
            flags_[iduserSocs]=1;
    }
    file.remove();
    sendToClient(clientSocket,count,flags_[iduserSocs]);

}

void Server::sendToClient(QTcpSocket* pSocket, int count,int flag)
{
    QTextStream out(pSocket);
    int iduserSocs=pSocket->socketDescriptor();
    QString s=QString::number(count);

    s+="-1";
    out<<s;
    if (flag==1) {
        pSocket->close();
        sClients_.remove(iduserSocs);
        flags_.remove(iduserSocs);
    }
}
