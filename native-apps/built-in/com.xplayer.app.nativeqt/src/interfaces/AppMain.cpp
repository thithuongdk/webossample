/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the documentation of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "AppMain.h"
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>

AppMain* AppMain::m_instance = nullptr;

AppMain::AppMain(QObject *parent)
    : QObject(parent)
    , m_service(nullptr)
    , m_windown(nullptr)
{
}

AppMain::~AppMain()
{
    PmLogInfo(Log::getPmLogContext(), "APPMAIN", 1, PMLOGKS("x", "delete AppMain"),  " ");
    if (m_service != nullptr) {
        delete m_service;
    }
    if (m_windown != nullptr) {
        delete m_windown;
    }
}

AppMain *AppMain::instance(QObject* parent)
{
    if(m_instance == nullptr) {
        m_instance = new AppMain(parent);
    }
    return m_instance;
}

void AppMain::initApplication()
{
    m_service = AppService::instance(this);

    connectSignalSlots();
    m_service->registerApp();
}

void AppMain::onCreateWindow()
{
    PmLogInfo(Log::getPmLogContext(), "APPMAIN", 1, PMLOGKS("funcall", "onCreateWindow"),  " ");
    if (m_windown == nullptr) {
        m_windown = new QQuickView(QUrl(QStringLiteral("/Windown.qml")));
    }

    m_windown->setProperty("appId", "com.xplayer.app.nativeqt");    
    m_windown->setProperty("displayAffinity", 0);  
    m_windown->setResizeMode(QQuickView::SizeRootObjectToView);
    m_windown->setColor(Qt::white);
    // m_windown->setColor(Qt::gray);
    m_windown->setPersistentGraphics(false);
    m_windown->setGeometry(0,0, 1080,720);
    m_windown->setSource(QUrl(QStringLiteral("qrc:/src/resources/qmls/Windown.qml")));
    m_windown->show();
    PmLogInfo(Log::getPmLogContext(), "APPMAIN", 1, PMLOGKS("funcall", "done onCreateWindow"),  " ");
}

void AppMain::connectSignalSlots() {
    connect(m_service, &AppService::createWindow,                 this, &AppMain::onCreateWindow);
}