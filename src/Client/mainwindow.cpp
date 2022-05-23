#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 自适应窗口大小
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect rect = screen->availableGeometry();
    setFixedSize(rect.width() * 8 / 11, rect.height() * 8 / 11);
    // 自适应调整控件位置
    ui->label_ip->move(this->width() / 4 + 5, this->height() / 4);
    ui->lineEdit->move(ui->label_ip->x() - 5, ui->label_ip->y() + 50);
    ui->label_port->move(ui->label_ip->x() + ui->lineEdit->width() + 50, ui->label_ip->y());
    ui->spinBox->move(ui->label_port->x() - 5, ui->label_port->y() + 50);
    ui->pushButton->move(ui->spinBox->x() + ui->spinBox->width() + 50, ui->spinBox->y() - 5);

    gameClient = new GameClient(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    gameClient->connectToServer(ui->lineEdit->text(), ui->spinBox->value());
}

void MainWindow::initBackGround()
{
    // 原有控件禁用
    ui->lineEdit->setDisabled(true);
    ui->pushButton->setDisabled(true);
    ui->spinBox->setDisabled(true);
    ui->label_ip->setDisabled(true);
    ui->label_port->setDisabled(true);
    // 原有控件不可视
    ui->lineEdit->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->spinBox->setVisible(false);
    ui->label_ip->setVisible(false);
    ui->label_port->setVisible(false);
    emit initBackGroundFinished();
}
