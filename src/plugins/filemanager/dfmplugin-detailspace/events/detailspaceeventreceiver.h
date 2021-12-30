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
#ifndef DETAILSPACEEVENTRECEIVER_H
#define DETAILSPACEEVENTRECEIVER_H

#include "dfmplugin_detailspace_global.h"

#include "services/filemanager/titlebar/titlebar_defines.h"

#include <dfm-framework/event/eventhandler.h>
#include <dfm-framework/event/eventcallproxy.h>

DPDETAILSPACE_BEGIN_NAMESPACE

class DetailSpaceEventReceiver : public dpf::EventHandler, dpf::AutoEventHandlerRegister<DetailSpaceEventReceiver>
{
    Q_OBJECT
public:
    using HandlerMap = QMap<QString, std::function<void(const dpf::Event &)>>;

    static EventHandler::Type type()
    {
        return EventHandler::Type::Sync;
    }

    static QStringList topics()
    {
        // TODO(zhangs): add custom topic
        return QStringList() << DSB_FM_NAMESPACE::TitleBar::EventTopic::kTitleBar;
    }

    explicit DetailSpaceEventReceiver();
    void eventProcess(const dpf::Event &event) override;

private:   // event topics
    void handleTitleBarTopic(const dpf::Event &event);
    void callHandler(const dpf::Event &event, const HandlerMap &map);

private:   // event data (sub topics)
    void handleTileBarShowDetailView(const dpf::Event &event);

private:
    HandlerMap eventTopicHandlers;
};

DPDETAILSPACE_END_NAMESPACE

#endif   // DETAILSPACEEVENTRECEIVER_H
