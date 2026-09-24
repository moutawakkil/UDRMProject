#ifndef WORKERTYPEUTILS_H
#define WORKERTYPEUTILS_H
#include <QString>
#include "dormitoryworker.h"

inline QString workerTypeToString(WorkerType type)
{
    switch (type) {
    case WorkerType::Manager:     return QObject::tr("Manager");
    case WorkerType::Security:    return QObject::tr("Security");
    case WorkerType::Maintenance: return QObject::tr("Maintenance");
    case WorkerType::Cleaner:     return QObject::tr("Cleaner");
    case WorkerType::Cook:        return QObject::tr("Cook");
    }
    return QObject::tr("Unknown");
}

inline WorkerType workerTypeFromIndex(int index)
{
    // Must match the order items are added to any "worker type" combo box.
    switch (index) {
    case 0: return WorkerType::Manager;
    case 1: return WorkerType::Security;
    case 2: return WorkerType::Maintenance;
    case 3: return WorkerType::Cleaner;
    case 4: return WorkerType::Cook;
    default: return WorkerType::Manager;
    }
}
#endif // WORKERTYPEUTILS_H
