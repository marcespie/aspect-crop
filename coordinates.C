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

#include <cmath>
#include <QGraphicsSceneMouseEvent>
#include <iostream>
#include "coordinates.h"
#include "imageitem.h"

template<typename T>
inline T absdiff(T a, T b)
{
	return a < b ? b - a : a - b;
}

void
coordinates::new_rubberBand(C a, C b)
{
	xi = 1;
	yj = 1;
	x[0] = x[1] = a;
	y[0] = x[1] = b;
	moving = false;
}

void
coordinates::find_handle(C a, C b)
{
	// find the nearest moving coordinates
	if (absdiff(x[0], a) < absdiff(x[1], a))
		xi = 0;
	else 
		xi = 1;
	if (absdiff(y[0], b) < absdiff(y[1], b))
		yj = 0;
	else 
		yj = 1;
	// if either coordinate is "in the middle" we switch to moving
	// the rectangle
	moving = absdiff(x[xi], a) > absdiff((x[0]+x[1])/2, a)
	    || absdiff(y[yj], b) > absdiff((y[0]+y[1])/2, b);

	if (!moving) {
		x[xi] = a;
		y[yj] = b;
		adjust();
	}
}

void
coordinates::adjust(double coeff)
{
	auto r = i->sceneBoundingRect();
	auto xmargin = absdiff(x[0], x[1])* coeff;
	for (auto& a: x)
		if (a < r.left()-xmargin)
			a = r.left()-xmargin;
		else if (a > r.right()+xmargin)
			a = r.right()+xmargin;

	auto ymargin = absdiff(y[0], y[1]) * coeff;
	for (auto& b: y)
		if (b < r.top()-ymargin)
			b = r.top()-ymargin;
		else if (b > r.bottom()+ymargin)
			b = r.bottom()+ymargin;

	if (ratio > 0 && constrained) {
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
	}
}

void
coordinates::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (moving) {
		auto deltax = event->scenePos().x() - event->lastScenePos().x();
		auto deltay = event->scenePos().y() - event->lastScenePos().y();
		x[0] += deltax;
		x[1] += deltax;
		y[0] += deltay;
		y[1] += deltay;
	} else  {
		x[xi] = event->scenePos().x();
		y[yj] = event->scenePos().y();
	}
	adjust();
}

void
coordinates::printGeometry(std::ostream& o)
{
	if (x[0] > x[1])
		std::swap(x[0], x[1]);
	if (y[0] > y[1])
		std::swap(y[0], y[1]);
	o << round(x[1]-x[0]) << "x" << round(y[1]-y[0]) << 
		    "+" << round(x[0]) << "+" << round(y[0]);
}

QRectF
coordinates::boundingRect() const
{
	return QRectF{x[0], y[0], x[1]-x[0], y[1]-y[0]};
}
