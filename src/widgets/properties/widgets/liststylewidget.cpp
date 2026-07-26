/*
 * SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "liststylewidget.hpp"
#include "properties/property.hpp"
#include <KLocalizedString>
#include <QComboBox>
#include <QHBoxLayout>

using namespace Qt::Literals::StringLiterals;

ListStyleWidget::ListStyleWidget(QWidget *parent)
    : PropertyWidget{parent}
    , m_comboBox{new QComboBox}
{
    m_widget = new QWidget(parent);
    const auto layout = new QHBoxLayout(m_widget);
    layout->setContentsMargins({});

    m_comboBox->addItem(i18n("None"), u"None"_s);
    m_comboBox->addItem(i18n("Bullet (Disc)"), u"ListDisc"_s);
    m_comboBox->addItem(i18n("Bullet (Circle)"), u"ListCircle"_s);
    m_comboBox->addItem(i18n("Bullet (Square)"), u"ListSquare"_s);
    m_comboBox->addItem(i18n("Numbered (Decimal)"), u"ListDecimal"_s);
    m_comboBox->addItem(i18n("Numbered (Lower Alpha)"), u"ListLowerAlpha"_s);
    m_comboBox->addItem(i18n("Numbered (Upper Alpha)"), u"ListUpperAlpha"_s);
    m_comboBox->addItem(i18n("Numbered (Lower Roman)"), u"ListLowerRoman"_s);
    m_comboBox->addItem(i18n("Numbered (Upper Roman)"), u"ListUpperRoman"_s);
    layout->addWidget(m_comboBox);

    connect(m_comboBox, &QComboBox::currentIndexChanged, this, [this] {
        Q_EMIT changed(value());
    });

    m_widget->hide();
}

void ListStyleWidget::setValue(const QVariant &val)
{
    const QSignalBlocker blocker(m_comboBox);

    const QString style = val.toString();
    const int index = m_comboBox->findData(style);
    if (index != -1) {
        m_comboBox->setCurrentIndex(index);
    } else {
        m_comboBox->setCurrentIndex(0);
    }
}

QString ListStyleWidget::name() const
{
    return i18n("List Style");
}

Property ListStyleWidget::value() const
{
    return Property(m_comboBox->currentData().toString(), Property::Type::ListStyle);
}

#include "moc_liststylewidget.cpp"
