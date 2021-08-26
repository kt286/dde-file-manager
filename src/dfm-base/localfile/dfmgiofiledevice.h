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
#ifndef DGIOFILEDEVICE_H
#define DGIOFILEDEVICE_H

#include "base/dabstractfiledevice.h"

class DFMGIOFileDevicePrivate;
class DFMGIOFileDevice : public DAbstractFileDevice
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(DFMGIOFileDevice)

public:
    explicit DFMGIOFileDevice(const QUrl &url);

    ~DFMGIOFileDevice() override;

    bool setFileUrl(const QUrl &url) override;

    bool open(OpenMode mode) override;
    void close() override;
    int handle() const override;
    qint64 size() const override;
    bool resize(qint64 size) override;
    bool isSequential() const override;
    qint64 pos() const override;
    bool seek(qint64 pos) override;
    bool flush() override;
    bool syncToDisk(const DAbstractFileDevice::SyncOperate &op = DAbstractFileDevice::DefaultOperate) override;
    void closeWriteReadFailed(const bool bwrite) override;
    void cancelAllOperate() override;

protected:
    explicit DFMGIOFileDevice();
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;
};

#endif // DGIOFILEDEVICE_H
