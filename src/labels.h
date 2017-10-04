#ifndef LABELS_H
#define LABELS_H

#include <QListWidgetItem>
#include <QJsonObject>

class LabelInfo  {
public:
	QString name         ;
	QString categorie    ;
	int     id           ;
	int     id_categorie ;
	QColor  color        ;
	QListWidgetItem *item;
	LabelInfo();
	LabelInfo(QString name, QString categorie, int id, int id_categorie, QColor color);
	void read(const QJsonObject &json);
	void write(QJsonObject &json) const;
};

class Name2Labels : public QMap<QString, LabelInfo> {
public:
	void read(const QJsonObject &json);
	void write(QJsonObject &json) const;
};

class Id2Labels : public QMap<int,const LabelInfo*> { };

Id2Labels getId2Label(const Name2Labels& labels);
Name2Labels defaulfLabels();

#endif