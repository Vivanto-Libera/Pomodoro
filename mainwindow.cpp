#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <setdialog.h>

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
    player= new QMediaPlayer(this);
    QAudioOutput *audioOutput= new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    settings= new QSettings("Viavnto","Pomodoro");
    if(!QFile::exists("items.bin"))
        init();
    else
        readData();
    aTimer= new QTimer(this);
    aTimer->setInterval(500);
    aTimer->setSingleShot(false);
    aTimer->start();
    pomoTimer= new QTimer(this);
    pomoTimer->stop();
    pomoTimer->setSingleShot(true);
    flushTimer= new QTimer(this);
    flushTimer->stop();
    flushTimer->setInterval(500);
    flushTimer->setSingleShot(false);
    ui->lab_pomoTime->setText(QString::asprintf("%1").arg(curSetting.focusTime,2,10,QChar('0'))+":00");
    ui->lineEdit->setText(motto);
    noteWindow= new NoteWindow(this);
    musicDialog= new MusicListsDialog(this);
    connect(aTimer,SIGNAL(timeout()),this,SLOT(setCurTime()));
    connect(flushTimer,SIGNAL(timeout()),this,SLOT(setTimeLab()));
    connect(pomoTimer,SIGNAL(timeout()),this,SLOT(do_pomoTimer_timeOut()));

    connect(player,&QMediaPlayer::positionChanged,this,&MainWindow::do_positionChanged);
    connect(player,&QMediaPlayer::durationChanged,this,&MainWindow::do_durationChanged);
    connect(player,&QMediaPlayer::sourceChanged,this,&MainWindow::do_sourceChanged);
    connect(player,&QMediaPlayer::playbackStateChanged,this,&MainWindow::do_stateChanged);
    connect(musicDialog->getListWidget(),&QListWidget::itemDoubleClicked,this,&MainWindow::do_musicItemDoubleClicked);
    connect(musicDialog->getComboBox(),&QComboBox::currentIndexChanged,this,&MainWindow::do_musicListChanged);
    if(musicDialog->getListWidget()->count()>0)
    {
        player->setSource(getUrlFromItem(musicDialog->getListWidget()->item(0)));
        ui->lab_musicName->setText((getUrlFromItem(musicDialog->getListWidget()->item(0))).fileName());
    }
}

MainWindow::~MainWindow()
{
    delete noteWindow;
    delete musicDialog;
    saveData();
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(curStatus==pomoStatus::Focus || curStatus==pomoStatus::Relax)
    {
        QMessageBox::StandardButton res=QMessageBox::question(this,tr("退出程序"),tr("正在使用番茄钟，确定要退出吗？"),QMessageBox::Yes|QMessageBox::No);
        if(res==QMessageBox::Yes)
            event->accept();
        else
            event->ignore();
    }
    else
        event->accept();
}

void MainWindow::init()
{
    listVisible=true;
    curSetting={25,5,20,4};
}

void MainWindow::readData()
{
    readItems();
    setItems();
    readSetting();
}

void MainWindow::readSetting()
{
    settings->beginGroup("pomodoro");
    curSetting.focusTime=settings->value("Focus Time").toInt();
    curSetting.shortBreak=settings->value("Short Break").toInt();
    curSetting.longBreak=settings->value("Long Break").toInt();
    curSetting.repeat=settings->value("Repeats").toInt();
    motto=settings->value("Motto").toString();
    ui->slider_volume->setValue(settings->value("Volume").toInt());
    settings->endGroup();
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
    motto=ui->lineEdit->text();
    saveSetting();
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

void MainWindow::saveSetting()
{
    settings->beginGroup("pomodoro");
    settings->setValue("Focus Time",curSetting.focusTime);
    settings->setValue("Short Break",curSetting.shortBreak);
    settings->setValue("Long Break",curSetting.longBreak);
    settings->setValue("Repeats",curSetting.repeat);
    settings->setValue("Motto",motto);
    settings->setValue("Volume",ui->slider_volume->value());
    settings->endGroup();
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

void MainWindow::setTimeLab()
{
    qint8 minu=pomoTimer->remainingTime()/1000/60;
    qint8 sec=pomoTimer->remainingTime()/1000%60;
    QString str=QString::asprintf("%1").arg(minu,2,10,QChar('0'))+":"+QString::asprintf("%1").arg(sec,2,10,QChar('0'));
    ui->lab_pomoTime->setText(str);
}

void MainWindow::do_pomoTimer_timeOut()
{
    QApplication::beep();
    if(curStatus==pomoStatus::Focus)
    {
        setStatus(pomoStatus::Relax);
        pomoRepeats++;
        if(pomoRepeats!=curSetting.repeat)
        {
            pomoTimer->setInterval(curSetting.shortBreak*60*1000);
        }
        else
        {
            pomoTimer->setInterval(curSetting.longBreak*60*1000);
        }
        pomoTimer->start();
        return;
    }
    else
    {
        if(pomoRepeats==curSetting.repeat)
        {
            resetPomo();
            return;
        }
        else
        {
            setStatus(pomoStatus::Focus);
            pomoTimer->setInterval(curSetting.focusTime*60*1000);
            pomoTimer->start();
            return;
        }
    }
}

void MainWindow::setStatus(pomoStatus newStatus)
{
    curStatus=newStatus;
    switch (newStatus) {
    case pomoStatus::Focus:
        ui->lab_pomoStatus->setText(tr("专注中"));
        break;
    case pomoStatus::Relax:
        ui->lab_pomoStatus->setText(tr("休息中"));
        break;
    case pomoStatus::Pause:
        ui->lab_pomoStatus->setText(tr("暂停中"));
        break;
    case pomoStatus::NoStart:
        ui->lab_pomoStatus->setText(tr("未开始"));
        break;
    }
}

void MainWindow::startPomo()
{
    QIcon aIcon(":/icons/images/ongoing.png");
    ui->btn_startOrPause->setIcon(aIcon);
    ui->btn_startOrPause->setToolTip(tr("暂停"));
    setStatus(pomoStatus::Focus);
    pomoTimer->setInterval(curSetting.focusTime*60*1000);
    flushTimer->start();
    pomoTimer->start();
}

void MainWindow::pausePomo()
{
    QIcon aIcon(":/icons/images/pomodoro.png");
    ui->btn_startOrPause->setIcon(aIcon);
    ui->btn_startOrPause->setToolTip(tr("继续"));
    lastStatus=curStatus;
    setStatus(pomoStatus::Pause);
    int remainTime=pomoTimer->remainingTime();
    pomoTimer->stop();
    pomoTimer->setInterval(remainTime);
    flushTimer->stop();
}

void MainWindow::resumePomo()
{
    QIcon aIcon(":/icons/images/ongoing.png");
    ui->btn_startOrPause->setIcon(aIcon);
    ui->btn_startOrPause->setToolTip(tr("暂停"));
    setStatus(lastStatus);
    flushTimer->start();
    pomoTimer->start();
}

void MainWindow::resetPomo()
{
    QIcon aIcon(":/icons/images/pomodoro.png");
    ui->btn_startOrPause->setIcon(aIcon);
    ui->btn_startOrPause->setToolTip(tr("开始"));
    setStatus(pomoStatus::NoStart);
    flushTimer->stop();
    pomoTimer->stop();
    pomoRepeats=0;
    ui->lab_pomoTime->setText(QString::asprintf("%1").arg(curSetting.focusTime,2,10,QChar('0'))+":00");
}

void MainWindow::nextMusic()
{
    if(ui->comboOrder->currentIndex()==1)
    {
        player->play();
        return;
    }
    if(ui->comboOrder->currentIndex()==2)
    {
        musicIndex=QRandomGenerator::global()->bounded(0,musicDialog->getListWidget()->count());
        player->setSource(getUrlFromItem(musicDialog->getListWidget()->item(musicIndex)));
        player->play();
        return;
    }
    if(ui->comboOrder->currentIndex()==0)
    {
        musicIndex++;
        musicIndex=musicIndex>=musicDialog->getListWidget()->count()?0:musicIndex;
        player->setSource(getUrlFromItem(musicDialog->getListWidget()->item(musicIndex)));
        player->play();
        return;
    }
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
    if(noteWindow->isVisible())
    {
        QMessageBox::critical(this,tr("错误"),tr("笔记已打开"));
        return;
    }
    noteWindow->show();
}


void MainWindow::on_btn_startOrPause_clicked()
{
    switch (curStatus) {
    case pomoStatus::NoStart:
        startPomo();
        return;
    case pomoStatus::Focus:
    case pomoStatus::Relax:
        pausePomo();
        return;
    case pomoStatus::Pause:
        resumePomo();
        return;
    }
}


void MainWindow::on_btn_reset_clicked()
{
    resetPomo();
}


void MainWindow::on_btn_setting_clicked()
{
    SetDialog *aDialog=new SetDialog(this);
    aDialog->ini(curSetting);
    int ret= aDialog->exec();
    if(ret == QDialog::Accepted)
    {
        curSetting=aDialog->setting();
        resetPomo();
    }
    delete aDialog;
}


void MainWindow::on_pushButton_3_clicked()
{
    if(musicDialog->isVisible())
    {
        QMessageBox::critical(this,tr("错误"),tr("歌单已打开"));
        return;
    }
    musicDialog->show();
}

void MainWindow::do_stateChanged(QMediaPlayer::PlaybackState state)
{
    if((state == QMediaPlayer::PausedState)||(!playing&&(state == QMediaPlayer::StoppedState)))
    {
        ui->btn_play->setIcon(QIcon(":/icons/images/play.png"));
        ui->btn_play->setToolTip(tr("播放"));
    }
    else if(state == QMediaPlayer::PlayingState)
    {
        ui->btn_play->setIcon(QIcon(":/icons/images/pause.png"));
        ui->btn_play->setToolTip(tr("暂停"));
    }
    if(playing && (state == QMediaPlayer::StoppedState))
    {
        nextMusic();
    }
}

void MainWindow::do_sourceChanged(const QUrl &media)
{
    ui->lab_musicName->setText(media.fileName());
    if(media.isEmpty())
    {
        durationTime=QString("00:00");
        ui->lab_musicTime->setText(positionTime+"/"+durationTime);
    }
}

void MainWindow::do_durationChanged(qint64 duration)
{
    ui->slider_position->setMaximum(duration);
    int secs= duration/1000;
    int mins= secs/60;
    secs=secs%60;
    durationTime=QString::asprintf("%1:%2").arg(mins,2,10,QChar('0')).arg(secs,2,10,QChar('0'));
    ui->lab_musicTime->setText(positionTime+"/"+durationTime);
}

void MainWindow::do_positionChanged(qint64 position)
{
    if(ui->slider_position->isSliderDown())
        return;
    ui->slider_position->setSliderPosition(position);
    int secs= position/1000;
    int mins= secs/60;
    secs=secs%60;
    positionTime=QString::asprintf("%1:%2").arg(mins,2,10,QChar('0')).arg(secs,2,10,QChar('0'));
    ui->lab_musicTime->setText(positionTime+"/"+durationTime);
}

void MainWindow::do_musicItemDoubleClicked(QListWidgetItem *aItem)
{
    playing=false;
    player->setSource(getUrlFromItem(aItem));
    player->play();
    playing=true;
    musicIndex=musicDialog->getListWidget()->row(aItem);
}

void MainWindow::do_musicListChanged(int index)
{
    musicIndex=0;
    playing=false;
    player->stop();
    if(musicDialog->getListWidget()->count()==0)
        player->setSource(QUrl());
    else
        player->setSource(getUrlFromItem(musicDialog->getListWidget()->item(0)));
}



QUrl MainWindow::getUrlFromItem(QListWidgetItem *item)
{
    return item->data(Qt::UserRole).toUrl();
}


void MainWindow::on_btn_previous_clicked()
{
    int row=musicDialog->getListWidget()->currentRow();
    row--;
    row =row<0?musicDialog->getListWidget()->count()-1:row;
    musicDialog->getListWidget()->setCurrentRow(row);
    playing=false;
    player->setSource(getUrlFromItem(musicDialog->getListWidget()->currentItem()));
    player->play();
    playing=true;
}


void MainWindow::on_btn_next_clicked()
{
    nextMusic();
}


void MainWindow::on_btn_play_clicked()
{
    if(playing==true)
    {
        player->pause();
        playing=false;
    }
    else
    {
        player->play();
        playing=true;
    }
}


void MainWindow::on_comboOrder_currentIndexChanged(int index)
{
    if(index==0)
    {
        ui->btn_previous->setEnabled(true);
        ui->btn_next->setEnabled(true);
    }
    else if(index==1)
    {
        ui->btn_previous->setEnabled(false);
        ui->btn_next->setEnabled(false);
    }
    else if(index==2)
    {
        ui->btn_previous->setEnabled(false);
        ui->btn_next->setEnabled(true);
    }
}


void MainWindow::on_slider_volume_valueChanged(int value)
{
    if(value==0)
    {
        ui->labPic_sound->setPixmap(QPixmap(":/icons/images/mute.png"));
        player->audioOutput()->setVolume(0);
        return;
    }
    ui->labPic_sound->setPixmap(QPixmap(":/icons/images/speaker.png"));
    player->audioOutput()->setVolume(value/100.0);
}


void MainWindow::on_slider_position_valueChanged(int value)
{
    if(ui->slider_position->isSliderDown())
        player->setPosition(value);
}

