/****************************
 * Author: Sanghyeb(Sam) Lee
 * Date: Jan/2013
 * email: drminix@gmail.com
 * Copyright 2013 Sang hyeb(Sam) Lee MIT License
 * 
 * X-ray CT simulation & reconstruction 
 *****************************/

#include "matrixwidget.h"

matrixWidget::matrixWidget(QWidget *parent) :
    QWidget(parent)
{
}
matrixWidget::matrixWidget(QPoint tsource,QPoint tdetector,QSize tmatrix, double* tmatrixData) {
    this->source=tsource;
    this->detector=tdetector;
    this->matrixSize = tmatrix;
    this->matrixData = tmatrixData;
}

void matrixWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);


    int imagecenterX = this->width()/2 - (this->matrixSize.width()*5/2);
    int imagecenterY = this->height()/2 - (this->matrixSize.height()*5/2);

    //display matrix
    for(int i=0;i<matrixSize.height();i++) {
        for(int j=0;j<matrixSize.width();j++) {
            if(matrixData[i*matrixSize.width()+j]!=0) painter.setBrush(QBrush("#c56c00"));
           else painter.setBrush(Qt::NoBrush);
           painter.drawRect(j*5+imagecenterY,i*5+imagecenterX,5,5);
        }
    }

    int centerX = this->width()/2;
    int centerY = this->height()/2;

    //display source
    QPen pen(Qt::red, 5, Qt::SolidLine);
    painter.setPen(pen);
    painter.drawPoint(this->source.x()*5+centerX,this->source.y()*5+centerY);

    //display detector
    pen.setColor(Qt::blue);
    painter.setPen(pen);
    painter.drawPoint(this->detector.x()*5 +centerX,this->detector.y()*5+centerY);


}
