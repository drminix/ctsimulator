/****************************
 * Author: Sanghyeb(Sam) Lee
 * Date: Jan/2013
 * email: drminix@gmail.com
 * Copyright 2013 Sang hyeb(Sam) Lee MIT License
 * 
 * X-ray CT simulation & reconstruction 
 *****************************/

#include "imagewidget.h"


imageWidget::imageWidget(double* data,int twidth,int theight,drawingarea *md):
    QWidget(0), phantom_width(twidth), phantom_height(theight), maindrawingarea(md) {

   setStyleSheet("background-color:red;");
   //create an image phantom
   mymatrix = new matrix(data,theight,twidth);
   phantom = mymatrix->getQImage();

}

void imageWidget::paintEvent(QPaintEvent *) {
    //creates a painter object for drawing on this device
    QPainter painter(this);

    //use stylesheets
    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    //draw phantom
    if(this->phantom!=NULL ) {
        //new size
        QSize widgetSize= this->phantom->size();
        widgetSize.setWidth(widgetSize.width()*maindrawingarea->zoomfactor);
        widgetSize.setHeight(widgetSize.height()*maindrawingarea->zoomfactor);

        QImage scaledImage = phantom->scaled(widgetSize);

        QPointF point(this->width()/2 - scaledImage.width()/2,
                  this->height()/2 - scaledImage.height()/2);


        painter.drawImage(point,scaledImage);
    }


}
