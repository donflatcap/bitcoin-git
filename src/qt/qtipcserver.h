#ifndef QTIPCSERVER_H
#define QTIPCSERVER_H

#include <QString>

const int BITCOIN_IPC_CONNECT_TIMEOUT = 1000; // milliseconds
const QString BITCOIN_IPC_PREFIX("bitcoin:");

// Unique-to-data-directory server name:
QString ipcServerName();

// Returns true if bitcoin: URI successfully sent
// to running Bitcoin-Qt.
bool ipcSendCommands();


#endif // QTIPCSERVER_H
