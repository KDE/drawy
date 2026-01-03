// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QByteArray>
#include <QString>

namespace Common::Utils::Compression
{
/**
 * @brief Compresses data with Huffman coding and LZX.
 */
QByteArray compressData(const QByteArray &data);

/**
 * @brief Performs the inverse operation of compressData().
 */
QByteArray decompressData(const QByteArray &data);
} // namespace Common::Utils::Compression
