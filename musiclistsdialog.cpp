#include "musiclistsdialog.h"
#include "ui_musiclistsdialog.h"
#include <QFile>

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

