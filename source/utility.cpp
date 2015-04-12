/****************************
 * Author: Sanghyeb(Sam) Lee
 * Date: Jan/2013
 * email: drminix@gmail.com
 * Copyright 2013 Sang hyeb(Sam) Lee MIT License
 * 
 * X-ray CT simulation & reconstruction 
 *****************************/

#include "utility.h"

utility::utility()
{
}

//load phantom
matrix* utility::loadPhantom(QString filename) {
    int row;
    int column;
    //open file
    QFile inputfile(filename);
    inputfile.open(QFile::ReadOnly);
    QTextStream inputstream(&inputfile);

    QString line;
    QStringList list;
    row=0;
    column=0;
    int temp;

    //work out the information
    while(!inputstream.atEnd()) { //before reaching the end of stream
        line = inputstream.readLine(); //read line
        if(line==NULL) continue;
        row++;

        list = line.split('\t');
        temp = list.count();

        if(column==0) column = temp; //set column for the first time
        else if(column!=temp) { //inconsistent column size!
            return NULL;
        }

    }


    //initialize matrix data structure
    double* tempData = new double[row*column];
    matrix* tempM = new matrix(tempData,row,column);
    int data;

    //now read the information
    inputfile.seek(0);
    for(int i=0;i<tempM->M;i++) {
        line = inputstream.readLine(); //read line
        list = line.split('\t');

        for(int j=0;j<tempM->N;j++) {
            double temp = list[j].toDouble();
            int index = i*tempM->N + j;
            tempM->data[index] = temp; //assign data
        }
    }

    return tempM; //return the matrix data
}


double* utility::siddon_ray_trace_2d(double x_s,double y_s,double x_d,double y_d, double M, double N) {
    double tolerance = 0.00000001; //used to determine horizontl and vertical line
    vector<pair<double,double>>* crossing_points;
    vector<pair<double,double>>* line_indicies;
    vector<pair<double,double>>* crossing_points_horizontal;
    vector<pair<double,double>>* crossing_points_vertical;

    double ray_length;
    double cos_angle_ray_h;

    double* rays = new double[(int)(M*N)];
    memset(rays,0,sizeof(double)*M*N);

    if(fabs(x_s) < N/2 && fabs(y_s)<M/2) {
        std::cout<<"Error: source inside the grid"<<std::endl;
        return NULL;
    }else if(fabs(x_d) <N/2 && fabs(y_d) <M/2) {
        std::cout<<"Error: detector inside the grid"<<std::endl;
        return NULL;
    }

    double Lx = N;
    double x_sd = x_d - x_s; 
    double y_sd = y_d - y_s;



    if(y_sd<0) {
        y_sd = -y_sd;
        x_sd = -x_sd;
    }

    ray_length = sqrt(x_sd*x_sd + y_sd*y_sd);
    cos_angle_ray_h = (x_sd*Lx) / (ray_length*fabs(Lx));
    crossing_points = new vector<pair<double,double>>(); //crossing points
    line_indicies   = new vector<pair<double,double>>(); //line indicies(vertical line index, horizontal index) (x,y)
    crossing_points_horizontal = new vector<pair<double,double>>(); //crossing points horizontal
    crossing_points_vertical = new vector<pair<double,double>>(); //crossing points vertical

    //(1) vertical line
    if(fabs(cos_angle_ray_h)<tolerance) { // ang_ray_h ~ 90 degree
        if(x_s<-N/2 || x_s>N/2) {
            crossing_points->clear();
        }else {
            for(double i=(-M/2);i<=(M/2);i++) {
                pair<double,double> point(x_s,i);
                crossing_points->push_back(point);
            }

            int nr=0;
            for(int i=(-M/2);i<=(M/2);i++) {
                if(x_s>=i) { //bigger
                    nr++;
                }else {//else stop
                    break;
                }
            }
            if(nr==M+1) nr=M;
            nr=nr-1;
            for(double y=0;y<M+1;y++) {
                pair<double,double> point((double)nr,(double)y); 
                line_indicies->push_back(point); 
            }


        }

    //(2) horizontal line
    }else if(fabs(cos_angle_ray_h)>1-tolerance) { 
        if(y_s<-M/2 || y_s>M/2) {
            crossing_points->clear();
        }else {
            for(double i=(-N/2);i<=(N/2);i++) {
                pair<double,double> point(i,y_s);
                crossing_points->push_back(point);
            }
            int nr=0;
            for(double i=(-N/2);i<=(N/2);i++) {
                if(y_s>=i) { //bigger
                    nr++;
                }else {// else stop
                    break;
                }
            }
            if(nr==N+1) nr=N;
            nr=nr-1; //index starts from 0
            for(double x=0;x<(N+1);x++) {
                pair<double,double> point((double)x,(double)nr);
                line_indicies->push_back(point); //add point to the vector
            }

        }
    //(3) all other lines
    }else {
        double angle_ray_h = acos(cos_angle_ray_h); //angle between ray and horizontal line
        double tan_angle_ray_h = tan(angle_ray_h); //tan0 = o/a = slope

        for(double y=(-M/2);y<=(M/2);y++) {
            double x = ((y - y_s) + tan_angle_ray_h*x_s)/tan_angle_ray_h;
            pair<double,double> point(x,y);
            crossing_points_horizontal->push_back(point);
        }

        for(double x=(-N/2);x<=(N/2);x++) {
            double y = (tan_angle_ray_h*x) + (y_s - tan_angle_ray_h *x_s);
            pair<double,double> point(x,y);
            crossing_points_vertical->push_back(point);
        }

        vector<pair<double,double>>::iterator iterator_cross_horizontal;
        vector<pair<double,double>>::iterator iterator_cross_vertical;
        vector<pair<double,double>>::iterator iterator_cross_vertical_end;

        iterator_cross_horizontal = crossing_points_horizontal->begin();
        iterator_cross_vertical   = crossing_points_vertical->begin();

        if(tan_angle_ray_h<0) {
            //TODO
            iterator_cross_vertical_end = crossing_points_vertical->end();
            std::reverse(iterator_cross_vertical,iterator_cross_vertical_end);
        }

        double doubles=0; //same point counter
        int line_indicies_vertical=0;
        int line_indicies_horizontal=0;

        int deb1 = crossing_points_vertical->size();
        int deb2 = crossing_points_horizontal->size();

        while(iterator_cross_vertical!=crossing_points_vertical->end()&&
              iterator_cross_horizontal!=crossing_points_horizontal->end()) {
            //if they crosses the vertical and horizontal axis in the same points
            if(fabs((*iterator_cross_horizontal).second -
                   (*iterator_cross_vertical).second) < std::numeric_limits<double>::epsilon()) {

                crossing_points->push_back((*iterator_cross_horizontal));
                line_indicies->push_back(pair<double,double>(line_indicies_horizontal,line_indicies_vertical));
                iterator_cross_horizontal++;
                iterator_cross_vertical++;
                line_indicies_vertical++;
                line_indicies_horizontal++;

                doubles++;

            }else if((*iterator_cross_horizontal).second<
                      (*iterator_cross_vertical).second) {
    
                crossing_points->push_back((*iterator_cross_horizontal));
                line_indicies->push_back(pair<double,double>(line_indicies_vertical-1, line_indicies_horizontal));
                iterator_cross_horizontal++;
                line_indicies_horizontal++;


            }else {

                crossing_points->push_back((*iterator_cross_vertical));
                line_indicies->push_back(pair<double,double>(line_indicies_vertical, line_indicies_horizontal-1));
                iterator_cross_vertical++;
                line_indicies_vertical++;

            }
        }

        //appending the remaining points
        //(CASE-1) append remaining horizontal points
            int count=0;
            if(iterator_cross_horizontal!=crossing_points_horizontal->end()) {
                for(;iterator_cross_horizontal!=crossing_points_horizontal->end(); iterator_cross_horizontal++,
                    line_indicies_horizontal++) {
                    //add point to the list
                    count++;
                    crossing_points->push_back((*iterator_cross_horizontal));
                    line_indicies->push_back(pair<double,double>(line_indicies_vertical-1,line_indicies_horizontal));

                }
            }
            //(CASE-2) append remaining vertical points
            if(iterator_cross_vertical!=crossing_points_vertical->end()) {
                for(;iterator_cross_vertical!=crossing_points_vertical->end();  iterator_cross_vertical++,
                    line_indicies_vertical++) {
                    //add point to the list
                    crossing_points->push_back((*iterator_cross_vertical));
                    line_indicies->push_back(pair<double,double>(line_indicies_vertical,line_indicies_horizontal-1));

                    //advance to the next point
                }
         }



        //error checking
        if(crossing_points->size()!=line_indicies->size()) {
            std::cout<<"ERROR: crossing_points size != lin_indicies_size"<<std::endl;
        }

        //(step3) discard crossing outside of the grid
        vector<pair<double,double>>::iterator iterator_cross=crossing_points->begin();
        vector<pair<double,double>>::iterator iterator_line = line_indicies->begin();

        while(iterator_cross!=crossing_points->end()) {
            //check if outsides boundaries
            if((*iterator_cross).first<-N/2-std::numeric_limits<double>::epsilon()||
               (*iterator_cross).first>N/2+std::numeric_limits<double>::epsilon() ||
               (*iterator_cross).second<-M/2-std::numeric_limits<double>::epsilon()||
               (*iterator_cross).second>M/2+std::numeric_limits<double>::epsilon()) {
                //outside!!

                //remove
                iterator_cross = crossing_points->erase(iterator_cross);
                iterator_line = line_indicies->erase(iterator_line);
            }else {
                //inside!!

                //move to the next element
                iterator_cross++;
                iterator_line++;

            }

        }


        //downwards rays were reversed in the beginning
        //correct the vertical line numbering
        if(tan_angle_ray_h<0) {
           //adjust the numbers of vertical line which were numbered in the discending direction
            for(iterator_line = line_indicies->begin();iterator_line!=line_indicies->end();iterator_line++) {
                (*iterator_line).first = N - (*iterator_line).first;
                // line_ind(:,1) = N+1 - line_ind(:,1);
            }
        }


    }

    if(!crossing_points->empty()) { //if not empty
    //distance between consecutive crossing points.
        vector<pair<double,double>>::const_iterator current_iterator;
        vector<pair<double,double>>::const_iterator previous_iterator;
        vector<pair<double,double>>::const_iterator line_index;

        previous_iterator = crossing_points->begin();
        line_index = line_indicies->begin();
        int j=0; //iterate through all line_indicies
        for(current_iterator=crossing_points->begin()+1;current_iterator!=crossing_points->end();previous_iterator=current_iterator,current_iterator++) { //go through all the elements
            pair<double,double> distance_vector = pair<double,double>((*current_iterator).first - (*previous_iterator).first,
                                                                      (*current_iterator).second - (*previous_iterator).second);
            double distance = sqrt(distance_vector.first*distance_vector.first + distance_vector.second * distance_vector.second);
            //rays[line_indicies(j,2)][line_indicies(j,1)] = v_len(j)
            rays[(int)((*line_index).second * N + (*line_index).first)] = distance;
            line_index++;
        }


    }

    delete crossing_points;
    delete line_indicies;
    delete crossing_points_vertical;
    delete crossing_points_horizontal;

    return rays;
}
