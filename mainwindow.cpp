#include "mainwindow.h"
#include "ui_mainwindow.h"

QDataStream &operator<<(QDataStream &out,const MainWindow::aTaskItem &aItem)
{
    out<<aItem.aItem<<aItem.checked;
    return out;
}
QDataStream &operator>>(QDataStream &in,MainWindow::aTaskItem &aItem)
{
    in>>aItem.aItem>>aItem.checked;
    return in;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(!QFile::exists("items.bin"))
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
    saveData();
    delete ui;
}

void MainWindow::init()
{
    listVisible=true;
}

void MainWindow::readData()
{
    readItems();
    setItems();
}

void MainWindow::readItems()
{
    QFile aFile("items.bin");
    if(!aFile.open(QIODevice::ReadOnly))
        return;
    QDataStream fileStream(&aFile);
    fileStream.setVersion(QDataStream::Qt_6_9);
    fileStream.setByteOrder(QDataStream::BigEndian);
    while(!fileStream.atEnd())
    {
        aTaskItem aItem;
        fileStream>>aItem;
        taskListItems<<aItem;
    }
    aFile.close();
}

void MainWindow::saveData()
{
    saveItems();
}

void MainWindow::saveItems()
{
    QFile aFile("items.bin");
    if(!aFile.open(QIODevice::WriteOnly))
        return;
    QDataStream fileStream(&aFile);
    fileStream.setVersion(QDataStream::Qt_6_9);
    fileStream.setByteOrder(QDataStream::BigEndian);
    for(int i=0;i<taskListItems.count();i++)
    {
        fileStream<<taskListItems.at(i);
    }
    aFile.close();
}

void MainWindow::setItems()
{
    for(int i=0;i<taskListItems.count();i++)
    {
        QListWidgetItem *aItem= new QListWidgetItem;
        *aItem=taskListItems.at(i).aItem;
        aItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        if(taskListItems.at(i).checked)
            aItem->setCheckState(Qt::Checked);
        ui->taskList->addItem(aItem);
    }
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
    aTaskItem aTItem={*aItem,false};
    taskListItems<<aTItem;
}


void MainWindow::on_btn_deleteItem_clicked()
{
    int row= ui->taskList->currentRow();
    if(row==-1)
        return;
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
        taskListItems.replace(ui->taskList->row(item),{*item,true});
    }
    else
    {
        font.setStrikeOut(false);
        item->setForeground(Qt::black);
        taskListItems.replace(ui->taskList->row(item),{*item,false});
    }
    item->setFont(font);
}


void MainWindow::on_btn_note_clicked()
{
    NoteWindow *noteWindow= new NoteWindow(this);
    noteWindow->setAttribute(Qt::WA_DeleteOnClose);
    noteWindow->show();
}

