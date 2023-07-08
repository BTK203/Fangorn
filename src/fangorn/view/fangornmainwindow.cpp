#include <QResizeEvent>
#include "fangorn/view/fangornmainwindow.h"
#include "./ui_fangornmainwindow.h"

using namespace std::placeholders;

FangornMainWindow::FangornMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FangornMainWindow)
{
    ui->setupUi(this);

    #if FANGORN_FEATURE_TEST_LABEL == FEATURE_ENABLE
        __fangornRegisterTestLabelFunc(std::bind(&FangornMainWindow::setTestLabelText, this, _1));
    #else
        ui->testLabel->setVisible(false);
    #endif
}


FangornMainWindow::~FangornMainWindow() {
    delete ui;
}


#if FANGORN_FEATURE_TEST_LABEL == FEATURE_ENABLE

void FangornMainWindow::setTestLabelText(const std::string& text) {
    ui->testLabel->setText(QString::fromStdString(text));
}

#endif
