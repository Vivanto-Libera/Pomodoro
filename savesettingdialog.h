#ifndef SAVESETTINGDIALOG_H
#define SAVESETTINGDIALOG_H

#include <QDialog>
#include <mainwindow.h>

namespace Ui {
class SaveSettingDialog;
}

class SaveSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveSettingDialog(QWidget *parent = nullptr);
    ~SaveSettingDialog();
    struct savedSetting
    {
        QString title;
        MainWindow::pomoSetting setting;
    };
    MainWindow::pomoSetting getSetting();

private slots:
    void on_btn_changeName_clicked();

    void on_btn_add_clicked();

    void on_btn_delete_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_spin_focusTime_valueChanged(int arg1);

    void on_spin_ShortRelax_valueChanged(int arg1);

    void on_spin_longRelax_valueChanged(int arg1);

    void on_spin_repeatTimes_valueChanged(int arg1);

private:
    Ui::SaveSettingDialog *ui;
    QList<savedSetting> settings;
    void init();
    void setValues(MainWindow::pomoSetting aSetting);
    void readData();
    void saveData();
    void setData();
    void valueChanged();
};

#endif // SAVESETTINGDIALOG_H
