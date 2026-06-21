#include "floor.h"

Floor::Floor()
    : m_number(0)
{
}


Floor::Floor(int number)
    : m_number(number)
{
}


int Floor::getNumber() const
{
    return m_number;
}


const QList<Room>& Floor::getRooms() const
{
    return m_rooms;
}
QList<Room>& Floor::getRooms(){
     return m_rooms;
}

int Floor::getRoomCount() const
{
    return m_rooms.size();
}


bool Floor::addRoom(const Room& room)
{
    if (containsRoom(room.getNumber()))
        return false;

    m_rooms.append(room);
    return true;
}


bool Floor::removeRoom(int roomNumber)
{
    for (int i = 0; i < m_rooms.size(); i++)
    {
        if (m_rooms[i].getNumber() == roomNumber)
        {
            m_rooms.removeAt(i);
            return true;
        }
    }

    return false;
}


bool Floor::containsRoom(int roomNumber) const
{
    for (const Room& room : m_rooms)
    {
        if (room.getNumber() == roomNumber)
        {
            return true;
        }
    }

    return false;
}


Room* Floor::findRoom(int roomNumber)
{
    for (Room& room : m_rooms)
    {
        if (room.getNumber() == roomNumber)
        {
            return &room;
        }
    }

    return nullptr;
}


QString Floor::toString() const
{
    return "Floor " + QString::number(m_number) +
           ", Rooms: " + QString::number(m_rooms.size());
}