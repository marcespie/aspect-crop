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

#include <QGraphicsSceneMouseEvent>
#include <QRubberBand>
#include <QGraphicsView>
#include <iostream>
#include <tuple>
#include <cmath>
#include "imageitem.h"

ImageItem::ImageItem(QPixmap& p, QGraphicsView* parent, const char* s, double r): 
	QGraphicsPixmapItem(p), rubberBand(nullptr), view(parent),
	title(s), ratio(r)
{
}

ImageItem::~ImageItem()
{
	if (rubberBand)
		delete rubberBand;
}

template<typename T>
inline T absdiff(T a, T b)
{
	return a < b ? b - a : a - b;
}

void 
ImageItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (!rubberBand) {
		rubberBand = new QRubberBand(QRubberBand::Rectangle, view);
		rubberBand->setWindowOpacity(0.5);
		xi = 1;
		yj = 1;
		x[0] = event->scenePos().x();
		y[0] = event->scenePos().y();
	} else {
		auto a = event->scenePos().x();
		auto b = event->scenePos().y();
		if (absdiff(x[0], a) < absdiff(x[1], a))
			xi = 0;
		else 
			xi = 1;
		if (absdiff(y[0], b) < absdiff(y[1], b))
			yj = 0;
		else 
			yj = 1;
	}
	constrainRubberBand(event);
	rubberBand->show();
}

void
ImageItem::constrainRubberBand(QGraphicsSceneMouseEvent *event)
{
	x[xi] = event->scenePos().x();
	y[yj] = event->scenePos().y();

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

	adjustRubberBand();
}

void
ImageItem::adjustRubberBand()
{
	auto p = view->mapFromScene(x[0], y[0], x[1]-x[0], y[1]-y[0]);

	rubberBand->setGeometry(p.boundingRect());
}

void 
ImageItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	constrainRubberBand(event);
}

void 
ImageItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	constrainRubberBand(event);
}

void 
ImageItem::doTell() const
{
	std::cout << 
	    "--trim " <<round(x[1]-x[0]) << "x" << round(y[1]-y[0]) << 
	    "+" << round(x[0]) << "+" << round(y[0]) <<
	    " --focus " << title << "\n";
}


void 
ImageItem::paint(QPainter* p, const QStyleOptionGraphicsItem* i, QWidget* w)
{
	QGraphicsPixmapItem::paint(p, i, w);
	if (rubberBand)
		adjustRubberBand();
}
