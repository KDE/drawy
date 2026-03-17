/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "imagetooltest.hpp"
#include "tools/imagetool.hpp"
#include <QTest>
QTEST_GUILESS_MAIN(ImageToolTest)
using namespace Qt::Literals::StringLiterals;

ImageToolTest::ImageToolTest(QObject *parent)
    : QObject{parent}
{
}

void ImageToolTest::shouldHaveDefaultValues()
{
    const ImageTool t(nullptr);
    QCOMPARE(t.icon(), u"insert-image"_s);
    QCOMPARE(t.type(), Tool::Type::Image);
    const auto properties = QList<Property::Type>() << Property::Type::Opacity;
    QCOMPARE(t.allProperties().count(), 1);
    for (const auto &prop : properties) {
        QVERIFY(t.allProperties().contains(prop));
    }
}

#include "moc_imagetooltest.cpp"
