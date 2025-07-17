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
        ui->noteCombo->addItem(tr("未命名笔记"));
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

