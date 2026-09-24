#ifndef DORMITORYCONFIG_H
#define DORMITORYCONFIG_H

#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <utility>

struct RoomConfig
{
    int number;
    int capacity;


    QJsonObject toJson() const
    {
        QJsonObject json;

        json["number"] = number;
        json["capacity"] = capacity;

        return json;
    }


    static RoomConfig fromJson(const QJsonObject& json)
    {
        RoomConfig room;

        room.number = json["number"].toInt();
        room.capacity = json["capacity"].toInt();

        return room;
    }
};



struct FloorConfig
{
    int number;

    QList<RoomConfig> rooms;


    QJsonObject toJson() const
    {
        QJsonObject json;

        QJsonArray roomsArray;

        for (const RoomConfig& room : rooms)
        {
            roomsArray.append(room.toJson());
        }

        json["number"] = number;
        json["rooms"] = roomsArray;

        return json;
    }


    static FloorConfig fromJson(const QJsonObject& json)
    {
        FloorConfig floor;

        floor.number = json["number"].toInt();

        QJsonArray roomsArray = json["rooms"].toArray();

       // for (const QJsonValue& value : roomsArray)
        for (const QJsonValue& value : std::as_const(roomsArray))
        {
            floor.rooms.append(
                RoomConfig::fromJson(value.toObject())
                );
        }

        return floor;
    }
};



struct BlockConfig
{
    QString id;

    QList<FloorConfig> floors;


    QJsonObject toJson() const
    {
        QJsonObject json;

        QJsonArray floorsArray;

        for (const FloorConfig& floor : floors)
        {
            floorsArray.append(floor.toJson());
        }

        json["id"] = id;
        json["floors"] = floorsArray;

        return json;
    }


    static BlockConfig fromJson(const QJsonObject& json)
    {
        BlockConfig block;

        block.id = json["id"].toString();

        QJsonArray floorsArray = json["floors"].toArray();
        for (const QJsonValue& value : std::as_const(floorsArray))

        {
            block.floors.append(
                FloorConfig::fromJson(value.toObject())
                );
        }

        return block;
    }
};



struct DormitoryConfig
{
    QList<BlockConfig> blocks;


    QJsonObject toJson() const
    {
        QJsonObject json;

        QJsonArray blocksArray;

        for (const BlockConfig& block : blocks)
        {
            blocksArray.append(block.toJson());
        }

        json["blocks"] = blocksArray;

        return json;
    }


    static DormitoryConfig fromJson(const QJsonObject& json)
    {
        DormitoryConfig config;

        QJsonArray blocksArray = json["blocks"].toArray();

        for (const QJsonValue& value : std::as_const(blocksArray))
        {
            config.blocks.append(
                BlockConfig::fromJson(value.toObject())
                );
        }

        return config;
    }
};


#endif // DORMITORYCONFIG_H