#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <QString>
#include <QList>
#include <QMap>
#include <QDate>
#include <QJsonObject>
#include <QJsonArray>

struct MenuItem
{
    QString name;
    QString category;

    MenuItem() {}

    MenuItem(const QString& n, const QString& c)
        : name(n), category(c) {}

    QString toString() const
    {
        return name + " [" + category + "]";
    }

    QJsonObject toJson() const
    {
        QJsonObject json;

        json["name"] = name;
        json["category"] = category;

        return json;
    }

    static MenuItem fromJson(const QJsonObject& json)
    {
        return MenuItem(
            json["name"].toString(),
            json["category"].toString()
            );
    }
};


enum class MealPeriod
{
    Breakfast,
    Lunch,
    Dinner
};


struct DailyMenu
{
    QList<MenuItem> breakfast;
    QList<MenuItem> lunch;
    QList<MenuItem> dinner;
};


enum class ReservationStatus
{
    Reserved,
    Taken,
    Missed,
    Cancelled
};


struct MealReservation
{
    QString studentId;
    MealPeriod meal;
    QDate date;

    ReservationStatus status = ReservationStatus::Reserved;


    QJsonObject toJson() const
    {
        QJsonObject json;

        json["studentId"] = studentId;
        json["meal"] = static_cast<int>(meal);
        json["date"] = date.toString(Qt::ISODate);
        json["status"] = static_cast<int>(status);

        return json;
    }


    static MealReservation fromJson(const QJsonObject& json)
    {
        MealReservation reservation;

        reservation.studentId =
            json["studentId"].toString();

        reservation.meal =
            static_cast<MealPeriod>(
                json["meal"].toInt()
                );

        reservation.date =
            QDate::fromString(
                json["date"].toString(),
                Qt::ISODate
                );

        reservation.status =
            static_cast<ReservationStatus>(
                json["status"].toInt()
                );

        return reservation;
    }
};
// Represents meals that became available
// after the reservation period ended
struct LeftoverMeal
{
    MealPeriod meal;
    QDate date;
    int quantity = 0;


    QJsonObject toJson() const
    {
        QJsonObject json;

        json["meal"] = static_cast<int>(meal);
        json["date"] = date.toString(Qt::ISODate);
        json["quantity"] = quantity;

        return json;
    }


    static LeftoverMeal fromJson(const QJsonObject& json)
    {
        LeftoverMeal leftover;

        leftover.meal =
            static_cast<MealPeriod>(
                json["meal"].toInt()
                );

        leftover.date =
            QDate::fromString(
                json["date"].toString(),
                Qt::ISODate
                );

        leftover.quantity =
            json["quantity"].toInt();

        return leftover;
    }
};

// Stores student restaurant violations

struct StudentMealRecord
{   // Number of times the student took
    // a leftover meal without reservation
    int noReservationCount = 0;

    // Number of reservations missed
    int missedReservationCount = 0;

    // Student cannot reserve or eat
    // until this date
    QDate blockedUntil;


    QJsonObject toJson() const
    {
        QJsonObject json;

        json["noReservationCount"] =
            noReservationCount;

        json["missedReservationCount"] =
            missedReservationCount;

        json["blockedUntil"] =
            blockedUntil.toString(Qt::ISODate);

        return json;
    }


    static StudentMealRecord fromJson(
        const QJsonObject& json)
    {
        StudentMealRecord record;

        record.noReservationCount =
            json["noReservationCount"].toInt();

        record.missedReservationCount =
            json["missedReservationCount"].toInt();

        record.blockedUntil =
            QDate::fromString(
                json["blockedUntil"].toString(),
                Qt::ISODate
                );

        return record;
    }
};


class Restaurant
{
public:
    Restaurant();


    // Menu management

    void addItem(MealPeriod meal,
                 const MenuItem& item);

    void removeItem(MealPeriod meal,
                    int index);

    QList<MenuItem> getMenu(MealPeriod meal) const;

    void clearMenu(MealPeriod meal);


    // Reservation management

    bool reserveMeal(const QString& studentId,
                     MealPeriod meal,
                     const QDate& date);

    bool cancelReservation(const QString& studentId,
                           MealPeriod meal,
                           const QDate& date);


    // Taking meals

    // Reserved student:
    // Reserved -> Taken
    //
    // Non-reserved student:
    // Can only take from available leftovers.
    // Each successful case increases
    // noReservationCount.
    bool takeMeal(const QString& studentId,
                  MealPeriod meal,
                  const QDate& date);


    // End of meal period

    // Reserved students who did not come:
    // Reserved -> Missed
    //
    // The number of missed reservations
    // becomes available leftovers.
    void closeMealPeriod(MealPeriod meal,
                         const QDate& date);


    // Information functions

    QList<QString> getReservedStudents(
        MealPeriod meal,
        const QDate& date) const;

    int getReservationCount(
        MealPeriod meal,
        const QDate& date) const;


    // Returns number of available leftovers
    int getLeftoverCount(
        MealPeriod meal,
        const QDate& date) const;


    bool isBlocked(
        const QString& studentId,
        const QDate& currentDate) const;


    QString getStudentStatus(
        const QString& studentId) const;
    int getBreakfastServedCount() const;
    int getLunchServedCount() const;
    int getDinnerServedCount() const;
    QList<MealReservation> getAllReservations() const;
    QList<LeftoverMeal> getAllLeftovers() const;

    QString toString() const;
    QJsonObject toJson() const;
  static   Restaurant fromJson(const QJsonObject& json);

private:

    DailyMenu m_menu;


    // All student reservations
    QList<MealReservation> m_reservations;


    // Available meals after closing
    QList<LeftoverMeal> m_leftovers;


    // Student warnings and penalties
    QMap<QString, StudentMealRecord> m_records;

    void checkAndApplyBlock(const QString& studentId,
                            const QDate& date);
    void updateExpiredBlock(const QString& studentId);
    // Internal helpers
    QList<MenuItem>& getMenuList(MealPeriod meal);

    const QList<MenuItem>& getMenuList(
        MealPeriod meal) const;
    // Statistics
    int m_breakfastServed = 0;
    int m_lunchServed = 0;
    int m_dinnerServed = 0;

};


#endif // RESTAURANT_H