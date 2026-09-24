#ifndef FLOOR_H
#define FLOOR_H

#include <QList>
#include "rooms.h"

class Floor
{
public:
    Floor();
    Floor(int number);

    // Getters
    int getNumber() const;
    QList<Room>& getRooms();
   const QList<Room>& getRooms() const;
    int getRoomCount() const;

    // Room management
    bool addRoom(const Room& room);
    bool removeRoom(int roomNumber);
    bool containsRoom(int roomNumber) const;

    // Search
    Room* findRoom( int roomNumber);

    // Utility
    QString toString() const;


private:
    int m_number;

    // Composition: a floor has rooms
    QList<Room> m_rooms;
};

#endif // FLOOR_H