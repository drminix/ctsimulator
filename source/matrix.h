/****************************
 * Author: Sanghyeb(Sam) Lee
 * Date: Jan/2013
 * email: drminix@gmail.com
 * Copyright 2013 Sang hyeb(Sam) Lee MIT License
 * 
 * X-ray CT simulation & reconstruction 
 *****************************/

#ifndef MATRIX_H
#define MATRIX_H

#include<QString>
#include<QSize>
#include<QBitmap>

class matrix
{
public:
    matrix(double* tdata,int tM,int tN);

    void loadMatrix(QString filename); //load matrix
    double* data; //M x N matrix
    int M;
    int N;

    QSize getQSize();
    QImage* getQImage();
};

#endif // MATRIX_H
