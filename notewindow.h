#ifndef NOTEWINDOW_H
#define NOTEWINDOW_H

#include <QMainWindow>
#include <QFile>

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

private:
    Ui::NoteWindow *ui;
    QList<note> notes;
    void readNotes();
};

#endif // NOTEWINDOW_H
