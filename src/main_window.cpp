#include "main_window.h"
#include "utils.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QStringList>
#include <QMessageBox>
#include <QJsonDocument>
#include <QPainter>
#include <QJsonObject>
#include <QJsonArray>
#include <QColorDialog>
#include <QTextStream>
#include "pixel_annotation_tool_version.h"

#include "about_dialog.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{
	setupUi(this);
	setWindowTitle(QApplication::translate("MainWindow", "PixelAnnotationTool " PIXEL_ANNOTATION_TOOL_GIT_TAG, Q_NULLPTR));
	list_label->setSpacing(1);

	scroll_area = new QScrollArea;
	image_canvas = new ImageCanvas(scroll_area, this);
	save_action = new QAction(tr("&Save current image"), this);
	undo_action = new QAction(tr("&Undo"), this);
	redo_action = new QAction(tr("&Redo"), this);
	undo_action->setShortcuts(QKeySequence::Undo);
	redo_action->setShortcuts(QKeySequence::Redo);
	save_action->setShortcut(Qt::CTRL+Qt::Key_S);
	undo_action->setEnabled(false);
	redo_action->setEnabled(false);

	menuFile->addAction(save_action);
	menuEdit->addAction(undo_action);
	menuEdit->addAction(redo_action);

	image_canvas->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	image_canvas->setScaledContents(true);

	scroll_area->setBackgroundRole(QPalette::Dark);
	scroll_area->setWidget(image_canvas);

	setCentralWidget(scroll_area);

	connect(spinbox_scale, SIGNAL(valueChanged(double)), image_canvas, SLOT(scaleChanged(double)));
	connect(spinbox_alpha, SIGNAL(valueChanged(double)), image_canvas, SLOT(alphaChanged(double)));
	connect(spinbox_pen_size, SIGNAL(valueChanged(int)), image_canvas, SLOT(setSizePen(int)));
	connect(button_watershed, SIGNAL(released()), this, SLOT(runWatershed()));
	connect(checkbox_watershed_mask, SIGNAL(clicked()), image_canvas, SLOT(update()));
	connect(checkbox_manuel_mask, SIGNAL(clicked()), image_canvas, SLOT(update()));
	connect(actionClear, SIGNAL(triggered()), image_canvas, SLOT(clearMask()));
		
	connect(actionOpen_config_file, SIGNAL(triggered()), this, SLOT(loadConfigFile()));
	connect(actionSave_config_file, SIGNAL(triggered()), this, SLOT(saveConfigFile()));

	connect(undo_action, SIGNAL(triggered()), image_canvas, SLOT(undo()));
	connect(redo_action, SIGNAL(triggered()), image_canvas, SLOT(redo()));
	connect(save_action, SIGNAL(triggered()), image_canvas, SLOT(saveMask()));

	labels = defaulfLabels();

	loadConfigLabels();

	connect(list_label, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(changeLabel(QListWidgetItem*, QListWidgetItem*)));
	connect(list_label, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(changeColor(QListWidgetItem*)));

	
}
void MainWindow::loadConfigLabels() {
	list_label->clear();
	QMapIterator<QString, LabelInfo> it(labels);
	while (it.hasNext()) {
		it.next();
		const LabelInfo & label = it.value();
		QListWidgetItem * item = new QListWidgetItem(list_label);
		LabelWidget * label_widget = new LabelWidget(label,this);
		
		item->setSizeHint(label_widget->sizeHint());
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		list_label->addItem(item);
		list_label->setItemWidget(item, label_widget);
		labels[it.key()].item = item;
	}
	id_labels = getId2Label(labels);
}

void MainWindow::changeColor(QListWidgetItem* item) {
	LabelWidget * widget = static_cast<LabelWidget*>(list_label->itemWidget(item));
	LabelInfo & label = labels[widget->text()];
	QColor color = QColorDialog::getColor(label.color, this);
	if (color.isValid()) {
		label.color = color;
		widget->setNewLabel(label);
	}
	image_canvas->setId(label.id);
	image_canvas->updateMaskColor(id_labels);
	image_canvas->refresh();
}

void MainWindow::changeLabel(QListWidgetItem* current, QListWidgetItem* previous) {
	if (current == NULL && previous == NULL)
		return;

	LabelWidget * label;
	if (previous == NULL) {
		for (int i = 0; i < list_label->count(); i++) {
			LabelWidget * label = static_cast<LabelWidget*>(list_label->itemWidget(list_label->item(i)));
			label->setSelected(false);
		}
	} else {
		label = static_cast<LabelWidget*>(list_label->itemWidget(previous));
		label->setSelected(false);
	}

	if (current == NULL) current = previous;

	label = static_cast<LabelWidget*>(list_label->itemWidget(current));
	label->setSelected(true);

	QString str;
	QString key = label->text();
	QTextStream sstr(&str);
	sstr <<"label=["<< key <<"] id=[" << labels[key].id << "] categorie=[" << labels[key].categorie << "] color=[" << labels[key].color.name() << "]" ;
	statusBar()->showMessage(str);
	image_canvas->setId(labels[key].id);
}

void MainWindow::runWatershed() {
	image_canvas->setWatershedMask(watershed(image_canvas->getImage(), image_canvas->getMask().id));
	checkbox_watershed_mask->setCheckState(Qt::CheckState::Checked);
	//checkbox_manuel_mask->setCheckState(Qt::CheckState::Unchecked);
	image_canvas->update();
}

QString MainWindow::currentDir() const {
	QTreeWidgetItem *current = tree_widget_img->currentItem();
	if (!current || !current->parent())
		return "";

	return current->parent()->text(0);
}

QString MainWindow::currentFile() const {
	QTreeWidgetItem *current = tree_widget_img->currentItem();
	if (!current || !current->parent())
		return "";

	return current->text(0);
}

void MainWindow::on_tree_widget_img_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) {
	QString iFile = currentFile();
	QString iDir = currentDir();
	if (iFile.isEmpty() || iDir.isEmpty())
		return;

	QString filepath(iDir + "/" + iFile);
	image_canvas->loadImage(filepath);
}

void MainWindow::on_actionOpenDir_triggered() {
	statusBar()->clearMessage();
	QString openedDir = QFileDialog::getExistingDirectory(this, "Choose a directory to be read in", curr_open_dir);
	if (openedDir.isEmpty())
		return;

	curr_open_dir = openedDir;
	
	QTreeWidgetItem *currentTreeDir = new QTreeWidgetItem(tree_widget_img);
	tree_widget_img->setItemExpanded(currentTreeDir, true);
	currentTreeDir->setText(0, curr_open_dir);

	QDir current_dir(curr_open_dir);
	QStringList files = current_dir.entryList();
	static QStringList ext_img = { "png","jpg","bmp","pgm","jpeg" ,"jpe" ,"jp2" ,"pbm" ,"ppm" ,"tiff" ,"tif" };
	for (int i = 0; i < files.size(); i++) {
		if (files[i].size() < 4)
			continue;
		QString ext = files[i].section(".", -1, -1);
		bool is_image = false;
		for (int e = 0; e < ext_img.size(); e++) {
			if (ext.toLower() == ext_img[e]) {
				is_image = true;
				break;
			}
		}
		if (!is_image)
			continue;

		if( files[i].toLower().indexOf("_mask.png") > -1)
			continue;

		QTreeWidgetItem *currentFile = new QTreeWidgetItem(currentTreeDir);
		currentFile->setText(0, files[i]);
	}
//	setWindowTitle("PixelAnnotation - " + openedDir);
}


void MainWindow::saveConfigFile() {
	QString file = QFileDialog::getSaveFileName(this, tr("Save Config File"), QString(), tr("JSon file (*.json)"));
	QFile save_file(file);
	if (!save_file.open(QIODevice::WriteOnly)) {
		qWarning("Couldn't open save file.");
		return ;
	}
	QJsonObject object;
	labels.write(object);
	QJsonDocument saveDoc(object);
	save_file.write(saveDoc.toJson());
	save_file.close();
}

void MainWindow::loadConfigFile() {
	QString file = QFileDialog::getOpenFileName(this, tr("Open Config File"), QString(), tr("JSon file (*.json)"));
	QFile open_file(file);
	if (!open_file.open(QIODevice::ReadOnly)) {
		qWarning("Couldn't open save file.");
		return;
	}
	QJsonObject object;
	QByteArray saveData = open_file.readAll();
	QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

	labels.read(loadDoc.object());
	open_file.close();

	loadConfigLabels();
	update();

}

void MainWindow::on_actionAbout_triggered() {
	AboutDialog *d = new AboutDialog(this);
	d->setModal(true);
	d->show();
}