/****************************
 * Author: Sanghyeb(Sam) Lee
 * Date: Jan/2013
 * email: drminix@gmail.com
 * Copyright 2013 Sang hyeb(Sam) Lee MIT License
 * 
 * X-ray CT simulation & reconstruction 
 *****************************/

#ifndef MATRIXWIDGET_H
#define MATRIXWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QSize>
class matrixWidget : public QWidget
{
    Q_OBJECT
public:
    explicit matrixWidget(QWidget *parent = 0);
    matrixWidget(QPoint source,QPoint detector,QSize matrixSize, double* matrixData);
    void paintEvent(QPaintEvent *event);

signals:
    
public slots:
    
private:
    QPoint source;
    QPoint detector;
    QSize matrixSize;
    double* matrixData;

};

#endif // MATRIXWIDGET_H
