#include "label_widget.h"
#include "utils.h"

#include <QDebug>
#include <QPainter>

LabelWidget::LabelWidget(const LabelInfo &label, QWidget *parent , Qt::WindowFlags f ) :
	QLabel(parent, f) {
	setNewLabel(label);
	_is_selected = false;
}


void LabelWidget::setNewLabel(const LabelInfo &label) {
	_label = label;
	setText(label.name);
	setAlignment(Qt::AlignHCenter);
	setStyleSheet("QLabel { background-color : " + label.color.name() + "; color : " + invColor(_label.color).name() + ";  font: bold 14px; }");
}

void LabelWidget::setSelected(bool s) {
	_is_selected = s;
	repaint();
}

void LabelWidget::paintEvent(QPaintEvent *event) {
	QLabel::paintEvent(event);
	if (_is_selected) {
		QColor inv = invColor(_label.color);
		QPainter p(this);
		QPen pen(inv);
		pen.setWidth(4);
		p.setPen(pen);
		p.setBrush(QBrush(inv));
		p.setOpacity(0.3);
		p.drawRect(rect());
		p.end();
	}
}