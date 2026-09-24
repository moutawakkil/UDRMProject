#include "addmenuitemdialog.h"
#include "mealperiodutils.h"

#include <QLineEdit>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QMessageBox>

AddMenuItemDialog::AddMenuItemDialog(MealPeriod initialMeal,
                                     bool editMode,
                                     const MenuItem &existingItem,
                                     QWidget *parent)
    : QDialog(parent)
    , m_initialMeal(initialMeal)
{
    buildUi(editMode, existingItem);
}

void AddMenuItemDialog::buildUi(bool editMode, const MenuItem &existingItem)
{
    setWindowTitle(editMode ? tr("Edit Item") : tr("Add Item"));

    m_mealCombo = new QComboBox(this);
    m_mealCombo->addItem(mealPeriodToString(MealPeriod::Breakfast));
    m_mealCombo->addItem(mealPeriodToString(MealPeriod::Lunch));
    m_mealCombo->addItem(mealPeriodToString(MealPeriod::Dinner));
    m_mealCombo->setCurrentIndex(static_cast<int>(m_initialMeal));
    m_mealCombo->setEnabled(!editMode);

    m_nameEdit = new QLineEdit(existingItem.name, this);
    m_categoryEdit = new QLineEdit(existingItem.category, this);

    auto *form = new QFormLayout();
    form->addRow(tr("Meal:"), m_mealCombo);
    form->addRow(tr("Item Name:"), m_nameEdit);
    form->addRow(tr("Category:"), m_categoryEdit);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(form);
    mainLayout->addWidget(m_buttonBox);

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &AddMenuItemDialog::onAccept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    resize(320, 180);
}

void AddMenuItemDialog::onAccept()
{
    if (m_nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Add Item"), tr("Please enter an item name."));
        return;
    }
    if (m_categoryEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Add Item"), tr("Please enter a category."));
        return;
    }
    accept();
}

MenuItem AddMenuItemDialog::getItem() const
{
    return MenuItem(m_nameEdit->text().trimmed(), m_categoryEdit->text().trimmed());
}

MealPeriod AddMenuItemDialog::getMeal() const
{
    return mealPeriodFromIndex(m_mealCombo->currentIndex());
}