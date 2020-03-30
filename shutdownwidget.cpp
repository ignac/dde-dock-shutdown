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

#include "dde-dock/constants.h"
#include "shutdownwidget.h"
#include <QApplication>
#include <QPainter>
#include <QDebug>
#include <QSvgRenderer>
#include <QMouseEvent>
#include <QIcon>

#define PLUGIN_STATE_KEY    "enable"

ShutdownWidget::ShutdownWidget(QWidget *parent)
    : QWidget(parent)
    ,  m_settings("deepin", "dde-dock-shutdown2")
{
}

bool ShutdownWidget::enabled()
{
    return m_settings.value(PLUGIN_STATE_KEY, true).toBool();
}

void ShutdownWidget::setEnabled(const bool b)
{
    m_settings.setValue(PLUGIN_STATE_KEY, b);
}

QSize ShutdownWidget::sizeHint() const
{
    QSize size(28, 28);
    return size;
}

void ShutdownWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
}

void ShutdownWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPainter painter(this);

    QRect iconRect(rect());
    QIcon icon("://resources/icons/normal.svg");

    icon.paint(&painter, iconRect, Qt::AlignCenter);
}
