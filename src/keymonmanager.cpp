/***************************************************************************
 *   Copyright (C) 2010 by Kai Dombrowe <just89@gmx.de>                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/


// own
#include "keymonmanager.h"
#include "keymon/manager.h"

// KDE
#include <kglobal.h>
#include <kdebug.h>
#include <kauth.h>
#include <klocalizedstring.h>


class KeyMonManagerSingleton
{
    public:
        KeyMonManager self;
};


K_GLOBAL_STATIC(KeyMonManagerSingleton, privateSelf)


KeyMonManager::KeyMonManager(QObject *parent)
    : QObject(parent), m_started(false)
{



}


KeyMonManager::~KeyMonManager()
{

//    stop(); crash

}


KeyMonManager *KeyMonManager::self()
{

    return &privateSelf->self;

}


QList<KeyMon::DeviceInfo> KeyMonManager::getInputDeviceList()
{

    return KeyMon::Manager::getInputDeviceList();

}


QString KeyMonManager::fileForDevice(const KeyMon::DeviceInfo &info)
{

    return KeyMon::Manager::fileForDevice(info);

}


QString KeyMonManager::error() const
{

    return m_error;

}


bool KeyMonManager::start(const QStringList &devs)
{

    if (m_started) {
        kDebug() << "already running...";
        return true;
    }
    kDebug() << "start..." << devs;

    KAuth::Action action("org.kde.recorditnow.helper.watch");
    ExecuteJob * watchJob=action.execute();
    connect(watchJob, SIGNAL(progressStep(QVariantMap)), this,
            SLOT(progressStep(QVariantMap)));
    connect(watchJob, SIGNAL(actionPerformed(ActionReply)), this,
            SLOT(actionPerformed(ActionReply)));

    QVariantMap args;
    args["Devs"] = devs;

    action.setArguments(args);
    //action.setExecutesAsync(true);

    kDebug() << "valid action:" << action.isValid();

    //KAuth::ActionReply reply = action.execute("org.kde.recorditnow.helper");
    action.setHelperId("org.kde.recorditnow.helper");
    if (!watchJob->exec()) {
            watchJob->disconnect(this);
            m_error = watchJob->errorString();
            return false;
    }
    m_started = true;

    kDebug() << "started: "  << "Failed?" << watchJob->error() << "error Description:" << watchJob->errorString();

    return true;

}


void KeyMonManager::stop()
{

    if (!m_started) {
        return;
    }
    kDebug() << "stopping...";

    KAuth::Action action("org.kde.recorditnow.helper.watch");
    action.setHelperId("org.kde.recorditnow.helper");
    ExecuteJob * watchJob=action.execute();
    watchJob->disconnect(this);
    watchJob->kill();

    m_started = false;

}


void KeyMonManager::progressStep(const QVariantMap &data)
{

    if (data.contains("Started")) {
        emit started();
        return;
    }

    KeyMon::Event event;
    event.key = static_cast<KeyMon::Event::Key>(data["Key"].toInt());
    event.pressed = data["Pressed"].toBool();
    event.keyCode = data.value("KeyCode").toInt();
    event.mouseEvent = data.value("MouseEvent").toBool();

    emit keyEvent(event);

}


void KeyMonManager::actionPerformed(const ActionReply &reply)
{

    if (reply.failed()) {
        m_error = parseError(reply.errorCode());
    }

    KAuth::Action action("org.kde.recorditnow.helper.watch");
    action.setHelperId("org.kde.recorditnow.helper");
    ExecuteJob * watchJob=action.execute();
    watchJob->disconnect(this);
    kDebug() << "action performed:" << reply.type() << reply.errorCode() << watchJob->errorString();

    m_started = false;
    emit stopped();

}


void KeyMonManager::actionStarted()
{

    emit started();

}

#include "keymonmanager.moc"
