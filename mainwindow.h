#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTimer>
#include <QTime>
#include <QListWidgetItem>
#include <QList>

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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer *aTimer;
    QList<aTaskItem> taskListItems;

    void init();
    void readData();
    void readItems();
    void saveData();
    void saveItems();
    void setItems();
    bool listVisible;
private slots:
    void setCurTime();
    void on_btn_listVis_clicked();
    void on_btn_addItem_clicked();
    void on_btn_deleteItem_clicked();
    void on_taskList_itemChanged(QListWidgetItem *item);
};
#endif // MAINWINDOW_H
