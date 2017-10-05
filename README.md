PixelAnnotationTool
============================

Software that allows you to manually and quickly annotate images in directories.
The method is pseudo manual because it uses the algorithm [watershed marked](http://docs.opencv.org/3.1.0/d7/d1b/group__imgproc__misc.html#ga3267243e4d3f95165d55a618c65ac6e1) of OpenCV. The general idea is to manually provide the marker with brushes and then to launch the algorithm. If at first pass the segmentation needs to be corrected, the user can refine the markers by drawing new ones on the erroneous areas (as show on video below).

[![gif_file](giphy.gif)](https://youtu.be/wxi2dInWDnI)

----------

### Building Dependencies :
* [Qt](https://www.qt.io/download-open-source/)  >= 5.x
* [CMake](https://cmake.org/download/) >= 2.8.x 
* [OpenCV](http://opencv.org/releases.html) >= 2.4.x 
* For Windows Compiler : Works under Visual Studio >= 2015

How to build go to [here](scripts_to_build)

### Download binaries :
Go to release [page](https://github.com/abreheret/PixelAnnotationTool/releases)

### Donate :
If you like, donate !

Donating is very simple - and secure. Please click [here](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=8K79VKWBS7352) to make a donation. 

**Thank you!**

Your donation will help me to maintain and update PixelAnnotationTool.

### License :

GNU Lesser General Public License v3.0 

Permissions of this copyleft license are conditioned on making available complete source code of licensed works and modifications under the same license or the GNU GPLv3. Copyright and license notices must be preserved. Contributors provide an express grant of patent rights. However, a larger work using the licensed work through interfaces provided by the licensed work may be distributed under different terms and without source code for the larger work.

[more](https://github.com/abreheret/PixelAnnotationTool/blob/master/LICENSE)
