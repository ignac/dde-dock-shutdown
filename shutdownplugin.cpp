/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *
 * Maintainer: sbw <sbw@sbw.so>
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

#include "shutdownplugin.h"
#include <QLabel>
#include <QApplication>
#include <QDesktopWidget>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QFormLayout>
#include <QFontMetrics>
#include <QSpinBox>

ShutdownPlugin::ShutdownPlugin(QObject *parent)
    : QObject(parent)
    , m_tipLabel(new QLabel)
    , m_settings("deepin", "dde-dock-shutdown2")
{
    m_tipLabel->setObjectName("shutdown_tip");
    m_tipLabel->setText("Power");
    m_tipLabel->setStyleSheet("color:white; padding:0px 3px;");

    m_centralWidget = new ShutdownWidget();

    connect(m_centralWidget, &ShutdownWidget::requestUpdateGeometry,
           [this] { m_proxyInter->itemUpdate(this, pluginName()); });
}

const QString ShutdownPlugin::pluginName() const
{
    return "shutdown2";
}

const QString ShutdownPlugin::pluginDisplayName() const
{
    return tr("Shutdown2");
}

void ShutdownPlugin::init(PluginProxyInterface *proxyInter)
{
    m_proxyInter = proxyInter;

    if (m_centralWidget->enabled())
        m_proxyInter->itemAdded(this, pluginName());
}

void ShutdownPlugin::pluginStateSwitched()
{
    m_centralWidget->setEnabled(!m_centralWidget->enabled());

    if (m_centralWidget->enabled())
        m_proxyInter->itemAdded(this, pluginName());
    else
        m_proxyInter->itemRemoved(this, pluginName());
}

bool ShutdownPlugin::pluginIsDisable()
{
    return !m_centralWidget->enabled();
}

int ShutdownPlugin::itemSortKey(const QString &itemKey)
{
    Q_UNUSED(itemKey);

    const QString key = QString("pos_%1").arg(displayMode());
    return m_settings.value(key, 0).toInt();
}

void ShutdownPlugin::setSortKey(const QString &itemKey, const int order)
{
    Q_UNUSED(itemKey);

    const QString key = QString("pos_%1").arg(displayMode());
    m_settings.setValue(key, order);
}

QWidget *ShutdownPlugin::itemWidget(const QString &itemKey)
{
   Q_UNUSED(itemKey);

    return m_centralWidget;
}

QWidget *ShutdownPlugin::itemTipsWidget(const QString &itemKey)
{
    Q_UNUSED(itemKey);

    return m_tipLabel;
}

const QString ShutdownPlugin::itemContextMenu(const QString &itemKey)
{
    Q_UNUSED(itemKey);

    QList<QVariant> items;
    items.reserve(6);

    QMap<QString, QVariant> shutdown;
    shutdown["itemId"] = "Shutdown";
    shutdown["itemText"] = tr("Shut down");
    shutdown["isActive"] = true;
    items.push_back(shutdown);

    QMap<QString, QVariant> reboot;
    reboot["itemId"] = "Restart";
    reboot["itemText"] = tr("Restart");
    reboot["isActive"] = true;
    items.push_back(reboot);

    QMap<QString, QVariant> suspend;
    suspend["itemId"] = "Suspend";
    suspend["itemText"] = tr("Suspend");
    suspend["isActive"] = true;
    items.push_back(suspend);

    QMap<QString, QVariant> lock;
    lock["itemId"] = "Lock";
    lock["itemText"] = tr("Lock");
    lock["isActive"] = true;
    items.push_back(lock);

    QMap<QString, QVariant> logout;
    logout["itemId"] = "Logout";
    logout["itemText"] = tr("Log out");
    logout["isActive"] = true;
    items.push_back(logout);

    QMap<QString, QVariant> power;
    power["itemId"] = "power";
    power["itemText"] = tr("Power settings");
    power["isActive"] = true;
    items.push_back(power);

    QMap<QString, QVariant> menu;
    menu["items"] = items;
    menu["checkableMenu"] = false;
    menu["singleCheck"] = false;

    return QJsonDocument::fromVariant(menu).toJson();
}

void ShutdownPlugin::invokedMenuItem(const QString &itemKey, const QString &menuId, const bool checked)
{
    Q_UNUSED(itemKey)
    Q_UNUSED(checked)

    if (menuId == "power")
        QProcess::startDetached("dbus-send --print-reply --dest=com.deepin.dde.ControlCenter /com/deepin/dde/ControlCenter com.deepin.dde.ControlCenter.ShowModule \"string:power\"");
    else if (menuId == "Lock")
        QProcess::startDetached("dbus-send", QStringList() << "--print-reply"
                                                           << "--dest=com.deepin.dde.lockFront"
                                                           << "/com/deepin/dde/lockFront"
                                                           << QString("com.deepin.dde.lockFront.Show"));
    else
        QProcess::startDetached("dbus-send", QStringList() << "--print-reply"
                                                           << "--dest=com.deepin.dde.shutdownFront"
                                                           << "/com/deepin/dde/shutdownFront"
                                                           << QString("com.deepin.dde.shutdownFront.%1").arg(menuId));
}
