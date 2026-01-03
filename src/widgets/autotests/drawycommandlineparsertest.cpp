/*
 * SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "drawycommandlineparsertest.h"
#include "drawycommandlineparser.h"
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
}
