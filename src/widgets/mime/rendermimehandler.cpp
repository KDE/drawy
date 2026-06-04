// SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mime/rendermimehandler.hpp"
#include "drawyglobalconfig.h"
#include "item/item.hpp"
#include <QByteArray>
#include <QGuiApplication>
#include <QImage>
#include <QMimeData>
#include <QPainter>
#include <QPalette>
#include <memory>

using namespace Qt::StringLiterals;

void RenderMimeHandler::contributeData(QMimeData &mimeData, const QList<std::shared_ptr<Item>> &selectedItems)
{
    QRectF boundingBox;

    for (const auto &item : std::as_const(selectedItems)) {
        boundingBox |= item->boundingBox();
    }

    QImage image(boundingBox.size().toSize(), QImage::Format_ARGB32);

    const bool isDark = QGuiApplication::palette().color(QPalette::Window).lightnessF() < 0.5;
    const QColor backgroundColor = isDark ? DrawyGlobalConfig::backgroundColorDark() : DrawyGlobalConfig::backgroundColorLight();
    image.fill(backgroundColor);

    QPainter painter(&image);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QTransform t{};
    t.translate(boundingBox.topLeft().x(), boundingBox.topLeft().y());

    painter.setTransform(t.inverted(), true);

    for (const auto &item : std::as_const(selectedItems)) {
        painter.save();
        painter.setTransform(item->transformObj(), true);

        item->draw(painter, QPointF{0, 0});

        painter.restore();
    }

    mimeData.setImageData(image);
}
