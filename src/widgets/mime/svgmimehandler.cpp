// SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mime/svgmimehandler.hpp"
#include "item/item.hpp"
#include "serializer/svgserializer.hpp"
#include <QByteArray>
#include <QMimeData>
#include <QXmlStreamWriter>
#include <memory>

using namespace Qt::StringLiterals;

void SvgMimeHandler::contributeData(QMimeData &mimeData, const QList<std::shared_ptr<Item>> &selectedItems)
{
    QByteArray svgData;
    QXmlStreamWriter svgWriter(&svgData);

    QList<std::shared_ptr<Item>> selectedList;

    for (auto &ptr : selectedItems) {
        selectedList.append(ptr);
    }

    SvgSerializer::writeSvg(svgWriter, selectedList);

    mimeData.setData(u"image/svg+xml"_s, svgData);
}
