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
#include <QSignalSpy>
#include <QTest>
#include <QXmlStreamWriter>

using namespace Qt::Literals::StringLiterals;

SvgExportTest::SvgExportTest(QObject *parent)
    : QObject{parent}
{
}

void SvgExportTest::shouldNotChangeOutput()
{
    QFontDatabase::addApplicationFont(u":/fonts/FuzzyBubbles.ttf"_s);

    LoadJob loadJob;

    loadJob.setFileName(QLatin1StringView(DRAWY_DATA_DIR) + u"/svg/export-test.drawy"_s);

    connect(&loadJob, &LoadJob::loadDone, [](const LoadJob::LoadInfo &info) {
        QByteArray data;
        QXmlStreamWriter writer(&data);
        writer.setAutoFormatting(true);

        SvgSerializer::writeSvg(writer, info.items);

        AutoTestHelper::compareFile(u"/svg/"_s, data, u"export-test.svg"_s);
    });

    loadJob.start();
}

QTEST_MAIN(SvgExportTest)

#include "moc_svgexporttest.cpp"
