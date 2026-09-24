#ifndef MEALPERIODUTILS_H
#define MEALPERIODUTILS_H

#include <QString>
#include "restaurent.h"

inline QString mealPeriodToString(MealPeriod meal)
{
    switch (meal) {
    case MealPeriod::Breakfast: return QObject::tr("Breakfast");
    case MealPeriod::Lunch:     return QObject::tr("Lunch");
    case MealPeriod::Dinner:    return QObject::tr("Dinner");
    }
    return QObject::tr("Unknown");
}

inline MealPeriod mealPeriodFromIndex(int index)
{
    // Must match the order items are added to any "meal" combo box
    // (index 0 reserved for "All" where applicable - callers must
    // offset accordingly).
    switch (index) {
    case 0: return MealPeriod::Breakfast;
    case 1: return MealPeriod::Lunch;
    case 2: return MealPeriod::Dinner;
    default: return MealPeriod::Breakfast;
    }
}

inline QString reservationStatusToString(ReservationStatus status)
{
    switch (status) {
    case ReservationStatus::Reserved:  return QObject::tr("Reserved");
    case ReservationStatus::Taken:     return QObject::tr("Taken");
    case ReservationStatus::Missed:    return QObject::tr("Missed");
    case ReservationStatus::Cancelled: return QObject::tr("Cancelled");
    }
    return QObject::tr("Unknown");
}

#endif // MEALPERIODUTILS_H
