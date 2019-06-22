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
#include "imageitem.h"

ImageItem::ImageItem(QPixmap& p, QWidget* parent): 
	QGraphicsPixmapItem(p), rubberBand(nullptr), window(parent)
{
	ratio = 16.0/9.0;
}

template<typename T>
inline T absdiff(T a, T b)
{
	return a < b ? b - a : a - b;
}

inline QPointF getpos(const QGraphicsSceneMouseEvent *event)
{
	return event->screenPos();
}

inline float getposx(const QGraphicsSceneMouseEvent *event)
{
	return getpos(event).x();
}

inline float getposy(const QGraphicsSceneMouseEvent *event)
{
	return getpos(event).y();
}

void 
ImageItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (!rubberBand) {
		rubberBand = new QRubberBand(QRubberBand::Rectangle);
		rubberBand->setWindowOpacity(0.5);
		xi = 1;
		yj = 1;
		x[0] = getposx(event);
		y[0] = getposy(event);
	} else {
		auto a = getposx(event);
		auto b = getposy(event);
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
ImageItem::adjustRubberBand(QGraphicsSceneMouseEvent *event)
{
	x[xi] = getposx(event);
	y[yj] = getposy(event);

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
ImageItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	adjustRubberBand(event);
}

void 
ImageItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//	rubberBand->hide();
}
