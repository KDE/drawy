/*
 * SPDX-FileCopyrightText: 2026 Nikolay Kochulin <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "svgexporttest.hpp"
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
    QFETCH(QString, input);
    QFETCH(QString, output);
    LoadJob loadJob(nullptr);
    loadJob.setFileName(QLatin1StringView(DRAWY_DATA_DIR) + u"/svg/"_s + input);
    connect(&loadJob, &LoadJob::loadDone, [output](const LoadJob::LoadInfo &info) {
        QByteArray data;
        QXmlStreamWriter writer(&data);
        writer.setAutoFormatting(true);

        SvgSerializer::writeSvg(writer, info.items);

        AutoTestHelper::compareFile(u"/svg/"_s, data, output);
    });
    loadJob.start();
}

#include "moc_svgexporttest.cpp"
