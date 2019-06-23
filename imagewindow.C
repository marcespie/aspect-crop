/*
 * Copyright (c) 2019 Marc Espie <espie@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <QtWidgets>
#include <iostream>
#include "imagewindow.h"
#include "imageitem.h"

ImageWindow::ImageWindow()
{
	scene = new QGraphicsScene(this);
	view = new QGraphicsView;
	view->setScene(scene);
	image = nullptr;

	scaleFactor = 0.3;
	view->setVisible(true);
	setCentralWidget(view);
	resize(QGuiApplication::primaryScreen()->availableSize() * 0.9);
	rescaleImage();
	createActions();
}

bool 
ImageWindow::loadPicture(const char* filename)
{
	QImageReader r(filename);
	r.setAutoTransform(true);
	const QImage i = r.read();
	if (i.isNull())
		return false;
	else {
		setImage(i, filename);
		return true;
	}
}

void 
ImageWindow::setImage(const QImage& i, const char* title)
{
	auto p = QPixmap::fromImage(i);
	if (image) {
		scene->removeItem(image);
		delete(image);
	}
	image = new ImageItem(p, view, title);
	scene->addItem(image);
}

void 
ImageWindow::zoomIn()
{
	scaleFactor *= 1.2;
	rescaleImage();
}

void 
ImageWindow::zoomOut()
{
	scaleFactor /= 1.2;
	rescaleImage();
}


void 
ImageWindow::rescaleImage()
{
	QMatrix matrix;
	matrix.scale(scaleFactor, scaleFactor);
	view->setMatrix(matrix);
}

void 
ImageWindow::createActions()
{
	QAction *zi = new QAction(this);
	zi->setShortcut(QKeySequence("+"));
	connect(zi, SIGNAL(triggered()), this, SLOT(zoomIn()));
	this->addAction(zi);
	QAction *zo = new QAction(this);
	zo->setShortcut(QKeySequence("-"));
	connect(zo, SIGNAL(triggered()), this, SLOT(zoomOut()));
	this->addAction(zo);
}

