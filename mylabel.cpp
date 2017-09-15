#include "mylabel.h"
#include <QMouseEvent>

MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{
}

void MyLabel::mouseMoveEvent(QMouseEvent *ev)
{
    emit sendCoordinates(ev->x(), ev->y());
}

void MyLabel::mousePressEvent(QMouseEvent *ev)
{
    emit sendCoordinates(ev->x(), ev->y());
}
