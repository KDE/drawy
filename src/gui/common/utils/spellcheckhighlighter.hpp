/*
 * SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "libdrawygui_export.h"

#include <QSyntaxHighlighter>
#include <Sonnet/GuessLanguage>
#include <Sonnet/Speller>

class LIBDRAWYGUI_EXPORT SpellCheckHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit SpellCheckHighlighter(QTextDocument *parent);
    ~SpellCheckHighlighter() override;

protected:
    void highlightBlock(const QString &text) override;

private:
    Sonnet::Speller m_speller;
    const Sonnet::GuessLanguage m_languageGuesser;
};
