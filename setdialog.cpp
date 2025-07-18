#include "setdialog.h"
#include "ui_setdialog.h"

SetDialog::SetDialog(QWidget *parent,MainWindow::pomoSetting aSetting)
    : QDialog(parent),ui(new Ui::SetDialog)
{
    ui->setupUi(this);
    ui->spin_focusTime->setValue(aSetting.focusTime);
    ui->spin_ShortRelax->setValue(aSetting.shortBreak);
    ui->spin_longRelax->setValue(aSetting.longBreak);
    ui->spin_repeatTimes->setValue(aSetting.repeat);
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

SetDialog::~SetDialog()
{
    delete ui;
}
