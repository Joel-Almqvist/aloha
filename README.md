# Aloha protocol in C++
by Joel Almqvist (joeal360) for TSIN01

## Intro
This project is an C++ implementation of the Aloha-protocol, both the vanilla version and the Bayesian-stabilization version.


## Execution
To run this code install all the dependencies and write "make" in the terminal to generate the executable "aloha" file. In aloha.cpp there are many constants at the top which change the behavior of the code, such as arrival rate and whether to generate graphs or not.

The default behavior is to generate graphs under the subfolder plots, the values of the constants used to create a graph is in its name.

## Dependencies
This project requires C++11, Python 2.7 and certain Python libraries used by the matplotlib C++ wrapper. Notably matplotlib, numpy and the dev-version of python are all required, but check https://github.com/lava/matplotlib-cpp to find all subdependencies.

## Acknowledgments
I did not write anything in matplotlibcpp.h, it is simply a dependency I added to make the installation easier. The file is copied from https://github.com/lava/matplotlib-cpp
