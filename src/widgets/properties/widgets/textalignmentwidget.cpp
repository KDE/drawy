/*
 * SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "textalignmentwidget.hpp"

#include "common/constants.hpp"
#include "properties/property.hpp"
#include <KLocalizedString>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QIcon>
#include <QToolButton>
using namespace Qt::Literals::StringLiterals;

TextAlignmentWidget::TextAlignmentWidget(QWidget *parent)
    : PropertyWidget{parent}
{
    m_widget = new QWidget(parent);
    m_widget->setObjectName(u"m_widget"_s);

    auto layout = new QHBoxLayout(m_widget);
    layout->setAlignment(Qt::AlignCenter);
    layout->setObjectName(u"layout"_s);

    m_alignLeft = new QToolButton(m_widget);
    m_alignLeft->setCheckable(true);
    m_alignLeft->setIcon(QIcon::fromTheme(u"format-justify-left"_s));
    m_alignLeft->setToolTip(i18nc("@info:tooltip", "Align Left"));
    m_alignLeft->setIconSize(Common::textAlignmentIconSize);
    m_alignLeft->setAutoRaise(true);
    m_alignLeft->setObjectName(u"m_alignLeft"_s);
    layout->addWidget(m_alignLeft);

    m_alignCenter = new QToolButton(m_widget);
    m_alignCenter->setCheckable(true);
    m_alignCenter->setIcon(QIcon::fromTheme(u"format-justify-center"_s));
    m_alignCenter->setToolTip(i18nc("@info:tooltip", "Align Center"));
    m_alignCenter->setIconSize(Common::textAlignmentIconSize);
    m_alignCenter->setAutoRaise(true);
    m_alignCenter->setObjectName(u"m_alignCenter"_s);
    layout->addWidget(m_alignCenter);

    m_alignRight = new QToolButton(m_widget);
    m_alignRight->setCheckable(true);
    m_alignRight->setIcon(QIcon::fromTheme(u"format-justify-right"_s));
    m_alignRight->setToolTip(i18nc("@info:tooltip", "Align Right"));
    m_alignRight->setIconSize(Common::textAlignmentIconSize);
    m_alignRight->setAutoRaise(true);
    m_alignRight->setObjectName(u"m_alignRight"_s);
    layout->addWidget(m_alignRight);

    m_alignJustify = new QToolButton(m_widget);
    m_alignJustify->setCheckable(true);
    m_alignJustify->setIcon(QIcon::fromTheme(u"format-justify-fill"_s));
    m_alignJustify->setToolTip(i18nc("@info:tooltip", "Justify"));
    m_alignJustify->setIconSize(Common::textAlignmentIconSize);
    m_alignJustify->setAutoRaise(true);
    m_alignJustify->setObjectName(u"m_alignJustify"_s);
    layout->addWidget(m_alignJustify);

    auto buttonGroup = new QButtonGroup(m_widget);
    buttonGroup->addButton(m_alignLeft, Qt::AlignLeft);
    buttonGroup->addButton(m_alignCenter, Qt::AlignCenter);
    buttonGroup->addButton(m_alignRight, Qt::AlignRight);
    buttonGroup->addButton(m_alignJustify, Qt::AlignJustify);
    buttonGroup->setObjectName(u"buttonGroup"_s);

    connect(buttonGroup, &QButtonGroup::idClicked, this, [this] {
        Q_EMIT changed(value());
    });

    m_widget->hide();
}

void TextAlignmentWidget::setValue(const QVariant &value)
{
    const QString align = value.toString();
    const QSignalBlocker blocker(m_widget->findChild<QButtonGroup *>());

    if (align == u"AlignLeft"_s) {
        m_alignLeft->setChecked(true);
    } else if (align == u"AlignCenter"_s) {
        m_alignCenter->setChecked(true);
    } else if (align == u"AlignRight"_s) {
        m_alignRight->setChecked(true);
    } else if (align == u"AlignJustify"_s) {
        m_alignJustify->setChecked(true);
    }
}

QString TextAlignmentWidget::name() const
{
    return i18n("Text Alignment");
}

Property TextAlignmentWidget::value() const
{
    QString alignment = u"AlignLeft"_s;
    if (m_alignCenter->isChecked()) {
        alignment = u"AlignCenter"_s;
    } else if (m_alignRight->isChecked()) {
        alignment = u"AlignRight"_s;
    } else if (m_alignJustify->isChecked()) {
        alignment = u"AlignJustify"_s;
    }
    return Property(alignment, Property::Type::TextAlignment);
}

#include "moc_textalignmentwidget.cpp"
