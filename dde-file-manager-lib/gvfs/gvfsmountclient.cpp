/*
 * Copyright (C) 2016 ~ 2018 Deepin Technology Co., Ltd.
 *               2016 ~ 2018 dragondjf
 *
 * Author:     dragondjf<dingjiangfeng@deepin.com>
 *
 * Maintainer: dragondjf<dingjiangfeng@deepin.com>
 *             zccrs<zhangjide@deepin.com>
 *             Tangtong<tangtong@deepin.com>
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

#include "gvfsmountclient.h"

#include "mountaskpassworddialog.h"

#include "app/filesignalmanager.h"
#include "app/define.h"

#include "views/windowmanager.h"

#include "singleton.h"

#include <QDebug>
#include <QTimer>


bool GvfsMountClient::AskingPassword = false;
QJsonObject GvfsMountClient::SMBLoginObj = {};
DFMUrlBaseEvent GvfsMountClient::MountEvent = DFMUrlBaseEvent(Q_NULLPTR, DUrl());

MountAskPasswordDialog* GvfsMountClient::AskPasswordDialog = nullptr;
QMutex GvfsMountClient::mutex;

GvfsMountClient::GvfsMountClient(QObject *parent) : QObject(parent)
{
    qDebug() << "Create GvfsMountClient";
    initConnect();
}

GvfsMountClient::~GvfsMountClient()
{

}

void GvfsMountClient::initConnect()
{
//    connect(fileSignalManager, &FileSignalManager::requestSMBMount, this, &GvfsMountClient::mountByEvent);
}

void GvfsMountClient::mount(GFile *file)
{
    GMountOperation *op;

    if (file == NULL)
        return;

    op = new_mount_op();

    g_file_mount_enclosing_volume (file, static_cast<GMountMountFlags>(0), op, NULL, mount_done_cb, op);
}

void GvfsMountClient::mount_sync(const QString &path)
{
    GFile* file = g_file_new_for_uri(path.toUtf8().constData());

    if (file == NULL)
        return;

    GMountOperation *op = new_mount_op();
    DThreadUtil::runInMainThread(&g_file_mount_enclosing_volume,
                                 file, static_cast<GMountMountFlags>(0),
                                 op, nullptr, mount_done_cb, op);

    if (QThread::currentThread() != qApp->thread())
        mutex.lock();
}

GMountOperation *GvfsMountClient::new_mount_op()
{
    GMountOperation *op;

    op = g_mount_operation_new ();

    g_object_set_data (G_OBJECT (op), "state", GINT_TO_POINTER (MOUNT_OP_NONE));

    g_signal_connect (op, "ask_password", G_CALLBACK (ask_password_cb), NULL);
//  g_signal_connect (op, "ask_question", G_CALLBACK (ask_question_cb), NULL);

  /* TODO: we *should* also connect to the "aborted" signal but since the
   *       main thread is blocked handling input we won't get that signal
   *       anyway...
   */
    return op;
}

void GvfsMountClient::mount_done_cb(GObject *object, GAsyncResult *res, gpointer user_data)
{
    gboolean succeeded;
    GError *error = NULL;
    GMountOperation *op = static_cast<GMountOperation*>(user_data);

    succeeded = g_file_mount_enclosing_volume_finish (G_FILE (object), res, &error);

    if (!succeeded)
    {
        qDebug() << "g_file_mount_enclosing_volume_finish" << succeeded << error;
        qDebug() << "username" << g_mount_operation_get_username(op) << error->message;
    }else{
        qDebug() << "g_file_mount_enclosing_volume_finish" << succeeded << AskingPassword;
        if (AskingPassword){
            SMBLoginObj.insert("id", MountEvent.url().toString());
            if (SMBLoginObj.value("passwordSave").toInt() == 2){
                SMBLoginObj.remove("password");
                emit fileSignalManager->requsetCacheLoginData(SMBLoginObj);
            }
            SMBLoginObj = {};
            AskingPassword = false;
        }else{
            qDebug() << "username" << g_mount_operation_get_username(op);
        }
    }

    mutex.unlock();

    emit fileSignalManager->requestChooseSmbMountedFile(MountEvent);
}

void GvfsMountClient::ask_password_cb(GMountOperation *op, const char *message, const char *default_user, const char *default_domain, GAskPasswordFlags flags)
{
    if (AskPasswordDialog){
        return;
    }

    bool anonymous = g_mount_operation_get_anonymous(op);
    GPasswordSave passwordSave = g_mount_operation_get_password_save(op);

    const char* default_password = g_mount_operation_get_password(op);

    qDebug() << "anonymous" << anonymous;
    qDebug() << "message" << message;
    qDebug() << "username" << default_user;
    qDebug() << "domain" << default_domain;
    qDebug() << "password" << default_password;
    qDebug() << "GAskPasswordFlags" << flags;
    qDebug() << "passwordSave" << passwordSave;

    QJsonObject obj;
    obj.insert("message", message);
    obj.insert("anonymous", anonymous);
    obj.insert("username", default_user);
    obj.insert("domain", default_domain);
    obj.insert("password", default_password);
    obj.insert("GAskPasswordFlags", flags);
    obj.insert("passwordSave", passwordSave);


    AskPasswordDialog = new MountAskPasswordDialog(WindowManager::getWindowById(MountEvent.windowId()));
    AskPasswordDialog->setLoginData(obj);

    if (MountEvent.url().isSMBFile()){
        AskPasswordDialog->setDomainLineVisible(true);
    }else{
        AskPasswordDialog->setDomainLineVisible(false);
    }

    int code = AskPasswordDialog->exec();

    if (code){
        QJsonObject loginObj = AskPasswordDialog->getLoginData();
        anonymous = loginObj.value("anonymous").toBool();
        QString username = loginObj.value("username").toString();
        QString domain = loginObj.value("domain").toString();
        QString password = loginObj.value("password").toString();
        GPasswordSave passwordsaveFlag =  static_cast<GPasswordSave>(loginObj.value("passwordSave").toInt());

        SMBLoginObj = loginObj;

        if ((flags & G_ASK_PASSWORD_ANONYMOUS_SUPPORTED) && anonymous)
        {
            g_mount_operation_set_anonymous (op, TRUE);
        }
        else
        {
            if (flags & G_ASK_PASSWORD_NEED_USERNAME)
            {
                g_mount_operation_set_username (op, username.toStdString().c_str());
            }

            if (flags & G_ASK_PASSWORD_NEED_DOMAIN)
            {
                g_mount_operation_set_domain (op, domain.toStdString().c_str());
            }

            if (flags & G_ASK_PASSWORD_NEED_PASSWORD)
            {
                g_mount_operation_set_password (op, password.toStdString().c_str());
            }

            if (flags & G_ASK_PASSWORD_SAVING_SUPPORTED){
                g_mount_operation_set_password_save(op, passwordsaveFlag);
            }
        }

        /* Only try anonymous access once. */
        if (anonymous &&
            GPOINTER_TO_INT (g_object_get_data (G_OBJECT (op), "state")) == MOUNT_OP_ASKED)
        {
            g_object_set_data (G_OBJECT (op), "state", GINT_TO_POINTER (MOUNT_OP_ABORTED));
            g_mount_operation_reply (op, G_MOUNT_OPERATION_ABORTED);
        }
        else
        {
            qDebug() << "g_mount_operation_reply before";
//            g_object_set_data (G_OBJECT (op), "state", GINT_TO_POINTER (MOUNT_OP_ASKED));
            g_mount_operation_reply (op, G_MOUNT_OPERATION_HANDLED);
            qDebug() << "g_mount_operation_reply end";
        }
        AskingPassword = true;
        qDebug() << "AskingPassword" << AskingPassword;

    }else{
        qDebug() << "cancel connect";
        g_object_set_data (G_OBJECT (op), "state", GINT_TO_POINTER (MOUNT_OP_ABORTED));
        g_mount_operation_reply (op, G_MOUNT_OPERATION_ABORTED);
    }
    AskPasswordDialog->deleteLater();
    AskPasswordDialog = NULL;
}
