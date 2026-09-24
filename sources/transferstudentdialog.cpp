#include "transferstudentdialog.h"

#include "dormitery.h"
#include "students.h"
#include "block.h"
#include "floor.h"
#include "rooms.h"

#include <QComboBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QPushButton>

TransferStudentDialog::TransferStudentDialog(const Dormitory &dormitory, QWidget *parent)
    : QDialog(parent)
    , m_dormitory(dormitory)
{
    buildUi();
    populateStudents();
    populateBlocks();
}

void TransferStudentDialog::buildUi()
{
    setWindowTitle(tr("Transfer Student"));

    m_studentCombo = new QComboBox(this);
    m_blockCombo   = new QComboBox(this);
    m_floorCombo   = new QComboBox(this);
    m_roomCombo    = new QComboBox(this);

    m_noRoomsLabel = new QLabel(tr("No available rooms on this floor."), this);
    m_noRoomsLabel->setStyleSheet("color: red;");
    m_noRoomsLabel->setVisible(false);

    auto *form = new QFormLayout();
    form->addRow(tr("Student:"), m_studentCombo);
    form->addRow(tr("New Block:"), m_blockCombo);
    form->addRow(tr("New Floor:"), m_floorCombo);
    form->addRow(tr("New Room:"), m_roomCombo);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, this);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(form);
    mainLayout->addWidget(m_noRoomsLabel);
    mainLayout->addWidget(m_buttonBox);

    connect(m_blockCombo, &QComboBox::currentIndexChanged,
            this, &TransferStudentDialog::onBlockChanged);
    connect(m_floorCombo, &QComboBox::currentIndexChanged,
            this, &TransferStudentDialog::onFloorChanged);


    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &TransferStudentDialog::onAccept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    resize(360, 240);
}

void TransferStudentDialog::populateStudents()
{
    m_studentCombo->clear();

    for (const Student &student : m_dormitory.getStudents()) {
        if (student.isAssigned()) {
            m_studentCombo->addItem(
                QStringLiteral("%1 - %2 (currently: %3)")
                    .arg(student.getId(), student.getFullName(), student.getRoomId()),
                student.getId());
        }
        else
        {         QMessageBox::warning(
                this,
                tr("No assigned students"),
                tr("There are no students assigned to rooms.\n"
                   "Please assign a student first.")
                );

            this->reject(); // closes the TransferStudentDialog
        }
    }

    if (m_studentCombo->count() == 0)
        m_studentCombo->addItem(tr("No assigned students"), QString());
}

void TransferStudentDialog::populateBlocks()
{
    m_blockCombo->clear();

    const QList<Block> blocks = m_dormitory.getBlocks();
    for (const Block &block : blocks) {
        m_blockCombo->addItem(block.getId(), block.getId());
    }

    if (m_blockCombo->count() == 0) {
        m_blockCombo->addItem(tr("No blocks"), QString());
        populateFloors(QString());
    }
    // currentIndexChanged(0) fires automatically and populates floors/rooms.
}

void TransferStudentDialog::populateFloors(const QString &blockId)
{
    m_floorCombo->clear();

    const QList<Block> blocks = m_dormitory.getBlocks();
    for (const Block &block : blocks) {
        if (block.getId() == blockId) {
            for (const Floor &floor : block.getFloors()) {
                m_floorCombo->addItem(tr("Floor %1").arg(floor.getNumber()),
                                      floor.getNumber());
            }
            break;
        }
    }

    if (m_floorCombo->count() == 0) {
        m_floorCombo->addItem(tr("No floors"), -1);
        populateRooms(blockId, -1);
    }
    // currentIndexChanged(0) fires automatically and populates rooms.
}

void TransferStudentDialog::populateRooms(const QString &blockId, int floorNumber)
{
    m_roomCombo->clear();

    const QList<Block> blocks = m_dormitory.getBlocks();
    for (const Block &block : blocks) {
        if (block.getId() != blockId)
            continue;

        for (const Floor &floor : block.getFloors()) {
            if (floor.getNumber() != floorNumber)
                continue;

            for (const Room &room : floor.getRooms()) {
                if (!room.isFull()) {
                    m_roomCombo->addItem(
                        tr("Room %1 (%2/%3)")
                            .arg(room.getNumber())
                            .arg(room.getOccupancy())
                            .arg(room.getCapacity()),
                        room.getNumber());
                }
            }
            break;
        }
        break;
    }

    m_noRoomsLabel->setVisible(m_roomCombo->count() == 0);
}

void TransferStudentDialog::onBlockChanged(int index)
{
    Q_UNUSED(index);
    populateFloors(m_blockCombo->currentData().toString());
}

void TransferStudentDialog::onFloorChanged(int index)
{
    Q_UNUSED(index);
    populateRooms(m_blockCombo->currentData().toString(),
                  m_floorCombo->currentData().toInt());
}

void TransferStudentDialog::onAccept()
{
    if (m_studentCombo->currentData().toString().isEmpty()) {
        QMessageBox::warning(this, tr("Transfer Student"),
                             tr("Please select a student."));
        return;
    }

    if ( m_roomCombo->currentData().toInt() <= 0) {
        QMessageBox::warning(this, tr("Transfer Student"),
                             tr("Please select an available room."));
        return;
    }

    accept();
}

QString TransferStudentDialog::getStudentId() const
{
    return m_studentCombo->currentData().toString();
}

QString TransferStudentDialog::getBlockId() const
{
    return m_blockCombo->currentData().toString();
}

int TransferStudentDialog::getFloorNumber() const
{
    return m_floorCombo->currentData().toInt();
}

int TransferStudentDialog::getRoomNumber() const
{
    return m_roomCombo->currentData().toInt();
}