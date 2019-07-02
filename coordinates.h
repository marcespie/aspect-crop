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
#ifndef COORDINATES_H
#define COORDINATES_H

#include <iosfwd>
class QGraphicsSceneMouseEvent;

using C = double;

class ImageItem;

class coordinates
{
public:
	C x[2], y[2];
private:
	int xi, yj;
	double ratio;
	bool& constrained;
	bool moving;
	ImageItem* const i;

public:
	void new_rubberBand(C a, C b);
	void find_handle(C a, C b);

	coordinates(double r, bool& c, ImageItem* i_): ratio{r}, constrained{c},
		i{i_}
	{
	}
	coordinates(const coordinates&) = default;
	void adjust(double =0.2);
	void mouseMoveEvent(QGraphicsSceneMouseEvent*);
	void printGeometry(std::ostream&);
};

#endif
