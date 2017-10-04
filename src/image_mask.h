#ifndef IMAGE_MASK_H
#define IMAGE_MASK_H

#include <QImage>
#include "labels.h"

struct  ColorMask {
	QColor id;
	QColor color;
};

struct ImageMask {
	QImage id;
	QImage color;

	ImageMask();
	ImageMask(const QString &file, Id2Labels id_labels);
	ImageMask(QSize s);

	void drawFillCircle(int x, int y, int pen_size, ColorMask cm);
	void drawPixel(int x, int y, ColorMask cm);
	void updateColor(const Id2Labels & labels);
};

#endif