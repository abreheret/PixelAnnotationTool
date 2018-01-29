/*
 * MidiEditor
 * Copyright (C) 2010  Markus Schwenk
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "about_dialog.h"
#include "pixel_annotation_tool_version.h"

#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QApplication>
#include <QVariant>
#include <QTextBrowser>
#include <QScrollArea>

AboutDialog::AboutDialog(QWidget *parent):QDialog(parent) {

	setMinimumWidth(550);
	setMaximumHeight(450);
	setWindowTitle(tr("About"));
	QGridLayout *layout = new QGridLayout(this);

	QScrollArea *a = new QScrollArea(this);

	
	QTextBrowser *content = new QTextBrowser(this);
	QString text = QString::fromLatin1(
	"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">"
	"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">"
	"p, li { white-space: pre-wrap; }"
	"</style></head>"
		"<body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">"
		"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Version Info : " INFO_PIXEL_ANNOTATION_TOOL_VERSION "</span></p>"
		"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p>"
		"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Author: </span><span style=\" font-size:10pt; font-weight:600;\">Amaury Bréhéret</span></p>"
		"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Software use :  </span><a href=\"http://opencv.org/\"><span style=\" font-size:10pt; text-decoration: underline; color:#0000ff;\">OpenCV</span></a><span style=\" font-size:10pt;\"> and </span><a href=\"https://www.qt.io/\"><span style=\" font-size:10pt; text-decoration: underline; color:#0000ff;\">Qt</span></a><span style=\" font-size:10pt;\">.</span></p>"
		"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Make donation : </span><a href=\"https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&amp;hosted_button_id=8K79VKWBS7352\"><span style=\" font-size:10pt; text-decoration: underline; color:#0000ff;\">Donate</span></a></p>"
		"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p>"
		"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Shortcuts : </span></p>"
		"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">- CTRL + Mouse Wheel: zoom image</span></p>"
		"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">- SHIFT + Mouse Wheel: Changing cicle size of brush</span></p>"
		"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">- SPACE: Run watershed algorithm</span></p>"
		"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">- Right Mouse Click on image : Captures and selects the label at the mouse coordinate.</span></p>"
		"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p>"
	"</body></html>");

	content->setText(text);
	resize(QSize(600,300));

	a->setWidgetResizable(true);
	a->setWidget(content);
	a->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	a->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	layout->addWidget(a, 2, 1, 2, 2);
	content->setStyleSheet("color: black; background-color: white; padding: 5px");

	content->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
	content->setOpenExternalLinks(true);

	layout->setRowStretch(3, 1);
	layout->setColumnStretch(1, 1);

	QFrame *f = new QFrame( this );
	f->setFrameStyle(QFrame::HLine | QFrame::Sunken);
	layout->addWidget(f, 4, 0, 1, 3);

	QPushButton *close = new QPushButton("Close");
	layout->addWidget(close, 5, 2, 1, 1);
	connect(close, SIGNAL(clicked()), this, SLOT(hide()));
}
