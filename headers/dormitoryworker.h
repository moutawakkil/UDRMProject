#ifndef DORMITORYWORKER_H
#define DORMITORYWORKER_H

#include "Person.h"
#include <QString>
#include <QJsonObject>

enum class WorkerType
{
    Manager,
    Security,
    Maintenance,
    Cleaner,
    Cook
};


class DormitoryWorker : public Person
{
public:
    DormitoryWorker();

    DormitoryWorker(const QString& id,
                    const QString& firstName,
                    const QString& lastName,
                    WorkerType workerType);


    // Getter
    WorkerType getWorkerType() const;


    // Setter
    void setWorkerType(WorkerType workerType);


    // Overrides from Person
    QString role() const override;

    QString toString() const override;


    QJsonObject toJson() const;

    static DormitoryWorker fromJson(
        const QJsonObject& json);

private:
    WorkerType m_workerType;
};


#endif // DORMITORYWORKER_H