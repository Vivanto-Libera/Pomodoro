#include "notewindow.h"
#include "ui_notewindow.h"
QDataStream &operator<<(QDataStream &out,const NoteWindow::note &aNote)
{
    out<<aNote.title<<aNote.text;
    return out;
}
QDataStream &operator>>(QDataStream &in,NoteWindow::note &aNote)
{
    in>>aNote.title>>aNote.text;
    return in;
}
NoteWindow::NoteWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NoteWindow)
{
    ui->setupUi(this);
    if(!QFile::exists("notes.bin"))
    {
        note aNote={ui->noteCombo->itemText(0),""};
        notes<<aNote;
    }
    else
        readNotes();
    ui->plainTextEdit->appendPlainText(notes.at(0).text);
}

NoteWindow::~NoteWindow()
{
    delete ui;
}

void NoteWindow::on_act_bold_triggered(bool checked)
{
    QTextCharFormat fmt= ui->plainTextEdit->currentCharFormat();
    if(checked)
        fmt.setFontWeight(QFont::Bold);
    else
        fmt.setFontWeight(QFont::Normal);
    ui->plainTextEdit->mergeCurrentCharFormat(fmt);
}


void NoteWindow::on_act_italic_triggered(bool checked)
{
    QTextCharFormat fmt= ui->plainTextEdit->currentCharFormat();
    fmt.setFontItalic(checked);
    ui->plainTextEdit->mergeCurrentCharFormat(fmt);
}


void NoteWindow::on_act_underline_triggered(bool checked)
{
    QTextCharFormat fmt= ui->plainTextEdit->currentCharFormat();
    fmt.setFontUnderline(checked);
    ui->plainTextEdit->mergeCurrentCharFormat(fmt);
}

void NoteWindow::readNotes()
{
    QFile aFile("notes.bin");
    if(!aFile.open(QIODevice::ReadOnly))
        return;
    QDataStream fileStream(&aFile);
    fileStream.setVersion(QDataStream::Qt_6_9);
    fileStream.setByteOrder(QDataStream::BigEndian);
    while(!fileStream.atEnd())
    {
        fileStream>>notes;
    }
    aFile.close();
}

void NoteWindow::setNotes()
{
    ui->noteCombo->clear();
    for(int i=0;i<notes.count();i++)
    {
        ui->noteCombo->addItem(notes.at(i).title);
    }
}


void NoteWindow::on_noteCombo_editTextChanged(const QString &arg1)
{
    note aNote({arg1,notes.at(ui->noteCombo->currentIndex()).text});
    notes.replace(ui->noteCombo->currentIndex(),aNote);
}


void NoteWindow::on_noteCombo_currentIndexChanged(int index)
{
    ui->plainTextEdit->setPlainText(notes.at(index).text);
}


void NoteWindow::on_btn_addNote_clicked()
{
    ui->noteCombo->addItem(tr("未命名笔记"));
    note aNote={ui->noteCombo->itemText(ui->noteCombo->count()-1),""};
    notes<<aNote;
    ui->noteCombo->setCurrentIndex(ui->noteCombo->count()-1);
}


void NoteWindow::on_btn_deleteNote_clicked()
{
    if(ui->noteCombo->count()!=1)
    {
        QMessageBox::StandardButton result;
        result=QMessageBox::question(this,tr("确认删除"),tr("是否删除该笔记？"));
        if(result!=QMessageBox::Yes)
            return;
    }
    else
    {
        QMessageBox::critical(this,tr("删除失败"),tr("无法删除最后一个笔记"));
        return;
    }
    notes.removeAt(ui->noteCombo->currentIndex());
    ui->noteCombo->removeItem(ui->noteCombo->currentIndex());
}

