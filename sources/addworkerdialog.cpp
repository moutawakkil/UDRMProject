#include "addworkerdialog.h"
#include "workertypeutils.h"

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QMessageBox>

AddWorkerDialog::AddWorkerDialog(QWidget *parent)
    : QDialog(parent)
{
    buildUi();
}

void AddWorkerDialog::buildUi()
{
    setWindowTitle(tr("Add Worker"));

    m_idEdit = new QLineEdit(this);
    m_idEdit->setPlaceholderText(tr("e.g. W001"));

    m_firstNameEdit = new QLineEdit(this);
    m_lastNameEdit  = new QLineEdit(this);

    m_workerTypeCombo = new QComboBox(this);
    m_workerTypeCombo->addItem(workerTypeToString(WorkerType::Manager));
    m_workerTypeCombo->addItem(workerTypeToString(WorkerType::Security));
    m_workerTypeCombo->addItem(workerTypeToString(WorkerType::Maintenance));
    m_workerTypeCombo->addItem(workerTypeToString(WorkerType::Cleaner));
    m_workerTypeCombo->addItem(workerTypeToString(WorkerType::Cook));

    auto *form = new QFormLayout();
    form->addRow(tr("Worker ID:"), m_idEdit);
    form->addRow(tr("First Name:"), m_firstNameEdit);
    form->addRow(tr("Last Name:"), m_lastNameEdit);
    form->addRow(tr("Worker Type:"), m_workerTypeCombo);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(form);
    mainLayout->addWidget(m_buttonBox);

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &AddWorkerDialog::onAccept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    resize(340, 200);
}

bool AddWorkerDialog::validate() const
{
    if (m_idEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(const_cast<AddWorkerDialog*>(this), tr("Add Worker"),
                             tr("Please enter a Worker ID."));
        return false;
    }

    if (m_firstNameEdit->text().trimmed().isEmpty() ||
        m_lastNameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(const_cast<AddWorkerDialog*>(this), tr("Add Worker"),
                             tr("Please enter the worker's first and last name."));
        return false;
    }

    return true;
}

void AddWorkerDialog::onAccept()
{
    if (!validate())
        return;

    accept();
}

DormitoryWorker AddWorkerDialog::getWorker() const
{
    return DormitoryWorker(m_idEdit->text().trimmed(),
                           m_firstNameEdit->text().trimmed(),
                           m_lastNameEdit->text().trimmed(),
                           workerTypeFromIndex(m_workerTypeCombo->currentIndex()));
}