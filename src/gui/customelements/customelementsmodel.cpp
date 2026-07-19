/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "customelementsmodel.hpp"

CustomElementsModel::CustomElementsModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

CustomElementsModel::~CustomElementsModel() = default;

int CustomElementsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) { // flat model
        return 0;
    }
    return mCustomElements.count();
}

QVariant CustomElementsModel::data([[maybe_unused]] const QModelIndex &index, [[maybe_unused]] int role) const
{
    if (index.row() < 0 || index.row() >= mCustomElements.count()) {
        return {};
    }
    const auto customElement = mCustomElements.at(index.row());
    switch (role) {
    case TimeStamp:
        return customElement.timeStamp();
    }
    return {};
}

QList<CustomElement> CustomElementsModel::customElements() const
{
    return mCustomElements;
}

void CustomElementsModel::addCustomElement(const CustomElement &element)
{
    const int numberOfElement = mCustomElements.count();
    beginInsertRows(QModelIndex(), numberOfElement, numberOfElement);
    mCustomElements.append(element);
    endInsertRows();
}

void CustomElementsModel::setCustomElements(const QList<CustomElement> &newCustomElements)
{
    beginResetModel();
    mCustomElements = newCustomElements;
    endResetModel();
}

bool CustomElementsModel::isEmpty() const
{
    return mCustomElements.isEmpty();
}

#include "moc_customelementsmodel.cpp"
