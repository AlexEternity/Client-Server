#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent),
    ui_ (new Ui::MainWindow)
{
    count_=0;
    socket_ = new QTcpSocket(this);
    connect(socket_, SIGNAL(readyRead()), this, SLOT(onSokReadyRead()));
    ui_->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete socket_;
    delete ui_;
}

void MainWindow::onSokReadyRead()
{
    QString message;
    QTextStream in(socket_);
    in>>message;
    QStringList list=message.split("-1");

    for (int i=0; i<list.size(); i++) {
        count_+=list[i].toInt();
    }
    ui_->label_3->setText("Matches: ");
    ui_->label_2->setText(QString::number(count_));

    ui_->pushButton_2->setEnabled(true);
    ui_->pushButton->setEnabled(true);
    ui_->lineEdit->setEnabled(true);
}

void MainWindow::onFileButtonClicked()
{
    oFile_=QFileDialog::getOpenFileName(0,"Open Dialog",""
                                        ,QString("Text Documents(*.txt)"));
}

void MainWindow::onSendButtonClicked()
{
    QRegExp regular;
    regular.setPattern("[A-Zа-яА-Яa-z0-9]+");
    word_=ui_->lineEdit->text();

    if(word_.isEmpty()||oFile_.isEmpty()) {
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

    if(oFile_.isNull()||!oFile_.contains(".txt")) {
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

    if(!regular.exactMatch(word_)||word_.contains(" ")) {
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

    ui_->label_3->setText(" ");
    ui_->label_2->setText(" ");
    ui_->pushButton->setDisabled(true);
    ui_->lineEdit->setDisabled(true);
    ui_->pushButton_2->setDisabled(true);

    count_=0;

    socket_->connectToHost("localhost",SB_PORT);

    QFile file(oFile_);
    if(!file.open(QFile::ReadWrite)) {
        QMessageBox* errbx = new QMessageBox("Error"
                                             ,"Can not open file"
                                             ,QMessageBox::Information
                                             ,QMessageBox::Yes
                                             ,NULL
                                             ,NULL);
        ui_->pushButton_2->setEnabled(true);
        ui_->pushButton->setEnabled(true);
        ui_->lineEdit->setEnabled(true);
        errbx->exec();
        delete errbx;
        return;
     }
     QTextStream f(&file);
     QTextStream out(socket_);

     out<<word_;
     out<<"#";
     out<<QString(file.readAll());
     out<<"~";
}
