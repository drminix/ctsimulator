/****************************
 * Author: Sanghyeb(Sam) Lee
 * Date: Jan/2013
 * email: drminix@gmail.com
 * Copyright 2013 Sang hyeb(Sam) Lee MIT License
 * 
 * X-ray CT simulation & reconstruction 
 *****************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //set utility
    myutility = new utility();
    ui->combo_projectionType->addItem("Parallel Projection");

    art_buffer=NULL;
    sirt_buffer=NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::animationLoaded() {
    //enable the buttons
    this->ui->btn_step4->setEnabled(true);
}

void MainWindow::on_phantomExplore_clicked()
{
    QString filename = QFileDialog::getOpenFileName();
    if(filename==NULL) return; //cancled
    //otherwise
    ui->phanton_filename->setText(filename);
}

void MainWindow::on_phantomLoad_clicked()
{
    drawingarea* mainwindow = new drawingarea();
    maindrawingarea = mainwindow;
    maindrawingarea->myutility = this->myutility;
    //Load Phantom
    matrix* phantom = myutility->loadPhantom(ui->phanton_filename->text());
    if(phantom==NULL) {
        QMessageBox::warning(this,QObject::tr("Invalid phantom file"),QObject::tr("Inconsistent column sizes!"),QMessageBox::Ok);
        return;
    }
    //load the phantom settings
    ui->phantom_size_x->setText(QString::number(phantom->M));
    ui->phantom_size_y->setText(QString::number(phantom->N));
    ui->phantom_location_x->setText(QString::number(0));
    ui->phantom_location_y->setText(QString::number(0));

    //set the phantom
    mainwindow->setPhantom(phantom->getQImage());
    mainwindow->setMatrix(phantom->data);
    //when you add a widget directly to
    //mdi area, a mdisubwindow is automatically
    //created and the widget is set as the central
    //widget of the mdisubwindow
    ui->mdiArea->addSubWindow(mainwindow);
    mainwindow->show();

    //show main window
    mainwindow->setMinimumSize(mainwindow->height,mainwindow->width);
    ui->mdiArea->currentSubWindow()->adjustSize();
    ui->mdiArea->currentSubWindow()->setWindowTitle("mainwindow");
    ui->btn_step2->setEnabled(true);


}

void MainWindow::on_pushButton_clicked()
{
    //add a gaussian noise with zero mean and sd
    double sd = this->ui->misc_sd->text().toDouble();
    double uniform1=0, uniform2=0; //two uniformly distributed random numbers
    double normal1=0,normal2=0;

    for(int i=0;i<this->maindrawingarea->sinogram_size.width();i++) {
        for(int j=0;j<this->maindrawingarea->sinogram_size.height();j+=2) {
            //1) generates two uniformly distributed random numbers
            while(uniform1 == 0.0)  uniform1 = 1.0 *rand()/RAND_MAX;
            uniform2 = 1.0 *rand()/RAND_MAX;

            //2) Uses BOX-MULLER technique of inverse transformation to turn
            //   uniformly distributed random numbers into two unit normal random numbers.
            normal1=sqrt(-2*log(uniform1)) * cos(2 * PI * uniform2);
            normal2=sqrt(-2*log(uniform1)) * sin(2 * PI * uniform2);

            //3) modify unit normal random numbers according to given mean and variance.
            //   we use a zero mean and sigma standard devision
            //   we use boundaryCheck function I defined for project1 to ensure that the pixel intensity doesn't go over bounary.
            //   since we have two different normal random numbers, we apply each of them to different rows.
            this->maindrawingarea->sinogram[i][j] =this->maindrawingarea->sinogram[i][j] + (normal1*sd);
            if(j!=this->maindrawingarea->sinogram_size.width()-1) {
                this->maindrawingarea->sinogram[i][j] =this->maindrawingarea->sinogram[i][j] + (normal2*sd);

            }
        }
    }

    //update all widgets
    QList<QMdiSubWindow *> list = this->ui->mdiArea->subWindowList();
    for(int i=0;i<list.count();i++) {
        list[i]->update();
    }
}


void MainWindow::on_btn_step2_clicked()
{
    //load source and detector
    this->maindrawingarea->loadSourceAndDetector(ui->txt_numberOfBins->text().toInt());
    //enable next stage
    ui->btn_step3->setEnabled(true);
}

void MainWindow::on_btn_step3_clicked()
{
    //load movement
    double startAngle = ui->txt_startAngle->text().toDouble();
    double finishAngle = ui->txt_finishAngle->text().toDouble();
    double incrementAngle = ui->txt_angleIncrement->text().toDouble();


    //set up load signal
    connect(this->maindrawingarea,SIGNAL(animationLoaded()),
            this,SLOT(animationLoaded()));

    //load movement
    this->maindrawingarea->visualizeLine = ui->cb_visualizeLine->isChecked();
    int nop = this->maindrawingarea->loadMovement(startAngle,finishAngle,incrementAngle);
    ui->txt_nop->setText(QString::number(nop)); //set nop



}

void MainWindow::on_btn_step4_clicked()
{
    //(1) set up system matrix A
    this->maindrawingarea->setupSystemMatrix();
    this->ui->step4_matrix_row->setText(QString::number(this->maindrawingarea->A_size.height()));
    this->ui->step4_matrix_column->setText(QString::number(this->maindrawingarea->A_size.width()));

    //enable debug buttons
    this->ui->debug_btnpath->setEnabled(true);
    this->ui->debug_showsino->setEnabled(true);
    this->ui->debug_close_button->setEnabled(true);
    this->ui->step5_group->setEnabled(true);
    this->ui->misc_group->setEnabled(true);


}



void MainWindow::on_cb_visualizeLine_toggled(bool checked)
{
    this->maindrawingarea->visualizeLine = checked;
}

void MainWindow::on_debug_btnpath_clicked()
{
    //error checking
    if((this->ui->debug_startmatrix->text().toInt()+this->ui->debug_numberOfMatrix->text().toInt())>=this->ui->step4_matrix_row->text().toInt()) {
        QMessageBox::warning(this,"matrix outside boundary error","matrix is outside boundary!");
        return;
    }
    QString titleArg("Proj(%1), Source(%2)");
    //visualize ray matrix for
    int startMatrix = ui->debug_startmatrix->text().toInt();
    int numberOfMatrix = ui->debug_numberOfMatrix->text().toInt();

    int widgetWidth = (int)(this->maindrawingarea->phantom->width()*(double)5*1.8);
    int widgetHeight = (int)(this->maindrawingarea->phantom->height()*(double)5*1.8);

    for(int i=0;i<numberOfMatrix;i++) {
        int projection_number = (startMatrix+i)/this->maindrawingarea->number_of_source_element;
        int source_number = (startMatrix+i)%this->maindrawingarea->number_of_source_element;

        QPoint source(this->maindrawingarea->source_projections[projection_number][source_number].real(),
                      this->maindrawingarea->source_projections[projection_number][source_number].imag());

        QPoint detector(this->maindrawingarea->detector_projections[projection_number][source_number].real(),
                        this->maindrawingarea->detector_projections[projection_number][source_number].imag());

        QSize matrixSize(this->maindrawingarea->phantom->size());

        //M*N
        double* matrixData = this->maindrawingarea->A[startMatrix+i];

        matrixWidget* drawing = new matrixWidget(source,detector,matrixSize,matrixData);
        //when you add a widget directly to
        //mdi area, a mdisubwindow is automatically
        //created and the widget is set as the central
        //widget of the mdisubwindow
        ui->mdiArea->addSubWindow(drawing);
        drawing->show();
        drawing->setMinimumSize(widgetWidth,widgetHeight);


        ui->mdiArea->currentSubWindow()->adjustSize();
        ui->mdiArea->currentSubWindow()->setWindowTitle(titleArg.arg(QString::number(projection_number),QString::number(source_number)));

    }
}


void MainWindow::on_debug_close_button_clicked()
{
    //TODO
    //delete A
    //delete source_projection
    //delete detector_projection
    //phantom
    //sinogram

    QList<QMdiSubWindow*> list = this->ui->mdiArea->subWindowList();
    for(int i=0;i<list.count();i++) {
        if(list[i]->windowTitle().compare(QString("mainwindow"))!=0) {
            list[i]->close(); //close
        }

    }
}

void MainWindow::on_debug_showsino_clicked()
{
    //show sinogram
    sinogramwidget* drawing = new sinogramwidget(this->maindrawingarea->sinogram,QSize(this->maindrawingarea->number_of_projections,this->maindrawingarea->number_of_detector_element), this->maindrawingarea);

    int widgetWidth = drawing->sinogram_size.width()*2;
    int widgetHeight = drawing->sinogram_size.height()*2;
    //mdi area, a mdisubwindow is automatically
    //created and the widget is set as the central
    //widget of the mdisubwindow
    ui->mdiArea->addSubWindow(drawing);
    drawing->setMinimumSize(widgetWidth*this->maindrawingarea->zoomfactor,
                            widgetHeight*this->maindrawingarea->zoomfactor);
    drawing->show();

    ui->mdiArea->currentSubWindow()->adjustSize();
    ui->mdiArea->currentSubWindow()->setWindowTitle("sinogram");

  //  delete drawing;
}

void MainWindow::on_step5_start_clicked()
{


}

//slot called when art method has intermediate image for u.
void MainWindow::art_update(int iteration) {

    //show image matrix(MxN)
    imageWidget *imwidget = new imageWidget(this->art_buffer,
                                            this->maindrawingarea->phantom->height(),
                                            this->maindrawingarea->phantom->width(),
                                            this->maindrawingarea);

    //set the phantom
    ui->mdiArea->addSubWindow(imwidget);
    imwidget->show();

    //show main window
    if(imwidget->phantom_width<128) imwidget->setMinimumSize(128,128);
    else imwidget->setMinimumSize(imwidget->phantom_width*this->maindrawingarea->zoomfactor,imwidget->phantom_height*this->maindrawingarea->zoomfactor);
    ui->mdiArea->currentSubWindow()->adjustSize();
    ui->mdiArea->currentSubWindow()->setWindowTitle(QString("%1th iteration art").arg(iteration));


}

void MainWindow::art_finished() {
    delete artthread;
    //stop it
    this->ui->step5_art_start_2->setEnabled(true);
    this->ui->step5_art_stop->setEnabled(false);

}

void MainWindow::art_singleiteration(int i,double rs) {
    QString iteration("ART: %1th Iteration is complete! (residual=%2)\n");
    this->ui->txt_output->setText(iteration.arg(QString::number(i),QString::number(rs))
                                  +this->ui->txt_output->toPlainText());


}

//slot called when sirt method has intermediate image for u.
void MainWindow::sirt_update(int iteration) {

    //show image matrix(MxN)
    imageWidget *imwidget = new imageWidget(this->sirt_buffer,
                                            this->maindrawingarea->phantom->height(),
                                            this->maindrawingarea->phantom->width(),
                                            this->maindrawingarea);

    //set the phantom
    ui->mdiArea->addSubWindow(imwidget);
    imwidget->show();

    //show main window
    if(imwidget->phantom_width<128) imwidget->setMinimumSize(128,128);
    else imwidget->setMinimumSize(imwidget->phantom_width,imwidget->phantom_height);
    ui->mdiArea->currentSubWindow()->adjustSize();
    ui->mdiArea->currentSubWindow()->setWindowTitle(QString("%1th iteration art").arg(iteration));

}

void MainWindow::sirt_finished() {
    delete sirtthread;
    //stop it
    this->ui->step5_sirt_start->setEnabled(true);
    this->ui->step5_sirt_stop->setEnabled(false);

}

void MainWindow::sirt_singleiteration(int i,double rs) {
    QString iteration("SIRT: %1th Iteration is complete! (residual=%2)\n");
    this->ui->txt_output->setText(iteration.arg(QString::number(i),QString::number(rs))
                                  +this->ui->txt_output->toPlainText());


}

void MainWindow::on_step5_art_start_clicked()
{

}

void MainWindow::on_step5_art_start_2_clicked()
{
    int sweeps;
    double relaxation;
    int whentoshow;

    //create a thread for doing ART in the background.
    sweeps = this->ui->art_sweep_2->text().toInt();
    relaxation = this->ui->art_relaxation_2->text().toDouble();
    whentoshow = this->ui->art_sw_2->text().toInt();

    artthread = new ArtThread(this->maindrawingarea,sweeps,relaxation,whentoshow);
    artthread->setupReturnBuffer(&art_buffer);

    //connect signal and slots
    connect(artthread, SIGNAL(updateReady(int)), this, SLOT(art_update(int)));
    connect(artthread, SIGNAL(finished()), this,SLOT(art_finished()));
    connect(artthread, SIGNAL(singleiteration(int,double)), this,SLOT(art_singleiteration(int,double)));


    artthread->start();
    this->ui->step5_art_stop->setEnabled(true);
    this->ui->step5_art_start_2->setEnabled(false);

}

void MainWindow::on_step5_art_stop_clicked()
{
    artthread->running = false;
}

void MainWindow::on_step5_sirt_start_clicked()
{
    int sweeps;
    double relaxation;
    int whentoshow;

    //create a thread for doing ART in the background.
    sweeps = this->ui->sirt_sweep->text().toInt();
    relaxation = this->ui->sirt_relaxation->text().toDouble();
    whentoshow = this->ui->sirt_sw->text().toInt();

    sirtthread = new SirtThread(this->maindrawingarea,sweeps,relaxation,whentoshow);
    sirtthread->setupReturnBuffer(&sirt_buffer);

    //connect signal and slots
    connect(sirtthread, SIGNAL(updateReady(int)), this, SLOT(sirt_update(int)));
    connect(sirtthread, SIGNAL(finished()), this,SLOT(sirt_finished()));
    connect(sirtthread, SIGNAL(singleiteration(int,double)), this,SLOT(sirt_singleiteration(int,double)));

    sirtthread->start();

    this->ui->step5_sirt_stop->setEnabled(true);
    this->ui->step5_sirt_start->setEnabled(false);
}

void MainWindow::on_step5_sirt_stop_clicked()
{
    sirtthread->running = false;
}

void MainWindow::on_display_apply_clicked()
{
    this->maindrawingarea->zoomfactor = this->ui->display_zoomfactor->text().toDouble();

    QList<QMdiSubWindow *> list = this->ui->mdiArea->subWindowList();
    for(int i=0;i<list.count();i++) {
        list[i]->update();
    }
}
