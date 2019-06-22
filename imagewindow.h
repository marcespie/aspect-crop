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

class QLabel;
class QScrollArea;
class QScrollBar;
class QRubberBand;
class QMouseEvent;

class ImageWindow: public QMainWindow
{
	Q_OBJECT
public:
	ImageWindow();
	bool loadPicture(const QString&);
private slots:
	void zoomIn();
	void zoomOut();
	void mousePressEvent(QMouseEvent*);
	void mouseMoveEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*);
private:
	QImage image;
	QLabel *imageLabel;
	QScrollArea *scrollArea;
	QRubberBand *rubberBand;
	int x[2], y[2];
	int xi, yj;
	double scaleFactor;
	double ratio;
	void setImage(const QImage&);
	void rescaleImage(double);
	void adjustScrollBar(QScrollBar*, double);
	void createActions();
	void adjustRubberBand(QMouseEvent *);
};

#endif
