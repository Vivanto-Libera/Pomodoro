#ifndef MUSICLISTSDIALOG_H
#define MUSICLISTSDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class MusicListsDialog;
}

class MusicListsDialog : public QDialog
{
    Q_OBJECT

public:
    struct musicList
    {
        QString title;
        QList<QListWidgetItem> music;
    };
    QList<musicList> musicLists;
    explicit MusicListsDialog(QWidget *parent = nullptr);
    ~MusicListsDialog();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_btn_add_clicked();

private:
    Ui::MusicListsDialog *ui;
    void init();
    void readMusic();
    void setMusic();
    void saveMusic();
};

#endif // MUSICLISTSDIALOG_H
