#include "functioninput.h"
#include "ui_functioninput.h"
#include <QPushButton>

FunctionInput::FunctionInput(FormulaTree::Function *func, QWidget *parent) :
                                                QWidget(parent),
                                                ui(new Ui::FunctionInput)
{
  ui->setupUi(this);
  this->func = func;
  connect(ui->DrawButton, &QPushButton::clicked, this,
          &FunctionInput::on_DrawButton_clicked);
}

FunctionInput::~FunctionInput() { delete ui; }

void FunctionInput::on_DrawButton_clicked() {
  func->Generate(ui->FunctionInputText->toPlainText().toStdString());
}
