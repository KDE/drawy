/*
 * SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "drawycommandlineparser.hpp"
#include "config-drawy.hpp"
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
    parser->addOption(QCommandLineOption(QStringList() << optionParserFromEnum(OptionParser::Debug), i18nc("@info:shell", "Activate Debug Mode")));
#if HAVE_KUSERFEEDBACK
    parser->addOption(QCommandLineOption(optionParserFromEnum(OptionParser::FeedBack), i18nc("@info:shell", "Lists the available options for user feedback")));
#endif
    QCommandLineOption selfTestOption(optionParserFromEnum(OptionParser::SelfTest), QStringLiteral("internal, for automated testing"));
    selfTestOption.setFlags(QCommandLineOption::HiddenFromHelp);
    parser->addOption(selfTestOption);
}

QString DrawyCommandLineParser::optionParserFromEnum(OptionParser e)
{
    switch (e) {
    case OptionParser::FullScreen:
        return u"fullscreen"_s;
    case OptionParser::Debug:
        return u"debug"_s;
#if HAVE_KUSERFEEDBACK
    case OptionParser::FeedBack:
        return u"feedback"_s;
#endif
    case OptionParser::SelfTest:
        return u"self-test"_s;
    }
    return {};
}
