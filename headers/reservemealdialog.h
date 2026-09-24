#ifndef RESERVEMEALDIALOG_H
#define RESERVEMEALDIALOG_H

#include <QDialog>
#include <QDate>
#include "restaurent.h"

class QComboBox;
class QDateEdit;
class QDialogButtonBox;
class Dormitory;

// ReserveMealDialog
// ------------------
// Pick a student (excluding currently-blocked ones), a meal period,
// and a date, then call Dormitory::getRestaurant().reserveMeal(...)
// from the caller after exec() == QDialog::Accepted.
class ReserveMealDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReserveMealDialog(const Dormitory &dormitory, QWidget *parent = nullptr);

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

#endif // RESERVEMEALDIALOG_H
