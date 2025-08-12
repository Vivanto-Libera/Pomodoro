# Pomodoro

## 写在前面
本软件由一名大一学生参与校内比赛的参赛作品，由本人独立制作，做的可能不是很好，如有建议和BUG，欢迎指出。本人GitHub账号：[Vivanto-Libera][https://github.com/Vivanto-Libera]；本人邮箱：1425078256@qq.com；GitHub仓库地址：https://github.com/Vivanto-Libera/Pomodoro。

本软件的音乐播放器没有加入歌曲，大家可以自己加入喜欢的歌曲或者听力素材。

## 素材版权声明
图标素材来源：
https://www.iconfinder.com/search/icons?family=phosphor-bold

https://www.iconfinder.com/search/icons?family=fluent-solid-20px

https://www.iconfinder.com/search/icons?family=fluent-regular-24px

https://dev.mysql.com/downloads/installer/?use_xbridge3=true&loader_name=forest&need_sec_link=1&sec_link_scene=im&theme=light

## 需求分析

番茄钟是一种时间管理方法，由弗朗西斯科·西里洛在1980年代创立，核心是通过拆分时间提高专注度。
该方法将工作拆分为多个“番茄时间”，每个番茄时间设定为25分钟，期间专注工作，不做任何与任务无关的事；完成1个番茄时间后，休息5分钟；每完成4个番茄时间，进行一次较长休息（通常15-30分钟）。

### Pomodoro软件的基本功能

- 友好的人机交互界面

- 基本的番茄钟功能

- 待办事项功能

- 笔记功能

- 音频播放器
## 总体设计

本项目由Qt 6.9.1 minGW 13.1.0 64-bit, Qt Creator 17.0.0和C++开发，build system为qmake。

整个界面以红色为主色调进行设计，用了Qt样式表对一些组件的外观进行了设计。

实现了基本的番茄钟功能，同时可以自定义番茄钟的设置，让用户可以根据自己的实际情况进行调整。同时可以储存番茄钟的设置，使其可以根据不同情况使用不同的设置。

待办事项用了listwidget进行实现。

笔记功能包括了基本的文字编辑功能，同时可以储存多个笔记。

音乐播放器可以创建多个歌单，可以导入音频。

可以显示当前时间和输入座右铭。

## 详细设计

### 前端设计

设计了五个界面，五个类。分别为mainwindow（主界面）, notewindow（笔记界面）， musiclistsdialog（歌单界面）， setdialog（番茄钟设置）， savesettingdialog（番茄钟设置储存）
<div align="center">
<img width="1000" height="766" alt="image" src="https://github.com/user-attachments/assets/dbbb9ee1-aef6-412a-9891-169663555d71" />
<br>
<center><b><font size ='2'>mainwindow</font></font></b></center>
<br><br>
<img width="607" height="431" alt="image" src="https://github.com/user-attachments/assets/302d7254-e996-429b-b7ab-8f9ff4faf16b" />
<br>
<center><b><font size ='2'>notewindow</font></font></b></center>
<br><br>
<img width="296" height="333" alt="image" src="https://github.com/user-attachments/assets/e7ce94ce-f322-4152-89e5-65c0a925538f" />
<br>
<center><b><font size ='2'>musiclistsdialog</font></font></b></center>
<br><br>
<img width="251" height="244" alt="image" src="https://github.com/user-attachments/assets/d16da071-f0fe-4d46-8c82-337ac65aba9f" />
<br>
<center><b><font size ='2'>setdialog</font></font></b></center>
<br><br>
<img width="275" height="216" alt="image" src="https://github.com/user-attachments/assets/1db23845-d82a-4df3-b642-cb3e8c403c35" />
<br>
<center><b><font size ='2'>savesettingdialog</font></font></b></center>
</div>
<br>
    
### 番茄钟模块

定义了一个结构pomoSetting用于表示番茄钟的设置。
```c++
struct pomoSetting
    {
        qint8 focusTime;
        qint8 shortBreak;
        qint8 longBreak;
        qint8 repeat;
    };
```
定义了枚举pomoStatus来表示番茄钟的状态
```c++
enum pomoStatus{Focus,Relax,Pause,NoStart};
```

定义了两个QTimer pomoTimer（用于进行番茄钟倒计时）和flushTimer（用于刷新主界面上的倒计时显示）。当pomoTimer倒计时结束时发送信号触发槽函数do_pomoTimer_timeOut()。
```c++
void MainWindow::do_pomoTimer_timeOut()
{
    QApplication::beep();
    if (curStatus == pomoStatus::Focus)
    {
        setStatus(pomoStatus::Relax);
        pomoRepeats++;
        if (pomoRepeats != curPomoSetting.repeat)
        {
            pomoTimer->setInterval(curPomoSetting.shortBreak * 60000);
        }
        else
        {
            pomoTimer->setInterval(curPomoSetting.longBreak * 60000);
        }
        pomoTimer->start();
        return;
    }
    else
    {
        if (pomoRepeats == curPomoSetting.repeat)
        {
            resetPomo();
            return;
        }
        else
        {
            setStatus(pomoStatus::Focus);
            pomoTimer->setInterval(curPomoSetting.focusTime * 60000);
            pomoTimer->start();
            return;
        }
    }
}
```

并且进行状态改变：
```c++
void MainWindow::setStatus(pomoStatus newStatus)
{
    curStatus=newStatus;
    switch (newStatus) {
    case pomoStatus::Focus :
        ui->lab_pomoStatus->setText(tr("专注中"));
        break;
    case pomoStatus::Relax :
        ui->lab_pomoStatus->setText(tr("休息中"));
        break;
    case pomoStatus::Pause :
        ui->lab_pomoStatus->setText(tr("暂停中"));
        break;
    case pomoStatus::NoStart :
        ui->lab_pomoStatus->setText(tr("未开始"));
        break;
    }
}
```

flushTimer每0.5s发送一次信号刷新倒计时显示：
```c++
void MainWindow::setTimeLab()
{
    qint8 minu = pomoTimer->remainingTime() / 60000;
    qint8 sec = (pomoTimer->remainingTime() / 1000) % 60;
    QString str = QString::asprintf("%1").arg(minu, 2, 10, QChar('0')) + ":"
                  + QString::asprintf("%1").arg(sec, 2, 10, QChar('0'));
    ui->lab_pomoTime->setText(str);
}
```

当用户按下停止/开始按钮时触发函数：
```c++
void MainWindow::on_btn_startOrPause_clicked()
{
    switch (curStatus) {
    case pomoStatus::NoStart :
        startPomo();
        return;
    case pomoStatus::Focus :    //Focus和Relax相同处理
    case pomoStatus::Relax :
        pausePomo();
        return;
    case pomoStatus::Pause :
        resumePomo();
        return;
    }
}
```

其中startPomo：
```c++
void MainWindow::startPomo()
{
    QIcon aIcon(":/icons/images/ongoing.png");
    ui->btn_startOrPause->setIcon(aIcon);
    ui->btn_startOrPause->setToolTip(tr("暂停"));
    setStatus(pomoStatus::Focus);
    pomoTimer->setInterval(curPomoSetting.focusTime * 60000);
    flushTimer->start();
    pomoTimer->start();
}
```

pausePomo：
```c++
void MainWindow::pausePomo()
{
    QIcon aIcon(":/icons/images/pomodoro.png");
    ui->btn_startOrPause->setIcon(aIcon);
    ui->btn_startOrPause->setToolTip(tr("继续"));
    lastStatus = curStatus;
    setStatus(pomoStatus::Pause);
    int remainTime = pomoTimer->remainingTime();
    pomoTimer->stop();
    pomoTimer->setInterval(remainTime);
    flushTimer->stop();
}
```

resumePomo：
```c++
void MainWindow::resumePomo()
{
    QIcon aIcon(":/icons/images/ongoing.png");
    ui->btn_startOrPause->setIcon(aIcon);
    ui->btn_startOrPause->setToolTip(tr("暂停"));
    setStatus(lastStatus);
    flushTimer->start();
    pomoTimer->start();
}
```

当用户按下重置番茄钟按钮时或番茄钟结束时触发resetPomo函数：
```c++
void MainWindow::resetPomo()
{
    QIcon aIcon(":/icons/images/pomodoro.png");
    ui->btn_startOrPause->setIcon(aIcon);
    ui->btn_startOrPause->setToolTip(tr("开始"));
    setStatus(pomoStatus::NoStart);
    flushTimer->stop();
    pomoTimer->stop();
    pomoRepeats = 0;
    ui->lab_pomoTime->setText(QString::asprintf("%1")
                                  .arg(curPomoSetting.focusTime, 2, 10, QChar('0')) + ":00");
}
```

当用户按下番茄钟设置按钮时以模态的方式显示setdialog：
```c++
void MainWindow::on_btn_setting_clicked()
{
    SetDialog *aDialog = new SetDialog(this);
    aDialog->ini(curPomoSetting);
    int ret = aDialog->exec();
    if (ret == QDialog::Accepted)
    {
        curPomoSetting=aDialog->setting();
        resetPomo();
    }
    delete aDialog;
}
```

如果用户按下“确认"时SetDialog返回新的设置。

当用户在setdialog中按下存储设置时也以模态的方式调用savesettingdialog，并返回选择的设置。
```c++
void SetDialog::on_btn_save_clicked()
{
    SaveSettingDialog *aDialog = new SaveSettingDialog(this);
    int ret = aDialog->exec();
    if (ret == QDialog::Accepted)
    {
        ini(aDialog->getSetting());
    }
    delete aDialog;
}
```
这两个dialog的具体实现我就不赘述了，如果感兴趣可以看源代码。

当用户试图在番茄钟启动的情况下退出软件时将会弹出弹窗询问是否退出。
```c++
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (curStatus == pomoStatus::Focus || curStatus == pomoStatus::Relax)
    {
        QMessageBox::StandardButton res = QMessageBox::question(this, tr("退出程序"),
                                    tr("正在使用番茄钟，确定要退出吗？"), QMessageBox::Yes | QMessageBox::No);
        if (res == QMessageBox::Yes)
        {
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
    else
    {
        event->accept();
    }
}
```

### 待办事项

为了方便保存列表，我创建了一个结构aTaskItem用于储存单个项。
```c++
struct aTaskItem
    {
        QListWidgetItem aItem;
        bool checked;
    };
```

按下按钮隐藏/显示时，执行函数：
```c++
void MainWindow::on_btn_listVis_clicked()
{
    if (listVisible)
    {
        listVisible = false;
        ui->btn_listVis->setText(tr("显示"));
        ui->taskList->setHidden(true);
        ui->btn_addItem->setEnabled(false);
        ui->btn_deleteItem->setEnabled(false);
    }
    else
    {
        listVisible = true;
        ui->btn_listVis->setText(tr("隐藏"));
        ui->taskList->setHidden(false);
        ui->btn_addItem->setEnabled(true);
        ui->btn_deleteItem->setEnabled(true);
    }
}
```

添加和删除项：
```c++
void MainWindow::on_btn_addItem_clicked()
{
    QListWidgetItem *aItem = new QListWidgetItem(tr("新项"));
    aItem->setCheckState(Qt::Unchecked);
    aItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable
                    | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    ui->taskList->addItem(aItem);
    aTaskItem aTItem = {*aItem, false};
    taskListItems << aTItem;
}

void MainWindow::on_btn_deleteItem_clicked()
{
    int row = ui->taskList->currentRow();
    if (row == -1)
    {
        return;
    }
    QListWidgetItem *aItem = ui->taskList->takeItem(row);
    delete aItem;
    taskListItems.removeAt(row);
}
```

当用户勾选了项的复选框会有划掉的效果，函数：
```c++
void MainWindow::on_taskList_itemChanged(QListWidgetItem *item)
{
    QFont font = item->font();
    if (item->checkState() == Qt::Checked)
    {
        font.setStrikeOut(true);
        item->setForeground(Qt::gray);
        taskListItems.replace(ui->taskList->row(item), {*item,true});
    }
    else
    {
        font.setStrikeOut(false);
        item->setForeground(Qt::black);
        taskListItems.replace(ui->taskList->row(item), {*item,false});
    }
    item->setFont(font);
}
```

### 笔记模块

用户可以通过组合框选择要编辑哪一份笔记。这个模块的代码大都很简单，不再赘述。

### 音乐模块

播放器将读取musiclistsdialog中歌单的歌曲进行播放。
一些关于设置主界面标签文本的函数：

```c++
void MainWindow::do_sourceChanged(const QUrl &media)
{
    ui->lab_musicName->setText(media.fileName());
    if (media.isEmpty())
    {
        durationTime = QString("00:00");
        ui->lab_musicTime->setText(positionTime + "/" + durationTime);
    }
}

void MainWindow::do_durationChanged(qint64 duration)
{
    ui->slider_position->setMaximum(duration);
    int secs = duration / 1000;
    int mins = secs / 60;
    secs = secs % 60;
    durationTime = QString::asprintf("%1:%2").arg(mins, 2, 10, QChar('0')).arg(secs, 2, 10, QChar('0'));
    ui->lab_musicTime->setText(positionTime + "/" + durationTime);
}

void MainWindow::do_positionChanged(qint64 position)
{
    if (ui->slider_position->isSliderDown())
    {
        return;
    }
    ui->slider_position->setSliderPosition(position);
    int secs = position/1000;
    int mins = secs/60;
    secs = secs % 60;
    positionTime = QString::asprintf("%1:%2").arg(mins, 2, 10, QChar('0')).arg(secs, 2, 10, QChar('0'));
    ui->lab_musicTime->setText(positionTime + "/" + durationTime);
}
```

下一首：
```c++
void MainWindow::nextMusic()
{
    //列表循环
    if(ui->comboOrder->currentIndex() == 0)
    {
        playing=false;
        if(!musicDialog->getListWidget()->count())
        {
            return;
        }
        musicIndex++;
        musicIndex=musicIndex >= musicDialog->getListWidget()->count() ? 0 : musicIndex;
        player->setSource(getUrlFromItem(musicDialog->getListWidget()->item(musicIndex)));
        player->play();
        playing=true;
        return;
    }

    //单曲循环
    if (ui->comboOrder->currentIndex() == 1)
    {
        player->play();
        return;
    }

    //随机播放
    if (ui->comboOrder->currentIndex() == 2)
    {
        if (!musicDialog->getListWidget()->count())
        {
            return;
        }
        musicIndex = QRandomGenerator::global()->bounded(0, musicDialog->getListWidget()->count());
        player->setSource(getUrlFromItem(musicDialog->getListWidget()->item(musicIndex)));
        player->play();
        return;
    }
}
```

上一首：
```c++
void MainWindow::on_btn_previous_clicked()
{
    if (!musicDialog->getListWidget()->count())
    {
        return;
    }
    musicIndex--;
    musicIndex = musicIndex < 0 ? (musicDialog->getListWidget()->count() - 1) : musicIndex;
    playing = false;
    player->setSource(getUrlFromItem(musicDialog->getListWidget()->item(musicIndex)));
    player->play();
    playing = true;
}
```

歌曲状态变化的槽函数：
```c++
void MainWindow::do_stateChanged(QMediaPlayer::PlaybackState state)
{
    if ((state == QMediaPlayer::PausedState) || (!playing && (state == QMediaPlayer::StoppedState)))
    {
        ui->btn_play->setIcon(QIcon(":/icons/images/play.png"));
        ui->btn_play->setToolTip(tr("播放"));
    }
    else if (state == QMediaPlayer::PlayingState)
    {
        ui->btn_play->setIcon(QIcon(":/icons/images/pause.png"));
        ui->btn_play->setToolTip(tr("暂停"));
    }
    else if (playing && (state == QMediaPlayer::StoppedState))
    {
        nextMusic();
    }
}
```

