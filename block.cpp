#include "block.h"

Block::Block()
{
}


Block::Block(const QString& id)
    : m_id(id)
{
}


QString Block::getId() const
{
    return m_id;
}


const QList<Floor>& Block::getFloors() const
{
    return m_floors;
}
QList<Floor>& Block::getFloors(){
    return m_floors;
}

int Block::getFloorCount() const
{
    return m_floors.size();
}


bool Block::addFloor(const Floor& floor)
{
    if (containsFloor(floor.getNumber()))
        return false;

    m_floors.append(floor);
    return true;
}


bool Block::removeFloor(int number)
{
    for (int i = 0; i < m_floors.size(); i++)
    {
        if (m_floors[i].getNumber() == number)
        {
            m_floors.removeAt(i);
            return true;
        }
    }

    return false;
}


bool Block::containsFloor(int number) const
{
    for (const Floor& floor : m_floors)
    {
        if (floor.getNumber() == number)
            return true;
    }

    return false;
}


Floor* Block::findFloor(int number)
{
    for (Floor& floor : m_floors)
    {
        if (floor.getNumber() == number)
            return &floor;
    }

    return nullptr;
}


QString Block::toString() const
{
    return "Block " + m_id +
           ", Floors: " +
           QString::number(m_floors.size());
}