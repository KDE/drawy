// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "libdrawygui_export.h"
#include <QByteArray>

namespace Common::Utils::Compression
{
/**
 * @brief Compresses data with Huffman coding and LZX.
 */
[[nodiscard]] LIBDRAWYGUI_EXPORT QByteArray compressData(const QByteArray &data);

/**
 * @brief Performs the inverse operation of compressData().
 */
[[nodiscard]] LIBDRAWYGUI_EXPORT QByteArray decompressData(const QByteArray &data);
} // namespace Common::Utils::Compression
