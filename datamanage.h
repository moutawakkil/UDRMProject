#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QString>
#include <QJsonObject>
#include <QList>

#include "dormitery.h"
#include "dormitoryConfig.h"
#include "students.h"
#include "dormitoryworker.h"
#include "restaurent.h"


class DataManager
{
public:
    DataManager();

    // Check first application start
    bool hasDormitoryConfig() const;
    DormitoryConfig loadConfig() const;

    bool saveConfig(
        const DormitoryConfig& config
        ) const;


    // Create the complete Dormitory object
    Dormitory loadDormitory() const;


    // Save the complete Dormitory object
    bool saveDormitory(const Dormitory& dormitory) const;


private:
    // File names
    QString m_configFile;
    QString m_studentsFile;
    QString m_workersFile;
    QString m_restaurantFile;


    // Generic JSON operations
    QJsonObject readJson(
        const QString& fileName) const;

    bool writeJson(
        const QString& fileName,
        const QJsonObject& json) const;


    // Specific loading functions

    QList<Student> loadStudents() const;

    QList<DormitoryWorker> loadWorkers() const;

    Restaurant loadRestaurant() const;
    // Specific saving functions
    bool saveStudents(
        const QList<Student>& students) const;

    bool saveWorkers(
        const QList<DormitoryWorker>& workers) const;

    bool saveRestaurant(
        const Restaurant& restaurant) const;
};

#endif // DATAMANAGER_H