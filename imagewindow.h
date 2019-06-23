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
#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <QMainWindow>
#include <QImage>

class QGraphicsScene;
class QMouseEvent;
class QGraphicsView;
class ImageItem;

class ImageWindow: public QMainWindow
{
	Q_OBJECT
public:
	ImageWindow();
	void setCarousel(int, char**);
	void setRatio(int, int);
	bool nextPicture();
private slots:
	void zoomIn();
	void zoomOut();
	void goNext();
private:
	QGraphicsScene *scene;
	QGraphicsView *view;
	ImageItem *image;
	double scaleFactor;
	double ratio;
	void setImage(const QImage&, const char*);
	void rescaleView();
	void createActions();
	bool loadPicture(const char*);
	int n;
	int i;
	char **names;
};

#endif
