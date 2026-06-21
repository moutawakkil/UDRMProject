#include "takemealdialog.h"
#include "mealperiodutils.h"
#include "dormitery.h"
#include "students.h"

#include <QComboBox>
#include <QDateEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QMessageBox>

TakeMealDialog::TakeMealDialog(const Dormitory &dormitory, QWidget *parent)
    : QDialog(parent)
    , m_dormitory(dormitory)
{
    buildUi();
}

void TakeMealDialog::buildUi()
{
    setWindowTitle(tr("Take Meal"));

    m_studentCombo = new QComboBox(this);
    const QDate today = QDate::currentDate();
    for (const Student &student : m_dormitory.getStudents()) {
        if (!m_dormitory.getRestaurant().isBlocked(student.getId(), today)) {
            m_studentCombo->addItem(
                QStringLiteral("%1 - %2").arg(student.getId(), student.getFullName()),
                student.getId());
        }
    }
    if (m_studentCombo->count() == 0)
        m_studentCombo->addItem(tr("No eligible students"), QString());

    m_mealCombo = new QComboBox(this);
    m_mealCombo->addItem(mealPeriodToString(MealPeriod::Breakfast));
    m_mealCombo->addItem(mealPeriodToString(MealPeriod::Lunch));
    m_mealCombo->addItem(mealPeriodToString(MealPeriod::Dinner));

    m_dateEdit = new QDateEdit(today, this);
    m_dateEdit->setCalendarPopup(true);

    auto *form = new QFormLayout();
    form->addRow(tr("Student:"), m_studentCombo);
    form->addRow(tr("Meal:"), m_mealCombo);
    form->addRow(tr("Date:"), m_dateEdit);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(form);
    mainLayout->addWidget(m_buttonBox);

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &TakeMealDialog::onAccept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    resize(320, 200);
}

void TakeMealDialog::onAccept()
{
    if (m_studentCombo->currentData().toString().isEmpty()) {
        QMessageBox::warning(this, tr("Take Meal"), tr("Please select a student."));
        return;
    }
    accept();
}

QString TakeMealDialog::getStudentId() const
{
    return m_studentCombo->currentData().toString();
}

MealPeriod TakeMealDialog::getMeal() const
{
    return mealPeriodFromIndex(m_mealCombo->currentIndex());
}

QDate TakeMealDialog::getDate() const
{
    return m_dateEdit->date();
}