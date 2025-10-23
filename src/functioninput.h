#pragma once

#ifndef FUNCTIONINPUT_H
#define FUNCTIONINPUT_H

#include <QWidget>
#include <functiondrawer.h>
#include <function.h>

namespace Ui {
class FunctionInput;
}

class FunctionInput : public QWidget
{
  Q_OBJECT

 public:
  explicit FunctionInput(FormulaTree::Function *func, QWidget *parent = nullptr);
  ~FunctionInput();

 private slots:
  void on_DrawButton_clicked();

 private:
  Ui::FunctionInput *ui;
  FormulaTree::Function *func;
};

#endif // FUNCTIONINPUT_H
