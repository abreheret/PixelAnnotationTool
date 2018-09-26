#include "image_mask.h"
#include "utils.h"

#include <QPainter>

ImageMask::ImageMask() {}
ImageMask::ImageMask(const QString &file, Id2Labels id_labels) {
	id = mat2QImage(cv::imread(file.toStdString()));
	color = idToColor(id, id_labels);
}
ImageMask::ImageMask(QSize s) {
	id = QImage(s, QImage::Format_RGB888);
	color = QImage(s, QImage::Format_RGB888);
	id.fill(QColor(0, 0, 0));
	color.fill(QColor(0, 0, 0));
}

void ImageMask::drawFillCircle(int x, int y, int pen_size, ColorMask cm) {
	QPen pen(QBrush(cm.id), 1.0);
	QPainter painter_id(&id);
	painter_id.setRenderHint(QPainter::Antialiasing, false);
	painter_id.setPen(pen);
	painter_id.setBrush(QBrush(cm.id));
	painter_id.drawEllipse(x, y, pen_size, pen_size);
	painter_id.end();

	QPainter painter_color(&color);
	QPen pen_color(QBrush(cm.color), 1.0);
	painter_color.setRenderHint(QPainter::Antialiasing, false);
	painter_color.setPen(pen_color);
	painter_color.setBrush(QBrush(cm.color));
	painter_color.drawEllipse(x, y, pen_size, pen_size);
	painter_color.end();
}

void ImageMask::drawPixel(int x, int y, ColorMask cm) {
	id.setPixelColor(x, y, cm.id);
	color.setPixelColor(x, y, cm.color);
}

void ImageMask::updateColor(const Id2Labels & labels) {
	idToColor(id, labels, &color);
}

void ImageMask::exchangeLabel(int x, int y, const Id2Labels& id_labels, ColorMask cm) {

	QColor current_id = id.pixelColor(QPoint(x, y));
	if (current_id.red() == 0 || current_id.green() == 0 || current_id.blue() == 0)
		return;

	cv::Mat id_mat = qImage2Mat(id);
	cv::floodFill(id_mat, cv::Point(x, y), cv::Scalar(cm.id.red(), cm.id.green(), cm.id.blue()), 0, cv::Scalar(0, 0, 0), cv::Scalar(0, 0, 0));

	id = mat2QImage(id_mat);
	color = idToColor(id, id_labels);

}