#include "dormitoryworker.h"

#include <QJsonObject>
DormitoryWorker::DormitoryWorker()
    : Person(),
    m_workerType(WorkerType::Cleaner)
{
}


DormitoryWorker::DormitoryWorker(const QString& id,
                                 const QString& firstName,
                                 const QString& lastName,
                                 WorkerType workerType)
    : Person(id, firstName, lastName),
    m_workerType(workerType)
{
}


// Getter

WorkerType DormitoryWorker::getWorkerType() const
{
    return m_workerType;
}


// Setter

void DormitoryWorker::setWorkerType(WorkerType workerType)
{
    m_workerType = workerType;
}


// Overrides from Person

QString DormitoryWorker::role() const
{
    return "Dormitory Worker";
}


QString DormitoryWorker::toString() const
{
    QString type;

    switch (m_workerType)
    {
    case WorkerType::Manager:
        type = "Manager";
        break;

    case WorkerType::Security:
        type = "Security";
        break;

    case WorkerType::Maintenance:
        type = "Maintenance";
        break;

    case WorkerType::Cleaner:
        type = "Cleaner";
        break;

    case WorkerType::Cook:
        type = "Cook";
        break;
    }

    return "ID: " + getId() +
           ", Name: " + getFullName() +
           ", Role: " + role() +
           ", Type: " + type;
}
QJsonObject DormitoryWorker::toJson() const
{
    QJsonObject json = Person::toJson();

    json["type"] = static_cast<int>(m_workerType);

    return json;
}
DormitoryWorker DormitoryWorker::fromJson(
    const QJsonObject& json)
{
    return DormitoryWorker(
        json["id"].toString(),
        json["firstName"].toString(),
        json["lastName"].toString(),
        static_cast<WorkerType>(
            json["type"].toInt()
            )
        );
}