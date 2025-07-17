#include "notewindow.h"
#include "ui_notewindow.h"

NoteWindow::NoteWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NoteWindow)
{
    ui->setupUi(this);
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

