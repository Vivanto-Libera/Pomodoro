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
    MainWindow::pomoSetting setting();
    void ini(MainWindow::pomoSetting aSetting);
    ~SetDialog();

private slots:
    void on_btn_reset_clicked();

private:
    Ui::SetDialog *ui;
};

#endif // SETDIALOG_H
