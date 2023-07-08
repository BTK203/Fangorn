#ifndef FANGORNMAINWINDOW_H
#define FANGORNMAINWINDOW_H

#include <QMainWindow>
#include "fangorn/fangornbase.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class FangornMainWindow; }
QT_END_NAMESPACE

class FangornMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    FangornMainWindow(QWidget *parent = nullptr);
    ~FangornMainWindow();

private:
    Ui::FangornMainWindow *ui;
};
#endif // FANGORNMAINWINDOW_H
