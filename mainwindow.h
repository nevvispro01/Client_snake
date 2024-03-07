#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void connectToServer(const QString &hostName, quint16 port);

private slots:
    void onConnected();

    void onReadyRead();

    void onDisconnected();

    void slotSetDirection(int direction);

private:
    static const int FieldWidth = 50;
    static const int FieldHeight = 30;

    int number{0};

    QTcpSocket *socket;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
