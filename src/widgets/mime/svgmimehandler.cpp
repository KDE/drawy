// SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mime/svgmimehandler.hpp"
#include "common/constants.hpp"
#include "drawyglobalconfig.h"
#include "item/item.hpp"
#include "serializer/svgserializer.hpp"
#include <QByteArray>
#include <QGuiApplication>
#include <QMimeData>
#include <QPalette>
#include <QXmlStreamWriter>
#include <memory>

using namespace Qt::StringLiterals;

void SvgMimeHandler::contributeData(QMimeData &mimeData, const QList<std::shared_ptr<Item>> &selectedItems)
{
    QByteArray svgData;
    QXmlStreamWriter svgWriter(&svgData);

    const bool isDark = QGuiApplication::palette().color(QPalette::Window).lightnessF() < 0.5;
    const QColor backgroundColor = isDark ? DrawyGlobalConfig::backgroundColorDark() : DrawyGlobalConfig::backgroundColorLight();
    SvgSerializer::writeSvg(svgWriter, selectedItems, backgroundColor);

    mimeData.setData(u"image/svg+xml"_s, svgData);
}
