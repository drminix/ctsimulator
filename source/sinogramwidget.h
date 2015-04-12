/****************************
 * Author: Sanghyeb(Sam) Lee
 * Date: Jan/2013
 * email: drminix@gmail.com
 * Copyright 2013 Sang hyeb(Sam) Lee MIT License
 * 
 * X-ray CT simulation & reconstruction 
 *****************************/

#ifndef SINOGRAMWIDGET_H
#define SINOGRAMWIDGET_H

#include <QWidget>
#include <QSize>
#include <QPainter>
#include "drawingarea.h"
class sinogramwidget : public QWidget
{
    Q_OBJECT
public:
    explicit sinogramwidget(QWidget *parent = 0);
    sinogramwidget(double**,QSize,drawingarea*);
    void paintEvent(QPaintEvent *event);

signals:
    
public slots:

public:
    double** sinogram;
    QSize sinogram_size;
    drawingarea* maindrawingarea;

    
};

#endif // SINOGRAMWIDGET_H
