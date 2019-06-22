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

ImageItem::ImageItem(QPixmap& p, QGraphicsView* parent, const char* s): 
	QGraphicsPixmapItem(p), rubberBand(nullptr), view(parent),
	title(s)
{
	ratio = 9.0/16.0;
}

template<typename T>
inline T absdiff(T a, T b)
{
	return a < b ? b - a : a - b;
}

inline std::pair<int, int> getpos(QWidget *window, 
    const QGraphicsSceneMouseEvent *event)
{
	auto p = window->mapFromGlobal(event->screenPos());
	return std::pair(p.x(), p.y());
}

void 
ImageItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (!rubberBand) {
		rubberBand = new QRubberBand(QRubberBand::Rectangle, view);
		rubberBand->setWindowOpacity(0.5);
		xi = 1;
		yj = 1;
		std::tie(x[0], y[0]) = getpos(view, event);
	} else {
		auto [a, b] = getpos(view, event);
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
	std::tie( x[xi], y[yj]) = getpos(view, event);

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
	auto a = view->mapToScene(x[0], y[0]);
	auto b = view->mapToScene(x[1], y[1]);
	auto x0 = a.x();
	auto y0 = a.y();
	auto x1 = b.x();
	auto y1 = b.y();

	std::cout << "--trim " <<round(x1-x0) << "x" << round(y1-y0) << 
	    "+" << round(x0) << "+" << round(y0) <<" --focus " << title << "\n";
}
