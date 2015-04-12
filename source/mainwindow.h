/****************************
 * Author: Sanghyeb(Sam) Lee
 * Date: Jan/2013
 * email: drminix@gmail.com
 * Copyright 2013 Sang hyeb(Sam) Lee MIT License
 * 
 * X-ray CT simulation & reconstruction 
 *****************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include<QFileDialog>
#include<QTextEdit>
#include<QMdiSubWindow>
#include<cmath>
#include "utility.h"
#include "drawingarea.h"
#include "matrixwidget.h"
#include "sinogramwidget.h"
#include "artthread.h"
#include "sirtthread.h"

#include "imagewidget.h"

#define PI 3.14159265

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_phantomExplore_clicked();

    void on_phantomLoad_clicked();

    void on_pushButton_clicked();

    void on_btn_step2_clicked();

    void on_btn_step3_clicked();

    void on_btn_step4_clicked();

    void on_cb_visualizeLine_toggled(bool checked);

    void on_debug_btnpath_clicked();

    void on_debug_close_button_clicked();

    void on_debug_showsino_clicked();

    void on_step5_start_clicked();

    void on_step5_art_start_clicked();

    //art
    void art_finished();
    void art_update(int iteration);
    void art_singleiteration(int iteration, double rs);

    //sirt
    void sirt_finished();
    void sirt_update(int iteration);
    void sirt_singleiteration(int iteration, double rs);

    void on_step5_art_start_2_clicked();

    void on_step5_art_stop_clicked();

    //step3 animation finished
    void animationLoaded();

    void on_step5_sirt_start_clicked();

    void on_step5_sirt_stop_clicked();

    void on_display_apply_clicked();

private:
    Ui::MainWindow *ui;
    utility* myutility;
    drawingarea* maindrawingarea;
    //art
    ArtThread* artthread;
    double* art_buffer;
    //sirt
    SirtThread* sirtthread;
    double* sirt_buffer;

public:

};

#endif // MAINWINDOW_H
