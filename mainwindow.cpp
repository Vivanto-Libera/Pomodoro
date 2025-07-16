#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(QFile::exists("data.bin"))
        init();
    aTimer= new QTimer(this);
    aTimer->setTimerType(Qt::CoarseTimer);
    aTimer->setInterval(500);
    aTimer->setSingleShot(false);
    aTimer->start();
    connect(aTimer,SIGNAL(timeout()),this,SLOT(setCurTime()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    listVisible=true;

}

void MainWindow::setCurTime()
{
    QTime curTime= QTime::currentTime();
    QString str=curTime.toString("hh:mm:ss");
    ui->lab_curTime->setText(str);
}

void MainWindow::on_btn_listVis_clicked()
{
    if(listVisible)
    {
        listVisible=false;
        ui->btn_listVis->setText(tr("显示"));
        ui->listWidget->setVisible(false);
        ui->btn_addItem->setEnabled(false);
        ui->btn_deleteItem->setEnabled(false);
    }
    else
    {
        listVisible=true;
        ui->btn_listVis->setText(tr("隐藏"));
        ui->listWidget->setVisible(true);
        ui->btn_addItem->setEnabled(true);
        ui->btn_deleteItem->setEnabled(true);
    }
}

