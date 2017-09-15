#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QObject>

class MyLabel : public QLabel
{
    Q_OBJECT

public:
    MyLabel(QWidget *parent = 0);
    /*virtual*/ void mouseMoveEvent(QMouseEvent *ev);
    /*virtual*/ void mousePressEvent(QMouseEvent *ev);

signals:
    void sendCoordinates(int x, int y);
};

#endif // MYLABEL_H
