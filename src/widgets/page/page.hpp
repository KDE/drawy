/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "data-structures/quadtree.hpp"
#include "libdrawywidgets_private_export.h"
#include <memory>
class QuadTree;
class ApplicationContext;
class LIBDRAWYWIDGETS_TESTS_EXPORT Page : public QObject
{
    Q_OBJECT
public:
    explicit Page(ApplicationContext *context);
    ~Page();

    [[nodiscard]] QString name() const;
    void setName(const QString &newName);

private:
    QString mName;
    std::unique_ptr<QuadTree> m_quadtree{nullptr};
    ApplicationContext *const m_applicationContext;
};
