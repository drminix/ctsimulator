/****************************
 * Author: Sanghyeb(Sam) Lee
 * Date: Jan/2013
 * email: drminix@gmail.com
 * Copyright 2013 Sang hyeb(Sam) Lee MIT License
 * 
 * X-ray CT simulation & reconstruction 
 *****************************/

#ifndef ARTTHREAD_H
#define ARTTHREAD_H

#include <QThread>
#include <iostream>
#include <cstring>
#include"drawingarea.h"

class ArtThread : public QThread
{
    Q_OBJECT
public:
    explicit ArtThread(QObject *parent = 0);
    ArtThread(drawingarea* tarea,int sweeps,double relaxation, int whentoshow);
    void setupReturnBuffer(double** pBuffer);
    void run();

signals:
    void updateReady(int iteration);
    void singleiteration(int iteration,double rs);

public slots:

public:
    drawingarea* maindrawingarea;
    int os;
    int sweeps;
    double relaxation;
    int whentoshow;
    double** A; //A
    double*  x; //x
    double*  b; //b
    double** pX; //address to buffer in main app

    bool running;
};

#endif // ARTTHREAD_H
