/*
 * SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "drawycommandlineparser.hpp"
#include <KLocalizedString>
#include <QCommandLineParser>

using namespace Qt::Literals::StringLiterals;
DrawyCommandLineParser::DrawyCommandLineParser(QCommandLineParser *parser)
{
    initializeCommandLine(parser);
}

void DrawyCommandLineParser::initializeCommandLine(QCommandLineParser *parser)
{
    parser->addOption(QCommandLineOption(QStringList() << optionParserFromEnum(OptionParser::FullScreen), i18nc("@info:shell", "Fullscreen")));
}

QString DrawyCommandLineParser::optionParserFromEnum(OptionParser e)
{
    switch (e) {
    case OptionParser::FullScreen:
        return u"fullscreen"_s;
    }
    return {};
}
