/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -i ../accesscontroldbus.h -c AccessControlManagerAdaptor -l AccessControlDBus -a dbusadaptor/accesscontrolmanager_adaptor accesscontroldbus.xml
 *
 * qdbusxml2cpp is Copyright (C) 2017 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#include "dbusadaptor/accesscontrolmanager_adaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class AccessControlManagerAdaptor
 */

AccessControlManagerAdaptor::AccessControlManagerAdaptor(AccessControlDBus *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

AccessControlManagerAdaptor::~AccessControlManagerAdaptor()
{
    // destructor
}

QString AccessControlManagerAdaptor::FileManagerReply(int policystate)
{
    // handle method call com.deepin.filemanager.daemon.AccessControlManager.FileManagerReply
    return parent()->FileManagerReply(policystate);
}

QVariantList AccessControlManagerAdaptor::QueryAccessPolicy()
{
    // handle method call com.deepin.filemanager.daemon.AccessControlManager.QueryAccessPolicy
    return parent()->QueryAccessPolicy();
}

QVariantList AccessControlManagerAdaptor::QueryVaultAccessPolicy()
{
    // handle method call com.deepin.filemanager.daemon.AccessControlManager.QueryVaultAccessPolicy
    return parent()->QueryVaultAccessPolicy();
}

int AccessControlManagerAdaptor::QueryVaultAccessPolicyVisible()
{
    // handle method call com.deepin.filemanager.daemon.AccessControlManager.QueryVaultAccessPolicyVisible
    return parent()->QueryVaultAccessPolicyVisible();
}

QString AccessControlManagerAdaptor::SetAccessPolicy(const QVariantMap &policy)
{
    // handle method call com.deepin.filemanager.daemon.AccessControlManager.SetAccessPolicy
    return parent()->SetAccessPolicy(policy);
}

QString AccessControlManagerAdaptor::SetVaultAccessPolicy(const QVariantMap &policy)
{
    // handle method call com.deepin.filemanager.daemon.AccessControlManager.SetVaultAccessPolicy
    return parent()->SetVaultAccessPolicy(policy);
}

