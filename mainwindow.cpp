#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(QFile::exists("data.bin"))
        init();
    else
        readData();
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

void MainWindow::readData()
{

}

void MainWindow::saveData()
{

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
        ui->taskList->setHidden(true);
        ui->btn_addItem->setEnabled(false);
        ui->btn_deleteItem->setEnabled(false);
    }
    else
    {
        listVisible=true;
        ui->btn_listVis->setText(tr("隐藏"));
        ui->taskList->setHidden(false);
        ui->btn_addItem->setEnabled(true);
        ui->btn_deleteItem->setEnabled(true);
    }
}


void MainWindow::on_btn_addItem_clicked()
{
    QListWidgetItem *aItem= new QListWidgetItem(tr("新项"));
    aItem->setCheckState(Qt::Unchecked);
    aItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    ui->taskList->addItem(aItem);
    taskListItems<<aItem;
}


void MainWindow::on_btn_deleteItem_clicked()
{
    int row= ui->taskList->currentRow();
    QListWidgetItem *aItem=ui->taskList->takeItem(row);
    delete aItem;
    taskListItems.removeAt(row);
}

void MainWindow::on_taskList_itemChanged(QListWidgetItem *item)
{
    QFont font = item->font();
    if(item->checkState()==Qt::Checked)
    {
        font.setStrikeOut(true);
        item->setForeground(Qt::gray);
    }
    else
    {
        font.setStrikeOut(false);
        item->setForeground(Qt::black);
    }
    item->setFont(font);
}

