#pragma once
#include "fangorn/fangorncommon.hpp"
#include "fangorn/view/fangornview.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class FangornMainWidget; }
QT_END_NAMESPACE

class FangornMainWidget : public FangornView
{
    Q_OBJECT

public:
    FangornMainWidget(QWidget *parent = nullptr);
    ~FangornMainWidget();

private:
    Ui::FangornMainWidget *ui;
};
