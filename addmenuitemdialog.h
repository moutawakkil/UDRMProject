#ifndef ADDMENUITEMDIALOG_H
#define ADDMENUITEMDIALOG_H

#include <QDialog>
#include "restaurent.h"

class QLineEdit;
class QComboBox;
class QDialogButtonBox;

// AddMenuItemDialog
// ------------------
// Used both to ADD a new MenuItem (meal period selectable) and to
// EDIT an existing one (meal period fixed/disabled, fields
// pre-filled). UI built in C++.
class AddMenuItemDialog : public QDialog
{
    Q_OBJECT

public:
    // editMode = false: "Add Item", meal combo enabled, defaults to
    // initialMeal. editMode = true: "Edit Item", meal combo disabled
    // (locked to initialMeal), name/category pre-filled.
    explicit AddMenuItemDialog(MealPeriod initialMeal,
                               bool editMode = false,
                               const MenuItem &existingItem = MenuItem(),
                               QWidget *parent = nullptr);

    MenuItem getItem() const;
    MealPeriod getMeal() const;

private slots:
    void onAccept();

private:
    void buildUi(bool editMode, const MenuItem &existingItem);

    MealPeriod m_initialMeal;

    QComboBox *m_mealCombo     = nullptr;
    QLineEdit *m_nameEdit      = nullptr;
    QLineEdit *m_categoryEdit  = nullptr;

    QDialogButtonBox *m_buttonBox = nullptr;
};

#endif // ADDMENUITEMDIALOG_H