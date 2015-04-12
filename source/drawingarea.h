/****************************
 * Author: Sanghyeb(Sam) Lee
 * Date: Jan/2013
 * email: drminix@gmail.com
 * Copyright 2013 Sang hyeb(Sam) Lee MIT License
 * 
 * X-ray CT simulation & reconstruction 
 *****************************/

#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <QWidget>
#include <QPainter>
#include <QBitmap>
#include <complex>
#include <QStyleOption>
#include <QThread>
#include <QTimer>
#include <iostream>
#include "utility.h"

using std::complex;

class drawingarea : public QWidget
{
    Q_OBJECT
public:
    explicit drawingarea(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);

    void setPhantom(QImage*);
    void setMatrix(double*);
    void loadSourceAndDetector(int q);
    int loadMovement(double startAngle,double endAngle, double incremetnAngle);
    void setupSystemMatrix();


signals:
    void animationLoaded();

public slots:
       void animate();
public:
    QImage* phantom;
    complex<double>* source;
    int number_of_source_element;
    complex<double>* detector;
    int number_of_detector_element;


    //gantry motion
    double startAngle;
    double endAngle;
    double incrementAngle;
    int animation_index;

    //sources and detectors projection
    int number_of_projections;
    complex<double>** source_projections;
    complex<double>** detector_projections;

    //current source and  to visualize
    complex<double>* current_source;
    complex<double>* current_detector;

public:
    int width;
    int height;

    int tocenterx;
    int tocentery;
    QTimer* timer;
    double** A; //system matrix
    double** sinogram; //sinogram
    double* matrix;

    QSize A_size; //A Size
    QSize sinogram_size; //sinogram size;
    bool visualizeLine;

    utility* myutility;
    double zoomfactor;

    double* x;
    double* b;

private slots:
    void on_btn_step3_clicked();
    void on_lineEdit_returnPressed();
    void on_btn_step4_clicked();
};

#endif // DRAWINGAREA_H
