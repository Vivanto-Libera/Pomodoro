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
    explicit SetDialog(QWidget *parent = nullptr,MainWindow::pomoSetting aSetting={25,5,20,4});
    MainWindow::pomoSetting setting();
    ~SetDialog();

private:
    Ui::SetDialog *ui;
};

#endif // SETDIALOG_H
