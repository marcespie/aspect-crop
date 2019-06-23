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
	n = 0;
	i = 0;
	names = nullptr;

	ratio = -1.0;
	scaleFactor = 0.5;
	setCentralWidget(view);
	resize(QGuiApplication::primaryScreen()->availableSize() * 0.9);
	view->setVisible(true);
	rescaleView();
	createActions();
}

void
ImageWindow::setRatio(int a, int b)
{
	ratio = double(a) / double(b);
}

void
ImageWindow::setCarousel(int argc, char **argv)
{
	n = argc;
	i = 0;
	names = argv;
}

bool
ImageWindow::nextPicture()
{
	for (; i != n; i++) {
		if (loadPicture(names[i])) {
			i++;
			return true;
		}
		std::cerr << "Couldn't load " << names[i] << "\n";
	}
	return false;
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
	image = new ImageItem(p, view, title, ratio);
	scene->addItem(image);
}

void 
ImageWindow::zoomIn()
{
	scaleFactor *= 1.2;
	rescaleView();
}

void 
ImageWindow::zoomOut()
{
	scaleFactor /= 1.2;
	rescaleView();
}


void 
ImageWindow::rescaleView()
{
	QMatrix matrix;
	matrix.scale(scaleFactor, scaleFactor);
	view->setMatrix(matrix);
}

void
ImageWindow::printNext()
{
	if (image)
		image->doTell();
	if (!nextPicture())
		exit(0);
}

void
ImageWindow::print()
{
	if (image)
		image->doTell();
}

void
ImageWindow::noprintNext()
{
	if (!nextPicture())
		exit(0);
}

void
ImageWindow::forceCrop()
{
	if (image)
		image->adjustNow();
}

void 
ImageWindow::createActions()
{
	auto z = new QAction(this);
	z->setShortcut(QKeySequence("+"));
	connect(z, SIGNAL(triggered()), this, SLOT(zoomIn()));
	this->addAction(z);

	z = new QAction(this);
	z->setShortcut(QKeySequence("-"));
	connect(z, SIGNAL(triggered()), this, SLOT(zoomOut()));
	this->addAction(z);

	z = new QAction(this);
	z->setShortcut(QKeySequence("n"));
	connect(z, SIGNAL(triggered()), this, SLOT(printNext()));
	this->addAction(z);

	z = new QAction(this);
	z->setShortcut(QKeySequence("s"));
	connect(z, SIGNAL(triggered()), this, SLOT(noprintNext()));
	this->addAction(z);

	z = new QAction(this);
	z->setShortcut(QKeySequence("p"));
	connect(z, SIGNAL(triggered()), this, SLOT(print()));
	this->addAction(z);

	z = new QAction(this);
	z->setShortcut(QKeySequence("f"));
	connect(z, SIGNAL(triggered()), this, SLOT(forceCrop()));
	this->addAction(z);
}
