#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gameclient.h"

#include <QMainWindow>
#include <QNetworkInterface>
#include <QScreen>
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

signals:
    void initBackGroundFinished();

private slots:
    void on_pushButton_clicked();
    void initBackGround();

private:
    Ui::MainWindow *ui;
    GameClient *gameClient;
};
#endif // MAINWINDOW_H
