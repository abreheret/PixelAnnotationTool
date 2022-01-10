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

    void drawLine(QPoint p1, QPoint p2, int pen_size, ColorMask cm);
    void drawFillCircle(QPoint point, int pen_size, ColorMask cm);
	void updateColor(const Id2Labels & labels);
	void exchangeLabel(int x, int y, const Id2Labels & id_labels, ColorMask cm);
};

#endif
