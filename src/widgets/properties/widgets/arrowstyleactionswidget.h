/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include "item/arrowutils.hpp"
#include <QToolButton>

class ArrowStyleActionsWidget : public QToolButton
{
    Q_OBJECT
public:
    explicit ArrowStyleActionsWidget(ArrowUtils::ArrowType arrowStyle, QWidget *parent = nullptr);
    ~ArrowStyleActionsWidget() override;

private:
    ArrowUtils::ArrowType mArrowStyle = ArrowUtils::ArrowType::None;
};
