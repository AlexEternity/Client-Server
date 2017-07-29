#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    count=0;
        _socket = new QTcpSocket(this);
        connect(_socket, SIGNAL(readyRead()), this, SLOT(onSokReadyRead()));
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete _socket;
    delete ui;
}

void MainWindow::onSokReadyRead()
{
    QString message;
    QTextStream in(_socket);
    in>>message;
    QStringList list=message.split("-1");

    for(int i=0;i<list.size();i++)
        count+=list[i].toInt();

    ui->label_3->setText("Найдено: ");
    ui->label_2->setText(QString::number(count));

    ui->pushButton_2->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->lineEdit->setEnabled(true);
}

void MainWindow::on_pushButton_clicked()
{
    OFile=QFileDialog::getOpenFileName(0,"Open Dialog","",QString("Text Documents(*.txt)"));
}

void MainWindow::on_pushButton_2_clicked()
{
    QRegExp re;
    re.setPattern("[A-Zа-яА-Яa-z0-9]+");
    Word=ui->lineEdit->text();

    if(Word.isEmpty()||OFile.isEmpty())
    {
        QMessageBox* errbx = new QMessageBox("Error"
                                             ,"You didn't add file or word"
                                             ,QMessageBox::Information
                                             ,QMessageBox::Yes
                                             ,NULL
                                             ,NULL);
        errbx->exec();
        delete errbx;
        return;
    }

    if(OFile.isNull()||!OFile.contains(".txt"))
    {
        QMessageBox* errbx = new QMessageBox("Error"
                                             ,"File not found or not .txt"
                                             ,QMessageBox::Information
                                             ,QMessageBox::Yes
                                             ,NULL
                                             ,NULL);
            errbx->exec();
            delete errbx;
            return;
    }

    if(!re.exactMatch(Word)||Word.contains(" "))
    {
        QMessageBox* errbx = new QMessageBox("Error"
                                             ,"Write one word(only numbers and letters)"
                                             ,QMessageBox::Information
                                             ,QMessageBox::Yes
                                             ,NULL
                                             ,NULL);
        errbx->exec();
        delete errbx;
        return;
     }

    ui->label_3->setText(" ");
    ui->label_2->setText(" ");
    ui->pushButton->setDisabled(true);
    ui->lineEdit->setDisabled(true);
    ui->pushButton_2->setDisabled(true);

    count=0;

    _socket->connectToHost("localhost",sbPort);

    QFile file(OFile);
    if(!file.open(QFile::ReadWrite))
    {
        QMessageBox* errbx = new QMessageBox("Error"
                                             ,"Can not open file"
                                             ,QMessageBox::Information
                                             ,QMessageBox::Yes
                                             ,NULL
                                             ,NULL);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton->setEnabled(true);
        ui->lineEdit->setEnabled(true);
        errbx->exec();
        delete errbx;
        return;
     }
     QTextStream f(&file);
     QTextStream out(_socket);

     out<<Word;
     out<<"#";
     out<<QString(file.readAll());
     out<<"~";
}
