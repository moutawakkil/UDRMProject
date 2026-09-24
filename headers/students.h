#ifndef STUDENT_H
#define STUDENT_H

#include "person.h"
#include <QString>
#include <QJsonObject>
class Student : public Person
{
public:
    Student();

    Student(const QString& id,
            const QString& firstName,
            const QString& lastName,
            const QString& university,
            int academicYear);

    // Getters
    QString getUniversity() const;

    int getAcademicYear() const;

    QString getRoomId() const;


    // Setters
    void setUniversity(const QString& university);

    void setAcademicYear(int year);

    void setRoomId(const QString& roomId);


    // Overrides from Person
    QString role() const override;

    QString toString() const override;


    // Utilities
    bool isAssigned() const;
   //from & to Json
    QJsonObject toJson() const;

    static Student fromJson(const QJsonObject& json);
private:
    QString m_university;

    int m_academicYear;

    // Example: A-2-101
    // Empty string means not assigned
    QString m_roomId;
};

#endif // STUDENT_H