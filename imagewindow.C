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

ImageWindow::ImageWindow()
{
	scene = new QGraphicsScene(this);
	view = new QGraphicsView;
	view->setScene(scene);

	rubberBand = nullptr;
	scaleFactor = 1;
	ratio = 16.0/9.0;
	view->setVisible(true);
	setCentralWidget(view);
	resize(QGuiApplication::primaryScreen()->availableSize() * 0.9);
	createActions();
}

bool 
ImageWindow::loadPicture(const QString& filename)
{
	QImageReader r(filename);
	r.setAutoTransform(true);
	const QImage i = r.read();
	if (i.isNull())
		return false;
	else {
		setImage(i);
		return true;
	}
	rubberBand = nullptr;
}

void 
ImageWindow::setImage(const QImage& i)
{
	image = i;

	auto p = QPixmap::fromImage(image);
	scene->addPixmap(p);
}

void 
ImageWindow::zoomIn()
{
	rescaleImage(1.25);
}

void 
ImageWindow::zoomOut()
{
	rescaleImage(0.8);
}


void 
ImageWindow::rescaleImage(double factor)
{
	scaleFactor *= factor;
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

template<typename T>
inline T absdiff(T a, T b)
{
	return a < b ? b - a : a - b;
}

void 
ImageWindow::mousePressEvent(QMouseEvent *event)
{
	if (!rubberBand) {
		rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
		xi = 1;
		yj = 1;
		x[0] = event->pos().x();
		y[0] = event->pos().y();
	} else {
		int a = event->pos().x();
		int b = event->pos().y();
		if (absdiff(x[0], a) < absdiff(x[1], a))
			xi = 0;
		else 
			xi = 1;
		if (absdiff(y[0], b) < absdiff(y[1], b))
			yj = 0;
		else 
			yj = 1;
	}
	adjustRubberBand(event);
	rubberBand->show();
}

void
ImageWindow::adjustRubberBand(QMouseEvent *event)
{
	x[xi] = event->pos().x();
	y[yj] = event->pos().y();

	auto d = (x[1]-x[0]) - (y[1]-y[0]) * ratio;
	if (d > 0.0) {
		if (xi == 1)
			x[xi] -= d;
		else
			x[xi] += d;
	} else if (d < 0.0) {
		if (yj == 1)
			y[yj] += d/ratio;
		else
			y[yj] -= d/ratio;
	}

	rubberBand->setGeometry(x[0], y[0], x[1]-x[0], y[1]-y[0]);
}

void 
ImageWindow::mouseMoveEvent(QMouseEvent *event)
{
	adjustRubberBand(event);
}

void 
ImageWindow::mouseReleaseEvent(QMouseEvent *event)
{
//	rubberBand->hide();
}
