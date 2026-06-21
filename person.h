#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QJsonObject>
class Person
{
public:
    Person();

    Person(const QString& id,
           const QString& firstName,
           const QString& lastName);

    virtual ~Person();

    // Getters
    QString getId() const;
    QString getFirstName() const;
    QString getLastName() const;
    QString getFullName() const;

    // Setters
    void setFirstName(const QString& firstName);
    void setLastName(const QString& lastName);

    // Polymorphism
    virtual QString role() const = 0; // Pure virtual function

    virtual QString toString() const;
     QJsonObject toJson() const;
protected:
    QString m_id;
    QString m_firstName;
    QString m_lastName;
};

#endif // PERSON_H