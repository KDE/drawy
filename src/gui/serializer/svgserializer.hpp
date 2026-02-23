/*
 * SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once
#include "item/item.hpp"
#include "libdrawygui_export.h"
class QXmlStreamWriter;
namespace SvgSerializer
{

LIBDRAWYGUI_EXPORT void writeSvg(QXmlStreamWriter &writer, QList<std::shared_ptr<Item>> items);

} // namespace SvgSerializer
