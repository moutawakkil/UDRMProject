#include "person.h"
#include <QJsonObject>

Person::Person()
{
}


Person::Person(const QString& id,
               const QString& firstName,
               const QString& lastName)
    : m_id(id),
    m_firstName(firstName),
    m_lastName(lastName)
{
}


Person::~Person()
{
}


QString Person::getId() const
{
    return m_id;
}


QString Person::getFirstName() const
{
    return m_firstName;
}


QString Person::getLastName() const
{
    return m_lastName;
}


QString Person::getFullName() const
{
    return m_firstName + " " + m_lastName;
}


void Person::setFirstName(const QString& firstName)
{
    m_firstName = firstName;
}


void Person::setLastName(const QString& lastName)
{
    m_lastName = lastName;
}


QString Person::toString() const
{
    return "ID: " + m_id +
           "\nName: " + getFullName() +
           "\nRole: " + role();
}
QJsonObject Person::toJson() const
{
    QJsonObject json;

    json["id"] = m_id;
    json["firstName"] = m_firstName;
    json["lastName"] = m_lastName;

    return json;
}