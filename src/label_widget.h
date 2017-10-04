#ifndef LABEL_WIDGET_H 
#define LABEL_WIDGET_H

#include <QLabel>
#include "labels.h"


class LabelWidget : public QLabel {
	Q_OBJECT

	bool      _is_selected;
	LabelInfo _label      ;

public:

	LabelWidget(const LabelInfo &label, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

	
	void setNewLabel(const LabelInfo &label);
	void setSelected(bool s);

public : // override
	void paintEvent(QPaintEvent *event) override;

};


#endif //LABEL_WIDGET_H