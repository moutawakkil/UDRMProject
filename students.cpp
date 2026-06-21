#include "students.h"
#include <QJsonObject>

// Constructors

Student::Student()
    : Person(),
    m_academicYear(0),
    m_roomId("")
{
}


Student::Student(const QString& id,
                 const QString& firstName,
                 const QString& lastName,
                 const QString& university,
                 int academicYear)
    : Person(id, firstName, lastName),
    m_university(university),
    m_academicYear(academicYear),
    m_roomId("")
{
}


// Getters

QString Student::getUniversity() const
{
    return m_university;
}


int Student::getAcademicYear() const
{
    return m_academicYear;
}


QString Student::getRoomId() const
{
    return m_roomId;
}


// Setters

void Student::setUniversity(const QString& university)
{
    m_university = university;
}


void Student::setAcademicYear(int year)
{
    m_academicYear = year;
}


void Student::setRoomId(const QString& roomId)
{
    m_roomId = roomId;
}


// Overrides from Person

QString Student::role() const
{
    return "Student";
}


QString Student::toString() const
{
    QString info =
        "ID: " + getId() +
        ", Name: " + getFullName() +
        ", Role: " + role() +
        ", University: " + m_university +
        ", Academic Year: " + QString::number(m_academicYear);

    if (isAssigned())
    {
        info += ", Room: " + m_roomId;
    }
    else
    {
        info += ", Not Assigned";
    }

    return info;
}


// Utility Methods

bool Student::isAssigned() const
{
    return !m_roomId.isEmpty();
}
Student Student::fromJson(const QJsonObject& json)
{
    Student student(
        json["id"].toString(),
        json["firstName"].toString(),
        json["lastName"].toString(),
        json["university"].toString(),
        json["academicYear"].toInt()
        );

    student.setRoomId(json["roomId"].toString());

    return student;
}
QJsonObject Student::toJson() const
{
    QJsonObject json = Person::toJson();

    json["university"] = m_university;
    json["academicYear"] = m_academicYear;
    json["roomId"] = m_roomId;

    return json;
}