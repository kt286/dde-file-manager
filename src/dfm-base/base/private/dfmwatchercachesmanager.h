/*
 * Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co., Ltd.
 *
 * Author:     yanghao<yanghao@uniontech.com>
 *
 * Maintainer: zhengyouge<zhengyouge@uniontech.com>
 *             yanghao<yanghao@uniontech.com>
 *             hujianzhong<hujianzhong@uniontech.com>
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

#ifndef DWATCHERCACHESMANAGER_H
#define DWATCHERCACHESMANAGER_H

#include <QObject>
#include <QUrl>

#include "dfm-base/base/dabstractfilewatcher.h"
class DFMWacherFactory;
class DFMInfoCachesManager;
////这个类是对Url的强对应，必须保证url的唯一性
class DFMWatcherCachesManagerPrivate;
class DFMWatcherCachesManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), DFMWatcherCachesManager)
    friend DFMWacherFactory;
    friend DFMInfoCachesManager;
public:
    explicit DFMWatcherCachesManager(QObject *parent = nullptr);
    virtual ~DFMWatcherCachesManager();
private:
    //获取全局实例
    /* @method instance
     * @brief 获取全局的文件监视器的缓存管理模块实例
     *  只有提供给自己使用DFMWacherFactory，不对外做操作防止出现多个watcher
     * @return DWatcherCachesManager 返回全局的文件监视器的缓存管理模块实例
     */
    static DFMWatcherCachesManager& instance();
    /* @method getCacheWatcher
     * @brief 根据url获取缓存中对应的监视器
     *  只有提供给自己使用DFMWacherFactory，不对外做操作防止出现多个watcher
     * @param const QUrl &url 需要构造的Url
     * @return QSharedPointer<T> 返回相应的缓存的watcher，如果没有
     * 就返回空指针
     */
    QSharedPointer<DAbstractFileWatcher> getCacheWatcher(const QUrl &url);
    /* @method cacheWatcher
     * @brief 缓存url对应的watcher
     *  只有提供给自己使用DFMWacherFactory，不对外做操作防止出现多个watcher
     *  watcher必须有值，如果为nullptr那就不会缓存
     * @param const QUrl &url 需要构造的Url
     * @param QSharedPointer<DAbstractFileWatcher> &watcher 缓存的watcher
     * @return void
     */
    void cacheWatcher(const QUrl &url, const QSharedPointer<DAbstractFileWatcher> &watcher);
    /* @method removCacheWatcher
     * @brief 根据Url移除当前缓存的watcher
     *  只有提供给自己使用DFMWacherFactory，不对外做操作防止出现多个watcher
     *  首先需要注册scheme到DFMUrlRoute类
     *  其次需要注册scheme到DFMSchemeFactory<T>类
     * @param const QUrl &url 需要移除的url
     * @return void
     */
    void removCacheWatcher(const QUrl &url);

protected:
    QScopedPointer<DFMWatcherCachesManagerPrivate> d_ptr;
};

#endif // DWATCHERCACHESMANAGER_H
