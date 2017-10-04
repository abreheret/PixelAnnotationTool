/**
 * Image Annotation Tool for image annotations with pixelwise masks
 *
 * Author: Rudra Poudel
 */
#include <QApplication>
#include <QFile>
#include "main_window.h"
#include <QtDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	MainWindow win;
	win.show();

    return app.exec();
}
