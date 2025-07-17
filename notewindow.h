#ifndef NOTEWINDOW_H
#define NOTEWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QMessageBox>

namespace Ui {
class NoteWindow;
}

class NoteWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NoteWindow(QWidget *parent = nullptr);
    ~NoteWindow();
    struct note
    {
        QString title;
        QString text;
    };

private slots:
    void on_act_bold_triggered(bool checked);

    void on_act_italic_triggered(bool checked);

    void on_act_underline_triggered(bool checked);

    void on_noteCombo_editTextChanged(const QString &arg1);

    void on_noteCombo_currentIndexChanged(int index);

    void on_btn_addNote_clicked();

    void on_btn_deleteNote_clicked();

private:
    Ui::NoteWindow *ui;
    QList<note> notes;
    void readNotes();
    void setNotes();
};

#endif // NOTEWINDOW_H
