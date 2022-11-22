/*
 * Copyright (C) 2022 Uniontech Software Technology Co., Ltd.
 *
 * Author:     liuzhangjian<liuzhangjian@uniontech.com>
 *
 * Maintainer: liuzhangjian<liuzhangjian@uniontech.com>
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
#ifndef SEARCHHELPER_H
#define SEARCHHELPER_H

#include "dfmplugin_search_global.h"

#include "dfm-base/dfm_global_defines.h"

#include <QUrl>
#include <QWidget>

namespace dfmplugin_search {

class SearchHelper final : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(SearchHelper)

public:
    static SearchHelper *instance();

    inline static QString scheme()
    {
        return "search";
    }

    static QUrl rootUrl();
    static bool isRootUrl(const QUrl &url);
    static bool isSearchFile(const QUrl &url);
    static QUrl searchTargetUrl(const QUrl &searchUrl);
    static QString searchKeyword(const QUrl &searchUrl);
    static QString searchWinId(const QUrl &searchUrl);

    static QUrl setSearchKeyword(const QUrl &searchUrl, const QString &keyword);
    static QUrl setSearchTargetUrl(const QUrl &searchUrl, const QUrl &targetUrl);
    static QUrl setSearchWinId(const QUrl &searchUrl, const QString &winId);

    static QUrl fromSearchFile(const QString &filePath);
    static QUrl fromSearchFile(const QUrl &targetUrl, const QString &keyword, const QString &winId);
    static bool showTopWidget(QWidget *w, const QUrl &url);

    bool customColumnRole(const QUrl &rootUrl, QList<DFMGLOBAL_NAMESPACE::ItemRoles> *roleList);
    bool customRoleDisplayName(const QUrl &rootUrl, const DFMGLOBAL_NAMESPACE::ItemRoles role, QString *displayName);

    // disable paste
    bool blockPaste(quint64 winId, const QList<QUrl> &fromUrls, const QUrl &to);

    bool searchIconName(const QUrl &url, QString *iconName);

    QString checkWildcardAndToRegularExpression(const QString &pattern);
    QString wildcardToRegularExpression(const QString &pattern);
    inline QString anchoredPattern(const QString &expression)
    {
        return QLatin1String("\\A(?:")
                + expression
                + QLatin1String(")\\z");
    }
    bool isHiddenFile(const QString &fileName, QHash<QString, QSet<QString>> &filters, const QString &searchPath);

private:
    explicit SearchHelper(QObject *parent = nullptr);
    ~SearchHelper() override;
};

}

#endif   // SEARCHHELPER_H
