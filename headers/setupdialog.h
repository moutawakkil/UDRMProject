/*#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QDialog>

namespace Ui {
class SetupDialog;
}

class SetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetupDialog(QWidget *parent = nullptr);
    ~SetupDialog();

private:
    Ui::SetupDialog *ui;
};

#endif // SETUPDIALOG_H*/
#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QDialog>
#include <QList>
#include "dormitoryConfig.h"

class QStackedWidget;
class QLabel;
class QSpinBox;
class QPushButton;
class QTableWidget;
class QDialogButtonBox;

// SetupDialog
// -----------
// First-run wizard used when no dormitoryConfig.json exists yet.
// The whole UI is built in C++ (no .ui file) inside the constructor.
//
// Logical steps, all hosted inside a QStackedWidget:
//
//   1) "blocks" page   -> ask how many Blocks the dormitory has,
//                         generate a table (Block ID | Floors Number)
//                         where the user types how many floors each
//                         block has.
//
//   2) "floor" page    -> repeated once per block. Ask how many floors
//                         that block has (pre-filled from step 1, but
//                         editable again), generate a table
//                         (Floor number | rooms number).
//
//   3) "rooms" widget  -> nested INSIDE the "floor" page (same page,
//                         shown/hidden in place of the floor widgets).
//                         For the floor currently being configured,
//                         ask how many rooms it has and generate a
//                         table (room number | room capacity).
//                         "back" saves the floor's rooms and returns
//                         to the floor widgets so the user can move
//                         to the next floor.
//
// Once every floor of every block has its rooms configured, the
// wizard finalizes the DormitoryConfig and accept()s the dialog.
class SetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetupDialog(QWidget *parent = nullptr);
    ~SetupDialog() override = default;

    // The fully built configuration, valid once the dialog has been
    // accepted (exec() == QDialog::Accepted).
    DormitoryConfig getConfig() const;

private slots:
    // Page 1 - "blocks"
    void onGenerateBlocksClicked();
    void onNextToFloorsClicked();

    // Page 2 - "floor"
    void onGenerateFloorsClicked();

    void onNextToRoomsClicked();

    // Page 2 (nested) - "rooms"
    void onGenerateRoomsClicked();
    void onNextRoomClicked();

private:
    // ---- UI construction ------------------------------------------------
    void buildUi();
    QWidget* buildBlocksPage();
    QWidget* buildFloorPage();      // also builds the nested rooms widget

    // ---- Widgets ----------------------------------------------------------

    QStackedWidget   *m_stackedWidget = nullptr;
    QDialogButtonBox *m_buttonBox     = nullptr;

    // Page 1 - blocks
    QWidget      *m_blocksPage         = nullptr;
    QSpinBox     *m_blockCountSpinBox  = nullptr;
    QPushButton  *m_generateBlocksBtn  = nullptr;
    QTableWidget *m_blocksTable        = nullptr;
    QPushButton  *m_nextToFloorsBtn    = nullptr;

    // Page 2 - floor (own widgets)
    QWidget      *m_floorPage          = nullptr;
    QLabel       *m_blockLabel         = nullptr;
    QSpinBox     *m_floorCountSpinBox  = nullptr;
    QPushButton  *m_generateFloorsBtn  = nullptr;
    QTableWidget *m_floorsTable        = nullptr;

    QPushButton  *m_nextToRoomsBtn     = nullptr;

    // Page 2 (nested) - rooms
    QWidget      *m_roomsWidget        = nullptr;
    QLabel       *m_floorNumberLabel   = nullptr;
    QSpinBox     *m_roomCountSpinBox   = nullptr;
    QPushButton  *m_generateRoomsBtn   = nullptr;
    QTableWidget *m_roomsTable         = nullptr;
      QPushButton  *m_nextRoomBtn    = nullptr;
    // ---- Wizard state -----------------------------------------------------

    // Result being assembled.
    DormitoryConfig m_config;

    // Number of floors requested for each block, read from the
    // blocks table right after "next" is pressed on page 1.
    QList<int> m_blockFloorCounts;

    // Block currently being filled in (its floors are appended to it
    // one by one as the user finishes each floor's rooms).
    BlockConfig m_currentBlock;

    // Index (0-based) of the block currently being configured.
    int m_currentBlockIndex = 0;

    // 1-based index of the floor currently being configured within
    // the current block.
    int m_currentFloorIndex = 1;

    // Floor currently being filled in with rooms.
    FloorConfig m_currentFloor;

    // ---- Helpers ------------------------------------------------------------

    // Generates "A", "B", ..., "Z", "AA", "AB", ... for a 0-based index.
    static QString blockIdForIndex(int index);

    // Reads the blocks table into m_blockFloorCounts.
    // Returns false (and shows a warning) if any row is invalid.
    bool readBlocksTable();

    // Prepares the "floor" page for the block at blockIndex:
    // resets the floors table/spinbox and updates the block label.
    void setupFloorPageForBlock(int blockIndex);

    // Switches the "floor" page so the floor-entry widgets are
    // visible and the rooms-entry widgets are hidden.
    void showFloorWidgets();

    // Switches the "floor" page so the rooms-entry widgets are
    // visible and the floor-entry widgets are hidden.
    void showRoomsWidgets();

    // Reads the rooms table into m_currentFloor.rooms.
    // Returns false (and shows a warning) if any row is invalid.
    bool readRoomsTable();

    // Called once the very last block/floor/room has been entered:
    // builds m_config and accept()s the dialog.
    void finishSetup();
};

#endif // SETUPDIALOG_H

