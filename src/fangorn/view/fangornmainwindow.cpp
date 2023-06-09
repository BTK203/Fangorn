#include "fangorn/view/fangornmainwindow.h"
#include "./ui_fangornmainwindow.h"

FangornMainWindow::FangornMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FangornMainWindow)
{
    ui->setupUi(this);
}

FangornMainWindow::~FangornMainWindow()
{
    delete ui;
}

