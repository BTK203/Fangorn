#include "fangorn/view/fangornmainwidget.hpp"
#include "./ui_fangornmainwidget.h"

FangornMainWidget::FangornMainWidget(QWidget *parent)
 : FangornView(parent),
   ui(new Ui::FangornMainWidget)
{
  ui->setupUi(this);
}

FangornMainWidget::~FangornMainWidget()
{
    delete ui;
}
