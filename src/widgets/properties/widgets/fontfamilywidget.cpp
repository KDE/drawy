/*
 * SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "fontfamilywidget.hpp"

#include "drawyglobalconfig.h"
#include "properties/property.hpp"

#include <KLocalizedString>
#include <QFont>
#include <QFontComboBox>
#include <QFontDatabase>
#include <QLineEdit>
#include <QListView>
using namespace Qt::Literals::StringLiterals;

namespace
{
constexpr int fontSize = 8;
constexpr int popupWidth = 500;
}

FontFamilyWidget::FontFamilyWidget(QWidget *parent)
    : PropertyWidget{parent}
    , m_fontComboBox(new QFontComboBox(parent))
{
    m_fontComboBox->hide();

    QFont font = m_fontComboBox->font();
    font.setPointSize(fontSize);
    m_fontComboBox->setFont(font);
    m_fontComboBox->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    m_fontComboBox->setCurrentFont(QFont(DrawyGlobalConfig::self()->fontFamily()));
    m_fontComboBox->lineEdit()->setCursorPosition(0);
    m_fontComboBox->setObjectName(u"m_fontComboBox"_s);
    m_widget = m_fontComboBox;

    connect(m_fontComboBox, &QFontComboBox::currentIndexChanged, this, [this] {
        const QString font = m_fontComboBox->currentFont().family();
        if (QFontDatabase::families().contains(font, Qt::CaseInsensitive)) {
            DrawyGlobalConfig::self()->setFontFamily(font);
            DrawyGlobalConfig::self()->save();
            Q_EMIT changed(value());
        }
    });

    connect(m_fontComboBox->lineEdit(), &QLineEdit::editingFinished, this, [this] {
        const QString font = m_fontComboBox->currentFont().family();
        if (QFontDatabase::families().contains(font, Qt::CaseInsensitive)) {
            DrawyGlobalConfig::self()->setFontFamily(font);
            DrawyGlobalConfig::self()->save();
            Q_EMIT changed(value());
        } else {
            m_fontComboBox->setCurrentFont(QFont(DrawyGlobalConfig::self()->fontFamily()));
        }
    });
    m_fontComboBox->installEventFilter(this);
}

void FontFamilyWidget::updateWidget()
{
    const QSignalBlocker blocker(m_fontComboBox);
    m_fontComboBox->setCurrentFont(QFont(DrawyGlobalConfig::self()->fontFamily()));
}

bool FontFamilyWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_fontComboBox && event->type() == QEvent::Show) {
        m_fontComboBox->view()->window()->setFixedWidth(popupWidth);
    }
    return PropertyWidget::eventFilter(watched, event);
}

QString FontFamilyWidget::name() const
{
    return i18n("Font Family");
}

Property FontFamilyWidget::value() const
{
    return Property{m_fontComboBox->currentFont().family(), Property::Type::FontFamily};
}

void FontFamilyWidget::setValue(const QVariant &val)
{
    const QSignalBlocker blocker(m_fontComboBox);
    m_fontComboBox->setCurrentFont(QFont(val.toString()));
    m_fontComboBox->lineEdit()->setCursorPosition(0);
}

#include "moc_fontfamilywidget.cpp"
