/*
 * SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "drawycommandlineparsertest.hpp"
#include "config-drawy.hpp"
#include "drawycommandlineparser.hpp"
#include <QTest>
QTEST_GUILESS_MAIN(DrawyCommandLineParserTest)
using namespace Qt::Literals::StringLiterals;

DrawyCommandLineParserTest::DrawyCommandLineParserTest(QObject *parent)
    : QObject{parent}
{
}

void DrawyCommandLineParserTest::shouldHaveDefaultValues()
{
    QCOMPARE(DrawyCommandLineParser::optionParserFromEnum(DrawyCommandLineParser::OptionParser::FullScreen), u"fullscreen"_s);
    QCOMPARE(DrawyCommandLineParser::optionParserFromEnum(DrawyCommandLineParser::OptionParser::Debug), u"debug"_s);
#if HAVE_KUSERFEEDBACK
    QCOMPARE(DrawyCommandLineParser::optionParserFromEnum(DrawyCommandLineParser::OptionParser::FeedBack), u"feedback"_s);
#endif
}

#include "moc_drawycommandlineparsertest.cpp"
