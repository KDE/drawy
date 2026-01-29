// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "toolbar.hpp"
#include <QApplication>
#include <QStyle>
#include <qlogging.h>

using namespace Qt::Literals::StringLiterals;
ToolBar::ToolBar(QWidget *parent)
    : QFrame{parent}
    , m_group(new QButtonGroup(this))
    , m_layout(new QHBoxLayout(this))
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setFrameShape(QFrame::StyledPanel);
    setFrameShadow(QFrame::Raised);
    setAutoFillBackground(true);

    QPalette pal{QApplication::palette()};
    QColor borderColor{pal.color(QPalette::Text)};
    borderColor.setAlpha(51); // 0.2%

    QString frameStyleSheet = QString(
                                  u"ToolBar {"_s
                                  u"  border: %1px solid %2;"_s
                                  u"  border-radius: %3px;"_s
                                  u"  background: %4;"_s
                                  u"}"_s)
                                  .arg(1)
                                  .arg(borderColor.name(QColor::HexArgb))
                                  .arg(style()->pixelMetric(QStyle::PM_ToolBarItemMargin))
                                  .arg(pal.color(QPalette::Window).name());

    setStyleSheet(frameStyleSheet);

    m_layout->setSpacing(style()->pixelMetric(QStyle::PM_LayoutHorizontalSpacing));
    connect(m_group, &QButtonGroup::idClicked, this, &ToolBar::onToolChanged);
}

ToolBar::~ToolBar() = default;

Tool &ToolBar::curTool() const
{
    const Tool::Type curID{static_cast<Tool::Type>(m_group->checkedId())};

    if (m_tools.find(curID) == m_tools.end())
        throw std::logic_error("Trying to access a non existent tool");

    return *m_tools.at(curID);
}

void ToolBar::addTool(const std::shared_ptr<Tool> &tool, Tool::Type type, const QString &name)
{
    if (!tool) {
        return;
    }

    auto btn{new QToolButton(this)};
    btn->setToolTip(name);
    btn->setIcon(QIcon::fromTheme(tool->icon()));
    btn->setAutoRaise(true);

    int iconSize{style()->pixelMetric(QStyle::PM_ToolBarIconSize)};
    btn->setIconSize(QSize{iconSize, iconSize});

    btn->setCheckable(true);
    btn->setCursor(Qt::PointingHandCursor);

    m_tools[type] = tool;
    m_group->addButton(btn, static_cast<int>(type));
    m_layout->addWidget(btn);
    if (m_tools.size() == 1) {
        m_group->button(static_cast<int>(type))->setChecked(true);
        Q_EMIT toolChanged(*tool);
    }
}

void ToolBar::changeTool(Tool::Type type)
{
    const int id{static_cast<int>(type)};

    m_group->button(id)->setChecked(true);
    Q_EMIT toolChanged(curTool());
}

Tool &ToolBar::tool(Tool::Type type) const
{
    return *m_tools.at(type);
}

void ToolBar::onToolChanged([[maybe_unused]] int id)
{
    Q_EMIT toolChanged(curTool());
}

#include "moc_toolbar.cpp"
