#ifndef TAKEMEALDIALOG_H
#define TAKEMEALDIALOG_H

#include <QDialog>
#include <QDate>
#include "restaurent.h"

class QComboBox;
class QDateEdit;
class QDialogButtonBox;
class Dormitory;

// TakeMealDialog
// ---------------
// Used as a fallback when "take meal" is clicked with no row
// selected in the History table (e.g. a non-reserved student taking
// a leftover meal). Pick student/meal/date, then the caller invokes
// Dormitory::getRestaurant().takeMeal(...).
class TakeMealDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TakeMealDialog(const Dormitory &dormitory, QWidget *parent = nullptr);

    QString getStudentId() const;
    MealPeriod getMeal() const;
    QDate getDate() const;

private slots:
    void onAccept();

private:
    void buildUi();

    const Dormitory &m_dormitory;

    QComboBox  *m_studentCombo = nullptr;
    QComboBox  *m_mealCombo    = nullptr;
    QDateEdit  *m_dateEdit     = nullptr;

    QDialogButtonBox *m_buttonBox = nullptr;
};

#endif // TAKEMEALDIALOG_H