/*
 * SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "libdrawygui_export.h"

#include <QSyntaxHighlighter>
#include <Sonnet/GuessLanguage>
#include <Sonnet/Settings>
#include <Sonnet/Speller>

class LIBDRAWYGUI_EXPORT SpellCheckHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit SpellCheckHighlighter(QTextDocument *parent);
    ~SpellCheckHighlighter() override = default;

    [[nodiscard]] bool isMisspelled(const QString &word, const QString &block);
    [[nodiscard]] QStringList getSuggestions(const QString &word) const;
    void ignoreWord(const QString &word);
    void addWordToDictionary(const QString &word);
    void setActive(bool active);

protected:
    void highlightBlock(const QString &text) override;

private:
    [[nodiscard]] LIBDRAWYGUI_NO_EXPORT bool isSpellcheckable(const QString &word) const;

    Sonnet::Speller m_speller;
    const Sonnet::GuessLanguage m_languageGuesser;
    const Sonnet::Settings *const m_settings;
    bool m_active{false};
};
