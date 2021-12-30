/*
 * Copyright (C) 2021 Uniontech Software Technology Co., Ltd.
 *
 * Author:     zhangsheng<zhangsheng@uniontech.com>
 *
 * Maintainer: max-lv<lvwujun@uniontech.com>
 *             lanxuesong<lanxuesong@uniontech.com>
 *             xushitong<xushitong@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "detailspace.h"
#include "utils/detailspacehelper.h"

#include "services/filemanager/windows/windowsservice.h"

#include <dfm-framework/framework.h>

DPDETAILSPACE_USE_NAMESPACE
DSB_FM_USE_NAMESPACE

void DetailSpace::initialize()
{
    // Note: Don't install detail view widget in here!
    // You shout install it when detail button clicked in titlebar
    auto &ctx = dpfInstance.serviceContext();
    Q_ASSERT_X(ctx.loaded(WindowsService::name()), "DetalSpace", "WindowService not loaded");
    auto windowService = ctx.service<WindowsService>(WindowsService::name());
    connect(windowService, &WindowsService::windowClosed, this, &DetailSpace::onWindowClosed, Qt::DirectConnection);
}

bool DetailSpace::start()
{
    return true;
}

dpf::Plugin::ShutdownFlag DetailSpace::stop()
{
    return kSync;
}

void DetailSpace::onWindowClosed(quint64 windId)
{
    DetailSpaceHelper::removeDetailSpace(windId);
}
