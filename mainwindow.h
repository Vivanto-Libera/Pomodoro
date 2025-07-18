#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTimer>
#include <QTime>
#include <QListWidgetItem>
#include <QList>
#include <notewindow.h>
#include <musiclistsdialog.h>
#include <QSettings>
#include <QtMultimediaWidgets>
#include <QtMultimedia>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    struct aTaskItem
    {
        QListWidgetItem aItem;
        bool checked;
    };
    struct pomoSetting
    {
        qint8 focusTime;
        qint8 shortBreak;
        qint8 longBreak;
        qint8 repeat;
    };

    enum pomoStatus{Focus,Relax,Pause,NoStart};

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    QTimer *aTimer;
    QTimer *pomoTimer;
    QTimer *flushTimer;
    QList<aTaskItem> taskListItems;
    pomoStatus curStatus=NoStart;
    pomoStatus lastStatus;
    qint8 pomoRepeats=0;
    pomoSetting curSetting;
    QSettings *settings;
    QString motto=tr("此处可输入座右铭");
    NoteWindow *noteWindow;

    MusicListsDialog *musicDialog;
    QMediaPlayer *player;
    QString durationTime;
    QString positionTime;
    QUrl getUrlFromItem(QListWidgetItem *Item);

    void init();
    void readData();
    void readSetting();
    void readItems();
    void saveData();
    void saveItems();
    void saveSetting();
    void setItems();
    bool listVisible;
    void setStatus(pomoStatus newStatus);
    void startPomo();
    void pausePomo();
    void resumePomo();
    void resetPomo();
private slots:
    void setCurTime();
    void setTimeLab();
    void do_pomoTimer_timeOut();
    void on_btn_listVis_clicked();
    void on_btn_addItem_clicked();
    void on_btn_deleteItem_clicked();
    void on_taskList_itemChanged(QListWidgetItem *item);
    void on_btn_note_clicked();
    void on_btn_startOrPause_clicked();
    void on_btn_reset_clicked();
    void on_btn_setting_clicked();
    void on_pushButton_3_clicked();
};
#endif // MAINWINDOW_H
