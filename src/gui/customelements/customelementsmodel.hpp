/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once
#include "customelement.hpp"
#include "libdrawygui_export.h"
#include <QAbstractListModel>
#include <QList>

class LIBDRAWYGUI_EXPORT CustomElementsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum CustomElementRoles {
        TimeStamp = Qt::UserRole + 1,
    };
    Q_ENUM(CustomElementRoles)

    explicit CustomElementsModel(QObject *parent = nullptr);
    ~CustomElementsModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] QList<CustomElement> customElements() const;
    void setCustomElements(const QList<CustomElement> &newCustomElements);

    [[nodiscard]] bool isEmpty() const;

    void addCustomElement(const CustomElement &element);

private:
    QList<CustomElement> mCustomElements;
};
