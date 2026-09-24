#ifndef DORMITORY_H
#define DORMITORY_H

#include <QString>
#include <QList>
#include "dormitoryConfig.h"
#include "block.h"
#include "students.h"
#include "dormitoryworker.h"
#include "restaurent.h"

class Dormitory
{
public:
    Dormitory();
Dormitory(const DormitoryConfig& config);

    // ---------------- Students ----------------

    bool addStudent(const Student& student);

    bool removeStudent(const QString& studentId);

    Student* findStudent(const QString& studentId);


      const QList<Student>& getStudents() const;

    // ---------------- Workers ----------------

    bool addWorker(const DormitoryWorker& worker);

    bool removeWorker(const QString& workerId);

    DormitoryWorker* findWorker(const QString& workerId);


  const QList<DormitoryWorker>& getWorkers() const;

    // ---------------- Building Structure ----------------

    QList<Block> getBlocks() const;


    // Find a room using its complete address:
    // Example: Block A, Floor 2, Room 33
    Room* findRoom(const QString& blockId,
                   int floorNumber,
                   int roomNumber);


    // ---------------- Accommodation ----------------

    // Assign a student to a room
    bool assignStudent(const QString& studentId,
                       const QString& blockId,
                       int floorNumber,
                       int roomNumber);


    // Remove a student from his current room
    bool removeStudentFromRoom(const QString& studentId);


    // Move a student from one room to another
    bool transferStudent(const QString& studentId,
                         const QString& newBlockId,
                         int newFloorNumber,
                         int newRoomNumber);


    // ---------------- Restaurant ----------------

    Restaurant& getRestaurant();

    const Restaurant& getRestaurant() const;

void setRestaurant(const Restaurant& restaurant);
    // ---------------- Statistics ----------------

    int getTotalStudents() const;

    int getTotalWorkers() const;

    int getTotalBlocks() const;

    int getTotalFloors() const;

    int getTotalRooms() const;

    int getAvailableRooms() const;


    // ---------------- Reports ----------------

    QString getStudentReport(const QString& studentId) const;

    QString getRoomReport(const QString& blockId,
                          int floorNumber,
                          int roomNumber) const;

    QString getBlockReport(const QString& blockId) const;

    QString getDormitoryReport() const;





private:

    // Building structure
    QList<Block> m_blocks;


    // All students in the dormitory
    QList<Student> m_students;


    // All employees of the dormitory
    QList<DormitoryWorker> m_workers;


    // The dormitory restaurant
    Restaurant m_restaurant;
};

#endif // DORMITORY_HTERY_H
