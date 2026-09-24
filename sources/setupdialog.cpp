/*#include "setupdialog.h"
#include "ui_setupdialog.h"

SetupDialog::SetupDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SetupDialog)
{
    ui->setupUi(this);
}

SetupDialog::~SetupDialog()
{
    delete ui;
}
*/
#include "setupdialog.h"

#include <QStackedWidget>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QTableWidgetItem>

SetupDialog::SetupDialog(QWidget *parent)
    : QDialog(parent)
{
    buildUi();
}

void SetupDialog::buildUi()
{
    setWindowTitle(tr("Dormitory Initial Setup"));

    auto *title = new QLabel(tr("UDRMS - Dormitory Initial Setup"), this);
    title->setAlignment(Qt::AlignHCenter);
    QFont titleFont = title->font();
    titleFont.setBold(true);
    titleFont.setPointSize(titleFont.pointSize() + 4);
    title->setFont(titleFont);

    m_stackedWidget = new QStackedWidget(this);

    m_blocksPage = buildBlocksPage();
    m_floorPage  = buildFloorPage();

    m_stackedWidget->addWidget(m_blocksPage);
    m_stackedWidget->addWidget(m_floorPage);
    m_stackedWidget->setCurrentWidget(m_blocksPage);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, this);

    QPushButton* okButton =
        m_buttonBox->button(QDialogButtonBox::Ok);

    okButton->setEnabled(false);

    connect(okButton,
            &QPushButton::clicked,
            this,
            &QDialog::accept);
   // connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(title);
    mainLayout->addWidget(m_stackedWidget);
    mainLayout->addWidget(m_buttonBox);

    resize(700, 560);

    showFloorWidgets();
}

QWidget* SetupDialog::buildBlocksPage()
{
    auto *page = new QWidget;
    auto *layout = new QVBoxLayout(page);

    auto *countRow = new QHBoxLayout();
    auto *countLabel = new QLabel(tr("Number of Blocks:"));
    m_blockCountSpinBox = new QSpinBox();
    m_blockCountSpinBox->setRange(0, 999);
    countRow->addWidget(countLabel);
    countRow->addWidget(m_blockCountSpinBox);
    layout->addLayout(countRow);

    m_generateBlocksBtn = new QPushButton(tr("Generate Blocks"));
    layout->addWidget(m_generateBlocksBtn);

    m_blocksTable = new QTableWidget(0, 2);
    m_blocksTable->setHorizontalHeaderLabels({tr("Block ID"), tr("Floors Number")});
    m_blocksTable->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(m_blocksTable);

    m_nextToFloorsBtn = new QPushButton(tr("next"));
    auto *navRow = new QHBoxLayout();
    navRow->addStretch();
    navRow->addWidget(m_nextToFloorsBtn);
    layout->addLayout(navRow);

    connect(m_generateBlocksBtn, &QPushButton::clicked,
            this, &SetupDialog::onGenerateBlocksClicked);
    connect(m_nextToFloorsBtn, &QPushButton::clicked,
            this, &SetupDialog::onNextToFloorsClicked);

    return page;
}

QWidget* SetupDialog::buildFloorPage()
{
    auto *page = new QWidget;
    auto *outerLayout = new QGridLayout(page);

    // ---- floor-entry widgets ----
    m_blockLabel = new QLabel(tr("Block: A"));

    auto *countRow = new QHBoxLayout();
    auto *countLabel = new QLabel(tr("Number of Floors:"));
    m_floorCountSpinBox = new QSpinBox();
    m_floorCountSpinBox->setRange(0, 999);
    countRow->addWidget(countLabel);
    countRow->addWidget(m_floorCountSpinBox);

    m_generateFloorsBtn = new QPushButton(tr("Generate Floors"));

    m_floorsTable = new QTableWidget(0, 2);
    m_floorsTable->setHorizontalHeaderLabels({tr("Floor number"), tr("rooms number")});
    m_floorsTable->horizontalHeader()->setStretchLastSection(true);

    m_nextToRoomsBtn = new QPushButton(tr("Next"));

    auto *floorContent = new QWidget;
    auto *floorLayout = new QVBoxLayout(floorContent);

    floorLayout->setContentsMargins(0, 0, 0, 0);

    floorLayout->addWidget(m_blockLabel);
    floorLayout->addLayout(countRow);
    floorLayout->addWidget(m_generateFloorsBtn);
    floorLayout->addWidget(m_floorsTable);

    auto *floorNavRow = new QHBoxLayout();

    floorNavRow->addStretch();
    floorNavRow->addWidget(m_nextToRoomsBtn);

    floorLayout->addLayout(floorNavRow);

    // ---- nested rooms-entry widget (overlays the floor content above) ----
    m_floorNumberLabel = new QLabel(tr("Floor 1"));

    auto *roomCountRow = new QHBoxLayout();
    auto *roomCountLabel = new QLabel(tr("Number of Rooms"));
    m_roomCountSpinBox = new QSpinBox();
    m_roomCountSpinBox->setRange(0, 999);
    roomCountRow->addWidget(roomCountLabel);
    roomCountRow->addWidget(m_roomCountSpinBox);

    m_generateRoomsBtn = new QPushButton(tr("Generate Rooms"));

    m_roomsTable = new QTableWidget(0, 2);
    m_roomsTable->setHorizontalHeaderLabels({tr("room number"), tr("room capacity")});
    m_roomsTable->horizontalHeader()->setStretchLastSection(true);


    m_nextRoomBtn = new QPushButton(tr("Next"));

    m_roomsWidget = new QWidget;

    auto *roomsLayout = new QVBoxLayout(m_roomsWidget);
    roomsLayout->setContentsMargins(0, 0, 0, 0);

    roomsLayout->addWidget(m_floorNumberLabel);
    roomsLayout->addLayout(roomCountRow);
    roomsLayout->addWidget(m_generateRoomsBtn);
    roomsLayout->addWidget(m_roomsTable);

    auto *roomsNavRow = new QHBoxLayout();

    roomsNavRow->addStretch();
    roomsNavRow->addWidget(m_nextRoomBtn);

    roomsLayout->addLayout(roomsNavRow);
    // Both the floor content and the rooms widget occupy the same cell;
    // showFloorWidgets()/showRoomsWidgets() toggle which one is visible.
    outerLayout->addWidget(floorContent, 0, 0);
    outerLayout->addWidget(m_roomsWidget, 0, 0);

    connect(m_generateFloorsBtn, &QPushButton::clicked,
            this, &SetupDialog::onGenerateFloorsClicked);
    connect(m_nextRoomBtn,
            &QPushButton::clicked,
            this,
            &SetupDialog::onNextRoomClicked);
    connect(m_nextToRoomsBtn, &QPushButton::clicked,
            this, &SetupDialog::onNextToRoomsClicked);
    connect(m_generateRoomsBtn, &QPushButton::clicked,
            this, &SetupDialog::onGenerateRoomsClicked);


    return page;
}

// =====================================================================
// Public
// =====================================================================

DormitoryConfig SetupDialog::getConfig() const
{
    return m_config;
}

// =====================================================================
// Helpers
// =====================================================================

QString SetupDialog::blockIdForIndex(int index)
{
    // 0 -> "A", 1 -> "B", ..., 25 -> "Z", 26 -> "AA", 27 -> "AB", ...
    QString id;
    int n = index;
    do {
        id.prepend(QChar('A' + (n % 26)));
        n = n / 26 - 1;
    } while (n >= 0);
    return id;
}

bool SetupDialog::readBlocksTable()
{
    const int rowCount = m_blocksTable->rowCount();

    if (rowCount == 0) {
        QMessageBox::warning(this, tr("Setup"),
                             tr("Please generate the blocks table first."));
        return false;
    }

    QList<int> floorCounts;
    floorCounts.reserve(rowCount);

    for (int row = 0; row < rowCount; ++row) {
        QTableWidgetItem *floorsItem = m_blocksTable->item(row, 1);
        bool ok = false;
        const int floors = floorsItem ? floorsItem->text().toInt(&ok) : 0;

        if (!ok || floors <= 0) {
            QMessageBox::warning(this, tr("Setup"),
                                 tr("Block %1: please enter a valid number of floors (> 0).")
                                     .arg(blockIdForIndex(row)));
            return false;
        }

        floorCounts.append(floors);
    }

    m_blockFloorCounts = floorCounts;
    return true;
}

void SetupDialog::setupFloorPageForBlock(int blockIndex)
{
    const QString id = blockIdForIndex(blockIndex);

    m_currentBlock = BlockConfig();
    m_currentBlock.id = id;

    m_currentFloorIndex = 1;

    m_blockLabel->setText(tr("Block: %1").arg(id));

    m_floorCountSpinBox->setValue(m_blockFloorCounts.value(blockIndex, 0));
    m_floorsTable->setRowCount(0);

    showFloorWidgets();

    m_stackedWidget->setCurrentWidget(m_floorPage);
}

void SetupDialog::showFloorWidgets()
{
    m_roomsWidget->setVisible(false);

    m_blockLabel->setVisible(true);
    m_floorCountSpinBox->setVisible(true);
    m_generateFloorsBtn->setVisible(true);
    m_floorsTable->setVisible(true);

    m_nextToRoomsBtn->setVisible(true);

}

void SetupDialog::showRoomsWidgets()
{
    m_blockLabel->setVisible(false);
    m_floorCountSpinBox->setVisible(false);
    m_generateFloorsBtn->setVisible(false);
    m_floorsTable->setVisible(false);

    m_nextToRoomsBtn->setVisible(false);

    m_roomsWidget->setVisible(true);
}

bool SetupDialog::readRoomsTable()
{
    const int rowCount = m_roomsTable->rowCount();

    if (rowCount == 0) {
        QMessageBox::warning(this, tr("Setup"),
                             tr("Please generate the rooms table first."));
        return false;
    }

    QList<RoomConfig> rooms;
    rooms.reserve(rowCount);

    for (int row = 0; row < rowCount; ++row) {
        QTableWidgetItem *numberItem = m_roomsTable->item(row, 0);
        QTableWidgetItem *capacityItem = m_roomsTable->item(row, 1);

        bool ok = false;
        const int capacity = capacityItem ? capacityItem->text().toInt(&ok) : 0;

        if (!ok || capacity <= 0) {
            QMessageBox::warning(this, tr("Setup"),
                                 tr("Room %1: please enter a valid capacity (> 0).")
                                     .arg(row + 1));
            return false;
        }

        RoomConfig room;
        room.number = numberItem ? numberItem->text().toInt() : row + 1;
        room.capacity = capacity;
        rooms.append(room);
    }

    m_currentFloor.rooms = rooms;
    return true;
}

void SetupDialog::finishSetup()
{
    m_buttonBox
        ->button(QDialogButtonBox::Ok)
        ->setEnabled(true);


    QMessageBox::information(
        this,
        tr("Setup Complete"),
        tr("All blocks, floors, rooms and capacities "
           "were entered successfully.\n"
           "Click OK to continue.")
        );
}
// =====================================================================
// Page 1 - blocks
// =====================================================================

void SetupDialog::onGenerateBlocksClicked()
{
    const int blockCount = m_blockCountSpinBox->value();

    if (blockCount <= 0) {
        QMessageBox::warning(this, tr("Setup"),
                             tr("Please enter a number of blocks greater than 0."));
        return;
    }

    m_blocksTable->setRowCount(blockCount);

    for (int row = 0; row < blockCount; ++row) {
        auto *idItem = new QTableWidgetItem(blockIdForIndex(row));
        idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
        m_blocksTable->setItem(row, 0, idItem);

        auto *floorsItem = new QTableWidgetItem(QStringLiteral("1"));
        m_blocksTable->setItem(row, 1, floorsItem);
    }
}

void SetupDialog::onNextToFloorsClicked()
{
    if (!readBlocksTable())
        return;

    m_config.blocks.clear();
    m_config.blocks.reserve(m_blockFloorCounts.size());

    m_currentBlockIndex = 0;
    setupFloorPageForBlock(m_currentBlockIndex);
}

// =====================================================================
// Page 2 - floor
// =====================================================================

void SetupDialog::onGenerateFloorsClicked()
{
    const int floorCount = m_floorCountSpinBox->value();

    if (floorCount <= 0) {
        QMessageBox::warning(this, tr("Setup"),
                             tr("Please enter a number of floors greater than 0."));
        return;
    }

    m_floorsTable->setRowCount(floorCount);

    for (int row = 0; row < floorCount; ++row) {
        auto *numberItem = new QTableWidgetItem(QString::number(row + 1));
        numberItem->setFlags(numberItem->flags() & ~Qt::ItemIsEditable);
        m_floorsTable->setItem(row, 0, numberItem);

        auto *roomsItem = new QTableWidgetItem(QStringLiteral("0"));

        m_floorsTable->setItem(row, 1, roomsItem);
    }

    m_currentFloorIndex = 1;
    m_currentBlock.floors.clear();
}



void SetupDialog::onNextToRoomsClicked()
{
    const int floorCount = m_floorsTable->rowCount();

    if (floorCount == 0) {
        QMessageBox::warning(this, tr("Setup"),
                             tr("Please generate the floors table first."));
        return;
    }

    if (m_currentFloorIndex <= floorCount) {
        // Move on to entering the rooms of the current floor.
        m_currentFloor = FloorConfig();
        m_currentFloor.number = m_currentFloorIndex;

        m_floorNumberLabel->setText(tr("Floor %1").arg(m_currentFloorIndex));
        m_roomCountSpinBox->setValue(0);
        m_roomsTable->setRowCount(0);

        showRoomsWidgets();
        return;
    }

    // All floors of the current block are configured: store the block
    // and either move to the next block or finish the wizard.
    m_config.blocks.append(m_currentBlock);
    ++m_currentBlockIndex;

    if (m_currentBlockIndex < m_blockFloorCounts.size()) {
        setupFloorPageForBlock(m_currentBlockIndex);
    } else {
        finishSetup();
    }
}

// =====================================================================
// Page 2 (nested) - rooms
// =====================================================================

void SetupDialog::onGenerateRoomsClicked()
{
    const int roomCount = m_roomCountSpinBox->value();

    if (roomCount <= 0) {
        QMessageBox::warning(this, tr("Setup"),
                             tr("Please enter a number of rooms greater than 0."));
        return;
    }

    m_roomsTable->setRowCount(roomCount);

    for (int row = 0; row < roomCount; ++row) {
        auto *numberItem = new QTableWidgetItem(QString::number(row + 1));
        numberItem->setFlags(numberItem->flags() & ~Qt::ItemIsEditable);
        m_roomsTable->setItem(row, 0, numberItem);

        auto *capacityItem = new QTableWidgetItem(QStringLiteral("1"));
        m_roomsTable->setItem(row, 1, capacityItem);
    }
}

void SetupDialog::onNextRoomClicked()
{
    if (!readRoomsTable())
        return;


    // Save current floor
    m_currentBlock.floors.append(m_currentFloor);


    // Update floor table
    QTableWidgetItem* item =
        m_floorsTable->item(
            m_currentFloorIndex - 1,
            1
            );

    if (item)
    {
        item->setText(
            QString::number(
                m_currentFloor.rooms.size()
                )
            );
    }


    // Go to next floor
    ++m_currentFloorIndex;


    if (m_currentFloorIndex <= m_floorsTable->rowCount())
    {
        // Prepare next floor
        m_currentFloor = FloorConfig();
        m_currentFloor.number = m_currentFloorIndex;

        m_floorNumberLabel->setText(
            tr("Floor %1")
                .arg(m_currentFloorIndex)
            );

        m_roomCountSpinBox->setValue(0);
        m_roomsTable->setRowCount(0);

        return;
    }


    // Current block finished
    m_config.blocks.append(m_currentBlock);

    ++m_currentBlockIndex;


    // More blocks remain
    if (m_currentBlockIndex < m_blockFloorCounts.size())
    {
        setupFloorPageForBlock(
            m_currentBlockIndex
            );

        return;
    }


    // Everything finished
    finishSetup();
}