PixelAnnotationTool
============================

Software that allows you to manually and quickly annotate images in directories.
The method is pseudo manual because it uses the algorithm [watershed marked](http://docs.opencv.org/3.1.0/d7/d1b/group__imgproc__misc.html#ga3267243e4d3f95165d55a618c65ac6e1) of OpenCV. The general idea is to manually provide the marker with brushes and then to launch the algorithm. Then to correct the errors, we are adding new markers on the erroneous borders and to relaunch the algorithm. This method is illustrated by the annimation gif below :

![gif_file](giphy.gif)

----------

### Building Dependencies
* [Qt](https://www.qt.io/download-open-source/)  >= 5.x
* [CMake](https://cmake.org/download/) >= 2.8.x 
* [OpenCV](http://opencv.org/releases.html) >= 2.4.x 
* For Windows Compiler : Works under Visual Studio >= 2015

How to build go to [here](scripts_to_build/README.md)

### License :

GNU Lesser General Public License v3.0 

Permissions of this copyleft license are conditioned on making available complete source code of licensed works and modifications under the same license or the GNU GPLv3. Copyright and license notices must be preserved. Contributors provide an express grant of patent rights. However, a larger work using the licensed work through interfaces provided by the licensed work may be distributed under different terms and without source code for the larger work.
[more](https://github.com/abreheret/PixelAnnotationTool/blob/master/LICENSE)
