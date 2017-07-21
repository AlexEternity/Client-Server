#include <QCoreApplication>

#include "serveri.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server* server=new Server();
    server->Sstart();
    return a.exec();
}
