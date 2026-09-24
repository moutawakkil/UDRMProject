#include "rooms.h"

Room::Room()
    : m_number(0),
    m_capacity(0)
{
}


Room::Room(int number, int capacity)
    : m_number(number),
    m_capacity(capacity)
{
}


int Room::getNumber() const
{
    return m_number;
}


int Room::getCapacity() const
{
    return m_capacity;
}


int Room::getOccupancy() const
{
    return m_studentIds.size();
}


QList<QString> Room::getStudentIds() const
{
    return m_studentIds;
}


bool Room::isFull() const
{
    return getOccupancy() >= m_capacity;
}


bool Room::isEmpty() const
{
    return m_studentIds.isEmpty();
}


bool Room::containsStudent(const QString& studentId) const
{
    return m_studentIds.contains(studentId);
}


bool Room::addStudent(const QString& studentId)
{
    if (isFull() || containsStudent(studentId))
    {
        return false;
    }

    m_studentIds.append(studentId);

    return true;
}


bool Room::removeStudent(const QString& studentId)
{
    int index = m_studentIds.indexOf(studentId);

    if (index == -1)
    {
        return false;
    }

    m_studentIds.removeAt(index);

    return true;
}


QString Room::toString() const
{
    QString info;

    info += "Room: " + QString::number(m_number);
    info += ", Capacity: " + QString::number(m_capacity);
    info += ", Occupancy: " + QString::number(getOccupancy());

    return info;
}