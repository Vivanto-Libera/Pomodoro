#include "notewindow.h"
#include "ui_notewindow.h"
#include <QTranslator>

extern QTranslator trans;

QDataStream &operator<<(QDataStream &out,const NoteWindow::note &aNote)
{
    out << aNote.title << aNote.text;
    return out;
}
QDataStream &operator>>(QDataStream &in,NoteWindow::note &aNote)
{
    in >> aNote.title >> aNote.text;
    return in;
}

NoteWindow::NoteWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NoteWindow)
{
    ui->setupUi(this);
    if (!QFile::exists("notes.bin"))
    {
        note aNote = {ui->noteCombo->itemText(0), ""};
        notes<<aNote;
    }
    else
    {
        readNotes();
        setNotes();
    }
    ui->plainTextEdit->setPlainText(notes.at(0).text);
}

NoteWindow::~NoteWindow()
{
    saveNotes();
    delete ui;
}

void NoteWindow::on_act_bold_triggered(bool checked)
{
    QTextCharFormat fmt = ui->plainTextEdit->currentCharFormat();
    if (checked)
    {
        fmt.setFontWeight(QFont::Bold);
    }
    else
    {
        fmt.setFontWeight(QFont::Normal);
    }
    ui->plainTextEdit->mergeCurrentCharFormat(fmt);
}


void NoteWindow::on_act_italic_triggered(bool checked)
{
    QTextCharFormat fmt = ui->plainTextEdit->currentCharFormat();
    fmt.setFontItalic(checked);
    ui->plainTextEdit->mergeCurrentCharFormat(fmt);
}


void NoteWindow::on_act_underline_triggered(bool checked)
{
    QTextCharFormat fmt = ui->plainTextEdit->currentCharFormat();
    fmt.setFontUnderline(checked);
    ui->plainTextEdit->mergeCurrentCharFormat(fmt);
}

void NoteWindow::readNotes()
{
    QFile aFile("notes.bin");
    if (!aFile.open(QIODevice::ReadOnly))
    {
        return;
    }
    QDataStream fileStream(&aFile);
    fileStream.setVersion(QDataStream::Qt_6_9);
    fileStream.setByteOrder(QDataStream::BigEndian);
    while (!fileStream.atEnd())
    {
        note aNote;
        fileStream >> aNote;
        notes << aNote;
    }
    aFile.close();
}

void NoteWindow::setNotes()
{
    ui->noteCombo->clear();
    for (int i = 0;i < notes.count();i++)
    {
        ui->noteCombo->addItem(notes.at(i).title);
    }
}

void NoteWindow::saveNotes()
{
    QFile aFile("notes.bin");
    if (!aFile.open(QIODevice::WriteOnly))
    {
        return;
    }
    QDataStream fileStream(&aFile);
    fileStream.setVersion(QDataStream::Qt_6_9);
    fileStream.setByteOrder(QDataStream::BigEndian);
    for (int i = 0;i < notes.count();i++)
    {
        fileStream << notes.at(i);
    }
    aFile.close();
}

void NoteWindow::on_noteCombo_currentIndexChanged(int index)
{
    if (index == -1)
    {
        return;
    }
    ui->plainTextEdit->setPlainText(notes.at(index).text);
}


void NoteWindow::on_btn_addNote_clicked()
{
    ui->noteCombo->addItem(tr("未命名笔记"));
    note aNote = {ui->noteCombo->itemText(ui->noteCombo->count() - 1), ""};
    notes<<aNote;
    ui->noteCombo->setCurrentIndex(ui->noteCombo->count() - 1);
}


void NoteWindow::on_btn_deleteNote_clicked()
{
    if (ui->noteCombo->count() != 1)
    {
        QMessageBox::StandardButton result;
        result = QMessageBox::question(this, tr("确认删除"), tr("是否删除该笔记？"));
        if (result == QMessageBox::Yes)
        {
            notes.removeAt(ui->noteCombo->currentIndex());
            ui->noteCombo->removeItem(ui->noteCombo->currentIndex());
            return;
        }
    }
    else
    {
        QMessageBox::critical (this,tr("删除失败"),tr("无法删除最后一个笔记"));
        return;
    }
}

void NoteWindow::on_plainTextEdit_textChanged()
{
    notes.replace(ui->noteCombo->currentIndex(), {notes.at(ui->noteCombo->currentIndex()).title,
                                                  ui->plainTextEdit->toPlainText()});
}


void NoteWindow::on_btn_changeName_clicked()
{
    bool ok = false;
    QString str = QInputDialog::getText(this, tr("修改笔记名称"), tr("请输入新名称"), QLineEdit::Normal,
                                        ui->noteCombo->currentText(), &ok);
    if(ok && !str.isEmpty())
    {
        ui->noteCombo->setItemText(ui->noteCombo->currentIndex(), str);
        notes.replace(ui->noteCombo->currentIndex(), {str, notes.at(ui->noteCombo->currentIndex()).text});
    }
}

void NoteWindow::do_language_changed(int index)
{
    switch (index) {
    case 0 :
        if (!trans.load("localization/Pomodoro_zh_CN.qm"))
        {
            return;
        }
        break;
    case 1 :
        if (!trans.load("localization/Pomodoro_zh_TW.qm"))
        {
            return;
        }
        break;
    case 2 :
        if (!trans.load("localization/Pomodoro_en.qm"))
        {
            return;
        }
        break;
    case 3 :
        if (!trans.load("localization/Pomodoro_eo.qm"))
        {
            return;
        }
        break;
    }
    ui->retranslateUi(this);
}

