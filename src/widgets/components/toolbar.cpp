// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "toolbar.hpp"

#include "context/applicationcontext.hpp"
#include "context/uicontext.hpp"
#include "iconmanager/iconmanager.hpp"
using namespace Qt::Literals::StringLiterals;
ToolBar::ToolBar(QWidget *parent)
    : QFrame{parent}
    , m_group(new QButtonGroup(this))
    , m_layout(new QHBoxLayout(this))
{
    setLayout(m_layout);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setFrameShape(QFrame::StyledPanel);
    setFrameShadow(QFrame::Raised);
    setAutoFillBackground(true);
    setProperty("class", u"drawlyFrame drawlyToolBar"_s);

    connect(m_group, &QButtonGroup::idClicked, this, &ToolBar::onToolChanged);
}

ToolBar::~ToolBar()
{
}

Tool *ToolBar::curTool() const
{
    const Tool::Type curID{static_cast<Tool::Type>(m_group->checkedId())};

    if (m_tools.find(curID) == m_tools.end())
        throw std::logic_error("Trying to access non existent tool");

    return m_tools.at(curID).get();
}

QVector<std::shared_ptr<Tool>> ToolBar::tools() const
{
    QVector<std::shared_ptr<Tool>> result;
    for (const auto &idToolPair : m_tools) {
        result.push_back(idToolPair.second);
    }

    return result;
}

void ToolBar::addTool(std::shared_ptr<Tool> tool, Tool::Type type)
{
    if (tool == nullptr)
        return;

    ApplicationContext *context{ApplicationContext::instance()};
    QPushButton *btn{new QPushButton(this)};
    btn->setIcon(context->uiContext()->iconManager()->icon(tool->icon()));

    btn->setCheckable(true);
    btn->setProperty("class", u"drawlyToolButton"_s);
    btn->setCursor(Qt::PointingHandCursor);

    m_tools[type] = tool;
    m_group->addButton(btn, static_cast<int>(type));
    m_layout->addWidget(btn);
    if (m_tools.size() == 1) {
        m_group->button(static_cast<int>(type))->setChecked(true);
        Q_EMIT toolChanged(tool.get());
    }
}

void ToolBar::changeTool(Tool::Type type)
{
    const int id{static_cast<int>(type)};

    m_group->button(id)->setChecked(true);
    Q_EMIT toolChanged(curTool());
}

Tool *ToolBar::tool(Tool::Type type) const
{
    return m_tools.at(type).get();
}

void ToolBar::onToolChanged(int id)
{
    Q_EMIT toolChanged(curTool());
}

#include "moc_toolbar.cpp"
