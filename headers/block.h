#ifndef BLOCK_H
#define BLOCK_H

#include <QString>
#include <QList>
#include "floor.h"

class Block
{
public:
    Block();
    Block(const QString& id);


    // Getters
    QString getId() const;
   const QList<Floor>& getFloors() const;
    QList<Floor>& getFloors();
    int getFloorCount() const;


    // Floor management
    bool addFloor(const Floor& floor);
    bool removeFloor(int number);
    bool containsFloor(int number) const;


    // Search
    Floor* findFloor(int number);


    // Utility
    QString toString() const;


private:
    QString m_id; // A, B, C...

    // Composition: a block has floors
    QList<Floor> m_floors;
};

#endif // BLOCK_H