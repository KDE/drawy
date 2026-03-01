/*
 * SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "imageitemtest.hpp"
#include "drawy_autotest_helper.hpp"
#include "item/image.hpp"
#include <QJsonObject>
#include <QTest>

QTEST_MAIN(ImageItemTest)

using namespace Qt::Literals::StringLiterals;

ImageItemTest::ImageItemTest(QObject *parent)
    : QObject{parent}
{
}

void ImageItemTest::shouldHaveDefaultValues()
{
    const ImageItem i;
    QCOMPARE(i.formType(), Item::FormType::Image);
    QVERIFY(!i.id().isEmpty());
    QVERIFY(!i.locked());
}

void ImageItemTest::shouldSerializeDefaultValue()
{
    ImageItem f;
    // Need to have an known id
    f.setId("acff679ae3c14260b56ef00f1d354883"_ba);
    const QJsonObject obj = f.serialize(-1);
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/image/"_s, ba, u"defaultvalue"_s);
}

void ImageItemTest::shouldSerializeImage()
{
    ImageItem f;
    // Need to have an known id
    f.setId("acff679ae3c14260b56ef00f1d354883"_ba);
    QPixmap pixmap;
    pixmap.load(QLatin1StringView(DRAWY_GUI_DATA_DIR) + u"/image/testimage.png"_s);
    f.setPixmap(pixmap);
    const QJsonObject obj = f.serialize(-1);
    const QJsonDocument doc(obj);
    const QByteArray ba = doc.toJson();
    AutoTestHelper::compareFile(u"/image/"_s, ba, u"image"_s);
}

#include "moc_imageitemtest.cpp"
