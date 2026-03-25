// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "arrowhead.hpp"
#include "circlearrowhead.hpp"
#include "diamondarrowhead.hpp"
#include "filledarrowhead.hpp"
#include "nonearrowhead.hpp"
#include "openarrowhead.hpp"
#include "unfilledarrowhead.hpp"

ArrowHead::ArrowHead(const QPointF &start, const QPointF &end)
    : m_start{start}
    , m_end{end}
{
}

ArrowHead::~ArrowHead() = default;

void ArrowHead::setBounds(const QPointF &start, const QPointF &end)
{
    m_start = start;
    m_end = end;
}

std::shared_ptr<ArrowHead> ArrowHead::create(ArrowHead::Type arrowType, const QPointF &start, const QPointF &end)
{
    switch (arrowType) {
    case ArrowHead::Type::Open:
        return std::make_shared<OpenArrowHead>(start, end);
    case ArrowHead::Type::Unfilled:
        return std::make_shared<UnfilledArrowHead>(start, end);
    case ArrowHead::Type::Filled:
        return std::make_shared<FilledArrowHead>(start, end);
    case ArrowHead::Type::Circle:
        return std::make_shared<CircleArrowHead>(start, end);
    case ArrowHead::Type::Diamond:
        return std::make_shared<DiamondArrowHead>(start, end);
    case ArrowHead::Type::None:
        return std::make_shared<NoneArrowHead>(start, end);
    }

    return std::make_shared<NoneArrowHead>(start, end);
}

#include "moc_arrowhead.cpp"
