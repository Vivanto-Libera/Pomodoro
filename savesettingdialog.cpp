#include "savesettingdialog.h"
#include "ui_savesettingdialog.h"

QDataStream &operator<<(QDataStream &out,const SaveSettingDialog::savedSetting &aSetting)
{
    out << aSetting.title << aSetting.setting.focusTime << aSetting.setting.longBreak
        << aSetting.setting.shortBreak << aSetting.setting.repeat;
    return out;
}
QDataStream &operator>>(QDataStream &in,SaveSettingDialog::savedSetting &aSetting)
{
    in >> aSetting.title >> aSetting.setting.focusTime >> aSetting.setting.longBreak
       >> aSetting.setting.shortBreak >> aSetting.setting.repeat;
    return in;
}

SaveSettingDialog::SaveSettingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SaveSettingDialog)
{
    ui->setupUi(this);
    if (!QFile::exists("settings.bin"))
    {
        init();
    }
    else
    {
        readData();
        setData();
    }
}

SaveSettingDialog::~SaveSettingDialog()
{
    saveData();
    delete ui;
}

MainWindow::pomoSetting SaveSettingDialog::getSetting()
{
    return settings.at(ui->comboBox->currentIndex()).setting;
}

void SaveSettingDialog::init()
{
    savedSetting aSetting = {tr("默认设置"), {25, 5, 20, 4}};
    settings<<aSetting;
    setValues(settings.at(0).setting);
    ui->comboBox->addItem(settings.at(0).title);
}

void SaveSettingDialog::setValues(MainWindow::pomoSetting aSetting)
{
    ui->spin_focusTime->setValue(aSetting.focusTime);
    ui->spin_ShortRelax->setValue(aSetting.shortBreak);
    ui->spin_longRelax->setValue(aSetting.longBreak);
    ui->spin_repeatTimes->setValue(aSetting.repeat);
}

void SaveSettingDialog::readData()
{
    QFile aFile("settings.bin");
    if (!aFile.open(QIODevice::ReadOnly))
    {
        return;
    }
    QDataStream fileStream(&aFile);
    fileStream.setVersion(QDataStream::Qt_6_9);
    fileStream.setByteOrder(QDataStream::BigEndian);
    while (!fileStream.atEnd())
    {
        savedSetting aSetting;
        fileStream >> aSetting;
        settings << aSetting;
    }
    aFile.close();
}

void SaveSettingDialog::saveData()
{
    QFile aFile("settings.bin");
    if (!aFile.open(QIODevice::WriteOnly))
    {
        return;
    }
    QDataStream fileStream(&aFile);
    fileStream.setVersion(QDataStream::Qt_6_9);
    fileStream.setByteOrder(QDataStream::BigEndian);
    for (int i = 0;i < settings.count();i++)
    {
        fileStream << settings.at(i);
    }
    aFile.close();
}

void SaveSettingDialog::setData()
{
    if (settings.count() == 0)
    {
        return;
    }
    setValues(settings.at(0).setting);
    for(int i = 0;i < settings.count();i++)
    {
        ui->comboBox->addItem(settings.at(i).title);
    }
}

void SaveSettingDialog::on_btn_changeName_clicked()
{
    bool ok = false;
    QString str = QInputDialog::getText(this,tr("修改设置名称"),tr("请输入新名称"),QLineEdit::Normal,ui->comboBox->currentText(),&ok);
    if (ok && !str.isEmpty())
    {
        ui->comboBox->setItemText(ui->comboBox->currentIndex(), str);
        settings.replace(ui->comboBox->currentIndex(),
                         {str, settings.at(ui->comboBox->currentIndex()).setting});
    }
}


void SaveSettingDialog::on_btn_add_clicked()
{
    ui->comboBox->addItem(tr("未命名设置"));
    savedSetting aSetting = {tr("未命名设置"), {1, 1, 1, 1}};
    settings<<aSetting;
    ui->comboBox->setCurrentIndex(ui->comboBox->count() - 1);
}

void SaveSettingDialog::on_btn_delete_clicked()
{
    if (ui->comboBox->count() != 1)
    {
        QMessageBox::StandardButton result;
        result = QMessageBox::question(this, tr("确认删除"), tr("是否删除该设置？"));
        if(result != QMessageBox::Yes)
        {
            return;
        }
    }
    else
    {
        QMessageBox::critical(this, tr("删除失败"), tr("无法删除最后一个设置"));
        return;
    }
    settings.removeAt(ui->comboBox->currentIndex());
    ui->comboBox->removeItem(ui->comboBox->currentIndex());
}

void SaveSettingDialog::on_comboBox_currentIndexChanged(int index)
{
    if (index == -1)
    {
        return;
    }
    setValues(settings.at(index).setting);
}

void SaveSettingDialog::on_spin_focusTime_valueChanged(int arg1)
{
    if (ui->comboBox->currentIndex() == -1)
    {
        return;
    }
    savedSetting aSetting = settings.at(ui->comboBox->currentIndex());
    aSetting.setting.focusTime = arg1;
    settings.replace(ui->comboBox->currentIndex(), aSetting);
}

void SaveSettingDialog::on_spin_ShortRelax_valueChanged(int arg1)
{
    if (ui->comboBox->currentIndex() == -1)
    {
        return;
    }
    savedSetting aSetting = settings.at(ui->comboBox->currentIndex());
    aSetting.setting.shortBreak = arg1;
    settings.replace(ui->comboBox->currentIndex(), aSetting);
}

void SaveSettingDialog::on_spin_longRelax_valueChanged(int arg1)
{
    if (ui->comboBox->currentIndex() == -1)
    {
        return;
    }
    savedSetting aSetting = settings.at(ui->comboBox->currentIndex());
    aSetting.setting.longBreak = arg1;
    settings.replace(ui->comboBox->currentIndex(), aSetting);
}

void SaveSettingDialog::on_spin_repeatTimes_valueChanged(int arg1)
{
    if (ui->comboBox->currentIndex() == -1)
    {
        return;
    }
    savedSetting aSetting = settings.at(ui->comboBox->currentIndex());
    aSetting.setting.repeat = arg1;
    settings.replace(ui->comboBox->currentIndex(), aSetting);
}
