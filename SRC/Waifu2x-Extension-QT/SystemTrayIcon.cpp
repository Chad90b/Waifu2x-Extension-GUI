﻿/*
    Copyright (C) 2020  Aaron Feng

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    My Github homepage: https://github.com/AaronFeng753
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
托盘图标的实现参考了来自简书的文章:
<<Qt5 程序系统托盘>>--小Q_wang
https://www.jianshu.com/p/a000044f1f4a
*/

/*
初始化托盘图标
*/
void MainWindow::Init_SystemTrayIcon()
{
    //初始化图标
    systemTray->setIcon(*MainIcon_QIcon);
    systemTray->setToolTip(tr("Waifu2x-Extension-GUI\nRight-click to show the menu."));
    //初始化点击动作
    connect(systemTray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));
    //初始化菜单动作
    minimumAct_SystemTrayIcon->setText(tr("Hide"));
    minimumAct_SystemTrayIcon->setToolTip(tr("Use the middle mouse button to click the\ntaskbar icon to quickly hide the window."));
    //===
    restoreAct_SystemTrayIcon->setText(tr("Show"));
    restoreAct_SystemTrayIcon->setToolTip(tr("Use the left mouse button to click the taskbar icon to quickly\ndisplay the window, double-click to maximize the window."));
    //===
    quitAct_SystemTrayIcon->setText(tr("Exit"));
    //===
    BackgroundModeAct_SystemTrayIcon->setText(tr("Background mode"));
    BackgroundModeAct_SystemTrayIcon->setToolTip(tr("Set the number of threads to \"1\" to reduce background resource usage."));
    //===
    connect(minimumAct_SystemTrayIcon, SIGNAL(triggered()), this, SLOT(SystemTray_hide_self()));
    connect(restoreAct_SystemTrayIcon, SIGNAL(triggered()), this, SLOT(SystemTray_showNormal_self()));
    connect(quitAct_SystemTrayIcon, SIGNAL(triggered()), this, SLOT(close()),Qt::UniqueConnection);
    connect(BackgroundModeAct_SystemTrayIcon, SIGNAL(triggered()), this, SLOT(EnableBackgroundMode_SystemTray()));
    //初始化菜单选项
    pContextMenu->setToolTipsVisible(1);
    pContextMenu->setToolTipDuration(-1);
    pContextMenu->addAction(minimumAct_SystemTrayIcon);
    pContextMenu->addAction(restoreAct_SystemTrayIcon);
    pContextMenu->addSeparator();
    pContextMenu->addAction(BackgroundModeAct_SystemTrayIcon);
    pContextMenu->addSeparator();
    pContextMenu->addAction(quitAct_SystemTrayIcon);
    systemTray->setContextMenu(pContextMenu);
    //显示托盘图标
    systemTray->show();
}
/*
托盘图标点击动作
*/
void MainWindow::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
        case QSystemTrayIcon::Trigger:
            //单击托盘图标,正常大小显示(如果之前是全屏,那么会变回正常窗口)
            this->showNormal();
            this->activateWindow();
            this->setWindowState((this->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
            break;
        case QSystemTrayIcon::DoubleClick:
            //双击托盘图标,全屏显示
            this->showMaximized();
            this->activateWindow();
            this->setWindowState((this->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
            break;
        case QSystemTrayIcon::MiddleClick:
            //鼠标中键点击托盘图标,隱藏窗口
            this->hide();
            break;
        default:
            break;
    }
}
/*
启用后台模式
(把所有线程数量改到 1 来减少后台资源占用)
*/
void MainWindow::EnableBackgroundMode_SystemTray()
{
    ui->spinBox_ThreadNum_image->setValue(1);
    ui->spinBox_ThreadNum_gif_internal->setValue(1);
    ui->spinBox_ThreadNum_video_internal->setValue(1);
    pContextMenu->hide();
}
/*
托盘消息
*/
void MainWindow::SystemTray_NewMessage(QString message)
{
    systemTray->showMessage(tr("Notification"),message,*MainIcon_QIcon,5000);
}

void MainWindow::SystemTray_hide_self()
{
    this->hide();
    pContextMenu->hide();
}

void MainWindow::SystemTray_showNormal_self()
{
    this->showNormal();
    pContextMenu->hide();
}


