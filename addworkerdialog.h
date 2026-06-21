#ifndef ADDWORKERDIALOG_H
#define ADDWORKERDIALOG_H

#include <QDialog>
#include "dormitoryworker.h"

class QLineEdit;
class QComboBox;
class QDialogButtonBox;

class AddWorkerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddWorkerDialog(QWidget *parent = nullptr);

    // Valid only after exec() == QDialog::Accepted.
    DormitoryWorker getWorker() const;

private slots:
    void onAccept();

private:
    void buildUi();
    bool validate() const;

    QLineEdit *m_idEdit        = nullptr;
    QLineEdit *m_firstNameEdit = nullptr;
    QLineEdit *m_lastNameEdit  = nullptr;
    QComboBox *m_workerTypeCombo = nullptr;

    QDialogButtonBox *m_buttonBox = nullptr;
};

#endif // ADDWORKERDIALOG_H
