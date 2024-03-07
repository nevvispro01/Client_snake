#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "graphicsscene.h"

#include <QHostAddress>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <iostream>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto scene = new GraphicsScene(QRectF(0,0,20*FieldWidth + 10,20*FieldHeight + 10));
    connect(scene, &GraphicsScene::signalSetDirection, this, &MainWindow::slotSetDirection);
    ui->graphicsView->setScene(std::move(scene));
    connectToServer("172.21.10.103", 11000);

    connect(socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectToServer(const QString &hostName, quint16 port)
{
    socket = new QTcpSocket();
    socket->connectToHost(QHostAddress(hostName), port);
}

void MainWindow::onConnected()
{
    qDebug() << "Connected to server";
}

void MainWindow::onReadyRead()
{
    QByteArray msg = socket->readAll();
    QByteArray msg_2 = msg;
    QByteArray res;

    auto start = msg_2.indexOf("!start");

    while (start != -1){

        auto end = msg_2.indexOf("!end");

        if (end == -1 or start > end){
            return;
        }

        res = msg_2.mid(start + 6, end-8);

        QJsonParseError error;
        QJsonDocument json = QJsonDocument::fromJson(res, &error);

        if (error.error != QJsonParseError::ParseError::NoError){
            std::cout << "Ошибка чтения сообщения" << error.errorString().toStdString() << std::endl;
            return;
        }

        QJsonObject j_obj = json.object();

        if (j_obj.contains("type")){
            if(j_obj["type"].toString() == "step"){

                ui->graphicsView->scene()->clear();
                ui->graphicsView->scene()->addRect(QRectF(4,4,20*FieldWidth + 1,20*FieldHeight + 1), QPen(QBrush(Qt::black), 5));
                int index{0};
                for (auto player : j_obj["players"].toArray()){

                    bool head{true};
                    for (auto pos : player.toArray()){

                        auto pos_obj = pos.toObject();
                        QPen pen;
                        if (index == 0){

                            pen.setBrush(Qt::blue);
                        }else{
                            pen.setBrush(Qt::red);
                        }
                        pen.setWidth(3);
                        if (head){
                            if (index == 0){

                                pen.setBrush(Qt::darkBlue);
                            }else{
                                pen.setBrush(Qt::darkRed);
                            }
                            head = false;
                        }

                        ui->graphicsView->scene()->addRect(QRectF(pos_obj["X"].toInt()*20,
                                                           pos_obj["Y"].toInt()*20,20,20), pen);
                    }
                    ++index;
                }
                for (auto pos : j_obj["posFood"].toArray()){

                    auto pos_obj = pos.toObject();
                    QPen pen;
                    pen.setBrush(Qt::yellow);
                    pen.setWidth(3);
                    ui->graphicsView->scene()->addEllipse(QRectF(pos_obj["X"].toInt()*20,
                                                       pos_obj["Y"].toInt()*20,20,20), pen);
                }
            }else if (j_obj["type"].toString() == "number"){

                number = j_obj["number"].toInt();
            }
        }


        msg_2.replace(0, end + 4, "");

        start = msg_2.indexOf("!start");

    }
}

void MainWindow::onDisconnected()
{
    qDebug() << "Disconnected from server";
}

void MainWindow::slotSetDirection(int direction)
{
    QJsonObject obj;
    obj["type"] = "direction";
    obj["key"] = direction;
    obj["number"] = number;

    QByteArray answer{"!start"};

    answer += QJsonDocument(obj).toJson(QJsonDocument::Compact);

    answer.push_back(0x1);
    answer.push_back(0x1);
    answer.push_back("!end");

    socket->write(answer);
}

