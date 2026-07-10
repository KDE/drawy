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
{
}

SpellCheckHighlighter::~SpellCheckHighlighter()
{
}

void SpellCheckHighlighter::highlightBlock(const QString &text)
{
    const QString language = m_languageGuesser.identify(text);
    m_speller.setLanguage(language);

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

            if (m_speller.isMisspelled(word)) {
                setFormat(start, length, format);
            }
        }
    }
}
