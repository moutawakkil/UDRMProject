#include "UDRMS.h"
#include "ui_UDRMS.h"
#include <QDateTime>
#include <QTimer>
#include <stdexcept>
#include <QMessageBox>
#include "addstudentdialog.h"
#include "assignstudentdialog.h"
#include "transferstudentdialog.h"
#include "workertypeutils.h"
#include "addworkerdialog.h"
#include "addmenuitemdialog.h"
#include "mealperiodutils.h"
#include "reservemealdialog.h"
#include "takemealdialog.h"
#include <QInputDialog>
#include <algorithm>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QLineEdit>
#include <QAbstractItemView>
#include <QSet>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //when the app start
     initializeApplication();
     ui->widget_3->hide();
    resize(840, 374);
        refreshAllPages();
    // Create the timer
    QTimer* timer = new QTimer(this);

    // Update time every second
    connect(timer, &QTimer::timeout, this, [this]()
            {
                QDateTime now = QDateTime::currentDateTime();

                ui->timeLabel->setText(
                    now.toString("hh:mm:ss")
                    );

                ui->dateLabel->setText(
                    now.toString("dddd, dd MMMM yyyy")
                    );
            });

    // Display the time immediately when the application starts
    QDateTime now = QDateTime::currentDateTime();

    ui->timeLabel->setText(
        now.toString("hh:mm:ss")
        );

    ui->dateLabel->setText(
        now.toString("dddd, dd MMMM yyyy")
        );

    // Start updating every second
    timer->start(1000);
    refreshDashboard();
    connect(ui->AddStudentpushB, &QPushButton::clicked,
            this, &MainWindow::on_AddStudentpushB_clicked);

    connect(ui->AssignStudentpushB, &QPushButton::clicked,
            this, &MainWindow::on_AssignStudentpushB_clicked);

    populateStudentFilters();
    populateStudentTable();

    connect(ui->searchForStudent, &QLineEdit::textChanged,
            this, &MainWindow::onStudentFiltersChanged);

    connect(ui->FilterBlocks, &QComboBox::currentIndexChanged,
            this, &MainWindow::onStudentFiltersChanged);
    connect(ui->FilterYear, &QComboBox::currentIndexChanged,
            this, &MainWindow::onStudentFiltersChanged);
    connect(ui->filterStatus, &QComboBox::currentIndexChanged,
            this, &MainWindow::onStudentFiltersChanged);
    connect(ui->filterUniversity, &QComboBox::currentIndexChanged,
            this, &MainWindow::onStudentFiltersChanged);


    populateRoomsFilters();
    populateRoomsTable();

    connect(ui->filterByBlock, &QComboBox::currentIndexChanged,
            this, &MainWindow::onRoomBlockFilterChanged);
    connect(ui->filterFloor, &QComboBox::currentIndexChanged,
            this, &MainWindow::onRoomFiltersChanged);
    connect(ui->filterRooms, &QComboBox::currentIndexChanged,
            this, &MainWindow::onRoomFiltersChanged);
    connect(ui->searchRoom, &QLineEdit::textChanged,
            this, &MainWindow::onRoomFiltersChanged);

    populateWorkersFilters();
    populateWorkersTable();

    connect(ui->searchworker, &QLineEdit::textChanged,
            this, &MainWindow::onWorkerFiltersChanged);
    connect(ui->filterBytype, &QComboBox::currentIndexChanged,
            this, &MainWindow::onWorkerFiltersChanged);

    populateMenuTables();

    populateHistoryFilters();
    populateHistoryTable();
    populateOverleftFilters();
    populateOverleftTable();

    connect(ui->filterBymeals, &QComboBox::currentIndexChanged,
            this, &MainWindow::onHistoryFiltersChanged);
    connect(ui->filterstatus, &QComboBox::currentIndexChanged,
            this, &MainWindow::onHistoryFiltersChanged);
    connect(ui->filterdate, &QComboBox::currentIndexChanged,
            this, &MainWindow::onHistoryFiltersChanged);

    connect(ui->filterOleftbymeals, &QComboBox::currentIndexChanged,
            this, &MainWindow::onOverleftFiltersChanged);
    connect(ui->filterOleftbydate, &QComboBox::currentIndexChanged,
            this, &MainWindow::onOverleftFiltersChanged);


      m_dataManager.saveDormitory(m_dormitory);
}

void MainWindow::persistDormitory()
{
    const bool ok = m_dataManager.saveDormitory(m_dormitory);
    qDebug() << "saveDormitory() returned:" << ok;

    if (!ok) {
        QMessageBox::warning(this, tr("Save"),
                             tr("Failed to save dormitory data to disk."));
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::refreshAllPages()
{
    refreshDashboard();

    populateStudentFilters();
    populateStudentTable();

    populateRoomsFilters();
    populateRoomsTable();

    populateWorkersFilters();
    populateWorkersTable();

    populateMenuTables();
    populateHistoryFilters();
    populateHistoryTable();
    populateOverleftFilters();
    populateOverleftTable();
}

void MainWindow::initializeApplication()
{
    try
    {
        if (!m_dataManager.hasDormitoryConfig())
        {
            QMessageBox::StandardButton answer;

            answer = QMessageBox::question(
                this,
                "Initial Setup Required",
                "No dormitory configuration was found.\n"
                "Would you like to create a new dormitory?",
                QMessageBox::Yes | QMessageBox::No
                );

            if (answer == QMessageBox::Yes)
            {
                SetupDialog dialog(this);

                if (dialog.exec() == QDialog::Accepted)
                {
                    DormitoryConfig config =
                        dialog.getConfig();

                    m_dormitory = Dormitory(config);

                    m_dataManager.saveConfig(config);
                }
                else
                {
                    close();
                }
            }
            else
            {
                close();
            }
        }
        else
        {
            // Normal startup
            m_dormitory = m_dataManager.loadDormitory();




        }
    }
    catch (const std::exception& e)
    {
        QMessageBox::critical(
            this,
            "Startup Error",
            e.what()
            );

        close();
    }

}

void MainWindow::on_dashbooard_2_clicked()
{
       ui->stackedWidget->setCurrentIndex(0);
}
void MainWindow::on_dashbooard_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

}

void MainWindow::on_AddStudentpushB_clicked()
{    AddStudentDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        Student newStudent = dialog.getStudent();

        if (m_dormitory.addStudent(newStudent)) {
            refreshDashboard();
        } else {
            QMessageBox::warning(this, tr("Add Student"),
                                 tr("A student with this ID already exists."));
        }
    }

}


void MainWindow::on_AssignStudentpushB_clicked()
{    AssignStudentDialog dialog(m_dormitory, this);

    if (dialog.exec() == QDialog::Accepted) {
        const QString studentId   = dialog.getStudentId();
        const QString blockId     = dialog.getBlockId();
        const int floorNumber     = dialog.getFloorNumber();
        const int roomNumber      = dialog.getRoomNumber();

        if (m_dormitory.assignStudent(studentId, blockId, floorNumber, roomNumber)) {
            refreshDashboard();
        } else {
            QMessageBox::warning(this, tr("Assign Student"),
                                 tr("Could not assign this student to the selected room."));
        }
    }

}
void MainWindow::refreshDashboard()
{
    // ---------------- Stat tiles (top row) ----------------
    const int totalStudents  = m_dormitory.getTotalStudents();
    const int totalWorkers   = m_dormitory.getTotalWorkers();
    const int totalRooms     = m_dormitory.getTotalRooms();
    const int availableRooms = m_dormitory.getAvailableRooms();
    const int totalBlocks    = m_dormitory.getTotalBlocks();
    const int totalFloors    = m_dormitory.getTotalFloors();

    const double occupancyRate = (totalRooms > 0)
                                     ? (100.0 * (totalRooms - availableRooms) / totalRooms)
                                     : 0.0;

    ui->totalStud->setText(QString::number(totalStudents));
    ui->totalWork->setText(QString::number(totalWorkers));
    ui->totalRooms->setText(QString::number(totalRooms));
    ui->availableRooms->setText(QString::number(availableRooms));
    ui->occupancyRate->setText(QString::number(occupancyRate, 'f', 0) + "%");

    // ---------------- Dormitory Overview group ----------------
    // These caption-named labels ARE the value labels - set their
    // text directly, don't use any other label for these.
    int totalCapacity = 0;
    int currentOccupancy = 0;
    const QList<Block> blocks = m_dormitory.getBlocks();
    for (const Block &block : blocks) {
        for (const Floor &floor : block.getFloors()) {
            for (const Room &room : floor.getRooms()) {
                totalCapacity += room.getCapacity();
                currentOccupancy += room.getOccupancy();
            }
        }
    }

    ui->Blocks->setText(QString::number(totalBlocks));
    ui->floors->setText(QString::number(totalFloors));
    ui->TotalCapacity->setText(QString::number(totalCapacity));
    ui->Currentoccurncy->setText(QString::number(currentOccupancy));
    ui->availableRoo->setText(QString::number(availableRooms));

    // ---------------- Restaurant Today group ----------------
    const Restaurant &restaurant = m_dormitory.getRestaurant();
    const QDate today = QDate::currentDate();

    // "served" counts: requires the small getter patch in
    // restaurant_getters_patch.txt (getBreakfastServedCount(), etc.)
    ui->BreakfestServed->setText(QString::number(restaurant.getBreakfastServedCount()));
    ui->lunchServed->setText(QString::number(restaurant.getLunchServedCount()));
    ui->dinnerServed->setText(QString::number(restaurant.getDinnerServedCount()));

    // Active reservations today, across all three meal periods.
    const int activeReservations =
        restaurant.getReservationCount(MealPeriod::Breakfast, today)
        + restaurant.getReservationCount(MealPeriod::Lunch, today)
        + restaurant.getReservationCount(MealPeriod::Dinner, today);
    ui->ActiveReservations->setText(QString::number(activeReservations));

    // Students currently blocked from the restaurant.
    int blockedStudents = 0;
    for (const Student &student : m_dormitory.getStudents()) {
        if (restaurant.isBlocked(student.getId(), today))
            ++blockedStudents;
    }
    ui->BlockedStudents->setText(QString::number(blockedStudents));
}


void MainWindow::on_students_1_clicked()
{
     ui->stackedWidget->setCurrentIndex(1);
}
void MainWindow::on_students_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

}
void MainWindow::populateStudentFilters()
{ui->StudentTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // ---- Status ----
    ui->filterStatus->clear();
    ui->filterStatus->addItem(tr("All"), QString());
    ui->filterStatus->addItem(tr("Assigned"), QStringLiteral("assigned"));
    ui->filterStatus->addItem(tr("Unassigned"), QStringLiteral("unassigned"));

    // ---- Year ----
    ui->FilterYear->clear();
    ui->FilterYear->addItem(tr("All"), -1);
    for (int year = 1; year <= 8; ++year)
        ui->FilterYear->addItem(QString::number(year), year);

    // ---- Block ----
    ui->FilterBlocks->clear();
    ui->FilterBlocks->addItem(tr("All"), QString());
    for (const Block &block : m_dormitory.getBlocks())
        ui->FilterBlocks->addItem(block.getId(), block.getId());

    // ---- University (distinct values found among current students) ----
    ui->filterUniversity->clear();
    ui->filterUniversity->addItem(tr("All"), QString());

    QSet<QString> universities;
    for (const Student &student : m_dormitory.getStudents())
        universities.insert(student.getUniversity());

    QStringList sortedUniversities = universities.values();
    sortedUniversities.sort(Qt::CaseInsensitive);
    for (const QString &university : sortedUniversities)
        ui->filterUniversity->addItem(university, university);
}

// Repopulates StudentTable from m_dormitory, applying the search box
// and the 4 filter combos. Call after any change to the student list
// (add / remove / assign / transfer) and whenever a filter/search
// value changes.
void MainWindow::populateStudentTable()
{
    const QString searchText   = ui->searchForStudent->text().trimmed();
    const QString blockFilter  = ui->FilterBlocks->currentData().toString();
    const int     yearFilter   = ui->FilterYear->currentData().toInt();
    const QString statusFilter = ui->filterStatus->currentData().toString();
    const QString uniFilter    = ui->filterUniversity->currentData().toString();

    ui->StudentTable->setRowCount(0);

    for (const Student &student : m_dormitory.getStudents()) {

        // ---- search (matches ID, first name or last name) ----
        if (!searchText.isEmpty()) {
            const bool matches =
                student.getId().contains(searchText, Qt::CaseInsensitive) ||
                student.getFirstName().contains(searchText, Qt::CaseInsensitive) ||
                student.getLastName().contains(searchText, Qt::CaseInsensitive);
            if (!matches)
                continue;
        }

        // ---- status filter ----
        if (statusFilter == QStringLiteral("assigned") && !student.isAssigned())
            continue;
        if (statusFilter == QStringLiteral("unassigned") && student.isAssigned())
            continue;

        // ---- block filter (parse "A-2-101" -> "A") ----
        if (!blockFilter.isEmpty()) {
            const QString roomId = student.getRoomId();
            const QString studentBlock = roomId.section('-', 0, 0);
            if (studentBlock != blockFilter)
                continue;
        }

        // ---- year filter ----
        if (ui->FilterYear->currentIndex() > 0 && student.getAcademicYear() != yearFilter)
            continue;

        // ---- university filter ----
        if (!uniFilter.isEmpty() && student.getUniversity() != uniFilter)
            continue;

        // ---- passed all filters: add the row ----
        const int row = ui->StudentTable->rowCount();
        ui->StudentTable->insertRow(row);

        ui->StudentTable->setItem(row, 0, new QTableWidgetItem(student.getId()));
        ui->StudentTable->setItem(row, 1, new QTableWidgetItem(student.getFirstName()));
        ui->StudentTable->setItem(row, 2, new QTableWidgetItem(student.getLastName()));
        ui->StudentTable->setItem(row, 3, new QTableWidgetItem(QString::number(student.getAcademicYear())));
        ui->StudentTable->setItem(row, 4, new QTableWidgetItem(student.getUniversity()));
        ui->StudentTable->setItem(row, 5, new QTableWidgetItem(
                                              student.isAssigned() ? tr("Assigned") : tr("Unassigned")));
        ui->StudentTable->setItem(row, 6, new QTableWidgetItem(
                                              student.isAssigned() ? student.getRoomId() : tr("-")));
    }
}

void MainWindow::onStudentFiltersChanged()
{
    populateStudentTable();
}

// ---------------------------------------------------------------------
// Action buttons
// ---------------------------------------------------------------------

void MainWindow::on_AddStudentpushB_2_clicked()
{
    AddStudentDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        Student newStudent = dialog.getStudent();

        if (m_dormitory.addStudent(newStudent)) {
            populateStudentFilters();
            populateStudentTable();
            refreshDashboard();
            persistDormitory();
        } else {
            QMessageBox::warning(this, tr("Add Student"),
                                 tr("A student with this ID already exists."));
        }
    }
}

void MainWindow::on_RemoveStudent_clicked()
{ ui->StudentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->StudentTable->setSelectionMode(QAbstractItemView::SingleSelection);
    const int row = ui->StudentTable->currentRow();

    if (row < 0) {
        QMessageBox::information(this, tr("Remove Student"),
                                 tr("Please select a student in the table first."));
        return;
    }

    const QString studentId = ui->StudentTable->item(row, 0)->text();

    const auto answer = QMessageBox::question(
        this, tr("Remove Student"),
        tr("Remove student %1? This cannot be undone.").arg(studentId),
        QMessageBox::Yes | QMessageBox::No);

    if (answer != QMessageBox::Yes)
        return;

    if (m_dormitory.removeStudent(studentId)) {
        populateStudentTable();
        refreshDashboard();
        persistDormitory();
    } else {
        QMessageBox::warning(this, tr("Remove Student"),
                             tr("Could not remove this student."));
    }
}

void MainWindow::on_AssignStudentpushB_2_clicked()
{
    AssignStudentDialog dialog(m_dormitory, this);

    if (dialog.exec() == QDialog::Accepted) {
        const QString studentId = dialog.getStudentId();
        const QString blockId   = dialog.getBlockId();
        const int floorNumber   = dialog.getFloorNumber();
        const int roomNumber    = dialog.getRoomNumber();

        if (m_dormitory.assignStudent(studentId, blockId, floorNumber, roomNumber)) {
            populateStudentTable();
            refreshDashboard();
            persistDormitory();
        } else {
            QMessageBox::warning(this, tr("Assign Student"),
                                 tr("Could not assign this student to the selected room."));
        }
    }
}

void MainWindow::on_TransferStudentpushB_3_clicked()
{
    TransferStudentDialog dialog(m_dormitory, this);

    if (dialog.exec() == QDialog::Accepted) {
        const QString studentId = dialog.getStudentId();
        const QString blockId   = dialog.getBlockId();
        const int floorNumber   = dialog.getFloorNumber();
        const int roomNumber    = dialog.getRoomNumber();

        if (m_dormitory.transferStudent(studentId, blockId, floorNumber, roomNumber)) {
            populateStudentTable();
            refreshDashboard();
            persistDormitory();
        } else {
            QMessageBox::warning(this, tr("Transfer Student"),
                                 tr("Could not transfer this student to the selected room."));
        }
    }
}


void MainWindow::on_rooms_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}
void MainWindow::on_rooms_1_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);

}

void MainWindow::populateRoomsFilters()
{
    // ---- Block filter ----
    ui->filterByBlock->clear();
    ui->filterByBlock->addItem(tr("All"), QString());
    for (const Block &block : m_dormitory.getBlocks())
        ui->filterByBlock->addItem(block.getId(), block.getId());

    // ---- Floor filter (starts as "All blocks" -> union of all floors) ----
    populateRoomFloorFilter(QString());

    // ---- Room status filter ----
    ui->filterRooms->clear();
    ui->filterRooms->addItem(tr("All"), QString());
    ui->filterRooms->addItem(tr("Available"), QStringLiteral("available"));
    ui->filterRooms->addItem(tr("Full"), QStringLiteral("full"));

    // Make the rooms table strictly read-only: no add/remove from
    // this page, no in-place editing either.
    ui->RoomsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->RoomsTable->setStyleSheet(
        "QTableWidget::item:selected {"
        "    background-color: rgb(148, 210, 233);"
        "    color: black;"
        "}"
        "QTableWidget {"
        "    background-color: white;"
        "    gridline-color: white;"
        "    border: none;"
        "}"
        "QTableWidget::item {"
        "    background-color: white;"
        "    color: black;"
        "    border: 1px solid white;"
        "}"
        "QHeaderView::section {"
        "    background-color: #FF8C00;"
        "    color: white;"
        "    font-weight: bold;"
        "    border: 1px solid white;"
        "    padding: 6px;"
        "}");
}

// Repopulates the floor combo for the currently selected block.
// If blockId is empty ("All"), shows the union of every floor number
// that exists in ANY block.
void MainWindow::populateRoomFloorFilter(const QString &blockId)
{
    ui->filterFloor->clear();
    ui->filterFloor->addItem(tr("All"), -1);

    QSet<int> floorNumbers;
    for (const Block &block : m_dormitory.getBlocks()) {
        if (!blockId.isEmpty() && block.getId() != blockId)
            continue;
        for (const Floor &floor : block.getFloors())
            floorNumbers.insert(floor.getNumber());
    }

    QList<int> sortedFloors = floorNumbers.values();
    std::sort(sortedFloors.begin(), sortedFloors.end());
    for (int number : sortedFloors)
        ui->filterFloor->addItem(tr("Floor %1").arg(number), number);
}

void MainWindow::onRoomBlockFilterChanged(int index)
{
    Q_UNUSED(index);
    populateRoomFloorFilter(ui->filterByBlock->currentData().toString());
    populateRoomsTable();
}

void MainWindow::onRoomFiltersChanged()
{
    populateRoomsTable();
}

// Walks Dormitory -> Block -> Floor -> Room and (re)fills RoomsTable.
// Columns: Room ID (Block-Floor-Room) | Capacity | Students (full
// names, comma-separated, empty if the room has no one assigned).
//
// The search box matches against the combined "Block-Floor-Room"
// string, so typing "A-2-101", or just "A-2", or just "A" all work
// as partial (contains) matches.
void MainWindow::populateRoomsTable()
{
    const QString blockFilter  = ui->filterByBlock->currentData().toString();
    const int     floorFilter  = ui->filterFloor->currentData().toInt();
    const bool    anyFloor     = (ui->filterFloor->currentIndex() == 0);
    const QString statusFilter = ui->filterRooms->currentData().toString();
    const QString searchText   = ui->searchRoom->text().trimmed();

    ui->RoomsTable->setRowCount(0);

    const QList<Block> blocks = m_dormitory.getBlocks();
    for (const Block &block : blocks) {
        if (!blockFilter.isEmpty() && block.getId() != blockFilter)
            continue;

        for (const Floor &floor : block.getFloors()) {
            if (!anyFloor && floor.getNumber() != floorFilter)
                continue;

            for (const Room &room : floor.getRooms()) {

                const QString roomId = QStringLiteral("%1-%2-%3")
                .arg(block.getId())
                    .arg(floor.getNumber())
                    .arg(room.getNumber());

                if (!searchText.isEmpty() &&
                    !roomId.contains(searchText, Qt::CaseInsensitive))
                    continue;

                if (statusFilter == QStringLiteral("available") && room.isFull())
                    continue;
                if (statusFilter == QStringLiteral("full") && !room.isFull())
                    continue;

                // Build "FirstName LastName, FirstName LastName, ..."
                // from the IDs stored in the room, looking each one
                // up in the dormitory's student list.
                QStringList studentNames;
                for (const QString &studentId : room.getStudentIds()) {
                    if (Student *student = m_dormitory.findStudent(studentId))
                        studentNames.append(student->getFullName());
                }
                const QString studentsText = studentNames.join(", ");

                const int row = ui->RoomsTable->rowCount();
                ui->RoomsTable->insertRow(row);

                ui->RoomsTable->setItem(row, 0, new QTableWidgetItem(roomId));
                ui->RoomsTable->setItem(row, 1, new QTableWidgetItem(QString::number(room.getCapacity())));
                ui->RoomsTable->setItem(row, 2, new QTableWidgetItem(studentsText));
            }
        }
    }
}

// ---------------------------------------------------------------------
// Action buttons (reuse the existing Student dialogs - rooms
// themselves can't be added/removed from this page).
// ---------------------------------------------------------------------

void MainWindow::on_AssignStudentpushB_3_clicked()
{
    AssignStudentDialog dialog(m_dormitory, this);

    if (dialog.exec() == QDialog::Accepted) {
        const QString studentId = dialog.getStudentId();
        const QString blockId   = dialog.getBlockId();
        const int floorNumber   = dialog.getFloorNumber();
        const int roomNumber    = dialog.getRoomNumber();

        if (m_dormitory.assignStudent(studentId, blockId, floorNumber, roomNumber)) {
            populateRoomsTable();
            refreshDashboard();
        } else {
            QMessageBox::warning(this, tr("Assign Student"),
                                 tr("Could not assign this student to the selected room."));
        }
    }
}

void MainWindow::on_TransferStudentpushB_4_clicked()
{
    TransferStudentDialog dialog(m_dormitory, this);

    if (dialog.exec() == QDialog::Accepted) {
        const QString studentId = dialog.getStudentId();
        const QString blockId   = dialog.getBlockId();
        const int floorNumber   = dialog.getFloorNumber();
        const int roomNumber    = dialog.getRoomNumber();

        if (m_dormitory.transferStudent(studentId, blockId, floorNumber, roomNumber)) {
            populateRoomsTable();
            refreshDashboard();
        } else {
            QMessageBox::warning(this, tr("Transfer Student"),
                                 tr("Could not transfer this student to the selected room."));
        }
    }
}

// ---------------------------------------------------------------------
// Action buttons (reuse the existing Student dialogs - rooms
// themselves can't be added/removed from this page).
// ---------------------------------------------------------------------
/*
void MainWindow::on_AssignStudentpushB_3_clicked()
{
    AssignStudentDialog dialog(m_dormitory, this);

    if (dialog.exec() == QDialog::Accepted) {
        const QString studentId = dialog.getStudentId();
        const QString blockId   = dialog.getBlockId();
        const int floorNumber   = dialog.getFloorNumber();
        const int roomNumber    = dialog.getRoomNumber();

        if (m_dormitory.assignStudent(studentId, blockId, floorNumber, roomNumber)) {
            populateRoomsTable();
            refreshDashboard();
        } else {
            QMessageBox::warning(this, tr("Assign Student"),
                                 tr("Could not assign this student to the selected room."));
        }
    }
}

void MainWindow::on_TransferStudentpushB_4_clicked()
{
    TransferStudentDialog dialog(m_dormitory, this);

    if (dialog.exec() == QDialog::Accepted) {
        const QString studentId = dialog.getStudentId();
        const QString blockId   = dialog.getBlockId();
        const int floorNumber   = dialog.getFloorNumber();
        const int roomNumber    = dialog.getRoomNumber();

        if (m_dormitory.transferStudent(studentId, blockId, floorNumber, roomNumber)) {
            populateRoomsTable();
            refreshDashboard();
        } else {
            QMessageBox::warning(this, tr("Transfer Student"),
                                 tr("Could not transfer this student to the selected room."));
        }
    }
}


*/








void MainWindow::on_worker_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::on_worker_1_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);

}

// Call once after the dormitory is loaded/created.
void MainWindow::populateWorkersFilters()
{
    ui->filterBytype->clear();
    ui->filterBytype->addItem(tr("All"), QString());
    ui->filterBytype->addItem(workerTypeToString(WorkerType::Manager), QStringLiteral("Manager"));
    ui->filterBytype->addItem(workerTypeToString(WorkerType::Security), QStringLiteral("Security"));
    ui->filterBytype->addItem(workerTypeToString(WorkerType::Maintenance), QStringLiteral("Maintenance"));
    ui->filterBytype->addItem(workerTypeToString(WorkerType::Cleaner), QStringLiteral("Cleaner"));
    ui->filterBytype->addItem(workerTypeToString(WorkerType::Cook), QStringLiteral("Cook"));

    // Read-only table: no in-place editing, only via Add/Remove buttons.
    ui->wokersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->wokersTable->setStyleSheet(
        "QTableWidget::item:selected {"
        "    background-color: rgb(148, 210, 233);"
        "    color: black;"
        "}"
        "QTableWidget {"
        "    background-color: white;"
        "    gridline-color: white;"
        "    border: none;"
        "}"
        "QTableWidget::item {"
        "    background-color: white;"
        "    color: black;"
        "    border: 1px solid white;"
        "}"
        "QHeaderView::section {"
        "    background-color: #FF8C00;"
        "    color: white;"
        "    font-weight: bold;"
        "    border: 1px solid white;"
        "    padding: 6px;"
        "}");
}

void MainWindow::populateWorkersTable()
{
    const QString searchText = ui->searchworker->text().trimmed();
    const QString typeFilter = ui->filterBytype->currentData().toString();

    ui->wokersTable->setRowCount(0);

    for (const DormitoryWorker &worker : m_dormitory.getWorkers()) {

        if (!searchText.isEmpty()) {
            const bool matches =
                worker.getId().contains(searchText, Qt::CaseInsensitive) ||
                worker.getFirstName().contains(searchText, Qt::CaseInsensitive) ||
                worker.getLastName().contains(searchText, Qt::CaseInsensitive);
            if (!matches)
                continue;
        }

        if (!typeFilter.isEmpty()) {
            // Compare against the same string the combo box stores.
            QString currentTypeKey;
            switch (worker.getWorkerType()) {
            case WorkerType::Manager:     currentTypeKey = "Manager"; break;
            case WorkerType::Security:    currentTypeKey = "Security"; break;
            case WorkerType::Maintenance: currentTypeKey = "Maintenance"; break;
            case WorkerType::Cleaner:     currentTypeKey = "Cleaner"; break;
            case WorkerType::Cook:        currentTypeKey = "Cook"; break;
            }
            if (currentTypeKey != typeFilter)
                continue;
        }

        const int row = ui->wokersTable->rowCount();
        ui->wokersTable->insertRow(row);

        ui->wokersTable->setItem(row, 0, new QTableWidgetItem(worker.getId()));
        ui->wokersTable->setItem(row, 1, new QTableWidgetItem(worker.getFirstName()));
        ui->wokersTable->setItem(row, 2, new QTableWidgetItem(worker.getLastName()));
        ui->wokersTable->setItem(row, 3, new QTableWidgetItem(
                                             workerTypeToString(worker.getWorkerType())));
    }
}

void MainWindow::onWorkerFiltersChanged()
{
    populateWorkersTable();
}

void MainWindow::on_addworker_clicked()
{
    AddWorkerDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        DormitoryWorker newWorker = dialog.getWorker();

        if (m_dormitory.addWorker(newWorker)) {
            populateWorkersTable();
            refreshDashboard();
            persistDormitory();
        } else {
            QMessageBox::warning(this, tr("Add Worker"),
                                 tr("A worker with this ID already exists."));
        }
    }
}

void MainWindow::on_removeworker_clicked()
{
    const int row = ui->wokersTable->currentRow();

    if (row < 0) {
        QMessageBox::information(this, tr("Remove Worker"),
                                 tr("Please select a worker in the table first."));
        return;
    }

    const QString workerId = ui->wokersTable->item(row, 0)->text();

    const auto answer = QMessageBox::question(
        this, tr("Remove Worker"),
        tr("Remove worker %1? This cannot be undone.").arg(workerId),
        QMessageBox::Yes | QMessageBox::No);

    if (answer != QMessageBox::Yes)
        return;

    if (m_dormitory.removeWorker(workerId)) {
        populateWorkersTable();
        refreshDashboard();
        persistDormitory();
    } else {
        QMessageBox::warning(this, tr("Remove Worker"),
                             tr("Could not remove this worker."));
    }
}





void MainWindow::on_retaurant_1_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_restaurant_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);

}
void MainWindow::on_restaurantMenu_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}
void MainWindow::on_restaurantMenu_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);

}





void MainWindow::populateMenuTables()
{
    // Configure all menu tables
    for (QTableWidget *table : {
                                ui->breakfestMenu,
                                ui->lunchMenu,
                                ui->dinnerMenu })
    {
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);

        // Select the whole row when clicked
        table->setSelectionBehavior(
            QAbstractItemView::SelectRows);

        // Only one row can be selected
        table->setSelectionMode(
            QAbstractItemView::SingleSelection);

        // Remove old menu items
        table->setRowCount(0);
    }


    const Restaurant &restaurant = m_dormitory.getRestaurant();


    struct MenuPage
    {
        QTableWidget *table;
        MealPeriod meal;
    };


    MenuPage pages[] =
        {
            { ui->breakfestMenu, MealPeriod::Breakfast },
            { ui->lunchMenu,     MealPeriod::Lunch },
            { ui->dinnerMenu,    MealPeriod::Dinner }
        };


    // Fill breakfast, lunch, and dinner tables
    for (const MenuPage &page : pages)
    {
        QList<MenuItem> items =
            restaurant.getMenu(page.meal);


        for (const MenuItem &item : items)
        {
            int row = page.table->rowCount();

            page.table->insertRow(row);

            page.table->setItem(
                row,
                0,
                new QTableWidgetItem(item.name));

            page.table->setItem(
                row,
                1,
                new QTableWidgetItem(item.category));
        }
    }
}



bool MainWindow::getSelectedMenuItem(
    MealPeriod &outMeal,
    int &outRow) const
{
    struct MenuPage
    {
        QTableWidget *table;
        MealPeriod meal;
    };


    MenuPage pages[] =
        {
            { ui->breakfestMenu, MealPeriod::Breakfast },
            { ui->lunchMenu,     MealPeriod::Lunch },
            { ui->dinnerMenu,    MealPeriod::Dinner }
        };


    for (const MenuPage &page : pages)
    {
        int row = page.table->currentRow();

        if (row >= 0)
        {
            outMeal = page.meal;
            outRow = row;

            return true;
        }
    }


    return false;
}
void MainWindow::on_additem_clicked()
{
    MealPeriod meal = MealPeriod::Breakfast;
    int row = -1;
    getSelectedMenuItem(meal, row); // pre-select the meal if any table has focus

    AddMenuItemDialog dialog(meal, /*editMode=*/false, MenuItem(), this);

    if (dialog.exec() == QDialog::Accepted) {
        m_dormitory.getRestaurant().addItem(dialog.getMeal(), dialog.getItem());
        populateMenuTables();
        persistDormitory();
    }
}

void MainWindow::on_edititem_clicked()
{
    MealPeriod meal;
    int row;
    if (!getSelectedMenuItem(meal, row)) {
        QMessageBox::information(this, tr("Edit Item"),
                                 tr("Please select an item in one of the menu tables first."));
        return;
    }

    const MenuItem existingItem = m_dormitory.getRestaurant().getMenu(meal).value(row);

    AddMenuItemDialog dialog(meal, /*editMode=*/true, existingItem, this);

    if (dialog.exec() == QDialog::Accepted) {
        // Restaurant has no in-place "update" - remove then re-add.
        m_dormitory.getRestaurant().removeItem(meal, row);
        m_dormitory.getRestaurant().addItem(meal, dialog.getItem());
        populateMenuTables();
        persistDormitory();
    }
}

void MainWindow::on_removeitem_clicked()
{
    MealPeriod meal;
    int row;
    if (!getSelectedMenuItem(meal, row)) {
        QMessageBox::information(this, tr("Remove Item"),
                                 tr("Please select an item in one of the menu tables first."));
        return;
    }

    const auto answer = QMessageBox::question(
        this, tr("Remove Item"), tr("Remove the selected item?"),
        QMessageBox::Yes | QMessageBox::No);

    if (answer != QMessageBox::Yes)
        return;

    m_dormitory.getRestaurant().removeItem(meal, row);
    populateMenuTables();
    persistDormitory();
}

void MainWindow::on_clearmenu_clicked()
{
    MealPeriod meal;
    int row;

    if (!getSelectedMenuItem(meal, row)) {
        // No table has a selection - ask which menu to clear.
        const QStringList options = {
            mealPeriodToString(MealPeriod::Breakfast),
            mealPeriodToString(MealPeriod::Lunch),
            mealPeriodToString(MealPeriod::Dinner)
        };
        bool ok = false;
        const QString choice = QInputDialog::getItem(
            this, tr("Clear Menu"), tr("Clear which menu?"), options, 0, false, &ok);
        if (!ok)
            return;
        meal = mealPeriodFromIndex(options.indexOf(choice));
        persistDormitory();
    }

    const auto answer = QMessageBox::question(
        this, tr("Clear Menu"),
        tr("Clear the entire %1 menu?").arg(mealPeriodToString(meal)),
        QMessageBox::Yes | QMessageBox::No);

    if (answer != QMessageBox::Yes)
        return;

    m_dormitory.getRestaurant().clearMenu(meal);
    populateMenuTables();
}


void MainWindow::on_mealsdistribution_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);

}
void MainWindow::on_mealsdistribution_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

// Call once after the dormitory is loaded/created.
void MainWindow::populateHistoryFilters()
{
    // Meal filter
    ui->filterBymeals->clear();

    ui->filterBymeals->addItem(tr("All"), -1);
    ui->filterBymeals->addItem(
        mealPeriodToString(MealPeriod::Breakfast), 0);
    ui->filterBymeals->addItem(
        mealPeriodToString(MealPeriod::Lunch), 1);
    ui->filterBymeals->addItem(
        mealPeriodToString(MealPeriod::Dinner), 2);


    // Status filter
    ui->filterstatus->clear();

    ui->filterstatus->addItem(tr("All"), -1);
    ui->filterstatus->addItem(
        reservationStatusToString(ReservationStatus::Reserved), 0);
    ui->filterstatus->addItem(
        reservationStatusToString(ReservationStatus::Taken), 1);
    ui->filterstatus->addItem(
        reservationStatusToString(ReservationStatus::Missed), 2);
    ui->filterstatus->addItem(
        reservationStatusToString(ReservationStatus::Cancelled), 3);


    // Date filter
    ui->filterdate->clear();

    ui->filterdate->addItem(tr("All"), QDate());

    QDate today = QDate::currentDate();

    ui->filterdate->addItem(
        tr("Today (%1)")
            .arg(today.toString("yyyy-MM-dd")),
        today);

    QDate yesterday = today.addDays(-1);

    ui->filterdate->addItem(
        tr("Yesterday (%1)")
            .arg(yesterday.toString("yyyy-MM-dd")),
        yesterday);

    QDate twoDaysAgo = today.addDays(-2);

    ui->filterdate->addItem(
        tr("2 days ago (%1)")
            .arg(twoDaysAgo.toString("yyyy-MM-dd")),
        twoDaysAgo);


    // History table settings
    ui->history->setEditTriggers(
        QAbstractItemView::NoEditTriggers);

    ui->history->setSelectionBehavior(
        QAbstractItemView::SelectRows);

    ui->history->setSelectionMode(
        QAbstractItemView::SingleSelection);



    // Update table automatically when filters change
    connect(ui->filterBymeals,
            &QComboBox::currentIndexChanged,
            this,
            &MainWindow::populateHistoryTable);

    connect(ui->filterstatus,
            &QComboBox::currentIndexChanged,
            this,
            &MainWindow::populateHistoryTable);

    connect(ui->filterdate,
            &QComboBox::currentIndexChanged,
            this,
            &MainWindow::populateHistoryTable);
}
void MainWindow::populateHistoryTable()
{
    int mealIndex =
        ui->filterBymeals->currentData().toInt();

    int statusIndex =
        ui->filterstatus->currentData().toInt();

    QDate dateFilter =
        ui->filterdate->currentData().toDate();


    ui->history->setRowCount(0);


    const QList<MealReservation> reservations =
        m_dormitory.getRestaurant().getAllReservations();


    for (const MealReservation &reservation : reservations)
    {
        // Meal filtering
        if (mealIndex != -1 &&
            static_cast<int>(reservation.meal) != mealIndex)
        {
            continue;
        }


        // Status filtering
        if (statusIndex != -1 &&
            static_cast<int>(reservation.status) != statusIndex)
        {
            continue;
        }


        // Date filtering
        if (dateFilter.isValid() &&
            reservation.date != dateFilter)
        {
            continue;
        }


        int row = ui->history->rowCount();

        ui->history->insertRow(row);


        ui->history->setItem(
            row,
            0,
            new QTableWidgetItem(reservation.studentId));


        ui->history->setItem(
            row,
            1,
            new QTableWidgetItem(
                mealPeriodToString(reservation.meal)));


        ui->history->setItem(
            row,
            2,
            new QTableWidgetItem(
                reservationStatusToString(reservation.status)));


        ui->history->setItem(
            row,
            3,
            new QTableWidgetItem(
                reservation.date.toString("yyyy-MM-dd")));
    }
}
void MainWindow::onHistoryFiltersChanged()
{
    populateHistoryTable();
}
void MainWindow::populateOverleftFilters()
{
    ui->filterOleftbymeals->clear();

    ui->filterOleftbymeals->addItem(tr("All"), -1);
    ui->filterOleftbymeals->addItem(
        mealPeriodToString(MealPeriod::Breakfast), 0);
    ui->filterOleftbymeals->addItem(
        mealPeriodToString(MealPeriod::Lunch), 1);
    ui->filterOleftbymeals->addItem(
        mealPeriodToString(MealPeriod::Dinner), 2);


    ui->filterOleftbydate->clear();

    ui->filterOleftbydate->addItem(tr("All"), QDate());

    QDate today = QDate::currentDate();

    ui->filterOleftbydate->addItem(
        tr("Today"),
        today);

    ui->filterOleftbydate->addItem(
        tr("Yesterday"),
        today.addDays(-1));

    ui->filterOleftbydate->addItem(
        tr("2 Days Ago"),
        today.addDays(-2));


    ui->overlefttable->setEditTriggers(
        QAbstractItemView::NoEditTriggers);

    ui->overlefttable->setSelectionMode(
        QAbstractItemView::SingleSelection);
}
void MainWindow::populateOverleftTable()
{
    int mealIndex =
        ui->filterOleftbymeals->currentData().toInt();

    QDate dateFilter =
        ui->filterOleftbydate->currentData().toDate();


    // Keep the 3 fixed rows and remove old records
    ui->overlefttable->setColumnCount(0);


    const QList<LeftoverMeal> leftovers =
        m_dormitory.getRestaurant().getAllLeftovers();


    for (const LeftoverMeal &leftover : leftovers)
    {
        if (mealIndex != -1 &&
            static_cast<int>(leftover.meal) != mealIndex)
        {
            continue;
        }

        if (dateFilter.isValid() &&
            leftover.date != dateFilter)
        {
            continue;
        }


        int column = ui->overlefttable->columnCount();

        ui->overlefttable->insertColumn(column);


        // Row 0 -> date
        ui->overlefttable->setItem(
            0,
            column,
            new QTableWidgetItem(
                leftover.date.toString("yyyy-MM-dd")));


        // Row 1 -> meal
        ui->overlefttable->setItem(
            1,
            column,
            new QTableWidgetItem(
                mealPeriodToString(leftover.meal)));


        // Row 2 -> quantity
        ui->overlefttable->setItem(
            2,
            column,
            new QTableWidgetItem(
                QString::number(leftover.quantity)));
    }
}
void MainWindow::onOverleftFiltersChanged()
{
    populateOverleftTable();
}
// Action buttons
void MainWindow::on_reserve_clicked()
{
    ReserveMealDialog dialog(m_dormitory, this);

    if (dialog.exec() == QDialog::Accepted) {
        const bool ok = m_dormitory.getRestaurant().reserveMeal(
            dialog.getStudentId(), dialog.getMeal(), dialog.getDate());

        if (ok) {
            populateHistoryTable();
            persistDormitory();
        } else {
            QMessageBox::warning(this, tr("Reserve Meal"),
                                 tr("Could not create this reservation "
                                    "(student may be blocked or already reserved)."));
        }
    }
}

void MainWindow::on_cancelReservation_clicked()
{
    const int row = ui->history->currentRow();

    if (row < 0) {
        QMessageBox::information(this, tr("Cancel Reservation"),
                                 tr("Please select a reservation in the History table first."));
        return;
    }

    const QString studentId = ui->history->item(row, 0)->text();
    const MealPeriod meal = mealPeriodFromIndex(
        ui->history->item(row, 1)->text() == mealPeriodToString(MealPeriod::Breakfast) ? 0 :
            ui->history->item(row, 1)->text() == mealPeriodToString(MealPeriod::Lunch)     ? 1 : 2);
    const QDate date = QDate::fromString(ui->history->item(row, 3)->text(), "yyyy-MM-dd");

    if (m_dormitory.getRestaurant().cancelReservation(studentId, meal, date)) {
        populateHistoryTable();
        persistDormitory();
    } else {
        QMessageBox::warning(this, tr("Cancel Reservation"),
                             tr("Could not cancel this reservation."));
    }
}

void MainWindow::on_takemeal_clicked()
{
    const int row = ui->history->currentRow();

    QString studentId;
    MealPeriod meal = MealPeriod::Breakfast;
    QDate date = QDate::currentDate();

    if (row >= 0) {
        // Use the selected reservation.
        studentId = ui->history->item(row, 0)->text();
        meal = mealPeriodFromIndex(
            ui->history->item(row, 1)->text() == mealPeriodToString(MealPeriod::Breakfast) ? 0 :
                ui->history->item(row, 1)->text() == mealPeriodToString(MealPeriod::Lunch)     ? 1 : 2);
        date = QDate::fromString(ui->history->item(row, 3)->text(), "yyyy-MM-dd");
    } else {
        // No selection - ask for student/meal/date (e.g. a
        // non-reserved student taking a leftover meal).
        TakeMealDialog dialog(m_dormitory, this);
        if (dialog.exec() != QDialog::Accepted)
            return;
        studentId = dialog.getStudentId();
        meal = dialog.getMeal();
        date = dialog.getDate();
    }

    if (m_dormitory.getRestaurant().takeMeal(studentId, meal, date)) {
        populateHistoryTable();
        populateOverleftTable();
        persistDormitory();
    } else {
        QMessageBox::warning(this, tr("Take Meal"),
                             tr("Could not register this meal "
                                "(student may be blocked or no leftovers available)."));
    }
}

void MainWindow::on_showStatus_clicked()
{
    QString studentId;

    const int row = ui->history->currentRow();
    if (row >= 0) {
        studentId = ui->history->item(row, 0)->text();
    } else {
        QStringList ids;
        for (const Student &student : m_dormitory.getStudents())
            ids << QStringLiteral("%1 - %2").arg(student.getId(), student.getFullName());

        bool ok = false;
        const QString choice = QInputDialog::getItem(
            this, tr("Show Status"), tr("Select a student:"), ids, 0, false, &ok);
        if (!ok || ids.isEmpty())
            return;
        studentId = choice.section(" - ", 0, 0);
    }

    const QString status = m_dormitory.getRestaurant().getStudentStatus(studentId);
    QMessageBox::information(this, tr("Student Status"), status);
}

void MainWindow::on_closeMPeriod_clicked()
{
    const int mealIndex = ui->filterBymeals->currentData().toInt();
    const QDate date = ui->filterdate->currentData().toDate();

    if (mealIndex == -1 || !date.isValid()) {
        QMessageBox::information(this, tr("Close Meal Period"),
                                 tr("Please select a specific Meal and Date "
                                    "in the filters above (not \"All\") first."));
        return;
    }

    const MealPeriod meal = mealPeriodFromIndex(mealIndex);

    const auto answer = QMessageBox::question(
        this, tr("Close Meal Period"),
        tr("Close the %1 period for %2? Missed reservations will become leftovers.")
            .arg(mealPeriodToString(meal), date.toString("yyyy-MM-dd")),
        QMessageBox::Yes | QMessageBox::No);

    if (answer != QMessageBox::Yes)
        return;

    m_dormitory.getRestaurant().closeMealPeriod(meal, date);
    populateHistoryTable();
    populateOverleftTable();
    persistDormitory();
}


void MainWindow::on_report_1_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}
void MainWindow::on_report_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);

}


