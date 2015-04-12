/****************************
 * Author: Sanghyeb(Sam) Lee
 * Date: Jan/2013
 * email: drminix@gmail.com
 * Copyright 2013 Sang hyeb(Sam) Lee MIT License
 * 
 * X-ray CT simulation & reconstruction 
 *****************************/

#include "sinogramwidget.h"

sinogramwidget::sinogramwidget(QWidget *parent) :
    QWidget(parent)
{

}

sinogramwidget::sinogramwidget(double** tsinogram,QSize tsize,drawingarea* md): QWidget(0), sinogram(tsinogram), sinogram_size(tsize),maindrawingarea(md) {

}

void sinogramwidget::paintEvent(QPaintEvent *event) {
    //display sinogram
    QPainter painter(this);

    //work out max value
    double max=0;
    for (int i = 0; i <sinogram_size.width(); i++)
    {
        for (int j = 0; j <sinogram_size.height(); j++)
        {
            double temp =  sinogram[i][j];
            if(temp>max) max = temp;
        }
    }

    //create an image
    QImage* qi = new QImage(sinogram_size.width(), sinogram_size.height(), QImage::Format_RGB32);
     for (int i = 0; i <sinogram_size.width(); i++)
     {
         for (int j = 0; j <sinogram_size.height(); j++)
         {
             int pixelvalue = (int)(sinogram[i][j]/max*255); //x(detector),y(projections)
            // pixelvalue =255;
             qi->setPixel(i, j,  qRgb(pixelvalue,pixelvalue, pixelvalue));
         }
     }

     QSize widgetSize= qi->size();
     widgetSize.setWidth(widgetSize.width()*maindrawingarea->zoomfactor);
     widgetSize.setHeight(widgetSize.height()*maindrawingarea->zoomfactor);

     QImage scaledImage = qi->scaled(widgetSize);

     QPointF point(this->width()/2 - scaledImage.width()/2,
               this->height()/2 - scaledImage.height()/2);
     painter.drawImage(point,scaledImage);
     this->resize(widgetSize);
     delete qi;
}
