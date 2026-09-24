#include "restaurent.h"


    Restaurant::Restaurant()
{
}


QList<MenuItem>& Restaurant::getMenuList(MealPeriod meal)
{
    switch (meal)
    {
    case MealPeriod::Breakfast:
        return m_menu.breakfast;

    case MealPeriod::Lunch:
        return m_menu.lunch;

    case MealPeriod::Dinner:
        return m_menu.dinner;
    }

    return m_menu.breakfast;
}


const QList<MenuItem>& Restaurant::getMenuList(MealPeriod meal) const
{
    switch (meal)
    {
    case MealPeriod::Breakfast:
        return m_menu.breakfast;

    case MealPeriod::Lunch:
        return m_menu.lunch;

    case MealPeriod::Dinner:
        return m_menu.dinner;
    }

    return m_menu.breakfast;
}


void Restaurant::addItem(MealPeriod meal, const MenuItem& item)
{
    getMenuList(meal).append(item);
}


void Restaurant::removeItem(MealPeriod meal, int index)
{
    QList<MenuItem>& menu = getMenuList(meal);

    if (index >= 0 && index < menu.size())
    {
        menu.removeAt(index);
    }
}


QList<MenuItem> Restaurant::getMenu(MealPeriod meal) const
{
    return getMenuList(meal);
}


void Restaurant::clearMenu(MealPeriod meal)
{
    getMenuList(meal).clear();
}


bool Restaurant::isBlocked(const QString& studentId,
                           const QDate& currentDate) const
{
    if (!m_records.contains(studentId))
        return false;

    return currentDate <= m_records[studentId].blockedUntil;
}
void Restaurant::checkAndApplyBlock(const QString& studentId,
                                    const QDate& date)
{
    StudentMealRecord& record = m_records[studentId];

    if (record.noReservationCount >= 3 ||
        record.missedReservationCount >= 3)
    {
        record.blockedUntil = date.addDays(7);
    }
}

void Restaurant::updateExpiredBlock(const QString& studentId)
{
    StudentMealRecord& record = m_records[studentId];

    if (record.blockedUntil.isValid() &&
        QDate::currentDate() > record.blockedUntil)
    {
        record.noReservationCount = 0;
        record.missedReservationCount = 0;

        record.blockedUntil = QDate();
    }
}


bool Restaurant::reserveMeal(const QString& studentId,
                             MealPeriod meal,
                             const QDate& date)
{
    updateExpiredBlock(studentId);

    if (isBlocked(studentId, date))
        return false;

    for (const MealReservation& reservation : m_reservations)
    {
        if (reservation.studentId == studentId &&
            reservation.meal == meal &&
            reservation.date == date &&
            reservation.status != ReservationStatus::Cancelled)
        {
            return false;
        }
    }

    MealReservation reservation;

    reservation.studentId = studentId;
    reservation.meal = meal;
    reservation.date = date;

    m_reservations.append(reservation);

    return true;
}


bool Restaurant::cancelReservation(const QString& studentId,
                                   MealPeriod meal,
                                   const QDate& date)
{
    for (MealReservation& reservation : m_reservations)
    {
        if (reservation.studentId == studentId &&
            reservation.meal == meal &&
            reservation.date == date &&
            reservation.status == ReservationStatus::Reserved)
        {
            reservation.status = ReservationStatus::Cancelled;
            return true;
        }
    }

    return false;
}


bool Restaurant::takeMeal(const QString& studentId,
                          MealPeriod meal,
                          const QDate& date)
{    updateExpiredBlock(studentId);


    if (isBlocked(studentId, date))
        return false;


    // Case 1: Student has reservation
    for (MealReservation& reservation : m_reservations)
    {
        if (reservation.studentId == studentId &&
            reservation.meal == meal &&
            reservation.date == date)
        {
            if (reservation.status == ReservationStatus::Reserved)
            {
                reservation.status = ReservationStatus::Taken;
                return true;
            }

            return false;
        }
    }


    // Case 2: Student has no reservation
    // Search for available leftovers
    for (LeftoverMeal& leftover : m_leftovers)
    {
        if (leftover.meal == meal &&
            leftover.date == date &&
            leftover.quantity > 0)
        {
            leftover.quantity--;

            StudentMealRecord& record = m_records[studentId];

            record.noReservationCount++;

            m_records[studentId].noReservationCount++;

            checkAndApplyBlock(studentId, date);

            return true;
        }
    }


    // No leftovers available
    return false;
}


void Restaurant::closeMealPeriod(MealPeriod meal,
                                 const QDate& date)
{
    int missedCount = 0;

    for (MealReservation& reservation : m_reservations)
    {
        if (reservation.meal == meal &&
            reservation.date == date &&
            reservation.status == ReservationStatus::Reserved)
        {
            reservation.status = ReservationStatus::Missed;

            missedCount++;

            m_records[reservation.studentId]
                .missedReservationCount++;

            checkAndApplyBlock(reservation.studentId, date);
        }
    }

    if (missedCount > 0)
    {
        LeftoverMeal leftover;

        leftover.meal = meal;
        leftover.date = date;
        leftover.quantity = missedCount;

        m_leftovers.append(leftover);
    }
}


QList<QString> Restaurant::getReservedStudents(
    MealPeriod meal,
    const QDate& date) const
{
    QList<QString> students;

    for (const MealReservation& reservation : m_reservations)
    {
        if (reservation.meal == meal &&
            reservation.date == date &&
            reservation.status == ReservationStatus::Reserved)
        {
            students.append(reservation.studentId);
        }
    }

    return students;
}


int Restaurant::getReservationCount(MealPeriod meal,
                                    const QDate& date) const
{
    int count = 0;

    for (const MealReservation& reservation : m_reservations)
    {
        if (reservation.meal == meal &&
            reservation.date == date &&
            reservation.status != ReservationStatus::Cancelled)
        {
            count++;
        }
    }

    return count;
}


int Restaurant::getLeftoverCount(MealPeriod meal,
                                 const QDate& date) const
{
    for (const LeftoverMeal& leftover : m_leftovers)
    {
        if (leftover.meal == meal &&
            leftover.date == date)
        {
            return leftover.quantity;
        }
    }

    return 0;
}


QString Restaurant::getStudentStatus(
    const QString& studentId) const
{
    if (!m_records.contains(studentId))
    {
        return "No warnings";
    }

    const StudentMealRecord& record = m_records[studentId];

    QString result;

    result += "Meals without reservation: "
              + QString::number(record.noReservationCount)
              + "\n";

    result += "Missed reservations: "
              + QString::number(record.missedReservationCount)
              + "\n";

    if (record.blockedUntil.isValid())
    {
        result += "Blocked until: "
                  + record.blockedUntil.toString("dd/MM/yyyy");
    }
    else
    {
        result += "Not blocked";
    }

    return result;
}


QString Restaurant::toString() const
{
    QString result;

    result += "Restaurant Report\n";
    result += "Reservations: "
              + QString::number(m_reservations.size())
              + "\n";

    result += "Students with records: "
              + QString::number(m_records.size());

    return result;
}
QJsonObject Restaurant::toJson() const
{
    QJsonObject json;


    // Menu
    QJsonObject menuObject;

    QJsonArray breakfastArray;
    for (const MenuItem& item : m_menu.breakfast)
    {
        breakfastArray.append(item.toJson());
    }

    QJsonArray lunchArray;
    for (const MenuItem& item : m_menu.lunch)
    {
        lunchArray.append(item.toJson());
    }

    QJsonArray dinnerArray;
    for (const MenuItem& item : m_menu.dinner)
    {
        dinnerArray.append(item.toJson());
    }

    menuObject["breakfast"] = breakfastArray;
    menuObject["lunch"] = lunchArray;
    menuObject["dinner"] = dinnerArray;

    json["menu"] = menuObject;


    // Reservations
    QJsonArray reservationsArray;

    for (const MealReservation& reservation : m_reservations)
    {
        reservationsArray.append(
            reservation.toJson()
            );
    }

    json["reservations"] = reservationsArray;


    // Leftover meals
    QJsonArray leftoversArray;

    for (const LeftoverMeal& leftover : m_leftovers)
    {
        leftoversArray.append(
            leftover.toJson()
            );
    }

    json["leftovers"] = leftoversArray;


    // Student records
    QJsonObject recordsObject;

    for (auto it = m_records.constBegin();
         it != m_records.constEnd();
         ++it)
    {
        recordsObject[it.key()] =
            it.value().toJson();
    }

    json["records"] = recordsObject;


    // Statistics
    json["breakfastServed"] = m_breakfastServed;
    json["lunchServed"] = m_lunchServed;
    json["dinnerServed"] = m_dinnerServed;


    return json;
}


Restaurant Restaurant::fromJson(const QJsonObject& json)
{
    Restaurant restaurant;


    // Menu
    QJsonObject menuObject =
        json["menu"].toObject();


    QJsonArray breakfastArray =
        menuObject["breakfast"].toArray();

    for (const QJsonValue& value : breakfastArray)
    {
        restaurant.m_menu.breakfast.append(
            MenuItem::fromJson(
                value.toObject()
                )
            );
    }


    QJsonArray lunchArray =
        menuObject["lunch"].toArray();

    for (const QJsonValue& value : lunchArray)
    {
        restaurant.m_menu.lunch.append(
            MenuItem::fromJson(
                value.toObject()
                )
            );
    }


    QJsonArray dinnerArray =
        menuObject["dinner"].toArray();

    for (const QJsonValue& value : dinnerArray)
    {
        restaurant.m_menu.dinner.append(
            MenuItem::fromJson(
                value.toObject()
                )
            );
    }


    // Reservations
    QJsonArray reservationsArray =
        json["reservations"].toArray();

    for (const QJsonValue& value : reservationsArray)
    {
        restaurant.m_reservations.append(
            MealReservation::fromJson(
                value.toObject()
                )
            );
    }


    // Leftovers
    QJsonArray leftoversArray =
        json["leftovers"].toArray();

    for (const QJsonValue& value : leftoversArray)
    {
        restaurant.m_leftovers.append(
            LeftoverMeal::fromJson(
                value.toObject()
                )
            );
    }


    // Student records
    QJsonObject recordsObject =
        json["records"].toObject();

    for (auto it = recordsObject.constBegin();
         it != recordsObject.constEnd();
         ++it)
    {
        restaurant.m_records.insert(
            it.key(),
            StudentMealRecord::fromJson(
                it.value().toObject()
                )
            );
    }


    // Statistics
    restaurant.m_breakfastServed =
        json["breakfastServed"].toInt();

    restaurant.m_lunchServed =
        json["lunchServed"].toInt();

    restaurant.m_dinnerServed =
        json["dinnerServed"].toInt();


    return restaurant;
}
int Restaurant::getBreakfastServedCount() const
{
    return m_breakfastServed;
}

int Restaurant::getLunchServedCount() const
{
    return m_lunchServed;
}

int Restaurant::getDinnerServedCount() const
{
    return m_dinnerServed;
}
QList<MealReservation> Restaurant::getAllReservations() const
{
    return m_reservations;
}

QList<LeftoverMeal> Restaurant::getAllLeftovers() const
{
    return m_leftovers;
}