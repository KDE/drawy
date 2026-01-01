// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "actionbar.hpp"

#include <format>
#include <stdexcept>

#include "../context/applicationcontext.hpp"
#include "../context/uicontext.hpp"
using namespace Qt::Literals::StringLiterals;
ActionBar::ActionBar(QWidget *parent)
    : QFrame{parent}
    , m_layout(new QHBoxLayout{this})
{
    m_layout->setSpacing(10);
    this->setLayout(m_layout);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

void ActionBar::addButton(const QString &tooltip, IconManager::Icon icon, int id)
{
    if (m_map.contains(id)) {
        throw std::logic_error("Button with same id exists in the ActionBar.");
    }

    ApplicationContext *context{ApplicationContext::instance()};

    m_map[id] = new QPushButton{this};
    m_map[id]->setIcon(context->uiContext()->iconManager()->icon(icon));
    m_map[id]->setToolTip(tooltip);

    m_map[id]->setProperty("class", u"drawlyActionButton"_s);
    m_map[id]->setCursor(Qt::PointingHandCursor);
    m_layout->addWidget(m_map[id]);
}

QPushButton *ActionBar::button(int id)
{
    if (!m_map.contains(id)) {
        throw std::logic_error(std::format("Button with id {} was not found.", id));
    }

    return m_map[id];
}

#include "moc_actionbar.cpp"
