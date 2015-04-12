/****************************
 * Author: Sanghyeb(Sam) Lee
 * Date: Jan/2013
 * email: drminix@gmail.com
 * Copyright 2013 Sang hyeb(Sam) Lee MIT License
 * 
 * X-ray CT simulation & reconstruction 
 *****************************/

#include "artthread.h"

ArtThread::ArtThread(QObject *parent) :
    QThread(parent)
{
}

ArtThread::ArtThread(drawingarea* tarea,int tsweeps,double trelaxation, int twhentoshow):
    maindrawingarea(tarea), sweeps(tsweeps), relaxation(trelaxation), whentoshow(twhentoshow) {

    A = maindrawingarea->A; //A
    x = new double[maindrawingarea->A_size.width()];
    b = new double[maindrawingarea->sinogram_size.width()*maindrawingarea->sinogram_size.height()];

    //setup it to running
    running = true;
}

inline void swap(double** x1,double** x2) {
    double* x_temp;
    x_temp = *x1;
    *x1 = *x2;
    *x2 = x_temp;
}

void ArtThread::setupReturnBuffer(double **pBuffer) {
    pX = pBuffer;
}

//perform art.
void ArtThread::run() {
    //Ax = b
    int a_row = this->maindrawingarea->A_size.height();
    int a_column = this->maindrawingarea->A_size.width();
    int x_row = a_column;
    int b_row = a_row;

    //buffer storig x_previous value
    double* x_prev = new double[a_column];

    std::cout<<"ArtThread: started."<<std::endl;

    int sino_width  = maindrawingarea->sinogram_size.width();
    int sino_height = maindrawingarea->sinogram_size.height();

    //initialize x(estimate image) to zero
    memset(x,0,sizeof(double)*x_row);

    for(int i=0;i<sino_width;i++) {
        memcpy(b+(sino_height*i),this->maindrawingarea->sinogram[i],sizeof(double)*sino_height);
    }


    //now perform ART
    //main iteration.
    double rs; //residual

    for(int main_iterator=0;main_iterator<sweeps&&running==true;main_iterator++) {
        //subiteration.. update our estimate(x) using one single row at atime.
        for(int i=0;i<a_row&&running==true;i++) {
            //after this command, x_prev points to x, x point to new area
            swap(&x_prev,&x);

            //work out update factor
            double update_factor;
            //work out sigma denominator and sigma nominator
            double denominator=0;
            double nominator=0;
            for(int h=0;h<a_column;h++) {
                nominator += A[i][h] * x_prev[h];
                denominator += A[i][h] * A[i][h];
            }

            if(denominator==0) {
                update_factor=0;
            }else {
                update_factor = (b[i] - nominator)/denominator;
            }

            //update x using sub iterator
            for(int j=0;j<x_row;j++) {
                x[j] = x_prev[j] + relaxation * A[i][j] * update_factor;
            }
        }
        //inform main window for availability
        if(((main_iterator+1)%this->whentoshow)==0) {
            if(*pX==NULL) { //if it is empty create it first
                *pX = new double[a_column];
            }
            //copy the image value
            memcpy(*pX,x,sizeof(double)*x_row);
            //notify the main window
            emit updateReady(main_iterator+1);
        }

        //work out residual.
        rs=0;
        for(int z=0;z<x_row;z++) {
            rs +=fabs(this->maindrawingarea->matrix[z] -
                      x[z]);
        }

        emit singleiteration(main_iterator+1,rs);
    }

}

