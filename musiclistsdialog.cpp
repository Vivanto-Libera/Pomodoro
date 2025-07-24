#include "musiclistsdialog.h"
#include "ui_musiclistsdialog.h"
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QTranslator>
extern QTranslator trans;
MusicListsDialog::MusicListsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MusicListsDialog)
{
    ui->setupUi(this);
    settings= new QSettings("Viavnto","Pomodoro");
    if(!QFile::exists("music.bin"))
    {
        init();
    }
    else
    {
        readMusic();
        setMusic();
    }
}

QListWidget *MusicListsDialog::getListWidget()
{
    return ui->listWidget;
}

QComboBox *MusicListsDialog::getComboBox()
{
    return ui->comboBox;
}

MusicListsDialog::~MusicListsDialog()
{
    saveMusic();
    delete ui;
}

void MusicListsDialog::init()
{
    listNames<<tr("初始歌单");
    QList<QUrl> aList;
    QUrl aUrl;
    aList<<aUrl;
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
        QList<QUrl> aList;
        fileStream>>aList;
        musicLists<<aList;
    }
    aFile.close();
    settings->beginGroup("pomodoro");
    listNames=settings->value("List Name").toStringList();
    settings->endGroup();
}

void MusicListsDialog::setMusic()
{
    for(int i=0;i<listNames.count();i++)
    {
        ui->comboBox->addItem(listNames.at(i));
    }
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
    settings->beginGroup("pomodoro");
    settings->setValue("List Name",listNames);
    settings->endGroup();
}

void MusicListsDialog::on_comboBox_currentIndexChanged(int index)
{
    if(index==-1)
        return;
    ui->listWidget->clear();
    for(int i=1;i<musicLists.at(index).count();i++)
    {
        QListWidgetItem *aItem=new QListWidgetItem(musicLists.at(index).at(i).fileName());
        aItem->setIcon(QIcon(":/icons/images/music.png"));
        aItem->setData(Qt::UserRole,musicLists.at(index).at(i));
        ui->listWidget->addItem(aItem);
    }
}


void MusicListsDialog::on_btn_add_clicked()
{
    ui->comboBox->addItem(tr("未命名歌单"));
    listNames<<tr("未命名歌单");
    QList<QUrl> aList;
    QUrl aUrl;
    aList<<aUrl;
    musicLists<<aList;
    ui->comboBox->setCurrentIndex(ui->comboBox->count()-1);
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
    musicLists.removeAt(ui->comboBox->currentIndex()+1);
    listNames.removeAt(ui->comboBox->currentIndex());
    ui->comboBox->removeItem(ui->comboBox->currentIndex());
}


void MusicListsDialog::on_btn_changeName_clicked()
{
    bool ok=false;
    QString str= QInputDialog::getText(this,tr("修改歌单名称"),tr("请输入新名称"),QLineEdit::Normal,ui->comboBox->currentText(),&ok);
    if(ok && !str.isEmpty())
    {
        ui->comboBox->setItemText(ui->comboBox->currentIndex(),str);
        listNames.replace(ui->comboBox->currentIndex(),str);
    }
}


void MusicListsDialog::on_btn_addMusic_clicked()
{
    QString curPath= QDir::homePath();
    QString dlgTitle= tr("选择音频文件");
    QString filter=tr("音频文件(*.mp3 *.wav *.wma *flac *ogg *m4a)");
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
        QUrl aUrl=aItem->data(Qt::UserRole).toUrl();
        musicLists[ui->comboBox->currentIndex()].append(aUrl);
    }
}


void MusicListsDialog::on_btn_deleteMusic_clicked()
{
    int index=ui->listWidget->currentRow();
    if(index==-1)
        return;
    QListWidgetItem *aItem=ui->listWidget->takeItem(index);
    delete aItem;
    musicLists[ui->comboBox->currentIndex()].removeAt(index+1);

}


void MusicListsDialog::on_btn_up_clicked()
{
    int index = ui->listWidget->currentRow();
    if(index<=0)
        return;
    QListWidgetItem *aItem=ui->listWidget->takeItem(index);
    ui->listWidget->insertItem(index-1,aItem);
    ui->listWidget->setCurrentItem(aItem);
    musicLists[ui->comboBox->currentIndex()].swapItemsAt(index+1,index);
}


void MusicListsDialog::on_btn_down_clicked()
{
    int index = ui->listWidget->currentRow();
    if(index==ui->listWidget->count()-1)
        return;
    QListWidgetItem *aItem=ui->listWidget->takeItem(index);
    ui->listWidget->insertItem(index+1,aItem);
    ui->listWidget->setCurrentItem(aItem);
    musicLists[ui->comboBox->currentIndex()].swapItemsAt(index+1,index+2);
}

void MusicListsDialog::do_language_changed(int index)
{
    switch (index) {
    case 0:
        if(!trans.load("Pomodoro_zh_CN.qm"))
            return;
        break;
    case 1:
        if(!trans.load("Pomodoro_zh_TW.qm"))
            return;
        break;
    case 2:
        if(!trans.load("Pomodoro_en.qm"))
            return;
        break;
    case 3:
        if(!trans.load("Pomodoro_esp.qm"))
            return;
        break;
    }
    ui->retranslateUi(this);
}

