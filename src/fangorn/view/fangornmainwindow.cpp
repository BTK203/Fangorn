#include "fangorn/view/fangornmainwindow.h"
#include "./ui_fangornmainwindow.h"

FangornMainWindow::FangornMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FangornMainWindow)
{
    ui->setupUi(this);

    //initialize defaults
    #if FANGORN_FEATURE_TEST_LABEL == FEATURE_ENABLE

    #endif
}

FangornMainWindow::~FangornMainWindow()
{
    delete ui;
}

