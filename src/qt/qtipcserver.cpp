// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "qtipcserver.h"
#include "guiconstants.h"
#include "ui_interface.h"
#include "util.h"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <QByteArray>
#include <QCoreApplication>
#include <QDataStream>
#include <QHash>
#include <QLocalSocket>
#include <QStringList>

using namespace boost;
using namespace boost::posix_time;

//
// Create a name that is unique for:
//  testnet / non-testnet
//  data directory
//
QString ipcServerName()
{
    QString name("BitcoinQt");

    // Append a simple hash of the datadir
    // Note that GetDataDir(true) returns a different path
    // for -testnet versus main net
    QString ddir(GetDataDir(true).string().c_str());
    uint h = qHash(ddir);
    QString hn; hn.setNum(h);
    name.append(hn);

    return name;
}

//
// Sending to the server is done synchronously, at startup:
//
bool ipcSendCommands()
{
    bool fResult = false;

    const QStringList& args = QCoreApplication::arguments();
    for (int i = 1; i < args.size(); i++)
    {
        if (!args[i].startsWith(BITCOIN_IPC_PREFIX, Qt::CaseInsensitive))
            continue;

        QLocalSocket* socket = new QLocalSocket();
        socket->connectToServer(ipcServerName(), QIODevice::WriteOnly);
        if (!socket->waitForConnected(BITCOIN_IPC_CONNECT_TIMEOUT))
            return false;

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);
        out << args[i];
        out.device()->seek(0);
        socket->write(block);
        socket->flush();

        socket->waitForBytesWritten(BITCOIN_IPC_CONNECT_TIMEOUT);
        socket->disconnectFromServer();
        delete socket;
        fResult = true;
    }
    return fResult;
}
