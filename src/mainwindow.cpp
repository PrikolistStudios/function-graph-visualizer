#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  overlay = new QStackedLayout;

  FunctionDrawer *drawer = new FunctionDrawer(this);

  FunctionInput *input_black = new FunctionInput(&drawer->funcs[0]),
                *input_red = new FunctionInput(&drawer->funcs[1]),
                *input_green = new FunctionInput(&drawer->funcs[2]);

  ui->InputForm->addRow("Black", input_black);
  ui->InputForm->addRow("Red", input_red);
  ui->InputForm->addRow("Green", input_green);

  ui->tabWidget->clear();
  ui->tabWidget->addTab(drawer, "Functions");
  drawer->update();
}

MainWindow::~MainWindow()
{
  delete ui;
}

