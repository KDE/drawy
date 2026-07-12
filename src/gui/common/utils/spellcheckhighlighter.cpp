/*
 * SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "spellcheckhighlighter.hpp"

#include <QTextBoundaryFinder>
#include <QTextCharFormat>

SpellCheckHighlighter::SpellCheckHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
    , m_settings(new Sonnet::Settings(this))
{
    m_speller.setLanguage(m_settings->defaultLanguage());
}

bool SpellCheckHighlighter::isMisspelled(const QString &word, const QString &block)
{
    if (m_settings->autodetectLanguage()) {
        const QString guessed = m_languageGuesser.identify(block, m_settings->preferredLanguages());
        if (!guessed.isEmpty()) {
            m_speller.setLanguage(guessed);
        }
    } else {
        m_speller.setLanguage(m_settings->defaultLanguage());
    }

    if (!m_speller.isValid()) {
        return false;
    }

    return isSpellcheckable(word) && m_speller.isMisspelled(word);
}

QStringList SpellCheckHighlighter::getSuggestions(const QString &word) const
{
    return m_speller.suggest(word);
}

void SpellCheckHighlighter::ignoreWord(const QString &word)
{
    m_speller.addToSession(word);
    rehighlight();
}

void SpellCheckHighlighter::addWordToDictionary(const QString &word)
{
    m_speller.addToPersonal(word);
    rehighlight();
}

bool SpellCheckHighlighter::isSpellcheckable(const QString &word) const
{
    if (m_settings->skipUppercase() && word.toUpper() == word) {
        return false;
    }
    if (m_settings->currentIgnoreList().contains(word)) {
        return false;
    }
    return true;
}

void SpellCheckHighlighter::highlightBlock(const QString &text)
{
    if (!m_settings->checkerEnabledByDefault()) {
        return;
    }

    if (m_settings->autodetectLanguage()) {
        const QString guessed = m_languageGuesser.identify(text, m_settings->preferredLanguages());
        if (!guessed.isEmpty()) {
            m_speller.setLanguage(guessed);
        }
    } else {
        m_speller.setLanguage(m_settings->defaultLanguage());
    }

    if (!m_speller.isValid()) {
        return;
    }

    QTextCharFormat format;
    format.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
    format.setUnderlineColor(Qt::red);

    QTextBoundaryFinder finder(QTextBoundaryFinder::Word, text);
    int start = 0;
    while (finder.toNextBoundary() != -1) {
        if (finder.boundaryReasons() & QTextBoundaryFinder::StartOfItem) {
            start = finder.position();
        } else if (finder.boundaryReasons() & QTextBoundaryFinder::EndOfItem) {
            const int end = finder.position();
            const int length = end - start;
            const QString word = text.mid(start, length);

            if (isSpellcheckable(word) && m_speller.isMisspelled(word)) {
                setFormat(start, length, format);
            }
        }
    }
}
