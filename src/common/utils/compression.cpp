// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "compression.hpp"

#include <zstd.h>

#include "drawy_debug.h"

namespace Common::Utils::Compression
{
QByteArray compressData(const QByteArray &data)
{
    size_t requiredOutputSize = ZSTD_compressBound(data.size());

    QByteArray output{static_cast<qsizetype>(requiredOutputSize), '\0'};

    size_t result = ZSTD_compress(output.data(), requiredOutputSize, data.data(), data.size(), 4);

    if (ZSTD_isError(result))
        throw std::runtime_error(ZSTD_getErrorName(result));

    output.resize(result);
    return output;
}

QByteArray decompressData(const QByteArray &data)
{
    unsigned long long originalSize = ZSTD_getFrameContentSize(data.data(), data.size());

    if (originalSize == ZSTD_CONTENTSIZE_ERROR)
        throw std::runtime_error("Invalid ZSTD frame");

    if (originalSize == ZSTD_CONTENTSIZE_UNKNOWN)
        throw std::runtime_error("Unknown decompressed size");

    QByteArray output(originalSize, '\0');

    size_t result = ZSTD_decompress(output.data(), originalSize, data.data(), data.size());

    if (ZSTD_isError(result))
        throw std::runtime_error(ZSTD_getErrorName(result));

    return output;
}
} // namespace Common::Utils::Compression
