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
#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <memory>

class QRubberBand;
class QGraphicsView;

class ImageItem: public QGraphicsPixmapItem
{
public:
	ImageItem(QPixmap&, QGraphicsView*, const char*, double);
	void doTell();
	void adjustNow();
	void testTrim();
	void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)
	    override;
	using C = double;
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent*) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;
private:
	std::unique_ptr<QRubberBand> rubberBand;
	QGraphicsView* view;
	const char* title;
	C x[2], y[2];
	int xi, yj;
	bool moving;
	double ratio;
	void adjustRubberBand(double =0.1);
	void setRubberBand();
};

#endif
