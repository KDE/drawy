/*
 * SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <QString>
#include <QXmlStreamWriter>

#include "serializer/svgpaintdevice.hpp"
#include "serializer/svgserializer.hpp"

using namespace Qt::Literals::StringLiterals;

void SvgSerializer::writeSvg(QXmlStreamWriter &stream, QList<std::shared_ptr<Item>> items, const QColor &backgroundColor)
{
    stream.writeStartDocument();

    stream.writeComment("Exported from Drawy");

    stream.writeStartElement("svg");
    stream.writeDefaultNamespace("http://www.w3.org/2000/svg");
    stream.writeNamespace("http://www.w3.org/1999/xlink", "xlink");
    stream.writeAttribute("version", "1.1");
    stream.writeAttribute("baseProfile", "full");

    QRectF boundingBox;

    for (const auto &item : std::as_const(items)) {
        boundingBox |= item->boundingBox();
    }

    stream.writeAttribute("width", QString::number(boundingBox.width()) + u"px"_s);
    stream.writeAttribute("height", QString::number(boundingBox.height()) + u"px"_s);
    stream.writeAttribute("viewBox", u"0 0 %3 %4"_s.arg(boundingBox.width()).arg(boundingBox.height()));

    SvgPaintDevice device(stream, (int)boundingBox.width(), (int)boundingBox.height());
    QPainter painter(&device);

    // Draw background.
    QTransform t;
    t.translate(boundingBox.topLeft().x(), boundingBox.topLeft().y());

    painter.setTransform(t.inverted(), true);
    painter.fillRect(boundingBox, backgroundColor);

    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    for (const auto &item : std::as_const(items)) {
        painter.save();

        painter.setTransform(item->transformObj(), true);
        item->draw(painter, QPointF{0, 0});

        painter.restore();
    }

    stream.writeEndElement();

    stream.writeEndDocument();
}
