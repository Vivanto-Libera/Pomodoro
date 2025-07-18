#include "musiclistsdialog.h"
#include "ui_musiclistsdialog.h"

MusicListsDialog::MusicListsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MusicListsDialog)
{
    ui->setupUi(this);
}

MusicListsDialog::~MusicListsDialog()
{
    delete ui;
}
