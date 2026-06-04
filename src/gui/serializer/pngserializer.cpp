/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "serializer/pngserializer.hpp"
#include <QString>
#include <QXmlStreamWriter>

using namespace Qt::Literals::StringLiterals;

void PngSerializer::writePng(QFile &file, QList<std::shared_ptr<Item>> items, const QColor &backgroundColor)
{
    QRectF boundingBox;

    for (const auto &item : std::as_const(items)) {
        boundingBox |= item->boundingBox();
    }

    QImage image(boundingBox.size().toSize(), QImage::Format_ARGB32);
    image.fill(backgroundColor);

    QPainter painter(&image);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QTransform t{};
    t.translate(boundingBox.topLeft().x(), boundingBox.topLeft().y());

    painter.setTransform(t.inverted(), true);

    for (const auto &item : std::as_const(items)) {
        painter.save();
        painter.setTransform(item->transformObj(), true);

        item->draw(painter, QPointF{0, 0});

        painter.restore();
    }
    image.save(&file, "PNG");
}
