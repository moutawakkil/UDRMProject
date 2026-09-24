#include "datamanage.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <utility>
#include <QDir>
#include <QDebug>

using namespace std ;
// Constructor
DataManager::DataManager()
    : m_configFile("data/dormitoryConfig.json"),
    m_studentsFile("data/students.json"),
    m_workersFile("data/dormitoryWorkers.json"),
    m_restaurantFile("data/restaurant.json")
{ QDir dir;

    if (!dir.exists("data"))
    {
        dir.mkdir("data");
    }
}


// Check whether the dormitory was already configured
bool DataManager::hasDormitoryConfig() const
{
    return QFile::exists(m_configFile);
}


// Read a JSON file and return its root object
QJsonObject DataManager::readJson(
    const QString& fileName) const
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        return QJsonObject();
    }

    QByteArray data = file.readAll();

    file.close();

    QJsonDocument document =
        QJsonDocument::fromJson(data);

    return document.object();
}


// Write a JSON object to a file
bool DataManager::writeJson(
    const QString& fileName,
    const QJsonObject& json) const
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Cannot open file:" << file.errorString();
        return false;
    }

    QJsonDocument document(json);

    file.write(document.toJson());

    file.close();

    return true;
}


// Load dormitory configuration
DormitoryConfig DataManager::loadConfig() const
{
    QJsonObject json =
        readJson(m_configFile);

    return DormitoryConfig::fromJson(json);
}


// Load students from students.json
QList<Student> DataManager::loadStudents() const
{
    QList<Student> students;
    QJsonObject json = readJson(m_studentsFile);

    QJsonArray studentsArray =
        json["students"].toArray();


    for (const QJsonValue& value : std::as_const(studentsArray))
    {
        students.append(
            Student::fromJson(value.toObject())
            );
    }

    return students;
}


// Load workers from dormitoryWorkers.json
QList<DormitoryWorker> DataManager::loadWorkers() const
{
    QList<DormitoryWorker> workers;
    QJsonObject json = readJson(m_workersFile);
    QJsonArray workersArray = json ["workers"].toArray();

    for (const QJsonValue& value :as_const(workersArray) )
    {
        workers.append(
            DormitoryWorker::fromJson(value.toObject())
            );
    }

    return workers;
}


// Load restaurant from restaurant.json
Restaurant DataManager::loadRestaurant() const
{
    QJsonObject json =
        readJson(m_restaurantFile);

    return Restaurant::fromJson(json);
}


// Build the complete Dormitory object
Dormitory DataManager::loadDormitory() const
{
    // Create the dormitory structure
    Dormitory dormitory(
        loadConfig()
        );


    // Inject students
    QList<Student> students =
        loadStudents();

    for (const Student& student : as_const(students))
    {
        dormitory.addStudent(student);
    }


    // Inject workers
    QList<DormitoryWorker> workers =
        loadWorkers();

    for (const DormitoryWorker& worker : as_const(workers))
    {
        dormitory.addWorker(worker);
    }


    // Inject restaurant
    dormitory.setRestaurant(
        loadRestaurant()
        );


    return dormitory;
}


/* Save students into students.json*/
bool DataManager::saveStudents(
    const QList<Student>& students) const
{
    QJsonArray studentsArray;

    for (const Student& student : students)
    {
        studentsArray.append(
            student.toJson()
            );
    }

    QJsonObject json;

    json["students"] =
        studentsArray;

    return writeJson(
        m_studentsFile,
        json
        );
}


// Save workers into dormitoryWorkers.json
bool DataManager::saveWorkers(
    const QList<DormitoryWorker>& workers) const
{
    QJsonArray workersArray;

    for (const DormitoryWorker& worker : workers)
    {
        workersArray.append(
            worker.toJson()
            );
    }

    QJsonObject json;

    json["workers"] =
        workersArray;

    return writeJson(
        m_workersFile,
        json
        );
}


// Save restaurant into restaurant.json
bool DataManager::saveRestaurant(
    const Restaurant& restaurant) const
{
    return writeJson(
        m_restaurantFile,
        restaurant.toJson()
        );
}


// Save all dynamic dormitory data
bool DataManager::saveDormitory(
    const Dormitory& dormitory) const
{
    bool success = true;


    success &= saveStudents(//means success = success && saveStudents
        dormitory.getStudents()
        );


    success &= saveWorkers(
        dormitory.getWorkers()
        );


    success &= saveRestaurant(
        dormitory.getRestaurant()
        );


    return success;
}
bool DataManager::saveConfig(
    const DormitoryConfig& config
    ) const
{
    return writeJson(
        m_configFile,
        config.toJson()
        );
}