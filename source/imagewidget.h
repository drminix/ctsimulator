/****************************
 * Author: Sanghyeb(Sam) Lee
 * Date: Jan/2013
 * email: drminix@gmail.com
 * Copyright 2013 Sang hyeb(Sam) Lee MIT License
 * 
 * X-ray CT simulation & reconstruction 
 *****************************/

#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QStyleOption>
#include <QImage>
#include "matrix.h"
#include "drawingarea.h"
class imageWidget : public QWidget
{
    Q_OBJECT
public:

    imageWidget(double* matrix_data,int width,int height,drawingarea*);
    void paintEvent(QPaintEvent *);

signals:
    
public slots:
    
public:
    int phantom_width;
    int phantom_height;
    matrix* mymatrix;
    QImage *phantom;

    drawingarea *maindrawingarea;

};

#endif // IMAGEWIDGET_H
