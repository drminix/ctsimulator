# ctsimulator
=====================

[X-ray Computed Tomography](http://en.wikipedia.org/wiki/X-ray_computed_tomography) simulator written in C/C++ and QT library.

This software allow you to perform (1) X-ray CT acquisition, (2) System matrix generation va siddon's algorithm, (3) Sinogram generation, (4) Image reconstruction with FBP and SART. This program is written for educational purpose, and freely available under MIT license.  (c) Sanghyeb(Sam) Lee MIT License

# Overview
There are four directories:
- [images]: screenshots
- [phantom]: phantom data
- [source]: C/C++ source code
- [windows-binary]: compiled executable for windows

# Example
When you start a program, the following window will pop up.
![](https://github.com/drminix/ctsimulator/blob/master/images/step0.jpg)

## Load phantom data
Use the following menu to load phantom data. The current version comes with 2 phantom data. You can add your own phantom.
![](https://github.com/drminix/ctsimulator/blob/master/images/step1.jpg)

## Setup up source and detector type
Specify projeciton type and number of bins 
![](https://github.com/drminix/ctsimulator/blob/master/images/step2.jpg)

## Set up detector movement
Specify (1) start angle(degree), (2) finish angle(degree), and (3) incremental angle(degree)
![](https://github.com/drminix/ctsimulator/blob/master/images/step3.jpg)

## Record forward projecito nand system matrix (A)
Specify number of projections and the system matrix(A). System matrix will be generated using siddon's algorithm.
You can also visualize the ray path and show sinogram.
![](https://github.com/drminix/ctsimulator/blob/master/images/step4.jpg)

## Apply reconstruction algorithm
You can use ART, SIRT, MLEM algorithms to reconstruct the phantom. It will visualize the phantom at each iteration
![](https://github.com/drminix/ctsimulator/blob/master/images/step5.jpg)

# Compilation
Use Qt 4.4 library to compile the source code


(c) Sanghyeb(Sam) Lee MIT License
