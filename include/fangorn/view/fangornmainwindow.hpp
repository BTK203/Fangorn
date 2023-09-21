#ifndef FANGORNMAINWINDOW_H
#define FANGORNMAINWINDOW_H

#include <QMainWindow>
#include "fangorn/fangorncommon.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class FangornMainWindow; }
QT_END_NAMESPACE

class FangornMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    FangornMainWindow(QWidget *parent = nullptr);
    ~FangornMainWindow();

    QWidget *getMainPanel();

    #if FANGORN_FEATURE_TEST_LABEL == FEATURE_ENABLE
    void setTestLabelText(const std::string& text);
    #endif

private:
    Ui::FangornMainWindow *ui;
};
#endif // FANGORNMAINWINDOW_H
