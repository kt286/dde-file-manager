/*
 * Copyright (C) 2022 Uniontech Software Technology Co., Ltd.
 *
 * Author:     lanxuesong<lanxuesong@uniontech.com>
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
#include "trashfilehelper.h"
#include "trashhelper.h"
#include "events/trasheventcaller.h"

#include "dfm-base/utils/fileutils.h"
#include "dfm-base/dfm_event_defines.h"
#include "dfm-base/utils/dialogmanager.h"
#include "dfm-base/file/local/localfilehandler.h"
#include "dfm-base/base/schemefactory.h"

#include <dfm-framework/event/event.h>
#include <dfm-io/dfmio_utils.h>

#include <QUrl>
#include <QFileInfo>

DPTRASH_USE_NAMESPACE
TrashFileHelper *TrashFileHelper::instance()
{
    static TrashFileHelper ins;
    return &ins;
}

TrashFileHelper::TrashFileHelper(QObject *parent)
    : QObject(parent)
{
}

bool TrashFileHelper::cutFile(const quint64 windowId, const QList<QUrl> sources, const QUrl target, const DFMBASE_NAMESPACE::AbstractJobHandler::JobFlags flags)
{
    if (target.scheme() != scheme())
        return false;

    if (sources.isEmpty())
        return true;

    const QUrl &urlSource = sources.first();
    if (Q_UNLIKELY(DFMBASE_NAMESPACE::FileUtils::isGvfsFile(urlSource) || DFMIO::DFMUtils::fileIsRemovable(urlSource))) {
        dpfSignalDispatcher->publish(DFMBASE_NAMESPACE::GlobalEventType::kDeleteFiles,
                                     windowId,
                                     sources, flags, nullptr);
    } else {
        dpfSignalDispatcher->publish(DFMBASE_NAMESPACE::GlobalEventType::kMoveToTrash,
                                     windowId,
                                     sources, flags, nullptr);
    }
    return true;
}

bool TrashFileHelper::copyFile(const quint64 windowId, const QList<QUrl> sources, const QUrl target, const DFMBASE_NAMESPACE::AbstractJobHandler::JobFlags flags)
{
    if (target.scheme() != scheme())
        return false;

    dpfSignalDispatcher->publish(DFMBASE_NAMESPACE::GlobalEventType::kMoveToTrash,
                                 windowId,
                                 sources, flags, nullptr);
    return true;
}

bool TrashFileHelper::copyFromFile(const quint64 windowId, const QList<QUrl> sources, const QUrl target, const dfmbase::AbstractJobHandler::JobFlags flags)
{
    if (sources.isEmpty())
        return false;

    const QUrl &fromUrl = sources.first();
    if (fromUrl.scheme() != scheme())
        return false;

    dpfSignalDispatcher->publish(DFMBASE_NAMESPACE::GlobalEventType::kCopyFromTrash, windowId,
                                 sources, target, flags, nullptr);
    return true;
}

bool TrashFileHelper::moveToTrash(const quint64 windowId, const QList<QUrl> sources, const DFMBASE_NAMESPACE::AbstractJobHandler::JobFlags flags)
{
    Q_UNUSED(flags)

    if (sources.isEmpty())
        return false;
    if (sources.first().scheme() != scheme())
        return false;

    dpfSignalDispatcher->publish(DFMBASE_NAMESPACE::GlobalEventType::kCleanTrash,
                                 windowId,
                                 sources,
                                 DFMBASE_NAMESPACE::AbstractJobHandler::DeleteDialogNoticeType::kDeleteTashFiles, nullptr);
    return true;
}

bool TrashFileHelper::deleteFile(const quint64 windowId, const QList<QUrl> sources, const DFMBASE_NAMESPACE::AbstractJobHandler::JobFlags flags)
{
    Q_UNUSED(flags)

    if (sources.isEmpty())
        return false;
    if (sources.first().scheme() != scheme())
        return false;

    dpfSignalDispatcher->publish(DFMBASE_NAMESPACE::GlobalEventType::kCleanTrash,
                                 windowId,
                                 sources,
                                 DFMBASE_NAMESPACE::AbstractJobHandler::DeleteDialogNoticeType::kDeleteTashFiles, nullptr);
    return true;
}

bool TrashFileHelper::openFileInPlugin(quint64 windowId, const QList<QUrl> urls)
{
    if (urls.isEmpty())
        return false;
    if (urls.first().scheme() != scheme())
        return false;

    bool isOpenFile = false;
    for (const QUrl &url : urls) {
        auto fileinfo = DFMBASE_NAMESPACE::InfoFactory::create<DFMBASE_NAMESPACE::AbstractFileInfo>(url);
        if (fileinfo && fileinfo->isFile()) {
            isOpenFile = true;
            break;
        }
    }
    if (isOpenFile) {
        const QString &strMsg = QObject::tr("Unable to open items in the trash, please restore it first");
        DialogManagerInstance->showMessageDialog(DFMBASE_NAMESPACE::DialogManager::kMsgWarn, strMsg);
    }
    return isOpenFile;
}

bool TrashFileHelper::blockPaste(quint64 winId, const QList<QUrl> &fromUrls, const QUrl &to)
{
    Q_UNUSED(winId)
    if (fromUrls.isEmpty())
        return false;

    if (fromUrls.first().scheme() == scheme() && to.scheme() == scheme()) {
        qDebug() << "The trash directory does not support paste!";
        return true;
    }
    return false;
}
