#pragma once

#ifndef FUNCTIONDRAWER_H
#define FUNCTIONDRAWER_H

#include <QWidget>
#include <function.h>
#include <QPainter>
#include <QColor>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPoint>
#include <QPointF>
#include <QList>

namespace Ui {
class FunctionDrawer;
}

class FunctionDrawer : public QWidget
{
  Q_OBJECT

 public:
  explicit FunctionDrawer(QWidget *parent = nullptr, QColor color = Qt::black);
  ~FunctionDrawer();

  void paintEvent(QPaintEvent *painter);
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);
  QVector<FormulaTree::Function> funcs;

 private:
  //FormulaTree::Function func;

  Ui::FunctionDrawer *ui;
  QPen pen;
  float zoom = 80;
  QPoint offset, prev_drag, pressed_pos;
  QPoint RemapToScreen(QPointF pos);
  QPointF RemapToWorld(QPoint pos);
  QVector<QColor> colors = {Qt::black, Qt::red, Qt::green};
};

#endif // FUNCTIONDRAWER_H
