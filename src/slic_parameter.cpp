#include "slic_parameter.h"
#include <iostream>

SlicParameter::SlicParameter()
    : QObject(),
      _algo(cv::ximgproc::SLIC),
     _region_size(100),
     _ruler(25.0),
     _connectivity(50),
     _iterations(8),
     _display_mode(0)
{

}

void SlicParameter::algorithmChanged(int choice)
{
    switch (choice)
    {
    case 0: _algo = cv::ximgproc::SLIC;  break;
    case 1: _algo = cv::ximgproc::SLICO; break;
    case 2: _algo = cv::ximgproc::MSLIC; break;
    default: _algo = cv::ximgproc::SLIC; ;break;
    }

}

void SlicParameter::regionSizeChanged(int region_size)
{
    _region_size = region_size;
}

void SlicParameter::rulerChanged(double ruler)
{
    _ruler = ruler;
}

void SlicParameter::connectivityChanged(int connectivity)
{
    _connectivity = connectivity;
}

void SlicParameter::iterationsChanged(int number_of_iterations)
{
    _iterations = number_of_iterations;
}

void SlicParameter::displayModeChanged(int display_mode)
{
    _display_mode = display_mode;
}
