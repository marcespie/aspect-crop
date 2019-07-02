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
#include <sstream>
#include <unistd.h>
#include "imageitem.h"
#include "system.h"


ImageItem::ImageItem(QPixmap& p, QGraphicsView* v, const char* s, double r,
    bool& cs): 
    QGraphicsPixmapItem{p}, rubberBand{nullptr}, view{v}, title{s}, 
    c{r,cs, this}
{
}

void 
ImageItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	auto a = event->scenePos().x();
	auto b = event->scenePos().y();
	if (!rubberBand) {
		rubberBand = 
		    std::make_unique<QRubberBand>(QRubberBand::Rectangle, view);
		c.new_rubberBand(a, b);
	} else {
		c.find_handle(a, b);
	}
	setRubberBand();
	rubberBand->show();
}

void
ImageItem::setRubberBand()
{
	auto p = view->mapFromScene(c.boundingRect());

	rubberBand->setGeometry(p.boundingRect());
}

void 
ImageItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	c.mouseMoveEvent(event);
	setRubberBand();
}

void 
ImageItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	mouseMoveEvent(event);
}

void
ImageItem::printGeometry(std::ostream& o)
{
	coordinates d { c };
	d.adjust(0.0);
	d.printGeometry(o);
}

void 
ImageItem::doTell()
{
	std::ostringstream s;
	printGeometry(s);
	std::string r = "--trim " + s.str() + " --focus " + title;
	if (r != last_tell) {
		std::cout << r << "\n";
		last_tell = r;
	}
}

void
ImageItem::testTrim()
{
	if (rubberBand) {
		doTell();
		int pid = fork();
		if (pid == -1)
			system_error("fork");
		if (pid == 0) {
			std::ostringstream s;
			printGeometry(s);
			execlp("xwallpaper",
				"xwallpaper", "--trim",
				s.str().c_str(), "--focus", title, nullptr);
			system_error("execve");
		}
		deal_with_child(pid);
	}
}

void
ImageItem::adjustNow()
{
	if (rubberBand) {
		c.adjust(0.0);
		setRubberBand();
	}
}

void 
ImageItem::paint(QPainter* p, const QStyleOptionGraphicsItem* i, QWidget* w)
{
	QGraphicsPixmapItem::paint(p, i, w);
	if (rubberBand)
		setRubberBand();

}
