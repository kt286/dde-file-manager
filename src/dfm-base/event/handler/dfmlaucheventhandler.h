/*
 * Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co., Ltd.
 *
 * Author:     liyigang<liyigang@uniontech.com>
 *
 * Maintainer: zhengyouge<zhengyouge@uniontech.com>
 *             yanghao<yanghao@uniontech.com>
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
#ifndef DFMLAUCHEVENTHANDLER_H
#define DFMLAUCHEVENTHANDLER_H

#include "event/protocol/dfmlauchevent.h"
#include "event/handler/dfmabstracteventhandler.h"

#include "base/dfmglobal.h"

#include <QWidget>

class DFMLauchEventHandler : public DFMAbstractEventHandler
{
public:
    DFMLauchEventHandler();

    virtual ~DFMLauchEventHandler() override;

    virtual bool canAsynProcess() override;

    virtual void event(const DFMEventPointer &event) override;

    virtual void openFile(const DFMLauchEventPointer &event);

    virtual void openFileByApp(const DFMLauchEventPointer &event);

    virtual void openUrlByTerminal(const DFMLauchEventPointer &event);

    virtual void compressFile(const DFMLauchEventPointer &event);

    virtual void decompressFile(const DFMLauchEventPointer &event);

    virtual void decompressFileHere(const DFMLauchEventPointer &event);

    virtual void writeUrlToClipboard(const DFMLauchEventPointer &event);
};

#endif // DFMLAUCHEVENTHANDLER_H
