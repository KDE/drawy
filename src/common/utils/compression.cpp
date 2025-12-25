/*
* Drawy - A simple brainstorming tool with an infinite canvas
* Copyright (C) 2025 - Prayag Jain <prayagjain2@gmail.com>
*
* Authors:
* 1. quarterstar - quarterstar@proton.me
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "compression.hpp"

#include <QDebug>
#include <zstd.h>

namespace Common::Utils::Compression {
QByteArray compressData(const QByteArray &data) {
    size_t requiredOutputSize = ZSTD_compressBound(data.size());

    QByteArray output{static_cast<qsizetype>(requiredOutputSize), '\0'};

    size_t result = ZSTD_compress(output.data(), requiredOutputSize, data.data(), data.size(), 4);

    if (ZSTD_isError(result))
        throw std::runtime_error(ZSTD_getErrorName(result));

    output.resize(result);
    return output;
}

QByteArray decompressData(const QByteArray &data) {
    unsigned long long originalSize =
        ZSTD_getFrameContentSize(data.data(), data.size());

    if (originalSize == ZSTD_CONTENTSIZE_ERROR)
        throw std::runtime_error("Invalid ZSTD frame");

    if (originalSize == ZSTD_CONTENTSIZE_UNKNOWN)
        throw std::runtime_error("Unknown decompressed size");

    QByteArray output(originalSize, '\0');

    size_t result = ZSTD_decompress(
        output.data(),
        originalSize,
        data.data(),
        data.size()
    );

    if (ZSTD_isError(result))
        throw std::runtime_error(ZSTD_getErrorName(result));

    return output;
}
}  // namespace Common::Utils::Compression
