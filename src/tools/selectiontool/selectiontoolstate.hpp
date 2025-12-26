// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

class ApplicationContext;

class SelectionToolState {
public:
    virtual ~SelectionToolState() = default;

    // The bool return value indicates whether to lock the state or not
    virtual bool mousePressed(ApplicationContext *context) = 0;
    virtual void mouseMoved(ApplicationContext *context) = 0;
    virtual bool mouseReleased(ApplicationContext *context) = 0;

protected:
    bool m_isActive{false};
};
