/****************************
 * Author: Sanghyeb(Sam) Lee
 * Date: Jan/2013
 * email: drminix@gmail.com
 * Copyright 2013 Sang hyeb(Sam) Lee MIT License
 * 
 * X-ray CT simulation & reconstruction 
 *****************************/

#ifndef UTILITY_H
#define UTILITY_H

#include"matrix.h"
#include<QFile>
#include<QTextStream>
#include<QStringList>
#include<QMessageBox>
#include<cmath>
#include<iostream>
#include<vector>
#include<limits>

using std::vector;
using std::pair;

class utility
{
public:
    utility();

    //load phantom
    matrix* loadPhantom(QString filename);

    //ray trace
    double* siddon_ray_trace_2d(double x_s,double y_s,double x_d,double y_d, double row,double column);

};

#endif // UTILITY_H
