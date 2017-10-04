#include "labels.h"
#include "utils.h"

#include <QListWidgetItem>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardItemModel>
#include <QColormap>
#include <QDebug>

LabelInfo::LabelInfo() {
	this->name = "unlabeled";
	this->categorie = "void";
	this->id = 0;
	this->id_categorie = 0;
	this->color = QColor(0, 0, 0);
	item = NULL;
}

LabelInfo::LabelInfo(QString name, QString categorie, int id, int id_categorie, QColor color) {
	this->name = name;
	this->categorie = categorie;
	this->id = id;
	this->id_categorie = id_categorie;
	this->color = color;
	item = NULL;
}

void LabelInfo::read(const QJsonObject &json) {
	id = json["id"].toInt();
	name = json["name"].toString();
	categorie = json["categorie"].toString();
	id_categorie = json["id_categorie"].toInt();
	QJsonArray jarray = json["color"].toArray();
	int r = jarray[0].toInt();
	int g = jarray[1].toInt();
	int b = jarray[2].toInt();
	color = QColor(r, g, b);
}

void LabelInfo::write(QJsonObject &json) const {
	json["id"] = id;
	json["name"] = name;
	json["categorie"] = categorie;
	json["id_categorie"] = id_categorie;
	QJsonArray jarray;
	jarray.append(color.red());
	jarray.append(color.green());
	jarray.append(color.blue());
	json["color"] = jarray;
}


void Name2Labels::read(const QJsonObject &json) {
	QJsonObject array = json["labels"].toObject();
	for (QJsonObject::iterator it = array.begin(); it != array.end(); it++) {
		QJsonObject object = it->toObject();
		LabelInfo label;
		label.read(object);
		(*this)[label.name] = label;
	}
}
void Name2Labels::write(QJsonObject &json) const {
	QMapIterator<QString, LabelInfo> it(*this);
	QJsonObject object;
	while (it.hasNext()) {
		it.next();
		const LabelInfo & label = it.value();
		QJsonObject child_object;
		label.write(child_object);
		object[it.key()] = child_object;
	}
	json["labels"] = object;
}


Id2Labels getId2Label(const Name2Labels& labels) {
	Id2Labels id_labels;
	QMapIterator<QString, LabelInfo> i(labels);
	while (i.hasNext()) {
		i.next();
		id_labels[i.value().id] = &i.value();
	}
	return id_labels;
}

Name2Labels defaulfLabels() {
	Name2Labels labels;
	// inspired by https://raw.githubusercontent.com/mcordts/cityscapesScripts/master/cityscapesscripts/helpers/labels.py
	labels["unlabeled"           ] = LabelInfo("unlabeled"           , "void"         ,  00, 0, QColor(  0,  0,  0));
	labels["ego vehicle"         ] = LabelInfo("ego vehicle"         , "void"         ,  01, 0, QColor(  0,  0,  0));
	labels["rectification border"] = LabelInfo("rectification border", "void"         ,  02, 0, QColor(  0,  0,  0));
	labels["out of roi"          ] = LabelInfo("out of roi"          , "void"         ,  03, 0, QColor(  0,  0,  0));
	labels["static"              ] = LabelInfo("static"              , "void"         ,  04, 0, QColor( 10,  0,  0));
	labels["dynamic"             ] = LabelInfo("dynamic"             , "void"         ,  05, 0, QColor(111, 74,  0));
	labels["ground"              ] = LabelInfo("ground"              , "void"         ,  06, 0, QColor( 81,  0, 81));
	labels["road"                ] = LabelInfo("road"                , "flat"         ,  07, 1, QColor(128, 64,128));
	labels["sidewalk"            ] = LabelInfo("sidewalk"            , "flat"         ,   8, 1, QColor(244, 35,232));
	labels["parking"             ] = LabelInfo("parking"             , "flat"         ,   9, 1, QColor(250,170,160));
	labels["rail track"          ] = LabelInfo("rail track"          , "flat"         ,  10, 1, QColor(230,150,140));
	labels["building"            ] = LabelInfo("building"            , "construction" ,  11, 2, QColor( 70, 70, 70));
	labels["wall"                ] = LabelInfo("wall"                , "construction" ,  12, 2, QColor(102,102,156));
	labels["fence"               ] = LabelInfo("fence"               , "construction" ,  13, 2, QColor(190,153,153));
	labels["guard rail"          ] = LabelInfo("guard rail"          , "construction" ,  14, 2, QColor(180,165,180));
	labels["bridge"              ] = LabelInfo("bridge"              , "construction" ,  15, 2, QColor(150,100,100));
	labels["tunnel"              ] = LabelInfo("tunnel"              , "construction" ,  16, 2, QColor(150,120, 90));
	labels["pole"                ] = LabelInfo("pole"                , "object"       ,  17, 3, QColor(153,153,153));
	labels["polegroup"           ] = LabelInfo("polegroup"           , "object"       ,  18, 3, QColor(153,153,153));
	labels["traffic light"       ] = LabelInfo("traffic light"       , "object"       ,  19, 3, QColor(250,170, 30));
	labels["traffic sign"        ] = LabelInfo("traffic sign"        , "object"       ,  20, 3, QColor(220,220,  0));
	labels["vegetation"          ] = LabelInfo("vegetation"          , "nature"       ,  21, 4, QColor(107,142, 35));
	labels["terrain"             ] = LabelInfo("terrain"             , "nature"       ,  22, 4, QColor(152,251,152));
	labels["sky"                 ] = LabelInfo("sky"                 , "sky"          ,  23, 5, QColor( 70,130,180));
	labels["person"              ] = LabelInfo("person"              , "human"        ,  24, 6, QColor(220, 20, 60));
	labels["rider"               ] = LabelInfo("rider"               , "human"        ,  25, 6, QColor(255,  0,  0));
	labels["car"                 ] = LabelInfo("car"                 , "vehicle"      ,  26, 7, QColor(  0,  0,142));
	labels["truck"               ] = LabelInfo("truck"               , "vehicle"      ,  27, 7, QColor(  0,  0, 70));
	labels["bus"                 ] = LabelInfo("bus"                 , "vehicle"      ,  28, 7, QColor(  0, 60,100));
	labels["caravan"             ] = LabelInfo("caravan"             , "vehicle"      ,  29, 7, QColor(  0,  0, 90));
	labels["trailer"             ] = LabelInfo("trailer"             , "vehicle"      ,  30, 7, QColor(  0,  0,110));
	labels["train"               ] = LabelInfo("train"               , "vehicle"      ,  31, 7, QColor(  0, 80,100));
	labels["motorcycle"          ] = LabelInfo("motorcycle"          , "vehicle"      ,  32, 7, QColor(  0,  0,230));
	labels["bicycle"             ] = LabelInfo("bicycle"             , "vehicle"      ,  33, 7, QColor(119, 11, 32));

	labels["road marking"        ] = LabelInfo("road marking"        , "flat"         ,  34, 1, QColor(250,250,250));
	labels["other"               ] = LabelInfo("other"               , "void"         ,  35, 0, QColor(128,128,128));
	//labels["license plate"       ] = LabelInfo("license plate"       , "vehicle"      ,  -1, 7, QColor(  0,  0,142));

	//QVector<QColor> cmap = colorMap(labels.size());
	//qDebug() << "cmlap size = " << cmap.size();
	//QMapIterator<QString, LabelInfo> it(labels);
	//int i = 0;
	//while (it.hasNext()) {
	//	it.next();
	//	labels[it.key()].color = cmap[i++];
	//}
	//labels["unlabeled"].color = QColor(0, 0, 0);
	
	return labels;

}