#ifndef MUSICLISTSDIALOG_H
#define MUSICLISTSDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include <QSettings>
#include <QComboBox>

namespace Ui {
class MusicListsDialog;
}

class MusicListsDialog : public QDialog
{
    Q_OBJECT

public:
    QStringList listNames;
    QList<QList<QUrl>> musicLists;
    explicit MusicListsDialog(QWidget *parent = nullptr);
    QListWidget* getListWidget();
    QComboBox* getComboBox();
    ~MusicListsDialog();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_btn_add_clicked();

    void on_btn_delete_clicked();

    void on_btn_changeName_clicked();

    void on_btn_addMusic_clicked();

    void on_btn_deleteMusic_clicked();

    void on_btn_up_clicked();

    void on_btn_down_clicked();

private:
    Ui::MusicListsDialog *ui;
    QSettings *settings;
    void init();
    void readMusic();
    void setMusic();
    void saveMusic();
};

#endif // MUSICLISTSDIALOG_H
