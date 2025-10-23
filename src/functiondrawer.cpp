#include "functiondrawer.h"
#include "ui_functiondrawer.h"

FunctionDrawer::FunctionDrawer(QWidget *parent, QColor color) :
                                                  QWidget(parent),
                                                  ui(new Ui::FunctionDrawer)
{
  ui->setupUi(this);
  pen.setColor(color);
  pen.setWidth(2);
  funcs.resize(3);
//  offset = QPoint(this->size().width() / 2, this->size().height() / 2);
}

FunctionDrawer::~FunctionDrawer() { delete ui; }

void FunctionDrawer::mouseMoveEvent(QMouseEvent *event) {
  QPoint drag = event->pos() - pressed_pos;
  offset += drag - prev_drag;
  prev_drag = drag;
  update();
}

void FunctionDrawer::mousePressEvent(QMouseEvent *event) {
  prev_drag.setX(0);
  prev_drag.setY(0);
  pressed_pos = event->pos();
}

void FunctionDrawer::wheelEvent(QWheelEvent *event) {
  float new_zoom = zoom;
  if (event->angleDelta().y() > 0) {
    new_zoom *= 1.05;
  } else {
    new_zoom *= 0.95;
  }
  QPoint mouse_pos = this->mapFromGlobal(QCursor::pos());
  offset += (-offset + mouse_pos) * (1 - new_zoom / zoom);
  zoom = new_zoom;
  update();
}

QPoint FunctionDrawer::RemapToScreen(QPointF pos) {
  return QPoint((pos * zoom + offset).x() + this->size().width() / 2,
                this->size().height() / 2 - (pos * zoom - offset).y());
}

QPointF FunctionDrawer::RemapToWorld(QPoint pos) {
  return QPointF((pos.x() - this->size().width() / 2 - offset.x()) / zoom,
                 (this->size().height() / 2 + offset.y() - pos.y()) / zoom);
}

void FunctionDrawer::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  pen.setColor(Qt::gray);
  painter.setPen(pen);

  int grid_size = zoom;
  QPoint start = RemapToScreen(QPointF(0, 0));
  while (start.x() <= size().width() || start.y() <= size().height()) {
    painter.drawLine(QPoint(start.x(), 0),
                     QPoint(start.x(), this->size().height()));
    painter.drawLine(QPoint(0, start.y()),
                     QPoint(this->size().width(), start.y()));
    start += {grid_size, grid_size};
  }

  start = RemapToScreen(QPointF(0, 0));
  while (start.x() >= 0 || start.y() >=0) {
    painter.drawLine(QPoint(start.x(), 0),
                     QPoint(start.x(), this->size().height()));
    painter.drawLine(QPoint(0, start.y()),
                     QPoint(this->size().width(), start.y()));
    start -= {grid_size, grid_size};
  }

  pen.setColor(Qt::black);
  painter.setPen(pen);
  start = RemapToScreen(QPointF(0, 0));
  painter.drawLine(QPoint(start.x(), 0),
                   QPoint(start.x(), this->size().height()));
  painter.drawLine(QPoint(0, start.y()),
                   QPoint(this->size().width(), start.y()));



  for (int i = 0; i < funcs.size(); ++i) {
    if (funcs[i].root == nullptr) continue;
    pen.setColor(colors[i]);
    painter.setPen(pen);
    for (int x = 0; x < this->size().width(); ++x) {
      try {
        double world_x_f = RemapToWorld(QPoint(x, 0)).x();
        double world_y_f = funcs[i].Solve(world_x_f);

        double world_x_s = RemapToWorld(QPoint(x + 1, 0)).x();
        double world_y_s = funcs[i].Solve(world_x_s);

        painter.drawLine(RemapToScreen(QPointF(world_x_f, world_y_f)),
                         RemapToScreen(QPointF(world_x_s, world_y_s)));
      } catch(std::invalid_argument& err) {
      }
    }
  }
}
