/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once
#include "item/item.hpp"
#include "libdrawygui_export.h"
#include <QFile>
class QXmlStreamWriter;
namespace PngSerializer
{
LIBDRAWYGUI_EXPORT void writePng(QFile &file, QList<std::shared_ptr<Item>> items, const QColor &backgroundColor);
}
