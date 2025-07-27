#ifndef SETDIALOG_H
#define SETDIALOG_H

#include <QDialog>
#include <mainwindow.h>

namespace Ui {
class SetDialog;
}

class SetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetDialog(QWidget *parent = nullptr);
    ~SetDialog();
    MainWindow::pomoSetting setting();
    void ini(MainWindow::pomoSetting aSetting);

private slots:
    void on_btn_reset_clicked();

    void on_btn_save_clicked();

private:
    Ui::SetDialog *ui;
};

#endif // SETDIALOG_H
