/*
 * SPDX-FileCopyrightText: 2026 Nikolay Kochulin <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "svgexporttest.hpp"
#include "common/constants.hpp"
#include "drawy_autotest_helper.hpp"
#include "jobs/loadjob.hpp"
#include "serializer/svgserializer.hpp"
#include <QFontDatabase>
#include <QTest>
#include <QXmlStreamWriter>

using namespace Qt::Literals::StringLiterals;
QTEST_MAIN(SvgExportTest)

SvgExportTest::SvgExportTest(QObject *parent)
    : QObject{parent}
{
}

void SvgExportTest::initTestCase()
{
    QFontDatabase::addApplicationFont(u":/fonts/FuzzyBubbles.ttf"_s);
}

void SvgExportTest::shouldNotChangeOutput_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");
    QTest::newRow("export-test.drawy") << u"export-test.drawy"_s << u"export-test.svg"_s;
}

void SvgExportTest::shouldNotChangeOutput()
{
#ifndef Q_OS_LINUX
    QSKIP(
        "Linux only test for now. Please contribute a Windows version of the "
        "file as the export algorithm is unstable.");
#endif
    QFETCH(const QString, input);
    QFETCH(const QString, output);
    LoadJob loadJob(nullptr);
    loadJob.setFileName(QLatin1StringView(DRAWY_DATA_DIR) + u"/svg/"_s + input);
    connect(&loadJob, &LoadJob::loadDone, this, [output](const LoadJob::LoadInfo &info) {
        QByteArray data;
        QXmlStreamWriter writer(&data);
        writer.setAutoFormatting(true);

        SvgSerializer::writeSvg(writer, info.items, Common::darkBackgroundColor);

        AutoTestHelper::compareFile(u"/svg/"_s, data, output);
    });
    loadJob.start();
}

void SvgExportTest::cleanupTestCase()
{
    QFontDatabase::removeAllApplicationFonts();
}

#include "moc_svgexporttest.cpp"
