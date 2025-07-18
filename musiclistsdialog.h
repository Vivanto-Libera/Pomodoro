#ifndef MUSICLISTSDIALOG_H
#define MUSICLISTSDIALOG_H

#include <QDialog>

namespace Ui {
class MusicListsDialog;
}

class MusicListsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MusicListsDialog(QWidget *parent = nullptr);
    ~MusicListsDialog();

private:
    Ui::MusicListsDialog *ui;
};

#endif // MUSICLISTSDIALOG_H
