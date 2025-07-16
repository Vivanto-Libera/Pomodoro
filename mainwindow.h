#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTimer>
#include <QTime>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void init();
    bool listVisible;
    QTimer *aTimer;
private slots:
    void setCurTime();
    void on_btn_listVis_clicked();
    void on_btn_addItem_clicked();
    void on_btn_deleteItem_clicked();
};
#endif // MAINWINDOW_H
