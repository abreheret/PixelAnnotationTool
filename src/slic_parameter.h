#ifndef SLIC_PARAMETER_H
#define SLIC_PARAMETER_H

#include <qobject.h>
#include <opencv2/ximgproc/slic.hpp>

class SlicParameter : public QObject {
    Q_OBJECT

public:
    SlicParameter();
    virtual ~SlicParameter() {};

    int algorithmSet()    const  {return _algo;}         //SLIC(0),SLICO(1),MSLIC(2)
    int regionSizeSet()   const  {return _region_size ;}
    double rulerSet()     const  {return _ruler       ;}
    int connectivitySet() const  {return _connectivity;}
    int iterationsSet()   const  {return _iterations  ;}
    int displayModeSet()  const  {return _display_mode;}



public slots :
    void algorithmChanged(int);
    void regionSizeChanged(int);
    void rulerChanged(double);
    void connectivityChanged(int);
    void iterationsChanged(int);
    void displayModeChanged(int);

private:
    cv::ximgproc::SLICType _algo  ;
    int              _region_size ;
    double           _ruler       ;
    int              _connectivity;
    int              _iterations  ;
    int              _display_mode;
};

#endif // SLIC_PARAMETER_H
