#include "main_window.h"
#include "utils.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QStringList>
#include <QJsonDocument>
#include <QPainter>
#include <QJsonObject>
#include <QJsonArray>
#include <QColorDialog>
#include <QTextStream>
#include <QMimeData>
#include "pixel_annotation_tool_version.h"

#include "about_dialog.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setupUi(this);
	setWindowTitle(QApplication::translate("MainWindow", "PixelAnnotationTool " PIXEL_ANNOTATION_TOOL_GIT_TAG, Q_NULLPTR));
	list_label->setSpacing(1);
    image_canvas = nullptr;
    _isLoadingNewLabels = false;
	save_action = new QAction(tr("&Save current image"), this);
    copy_mask_action = new QAction(tr("&Copy Mask"), this);
    paste_mask_action = new QAction(tr("&Paste Mask"), this);
    clear_mask_action = new QAction(tr("&Clear Mask mask"), this);
    close_tab_action = new QAction(tr("&Close current tab"), this);
    swap_action = new QAction(tr("&Swap check box Watershed"), this);
	undo_action = new QAction(tr("&Undo"), this);
	redo_action = new QAction(tr("&Redo"), this);
    next_file_action = new QAction(tr("&Select next file"), this);
    previous_file_action = new QAction(tr("&Select previous file"), this);
	undo_action->setShortcuts(QKeySequence::Undo);
	redo_action->setShortcuts(QKeySequence::Redo);
	save_action->setShortcut(Qt::CTRL + Qt::Key_S);
    swap_action->setShortcut(Qt::CTRL + Qt::Key_Space);
    copy_mask_action->setShortcut(Qt::CTRL + Qt::Key_C);
    paste_mask_action->setShortcut(Qt::CTRL + Qt::Key_V);
    clear_mask_action->setShortcut(Qt::CTRL + Qt::Key_R);
    close_tab_action->setShortcut(Qt::CTRL + Qt::Key_W);
    next_file_action->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_Down);
    previous_file_action->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_Up);
	undo_action->setEnabled(false);
	redo_action->setEnabled(false);

	menuFile->addAction(save_action);
    menuEdit->addAction(close_tab_action);
	menuEdit->addAction(undo_action);
	menuEdit->addAction(redo_action);
    menuEdit->addAction(copy_mask_action);
    menuEdit->addAction(paste_mask_action);
    menuEdit->addAction(clear_mask_action);
    menuEdit->addAction(swap_action);
    menuEdit->addAction(next_file_action);
    menuEdit->addAction(previous_file_action);

	tabWidget->clear();

	connect(button_watershed      , SIGNAL(released())                        , this, SLOT(runWatershed()  ));
    connect(swap_action           , SIGNAL(triggered())                       , this, SLOT(swapView()      ));
	connect(actionOpen_config_file, SIGNAL(triggered())                       , this, SLOT(loadConfigFile()));
	connect(actionSave_config_file, SIGNAL(triggered())                       , this, SLOT(saveConfigFile()));
    connect(close_tab_action      , SIGNAL(triggered())                       , this, SLOT(closeCurrentTab()));
    connect(copy_mask_action      , SIGNAL(triggered())                       , this, SLOT(copyMask()));
    connect(paste_mask_action     , SIGNAL(triggered())                       , this, SLOT(pasteMask()));
    connect(clear_mask_action     , SIGNAL(triggered())                       , this, SLOT(clearMask()));
    connect(next_file_action      , SIGNAL(triggered())                       , this, SLOT(nextFile()));
    connect(previous_file_action  , SIGNAL(triggered())                       , this, SLOT(previousFile()));
	connect(tabWidget             , SIGNAL(tabCloseRequested(int))            , this, SLOT(closeTab(int)   ));
	connect(tabWidget             , SIGNAL(currentChanged(int))               , this, SLOT(updateConnect(int)));
    connect(tree_widget_img       , SIGNAL(itemClicked(QTreeWidgetItem *,int)), this, SLOT(treeWidgetClicked()));

    registerShortcuts();

	labels = defaultLabels();
	loadConfigLabels();

	connect(list_label, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(changeLabel(QListWidgetItem*, QListWidgetItem*)));
	connect(list_label, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(changeColor(QListWidgetItem*)));

    list_label->setEnabled(false);

    setAcceptDrops(true);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e) {
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e) {
    for (const QUrl &url : e->mimeData()->urls()) {
        QDir d = QFileInfo(url.toLocalFile()).absoluteDir();
        QString absolute = d.absolutePath();
        curr_open_dir = absolute;
        this->openDirectory();
    }
}

void MainWindow::closeCurrentTab() {
    int index = tabWidget->currentIndex();
    if (index >= 0)
        closeTab(index);
}

void MainWindow::closeTab(int index) {
    ImageCanvas * ic = getImageCanvas(index);
    if (ic == nullptr)
        throw std::logic_error("error index");

    if (ic->isNotSaved()) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Current image is not saved",
            "You will close the current image. Would you like to save the image before?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            ic->saveMask();
        }
    }
    tabWidget->removeTab(index);
    delete ic;
    if (tabWidget->count() == 0) {
        image_canvas = nullptr;
        list_label->setEnabled(false);
    } else {
        image_canvas = getImageCanvas(std::min(index, tabWidget->count()-1));
    }
}

void MainWindow::registerShortcuts() {
    for (int i = 0; i < 40; i++) {
        const QString shortcut_key = stringForShortCut(i);
        auto *shortcut = new QShortcut(QKeySequence(shortcut_key), this);
        _shortcuts.append(shortcut);
        connect(shortcut, &QShortcut::activated, this, [=]{onLabelShortcut(i);});
    }
}

QString MainWindow::stringForShortCut(int id) const {
    return (id < 10) ? QString("%1").arg((id + 1) % 10) :
           (id < 20) ? QString("Ctrl+%1").arg((id + 1) % 10) :
           (id < 30) ? QString("Alt+%1").arg((id + 1) % 10) :
           (id < 40) ? QString("Ctrl+Alt+%1").arg((id + 1) % 10) :
           QString();
}

void MainWindow::loadConfigLabels() {
    _isLoadingNewLabels = true;
	list_label->clear();
	QMapIterator<QString, LabelInfo> it(labels);
	while (it.hasNext()) {
		it.next();
		const LabelInfo & label = it.value();
		auto * item = new QListWidgetItem(list_label);
		auto * label_widget = new LabelWidget(label,this);

		item->setSizeHint(label_widget->sizeHint());
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		list_label->addItem(item);
		list_label->setItemWidget(item, label_widget);

		auto& ref = labels[it.key()];
		ref.item = item;

        int id = list_label->row(item);

        if (id < 40) {
            QShortcut * shortcut = _shortcuts.at(id);
            QString text = label.name + " (" + shortcut->key().toString() + ")";
            label_widget->setText(text);
        }
	}
	id_labels = getId2Label(labels);
    _isLoadingNewLabels = false;
}

void MainWindow::changeColor(QListWidgetItem* item) {
	auto * widget = dynamic_cast<LabelWidget*>(list_label->itemWidget(item));
	LabelInfo & label = labels[widget->getName()];
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
    if (_isLoadingNewLabels)
        return;
	if (current == nullptr && previous == nullptr)
		return;

	LabelWidget * label;
	if (previous == nullptr) {
		for (int i = 0; i < list_label->count(); i++) {
			label = dynamic_cast<LabelWidget*>(list_label->itemWidget(list_label->item(i)));
			label->setSelected(false);
		}
	} else {
		label = dynamic_cast<LabelWidget*>(list_label->itemWidget(previous));
		label->setSelected(false);
	}

	if (current == nullptr) current = previous;

	label = dynamic_cast<LabelWidget*>(list_label->itemWidget(current));
	label->setSelected(true);

	QString str;
	QString key = label->getName();
	QTextStream sstr(&str);
	sstr <<"label=["<< key <<"] id=[" << labels[key].id << "] categorie=[" << labels[key].categorie << "] color=[" << labels[key].color.name() << "]" ;
	statusBar()->showMessage(str);
	image_canvas->setId(labels[key].id);
}

void MainWindow::runWatershed(ImageCanvas * ic) {
    QImage iwatershed = watershed(ic->getImage(), ic->getMask().id);
    if (!checkbox_border_ws->isChecked()) {
        iwatershed = removeBorder(iwatershed, id_labels);
    }
	ic->setWatershedMask(iwatershed);
	checkbox_watershed_mask->setCheckState(Qt::CheckState::Checked);
	ic->update();
}

void MainWindow::runWatershed() {
    ImageCanvas * ic = image_canvas;
    if (ic != nullptr)
        runWatershed(ic);
}

void MainWindow::setStarAtNameOfTab(bool star) {
    if (tabWidget->count() > 0) {
        int index = tabWidget->currentIndex();
        QString name = tabWidget->tabText(index);
        if (star && !name.endsWith("*")) { //add star
            name += "*";
            tabWidget->setTabText(index, name);
        } else if (!star && name.endsWith("*")) { //remove star
            int pos = name.lastIndexOf('*');
            name = name.left(pos);
            tabWidget->setTabText(index, name);
        }
    }
}

void MainWindow::updateConnect(const ImageCanvas * ic) {
    if (ic == nullptr) return;
    connect(spinbox_scale, SIGNAL(valueChanged(double)), ic, SLOT(scaleChanged(double)));
    connect(spinbox_alpha, SIGNAL(valueChanged(double)), ic, SLOT(alphaChanged(double)));
    connect(spinbox_pen_size, SIGNAL(valueChanged(int)), ic, SLOT(setSizePen(int)));
	connect(checkbox_watershed_mask, SIGNAL(clicked()), ic, SLOT(update()));
	connect(checkbox_manuel_mask, SIGNAL(clicked()), ic, SLOT(update()));
	connect(actionClear, SIGNAL(triggered()), ic, SLOT(clearMask()));
	connect(undo_action, SIGNAL(triggered()), ic, SLOT(undo()));
	connect(redo_action, SIGNAL(triggered()), ic, SLOT(redo()));
	connect(save_action, SIGNAL(triggered()), ic, SLOT(saveMask()));
    connect(checkbox_border_ws, SIGNAL(clicked()), this, SLOT(runWatershed()));
}

void MainWindow::allDisconnnect(const ImageCanvas * ic) {
    if (ic == nullptr) return;
    disconnect(spinbox_scale, SIGNAL(valueChanged(double)), ic, SLOT(scaleChanged(double)));
    disconnect(spinbox_alpha, SIGNAL(valueChanged(double)), ic, SLOT(alphaChanged(double)));
    disconnect(spinbox_pen_size, SIGNAL(valueChanged(int)), ic, SLOT(setSizePen(int)));
    disconnect(checkbox_watershed_mask, SIGNAL(clicked()), ic, SLOT(update()));
    disconnect(checkbox_manuel_mask, SIGNAL(clicked()), ic, SLOT(update()));
    disconnect(actionClear, SIGNAL(triggered()), ic, SLOT(clearMask()));
    disconnect(undo_action, SIGNAL(triggered()), ic, SLOT(undo()));
    disconnect(redo_action, SIGNAL(triggered()), ic, SLOT(redo()));
    disconnect(save_action, SIGNAL(triggered()), ic, SLOT(saveMask()));
    disconnect(checkbox_border_ws, SIGNAL(clicked()), this, SLOT(runWatershed()));
}

ImageCanvas * MainWindow::newImageCanvas() {
    auto * ic = new ImageCanvas( this);
	ic->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	ic->setScaledContents(true);
	updateConnect(ic);
	return ic;
}

void MainWindow::updateConnect(int index) {
    if (index < 0 || index >= tabWidget->count())
        return;
    allDisconnnect(image_canvas);
    image_canvas = getImageCanvas(index);
    list_label->setEnabled(image_canvas != nullptr);
	updateConnect(image_canvas);
}

ImageCanvas * MainWindow::getImageCanvas(int index) {
    auto * scroll_area = dynamic_cast<QScrollArea *>(tabWidget->widget(index));
    auto * ic = dynamic_cast<ImageCanvas*>(scroll_area->widget());
    return ic;
}

int MainWindow::getImageCanvas(QString name, ImageCanvas * ic) {
	for (int i = 0; i < tabWidget->count(); i++) {
		if (tabWidget->tabText(i).startsWith(name) ) {
            ic = getImageCanvas(i);
			return i;
		}
	}
	ic = newImageCanvas();
	QString iDir = currentDir();
	QString filepath(iDir + "/" + name);
	ic->loadImage(filepath);
    int index = tabWidget->addTab(ic->getScrollParent(), name);
	return index;
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

void MainWindow::treeWidgetClicked() {
    QString iFile = currentFile();
    QString iDir = currentDir();
    if (iFile.isEmpty() || iDir.isEmpty())
        return;

    allDisconnnect(image_canvas);
    int index = getImageCanvas(iFile, image_canvas);
    updateConnect(image_canvas);
    tabWidget->setCurrentIndex(index);
}

void MainWindow::on_tree_widget_img_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) {
    treeWidgetClicked();
}

void MainWindow::on_actionOpenDir_triggered() {
	statusBar()->clearMessage();
	QString openedDir = QFileDialog::getExistingDirectory(this, "Choose a directory to be read in", curr_open_dir);
	if (openedDir.isEmpty())
		return;

	curr_open_dir = openedDir;
    this->openDirectory();
}

void MainWindow::openDirectory()
{
	auto *currentTreeDir = new QTreeWidgetItem(tree_widget_img);
    currentTreeDir->setExpanded(true);
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

		if (files[i].toLower().indexOf("_mask.png") > -1)
			continue;

		auto *currentFile = new QTreeWidgetItem(currentTreeDir);
		currentFile->setText(0, files[i]);
	}
    // setWindowTitle("PixelAnnotation - " + openedDir);
}

void MainWindow::nextFile() {
    QTreeWidgetItem *currentItem = tree_widget_img->currentItem();
    if (!currentItem) return;
    QTreeWidgetItem *nextItem = tree_widget_img->itemBelow(currentItem);
    if (!nextItem) return;
    tree_widget_img->setCurrentItem(nextItem);
    treeWidgetClicked();
}

void MainWindow::previousFile() {
    QTreeWidgetItem *currentItem = tree_widget_img->currentItem();
    if (!currentItem) return;
    QTreeWidgetItem *previousItem = tree_widget_img->itemAbove(currentItem);
    if (!previousItem) return;
    tree_widget_img->setCurrentItem(previousItem);
    treeWidgetClicked();
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

	labels.clear();
	labels.read(loadDoc.object());
	open_file.close();

	loadConfigLabels();
	update();
}

void MainWindow::on_actionAbout_triggered() {
	auto *d = new AboutDialog(this);
	d->setModal(true);
	d->show();
}

void MainWindow::copyMask() {
    ImageCanvas * ic = getCurrentImageCanvas();
    if (ic == nullptr)
        return;

    _tmp = ic->getMask();
}

void MainWindow::pasteMask() {
    ImageCanvas * ic = getCurrentImageCanvas();
    if (ic == nullptr)
        return;

    ic->setActionMask(_tmp);
}

void MainWindow::clearMask() {
    ImageCanvas * ic = getCurrentImageCanvas();
    if (ic == nullptr)
        return;

    ImageMask clear(QSize(ic->width(),ic->height()));
    ic->setActionMask(clear);
}

ImageCanvas * MainWindow::getCurrentImageCanvas() {
    int index = tabWidget->currentIndex();
    if (index == -1)
        return nullptr;

    ImageCanvas * ic = getImageCanvas(index);
    return ic;
}

void MainWindow::swapView() {
    checkbox_watershed_mask->setCheckState(checkbox_watershed_mask->checkState() == Qt::CheckState::Checked ? Qt::CheckState::Unchecked : Qt::CheckState::Checked);
    update();
}

void MainWindow::update() {
    QWidget::update();
    ImageCanvas * ic = getCurrentImageCanvas();
    if (ic == nullptr)
        return;

    ic->update();
}

void MainWindow::onLabelShortcut(int row) {
    if (list_label->isEnabled() && row < list_label->count()) {
        list_label->setCurrentRow(row, QItemSelectionModel::ClearAndSelect);
        update();
    }
}
