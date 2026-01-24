/*
 * SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <QByteArray>
#include <QString>
#include <QXmlStreamWriter>

#include "item/item.hpp"

namespace SvgSerializer
{

void writeSvg(QXmlStreamWriter &writer, QList<std::shared_ptr<Item>> items);

}; // namespace SvgSerializer
