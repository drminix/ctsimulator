/****************************
 * Author: Sanghyeb(Sam) Lee
 * Date: Jan/2013
 * email: drminix@gmail.com
 * Copyright 2013 Sang hyeb(Sam) Lee MIT License
 * 
 * X-ray CT simulation & reconstruction 
 *****************************/

#include "matrix.h"

matrix::matrix(double *tdata, int tM, int tN) :
    data(tdata), M(tM), N(tN)
{
}

//return QSIZE
QSize matrix::getQSize() {
    return QSize(this->N,this->M); //width, height
}

//return bitmap
QImage* matrix::getQImage() {

    //create QImage
    QImage* image = new QImage(this->getQSize(), QImage::Format_RGB32);
    for(int i=0;i<this->M;i++) {
        for(int j=0;j<this->N;j++) {
            int temp = (int)(this->data[i*this->N+j]*255);

            image->setPixel(j, i, qRgb(temp,temp,temp)); //(x,y)
        }
    }

    return image;
}
