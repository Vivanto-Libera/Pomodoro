#include "musiclistsdialog.h"
#include "ui_musiclistsdialog.h"
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>

QDataStream &operator<<(QDataStream &out,const MusicListsDialog::musicList aList)
{
    out<<aList.title<<aList.music;
    return out;
}
QDataStream &operator>>(QDataStream &in,MusicListsDialog::musicList aList)
{
    in>>aList.title>>aList.music;
    return in;
}

MusicListsDialog::MusicListsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MusicListsDialog)
{
    ui->setupUi(this);
}

MusicListsDialog::~MusicListsDialog()
{
    saveMusic();
    delete ui;
}

void MusicListsDialog::init()
{
    musicList aList;
    aList.title=tr("初始歌单");
    musicLists<<aList;
    setMusic();
}

void MusicListsDialog::readMusic()
{
    QFile aFile("music.bin");
    if(!aFile.open(QIODevice::ReadOnly))
        return;
    QDataStream fileStream(&aFile);
    fileStream.setVersion(QDataStream::Qt_6_9);
    fileStream.setByteOrder(QDataStream::BigEndian);
    while(!fileStream.atEnd())
    {
        musicList aList;
        fileStream>>aList;
        musicLists<<aList;
    }
    aFile.close();
}

void MusicListsDialog::setMusic()
{
    ui->comboBox->addItem(musicLists.at(0).title);
}

void MusicListsDialog::saveMusic()
{
    QFile aFile("music.bin");
    if(!aFile.open(QIODevice::WriteOnly))
        return;
    QDataStream fileStream(&aFile);
    fileStream.setVersion(QDataStream::Qt_6_9);
    fileStream.setByteOrder(QDataStream::BigEndian);
    for(int i=0;i<musicLists.count();i++)
    {
        fileStream<<musicLists.at(i);
    }
    aFile.close();
}

void MusicListsDialog::on_comboBox_currentIndexChanged(int index)
{
    if(index==-1)
        return;
    ui->listWidget->clear();
    for(int i=0;i<musicLists.at(index).music.count();i++)
    {
        QListWidgetItem *aItem=new QListWidgetItem;
        *aItem=musicLists.at(index).music.at(i);
        ui->listWidget->addItem(aItem);
    }
}


void MusicListsDialog::on_btn_add_clicked()
{
    musicList aList;
    aList.title=tr("未命名歌单");
    musicLists<<aList;
    ui->comboBox->addItem(musicLists.at(musicLists.count()-1).title);
}


void MusicListsDialog::on_btn_delete_clicked()
{
    if(ui->comboBox->count()!=1)
    {
        QMessageBox::StandardButton result;
        result=QMessageBox::question(this,tr("确认删除"),tr("是否删除该歌单？"));
        if(result!=QMessageBox::Yes)
            return;
    }
    else
    {
        QMessageBox::critical(this,tr("删除失败"),tr("无法删除最后一个歌单"));
        return;
    }
    musicLists.removeAt(ui->comboBox->currentIndex());
    ui->comboBox->removeItem(ui->comboBox->currentIndex());
}


void MusicListsDialog::on_btn_changeName_clicked()
{
    bool ok=false;
    QString str= QInputDialog::getText(this,tr("修改歌单名称"),tr("请输入新名称"),QLineEdit::Normal,ui->comboBox->currentText(),&ok);
    if(ok && !str.isEmpty())
    {
        ui->comboBox->setItemText(ui->comboBox->currentIndex(),str);
        musicLists.replace(ui->comboBox->currentIndex(),{str,musicLists.at(ui->comboBox->currentIndex()).music});
    }
}


void MusicListsDialog::on_btn_addMusic_clicked()
{
    QString curPath= QDir::homePath();
    QString dlgTitle= tr("选择音频文件");
    QString filter=tr("音频文件(*.mp3 *.wav *.wma)");
    QStringList fileList= QFileDialog::getOpenFileNames(this,dlgTitle,curPath,filter);
    if(fileList.count()<1)
        return;
    for(int i=0;i<fileList.count();i++)
    {
        QString aFile=fileList.at(i);
        QFileInfo fileInfo(aFile);
        QListWidgetItem *aItem=new QListWidgetItem(fileInfo.fileName());
        aItem->setIcon(QIcon(":/icons/images/music.png"));
        aItem->setData(Qt::UserRole,QUrl::fromLocalFile(aFile));
        ui->listWidget->addItem(aItem);
        musicLists.value(ui->comboBox->currentIndex()).music.push_back(*aItem);
    }
}


void MusicListsDialog::on_btn_deleteMusic_clicked()
{
    int index=ui->listWidget->currentRow();
    if(index==-1)
        return;
    QListWidgetItem *aItem=ui->listWidget->takeItem(index);
    delete aItem;
    musicLists.value(ui->comboBox->currentIndex()).music.removeAt(index);

}

