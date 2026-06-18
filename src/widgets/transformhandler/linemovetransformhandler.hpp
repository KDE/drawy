// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "movetransformhandler.hpp"

class LineMoveTransformHandler : public MoveTransformHandler
{
public:
    void renderHandles(ApplicationContext *context) override;
    [[nodiscard]] bool shouldActivate(ApplicationContext *context) override;
};
