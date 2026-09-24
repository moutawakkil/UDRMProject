#include "addstudentdialog.h"

#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDate>

AddStudentDialog::AddStudentDialog(QWidget *parent)
    : QDialog(parent)
{
    buildUi();
}

void AddStudentDialog::buildUi()
{
    setWindowTitle(tr("Add Student"));

    m_idEdit = new QLineEdit(this);
    m_idEdit->setPlaceholderText(tr("e.g. S001"));

    m_firstNameEdit = new QLineEdit(this);
    m_lastNameEdit  = new QLineEdit(this);
    m_universityEdit = new QLineEdit(this);

    m_academicYearSpin = new QSpinBox(this);
    m_academicYearSpin->setRange(1, 8);
    m_academicYearSpin->setValue(1);

    auto *form = new QFormLayout();
    form->addRow(tr("Student ID:"), m_idEdit);
    form->addRow(tr("First Name:"), m_firstNameEdit);
    form->addRow(tr("Last Name:"), m_lastNameEdit);
    form->addRow(tr("University:"), m_universityEdit);
    form->addRow(tr("Academic Year:"), m_academicYearSpin);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(form);
    mainLayout->addWidget(m_buttonBox);

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &AddStudentDialog::onAccept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    resize(360, 220);
}

bool AddStudentDialog::validate() const
{
    if (m_idEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(const_cast<AddStudentDialog*>(this), tr("Add Student"),
                             tr("Please enter a Student ID."));
        return false;
    }

    if (m_firstNameEdit->text().trimmed().isEmpty() ||
        m_lastNameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(const_cast<AddStudentDialog*>(this), tr("Add Student"),
                             tr("Please enter the student's first and last name."));
        return false;
    }

    if (m_universityEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(const_cast<AddStudentDialog*>(this), tr("Add Student"),
                             tr("Please enter the student's university."));
        return false;
    }

    return true;
}

void AddStudentDialog::onAccept()
{
    if (!validate())
        return;

    accept();
}

Student AddStudentDialog::getStudent() const
{
    return Student(m_idEdit->text().trimmed(),
                   m_firstNameEdit->text().trimmed(),
                   m_lastNameEdit->text().trimmed(),
                   m_universityEdit->text().trimmed(),
                   m_academicYearSpin->value());
}