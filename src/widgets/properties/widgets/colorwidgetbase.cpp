// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "colorwidgetbase.hpp"

#include "properties/property.hpp"
#include <KLocalizedString>
#include <KSeparator>
#include <QButtonGroup>
#include <QColor>
#include <QColorDialog>
#include <QHBoxLayout>
#include <QToolButton>

using namespace Qt::Literals::StringLiterals;
ColorWidgetBase::ColorWidgetBase(QWidget *parent)
    : PropertyWidget{parent}
{
    m_widget = new QWidget{parent};
    m_group = new QButtonGroup{m_widget};
}

void ColorWidgetBase::initialize()
{
    auto layout{new QHBoxLayout{m_widget}};
    layout->setContentsMargins({});
    layout->setAlignment(Qt::AlignLeft);

    const QList<QColor> colors = defaultColors();

    auto colorButtonHLayout = new QHBoxLayout;
    colorButtonHLayout->setContentsMargins({});
    colorButtonHLayout->setSpacing(0);
    layout->addLayout(colorButtonHLayout);
    for (const QColor &color : colors) {
        auto btn{new QToolButton{m_widget}};
        btn->setCheckable(true);
        btn->setStyleSheet(u"background-color: "_s + color.name());
        btn->setProperty("color-value", color);
        colorButtonHLayout->addWidget(btn);
        m_group->addButton(btn);
    }

    auto separator = new KSeparator(Qt::Vertical, m_widget);
    separator->setObjectName(u"separator"_s);
    layout->addWidget(separator);

    m_currentColorButton = new QToolButton{m_widget};
    m_currentColorButton->setObjectName(u"m_currentColorButton"_s);
    m_currentColorButton->setCheckable(false);
    m_currentColorButton->setToolTip(i18nc("@info:tooltip", "Select Custom Color"));
    connect(m_currentColorButton, &QToolButton::clicked, [this]() {
        const QColor col = QColorDialog::getColor(m_currentColorButton->property("color-value").value<QColor>(), m_widget);
        if (col.isValid()) {
            assignCurrentColor(col);
            Q_EMIT changed(value());
        }
    });
    layout->addWidget(m_currentColorButton);

    connect(m_group, &QButtonGroup::idClicked, this, [this]() {
        const QColor selectedColor = m_group->checkedButton()->property("color-value").value<QColor>();
        assignCurrentColor(selectedColor);
        Q_EMIT changed(value());
    });
    m_group->buttons().at(0)->setChecked(true);
    assignCurrentColor(m_group->buttons().at(0)->property("color-value").value<QColor>());
    m_widget->hide();
}

void ColorWidgetBase::assignCurrentColor(const QColor &col)
{
    m_currentColorButton->setProperty("color-value", col);
    m_currentColorButton->setStyleSheet(u"background-color: "_s + col.name());
}

QString ColorWidgetBase::name() const
{
    return i18n("Color");
}

void ColorWidgetBase::setValue(const QVariant &val)
{
    assignCurrentColor(val.value<QColor>());
}

#include "moc_colorwidgetbase.cpp"
