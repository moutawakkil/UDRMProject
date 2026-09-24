#ifndef UDRMS_H
#define UDRMS_H

#include <QMainWindow>
#include <QDebug>
#include <QDialog>
#include "setupdialog.h"
#include "datamanage.h"
#include "dormitery.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_dashbooard_2_clicked();

    void on_AddStudentpushB_clicked();

    void on_AssignStudentpushB_clicked();

    void on_students_1_clicked();

    void on_rooms_2_clicked();

    void on_worker_2_clicked();

    void on_retaurant_1_clicked();

    void on_report_1_clicked();

    void on_dashbooard_3_clicked();

    void on_students_2_clicked();

    void on_worker_1_clicked();

    void on_rooms_1_clicked();

    void on_restaurant_2_clicked();

    void on_report_2_clicked();
    void on_AddStudentpushB_2_clicked();
    void on_RemoveStudent_clicked();
    void on_AssignStudentpushB_2_clicked();
    void on_TransferStudentpushB_3_clicked();

    void onStudentFiltersChanged();      // connected to search box + 4 combo boxes


    void on_restaurantMenu_clicked();


    void on_restaurantMenu_2_clicked();

    void on_mealsdistribution_2_clicked();

    void on_mealsdistribution_clicked();
    // Rooms page
    void on_AssignStudentpushB_3_clicked();
    void on_TransferStudentpushB_4_clicked();
    void onRoomFiltersChanged();
    void onRoomBlockFilterChanged(int index);

    // Workers page
    void on_addworker_clicked();
    void on_removeworker_clicked();
    void onWorkerFiltersChanged();
    // Menu management page
    void on_additem_clicked();
    void on_edititem_clicked();
    void on_removeitem_clicked();
    void on_clearmenu_clicked();

    // Meals distribution page
    void on_reserve_clicked();
    void on_cancelReservation_clicked();
    void on_takemeal_clicked();
    void on_showStatus_clicked();
    void on_closeMPeriod_clicked();
    void onHistoryFiltersChanged();
    void onOverleftFiltersChanged();

private:
 void persistDormitory();
    void initializeApplication();
     void refreshDashboard();
    Ui::MainWindow *ui;
    QDialog *setupdialog;
    Dormitory m_dormitory;
    DataManager m_dataManager;
    void populateStudentFilters();       // fills FilterBlocks / filterUniversity once
    void populateStudentTable();         // (re)fills StudentTable, respecting filters/search
    // Rooms page
    void populateRoomsFilters();
    void populateRoomFloorFilter(const QString &blockId);
    void populateRoomsTable();

    // Workers page
    void populateWorkersFilters();
    void populateWorkersTable();
    // Menu management page
    void populateMenuTables();
    bool getSelectedMenuItem(MealPeriod &outMeal, int &outRow) const;

    // Meals distribution page
    void populateHistoryFilters();
    void populateHistoryTable();
    void populateOverleftFilters();
    void populateOverleftTable();
  void refreshAllPages();
};
#endif // UDRMS_H
