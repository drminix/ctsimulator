/****************************
 * Author: Sanghyeb(Sam) Lee
 * Date: Jan/2013
 * email: drminix@gmail.com
 * Copyright 2013 Sang hyeb(Sam) Lee MIT License
 * 
 * X-ray CT simulation & reconstruction 
 *****************************/

#include "drawingarea.h"
#define PI 3.14159265

drawingarea::drawingarea(QWidget *parent) :
    QWidget(parent), width(512), height(512)
{
    this->tocenterx = width/2;
    this->tocentery = height/2;

    this->phantom==NULL;
    this->source=NULL;
    this->detector=NULL;
    this->current_detector=NULL;
    this->current_source = NULL;
    this->source_projections=NULL;
    this->detector_projections=NULL;
    this->visualizeLine = false;
    this->zoomfactor = 1;
    this->A =NULL;
    this->x = NULL;
    this->b = NULL;

    setStyleSheet("background-color:black;");

}


//drawing area
void drawingarea::paintEvent(QPaintEvent *event) {

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
        widgetSize.setWidth(widgetSize.width()*zoomfactor);
        widgetSize.setHeight(widgetSize.height()*zoomfactor);

        QImage scaledImage = phantom->scaled(widgetSize);

        QPointF point(this->width/2 - scaledImage.width()/2,
                  this->height/2 - scaledImage.height()/2);


        painter.drawImage(point,scaledImage);

    }

    //draw source
    if(this->current_source!=NULL) {
        QPen pen(Qt::red, 1, Qt::SolidLine);
        painter.setPen(pen);

        //draw sources
        for(int i=0;i<this->number_of_source_element;i++) {
            int x = (this->current_source[i].real()*zoomfactor)+this->tocenterx;
            int y = (this->current_source[i].imag()*zoomfactor)+this->tocentery;
            painter.drawPoint(x,y);
        }

    }

    //draw detector
    if(this->current_detector!=NULL) {
        QPen pen(Qt::blue, 1, Qt::SolidLine);
        painter.setPen(pen);

        //draw sources
        for(int i=0;i<this->number_of_detector_element;i++) {
            int x = (this->current_detector[i].real()*zoomfactor)+this->tocenterx;
            int y = (this->current_detector[i].imag()*zoomfactor)+this->tocentery;
            painter.drawPoint(x,y);
        }

    }

    //draw line
    if(this->visualizeLine==true) {
        for(int i=0;i<this->number_of_detector_element;i++) {
            QPen pen(Qt::yellow, 1, Qt::SolidLine);
            painter.setPen(pen);
            int x_s = this->current_source[i].real() + tocenterx;
            int y_s = this->current_source[i].imag() + tocentery;
            int x_d = this->current_detector[i].real() + tocenterx;
            int y_d = this->current_detector[i].imag() + tocentery;

            painter.drawLine(x_s,y_s,x_d,y_d);
        }
    }
}

//set the phantom
 void drawingarea::setPhantom(QImage* temp) {
     this->phantom = temp;
 }

//set matrix
 void drawingarea::setMatrix(double* temp) {
     this->matrix = temp;
 }

 //load source and detector
 void drawingarea::loadSourceAndDetector(int q) {
     //2 * R is the distance between source and detector
     double R = 1.001 * this->phantom->width() / sqrt(2);
     double h = R/(sqrt(2)*q);

     //real = x-axis
     //imaginary = y-axis
     //total number = 2q + 1;

     //set up sources
     this->number_of_source_element = 2*q+1;
     if(source!=NULL) delete source;

     source = new complex<double>[number_of_source_element];

     for(int i=0;i<this->number_of_source_element;i++) {
         double imag = -h*q + (i*h);
         source[i] = complex<double>(R,imag);
     }

     //set up detectors
     this->number_of_detector_element = 2*q+1;
     if(detector!=NULL) delete detector;
     detector = new complex<double>[number_of_detector_element];
     for(int i=0;i<this->number_of_detector_element;i++) {
         double imag = -h*q + (i*h);
         detector[i] = complex<double>(-R,imag);
     }

     this->current_source = source;
     this->current_detector = detector;
     this->update();


 }


 //load movement
int drawingarea::loadMovement(double startAngle,double endAngle, double incrementAngle) {

     //delete previous arrays
     if(source_projections!=NULL) {
         for(int i=0;i<number_of_projections;i++) {
             delete source_projections[i];
             delete detector_projections[i];
         }
     }
     delete source_projections;
     delete detector_projections;

     //load movement.
     this->number_of_projections = (int)((endAngle-startAngle+1)/incrementAngle);

     //create new arrays
     this->source_projections = new complex<double>*[this->number_of_projections];
     this->detector_projections = new complex<double>*[this->number_of_projections];


    //for each angle
     double angle=startAngle;
     double angleInRadian;
     for(int i=0;i<this->number_of_projections;i++) {
         //set up source and detector
         angle = startAngle + incrementAngle*i;
         angleInRadian = angle*PI/180;
         this->source_projections[i] = new complex<double>[this->number_of_source_element];
         for(int j=0;j<this->number_of_source_element;j++) {
             source_projections[i][j] = source[j] * exp(complex<double>(0,angleInRadian));
         }

         this->detector_projections[i] = new complex<double>[this->number_of_source_element];
         for(int z=0;z<this->number_of_detector_element;z++) {
             detector_projections[i][z] = detector[z] * exp(complex<double>(0,angleInRadian));
         }
    }

     //visualize
     animation_index=0;
     current_source = source_projections[animation_index];
     current_detector = detector_projections[animation_index];
     timer = new QTimer(this);
     timer->setInterval(5);
     QObject::connect(timer,SIGNAL(timeout()),this,SLOT(animate()));

     timer->start();

     return this->number_of_projections;
 }

void drawingarea::animate() {
    if(animation_index<number_of_projections-1) {
        animation_index++;
        current_source = source_projections[animation_index];
        current_detector = detector_projections[animation_index];
        std::cout<<"Animation index: "<<animation_index<<std::endl;
        update();
    } else {
        timer->stop();
        emit animationLoaded();
    }

}

void drawingarea::setupSystemMatrix() {
    //A = new double[];
    //A = 3660 x 16384
    //column: 128 * 128 = 16384 = all the pixel in the image
    //row: number of detector elements * all the angles
    //Aij --> how each element of jth serial image pixel contribute to ith serial projection
    //how each pixel

    //delete a previous array
    if(A!=NULL) {
        delete A;
    }

    A = new double*[this->number_of_detector_element*this->number_of_projections];
    A_size.setWidth(this->phantom->height()*this->phantom->width());  //width
    A_size.setHeight(number_of_source_element * number_of_projections); //height

    sinogram = new double*[this->number_of_projections];
    for(int i=0;i<this->number_of_projections;i++) {
        sinogram[i] = new double[number_of_detector_element];
    }

    //[detector_element][number_of_projections]
    sinogram_size.setHeight(this->number_of_detector_element);
    sinogram_size.setWidth(this->number_of_projections);

    double* ray;
    double sum;
    int matrix_column = A_size.width();

    //for all projections
    for(int i=0;i<number_of_projections;i++) {
        //for all source element
        for(int j=0;j<number_of_source_element;j++) {
            double x_s = source_projections[i][j].real(); //source x-coordinate
            double y_s = source_projections[i][j].imag(); //source y-coordinate
            double x_d = detector_projections[i][j].real(); //destination x-coordinate
            double y_d = detector_projections[i][j].imag(); //desitnation y-coordinate

            //A[i*number_of_detector_element+j]    
            //single row in A
            ray = myutility->siddon_ray_trace_2d(x_s,y_s,x_d,y_d, (double)this->phantom->height(),(double)this->phantom->width());
            A[i*number_of_source_element+j] = ray;
            //forward projection
            //sino(k,j) = sum(sum(image.*ray));
            //size of sino= detector element(61) x projections(60)
            sum=0;
            for(int k=0;k<matrix_column;k++) {
                sum +=ray[k]*matrix[k];
            }

            //[detector element][projections]
            sinogram[i][j] = sum;
        }
    }

}


void drawingarea::on_btn_step3_clicked()
{

}

void drawingarea::on_lineEdit_returnPressed() {

}

void drawingarea::on_btn_step4_clicked()
{

}
