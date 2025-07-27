#include "setdialog.h"
#include "ui_setdialog.h"
#include <savesettingdialog.h>

SetDialog::SetDialog(QWidget *parent)
    : QDialog(parent),ui(new Ui::SetDialog)
{
    ui->setupUi(this);
}

MainWindow::pomoSetting SetDialog::setting()
{
    MainWindow::pomoSetting newSetting;
    newSetting.focusTime=ui->spin_focusTime->value();
    newSetting.shortBreak=ui->spin_ShortRelax->value();
    newSetting.longBreak=ui->spin_longRelax->value();
    newSetting.repeat=ui->spin_repeatTimes->value();
    return newSetting;
}

void SetDialog::ini(MainWindow::pomoSetting aSetting)
{
    ui->spin_focusTime->setValue(aSetting.focusTime);
    ui->spin_ShortRelax->setValue(aSetting.shortBreak);
    ui->spin_longRelax->setValue(aSetting.longBreak);
    ui->spin_repeatTimes->setValue(aSetting.repeat);
}

SetDialog::~SetDialog()
{
    delete ui;
}

void SetDialog::on_btn_reset_clicked()
{
    ui->spin_focusTime->setValue(25);
    ui->spin_ShortRelax->setValue(5);
    ui->spin_longRelax->setValue(20);
    ui->spin_repeatTimes->setValue(4);
}


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

