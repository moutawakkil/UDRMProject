#include "dormitery.h"
// ---------------- Constructors ----------------
Dormitory::Dormitory()
{
}


Dormitory::Dormitory(const DormitoryConfig& config)
{
    for (const BlockConfig& blockConfig : config.blocks)
    {
        Block block(blockConfig.id);


        for (const FloorConfig& floorConfig : blockConfig.floors)
        {
            Floor floor(floorConfig.number);


            for (const RoomConfig& roomConfig : floorConfig.rooms)
            {
                Room room(
                    roomConfig.number,
                    roomConfig.capacity
                    );

                floor.addRoom(room);
            }


            block.addFloor(floor);
        }


        m_blocks.append(block);
    }
}


// ---------------- Student Management ----------------

bool Dormitory::addStudent(const Student& student)
{
    if (findStudent(student.getId()) != nullptr)
    {
        return false;
    }

    m_students.append(student);

    return true;
}


bool Dormitory::removeStudent(const QString& studentId)
{
    for (int i = 0; i < m_students.size(); i++)
    {
        if (m_students[i].getId() == studentId)
        {
            // Remove him from his room first
            removeStudentFromRoom(studentId);

            m_students.removeAt(i);

            return true;
        }
    }

    return false;
}


Student* Dormitory::findStudent(const QString& studentId)
{
    for (Student& student : m_students)
    {
        if (student.getId() == studentId)
        {
            return &student;
        }
    }

    return nullptr;
}


// ---------------- Worker Management ----------------

bool Dormitory::addWorker(const DormitoryWorker& worker)
{
    if (findWorker(worker.getId()) != nullptr)
    {
        return false;
    }


    m_workers.append(worker);

    return true;
}


bool Dormitory::removeWorker(const QString& workerId)
{
    for (int i = 0; i < m_workers.size(); i++)
    {
        if (m_workers[i].getId() == workerId)
        {
            m_workers.removeAt(i);

            return true;
        }
    }


    return false;
}


DormitoryWorker* Dormitory::findWorker(
    const QString& workerId)
{
    for (DormitoryWorker& worker : m_workers)
    {
        if (worker.getId() == workerId)
        {
            return &worker;
        }
    }


    return nullptr;
}
// ---------------- Building Structure ----------------

QList<Block> Dormitory::getBlocks() const
{
    return m_blocks;
}


// Find a room using Block ID + Floor number + Room number
Room* Dormitory::findRoom(
    const QString& blockId,
    int floorNumber,
    int roomNumber)
{
    for (Block& block : m_blocks)
    {
        if (block.getId() == blockId)
        {
            Floor* floor = block.findFloor(floorNumber);

            if (floor != nullptr)
            {
                return floor->findRoom(roomNumber);
            }
        }
    }

    return nullptr;
}


// ---------------- Accommodation ----------------


// Assign a student to a room
bool Dormitory::assignStudent(
    const QString& studentId,
    const QString& blockId,
    int floorNumber,
    int roomNumber)
{
    // Find the student
    Student* student = findStudent(studentId);

    if (student == nullptr)
    {
        return false;
    }


    // Student already has a room
    if (student->isAssigned())
    {
        return false;
    }


    // Find the room
    Room* room = findRoom(
        blockId,
        floorNumber,
        roomNumber
        );

    if (room == nullptr)
    {
        return false;
    }


    // Add the student ID inside the room
    if (!room->addStudent(studentId))
    {
        return false;
    }


    // Create the room address:
    // Example: A-2-33
    QString roomId =
        blockId + "-" +
        QString::number(floorNumber) + "-" +
        QString::number(roomNumber);


    // Update the student
    student->setRoomId(roomId);

    return true;
}


// Remove a student from his current room
bool Dormitory::removeStudentFromRoom(
    const QString& studentId)
{
    Student* student = findStudent(studentId);

    if (student == nullptr)
    {
        return false;
    }


    // Student has no room
    if (!student->isAssigned())
    {
        return false;
    }


    // Search every room until we find him
    for (Block& block : m_blocks)
    {
        for (Floor& floor : block.getFloors())
        {
            for (Room& room : floor.getRooms())
            {
                if (room.containsStudent(studentId))
                {
                    room.removeStudent(studentId);

                    // Clear the student's room
                    student->setRoomId("");

                    return true;
                }
            }
        }
    }


    return false;
}


// Move a student from one room to another
bool Dormitory::transferStudent(
    const QString& studentId,
    const QString& newBlockId,
    int newFloorNumber,
    int newRoomNumber)
{
    // Check if the student exists
    Student* student = findStudent(studentId);

    if (student == nullptr)
    {
        return false;
    }


    // The student must already have a room
    if (!student->isAssigned())
    {
        return false;
    }


    // Check that the new room exists
    Room* newRoom = findRoom(
        newBlockId,
        newFloorNumber,
        newRoomNumber
        );

    if (newRoom == nullptr)
    {
        return false;
    }


    // Check that the room can accept the student
    if (newRoom->isFull() ||
        newRoom->containsStudent(studentId))
    {
        return false;
    }


    // Now it is safe to remove the old assignment
    if (!removeStudentFromRoom(studentId))
    {
        return false;
    }


    // Add the student to the new room
    if (!newRoom->addStudent(studentId))
    {
        return false;
    }


    // Update the student's room address
    QString newRoomId =
        newBlockId + "-" +
        QString::number(newFloorNumber) + "-" +
        QString::number(newRoomNumber);

    student->setRoomId(newRoomId);

    return true;
}


// ---------------- Restaurant ----------------

Restaurant& Dormitory::getRestaurant()
{
    return m_restaurant;
}


const Restaurant& Dormitory::getRestaurant() const
{
    return m_restaurant;
}


void Dormitory::setRestaurant(
    const Restaurant& restaurant)
{
    m_restaurant = restaurant;
}


int Dormitory::getTotalStudents() const
{
    return m_students.size();
}

int Dormitory::getTotalWorkers() const
{
    return m_workers.size();
}

int Dormitory::getTotalBlocks() const
{
    return m_blocks.size();
}

int Dormitory::getTotalFloors() const
{
    int total = 0;

    for (const Block& block : m_blocks)
    {
        total += block.getFloorCount();
    }

    return total;
}

int Dormitory::getTotalRooms() const
{
    int total = 0;

    for (const Block& block : m_blocks)
    {
        for (const Floor& floor : block.getFloors())
        {
            total += floor.getRoomCount();
        }
    }

    return total;
}

int Dormitory::getAvailableRooms() const
{
    int available = 0;

    for (const Block& block : m_blocks)
    {
        for (const Floor& floor : block.getFloors())
        {
            for (const Room& room : floor.getRooms())
            {
                if (!room.isFull())
                {
                    available++;
                }
            }
        }
    }

    return available;
}
// ---------------- Reports ----------------


// Report about a student
QString Dormitory::getStudentReport(
    const QString& studentId) const
{
    for (const Student& student : m_students)
    {
        if (student.getId() == studentId)
        {
            return student.toString();
        }
    }

    return "Student not found.";
}


// Report about a room
QString Dormitory::getRoomReport(
    const QString& blockId,
    int floorNumber,
    int roomNumber) const
{
    for (const Block& block : m_blocks)
    {
        if (block.getId() == blockId)
        {
            for (const Floor& floor : block.getFloors())
            {
                if (floor.getNumber() == floorNumber)
                {
                    for (const Room& room : floor.getRooms())
                    {
                        if (room.getNumber() == roomNumber)
                        {
                            return room.toString();
                        }
                    }
                }
            }
        }
    }

    return "Room not found.";
}


// Report about a block
QString Dormitory::getBlockReport(
    const QString& blockId) const
{
    for (const Block& block : m_blocks)
    {
        if (block.getId() == blockId)
        {
            return block.toString();
        }
    }

    return "Block not found.";
}


// Complete dormitory report
QString Dormitory::getDormitoryReport() const
{
    QString report;

    report += "===== Dormitory Report =====\n\n";

    report += "Students: " +
              QString::number(getTotalStudents()) + "\n";

    report += "Workers: " +
              QString::number(getTotalWorkers()) + "\n";

    report += "Blocks: " +
              QString::number(getTotalBlocks()) + "\n";

    report += "Floors: " +
              QString::number(getTotalFloors()) + "\n";

    report += "Rooms: " +
              QString::number(getTotalRooms()) + "\n";

    report += "Available Rooms: " +
              QString::number(getAvailableRooms()) + "\n";

    return report;
}


const QList<Student>& Dormitory::getStudents() const
{
    return m_students;
}
const QList<DormitoryWorker>& Dormitory::getWorkers() const
{
    return m_workers;
}
