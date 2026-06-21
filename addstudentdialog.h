#ifndef ADDSTUDENTDIALOG_H
#define ADDSTUDENTDIALOG_H

#include <QDialog>
#include "students.h"

class QLineEdit;
class QSpinBox;
class QDialogButtonBox;

class AddStudentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddStudentDialog(QWidget *parent = nullptr);

    // Valid only after exec() == QDialog::Accepted.
    Student getStudent() const;

private slots:
    void onAccept();

private:
    void buildUi();
    bool validate() const;

    QLineEdit *m_idEdit          = nullptr;
    QLineEdit *m_firstNameEdit   = nullptr;
    QLineEdit *m_lastNameEdit    = nullptr;
    QLineEdit *m_universityEdit  = nullptr;
    QSpinBox  *m_academicYearSpin = nullptr;

    QDialogButtonBox *m_buttonBox = nullptr;
};

#endif // ADDSTUDENTDIALOG_H