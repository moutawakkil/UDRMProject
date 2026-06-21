#ifndef ROOM_H
#define ROOM_H

#include <QString>
#include <QList>

class Room
{
public:
    Room();
    Room(int number, int capacity);

    // Getters
    int getNumber() const;

    int getCapacity() const;

    // How many students are in the room
    int getOccupancy() const;

    QList<QString> getStudentIds() const;
    // State
    bool isFull() const;

    bool isEmpty() const;

    // Student management
    bool addStudent(const QString& studentId);

    bool removeStudent(const QString& studentId);

    // Check if a student is already in this room
    bool containsStudent(const QString& studentId) const;


    // Utility
    QString toString() const;

private:
    int m_number; // Example: 101

    int m_capacity;

    // Stores IDs of students assigned to this room
    QList<QString> m_studentIds;
};

#endif // ROOM_H