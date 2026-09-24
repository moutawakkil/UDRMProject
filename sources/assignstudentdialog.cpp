#include "assignstudentdialog.h"

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

AssignStudentDialog::AssignStudentDialog(const Dormitory &dormitory, QWidget *parent)
    : QDialog(parent)
    , m_dormitory(dormitory)
{
    buildUi();
    populateStudents();
    populateBlocks();
}

void AssignStudentDialog::buildUi()
{
    setWindowTitle(tr("Assign Student"));

    m_studentCombo = new QComboBox(this);
    m_blockCombo   = new QComboBox(this);
    m_floorCombo   = new QComboBox(this);
    m_roomCombo    = new QComboBox(this);

    m_noRoomsLabel = new QLabel(tr("No available rooms on this floor."), this);
    m_noRoomsLabel->setStyleSheet("color: red;");
    m_noRoomsLabel->setVisible(false);

    auto *form = new QFormLayout();
    form->addRow(tr("Student:"), m_studentCombo);
    form->addRow(tr("Block:"), m_blockCombo);
    form->addRow(tr("Floor:"), m_floorCombo);
    form->addRow(tr("Room:"), m_roomCombo);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(form);
    mainLayout->addWidget(m_noRoomsLabel);
    mainLayout->addWidget(m_buttonBox);

    connect(m_blockCombo, &QComboBox::currentIndexChanged,
            this, &AssignStudentDialog::onBlockChanged);
    connect(m_floorCombo, &QComboBox::currentIndexChanged,
            this, &AssignStudentDialog::onFloorChanged);

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &AssignStudentDialog::onAccept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    resize(360, 240);
}

void AssignStudentDialog::populateStudents()
{
    m_studentCombo->clear();

    for (const Student &student : m_dormitory.getStudents()) {
        if (!student.isAssigned()) {
            m_studentCombo->addItem(
                QStringLiteral("%1 - %2").arg(student.getId(), student.getFullName()),
                student.getId());
        }
    }

    if (m_studentCombo->count() == 0)
        m_studentCombo->addItem(tr("No unassigned students"), QString());
}

void AssignStudentDialog::populateBlocks()
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

void AssignStudentDialog::populateFloors(const QString &blockId)
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

void AssignStudentDialog::populateRooms(const QString &blockId, int floorNumber)
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

void AssignStudentDialog::onBlockChanged(int index)
{
    Q_UNUSED(index);
    populateFloors(m_blockCombo->currentData().toString());
}

void AssignStudentDialog::onFloorChanged(int index)
{
    Q_UNUSED(index);
    populateRooms(m_blockCombo->currentData().toString(),
                  m_floorCombo->currentData().toInt());
}

void AssignStudentDialog::onAccept()
{
    if (m_studentCombo->currentData().toString().isEmpty()) {
        QMessageBox::warning(this, tr("Assign Student"),
                             tr("Please select a student."));
        return;
    }

    if (m_roomCombo->count() == 0 || m_roomCombo->currentData().toInt() <= 0) {
        QMessageBox::warning(this, tr("Assign Student"),
                             tr("Please select an available room."));
        return;
    }

    accept();
}

QString AssignStudentDialog::getStudentId() const
{
    return m_studentCombo->currentData().toString();
}

QString AssignStudentDialog::getBlockId() const
{
    return m_blockCombo->currentData().toString();
}

int AssignStudentDialog::getFloorNumber() const
{
    return m_floorCombo->currentData().toInt();
}

int AssignStudentDialog::getRoomNumber() const
{
    return m_roomCombo->currentData().toInt();
}