/*
 * SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawywidgets_export.h"
#include <QString>
class QCommandLineParser;
class LIBDRAWYWIDGETS_EXPORT DrawyCommandLineParser
{
public:
    enum class OptionParser : uint8_t {
        FullScreen = 0,
    };
    explicit DrawyCommandLineParser(QCommandLineParser *parser);
    ~DrawyCommandLineParser();

    [[nodiscard]] static QString optionParserFromEnum(OptionParser e);

private:
    void initializeCommandLine(QCommandLineParser *parser);
};
