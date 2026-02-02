// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "boardlayout.hpp"

#include <QWidget>
#include <qnamespace.h>

BoardLayout::BoardLayout(QWidget *parent)
    : QLayout(parent)
{
    m_widgets[0] = &m_centralWidget;
    m_widgets[1] = &m_leftWidget;
    m_widgets[2] = &m_topWidget;
    m_widgets[3] = &m_rightWidget;
    m_widgets[4] = &m_bottomWidget;
}

BoardLayout::~BoardLayout()
{
    delete m_centralWidget;
    delete m_leftWidget;
    delete m_topWidget;
    delete m_rightWidget;
    delete m_bottomWidget;
}

void BoardLayout::addItem(QLayoutItem *item)
{
    if (m_centralWidget == nullptr) {
        m_centralWidget = item;
    } else {
        delete item;
    }
}

QLayoutItem *BoardLayout::itemAt(int index) const
{
    try {
        return *m_widgets.at(index);
    } catch (const std::out_of_range &) {
        return nullptr;
    }
}

QLayoutItem *BoardLayout::takeAt(int index)
{
    try {
        QLayoutItem *item{*m_widgets.at(index)};
        *(m_widgets.at(index)) = nullptr;
        return item;
    } catch (const std::out_of_range &) {
        return nullptr;
    }
}

void BoardLayout::setLeftWidget(QWidget *const item, bool pinned)
{
    delete m_leftWidget;
    m_leftWidget = new QWidgetItem(item);
    m_isLeftPinned = pinned;
    if (item) {
        item->raise();
    }
}

void BoardLayout::setTopWidget(QWidget *const item, bool pinned)
{
    delete m_topWidget;
    m_topWidget = new QWidgetItem(item);
    m_isTopPinned = pinned;
    if (item) {
        item->raise();
    }
}

void BoardLayout::setRightWidget(QWidget *const item, bool pinned)
{
    delete m_rightWidget;
    m_rightWidget = new QWidgetItem(item);
    m_isRightPinned = pinned;
    if (item) {
        item->raise();
    }
}

void BoardLayout::setBottomWidget(QWidget *const item, bool pinned)
{
    delete m_bottomWidget;
    m_bottomWidget = new QWidgetItem(item);
    m_isBottomPinned = pinned;
    if (item) {
        item->raise();
    }
}

void BoardLayout::setCentralWidget(QWidget *const item)
{
    delete m_centralWidget;
    m_centralWidget = new QWidgetItem(item);
}

void BoardLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
    if (m_centralWidget != nullptr) {
        m_centralWidget->setGeometry(rect);
        if (QWidget *w = m_centralWidget->widget()) {
            w->lower();
        }
    }

    if (m_leftWidget != nullptr) {
        QSize size{effectiveSize(m_leftWidget, rect.size())};
        m_leftWidget->setGeometry(QRect(m_margins, (rect.height() - size.height()) / 2, size.width(), size.height()));
    }
    if (m_rightWidget != nullptr) {
        QSize size{effectiveSize(m_rightWidget, rect.size())};
        m_rightWidget->setGeometry(QRect(rect.width() - size.width() - m_margins, (rect.height() - size.height()) / 2, size.width(), size.height()));
    }
    if (m_topWidget != nullptr) {
        QSize size{effectiveSize(m_topWidget, rect.size())};
        m_topWidget->setGeometry(QRect((rect.width() - size.width()) / 2 + m_margins, m_margins, size.width() - 2 * m_margins, size.height()));
    }
    if (m_bottomWidget != nullptr) {
        QSize size{effectiveSize(m_bottomWidget, rect.size())};
        m_bottomWidget->setGeometry(
            QRect((rect.width() - size.width()) / 2 + m_margins, rect.height() - size.height() - m_margins, size.width() - 2 * m_margins, size.height()));
    }
}

int BoardLayout::count() const
{
    // there are only 5 widgets so its o(1)
    int count{0};
    for (const auto item : m_widgets) {
        if (*item != nullptr) {
            count++;
        }
    }
    return count;
}

QSize BoardLayout::sizeHint() const
{
    return (m_centralWidget == nullptr ? QSize(0, 0) : m_centralWidget->sizeHint());
}

QSize BoardLayout::minimumSize() const
{
    return (m_centralWidget == nullptr ? QSize(10, 10) : m_centralWidget->minimumSize());
}

void BoardLayout::setMargins(int margins)
{
    m_margins = margins;
}

QSize BoardLayout::effectiveSize(QLayoutItem *item, const QSize &available)
{
    QSize s = item->sizeHint();
    QSize min = item->minimumSize();
    QSize max = item->maximumSize();

    if (item->expandingDirections() & Qt::Horizontal)
        s.setWidth(available.width());
    if (item->expandingDirections() & Qt::Vertical)
        s.setHeight(available.height());

    s.setWidth(qBound(min.width(), s.width(), max.width()));
    s.setHeight(qBound(min.height(), s.height(), max.height()));
    return s;
}

#include "moc_boardlayout.cpp"
