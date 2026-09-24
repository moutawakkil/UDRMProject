#ifndef ASSIGNSTUDENTDIALOG_H
#define ASSIGNSTUDENTDIALOG_H

#include <QDialog>
#include <QString>

class QComboBox;
class QLabel;
class QDialogButtonBox;
class Dormitory;

// AssignStudentDialog
// --------------------
// Lets the user pick an unassigned student and an available room
// (Block -> Floor -> Room, cascading combo boxes that only ever show
// rooms that are not already full). UI built entirely in C++.
//
// The dialog only READS the dormitory to populate its combo boxes;
// the actual assignment (Dormitory::assignStudent) is performed by
// the caller after exec() == QDialog::Accepted, using the getters
// below.
class AssignStudentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AssignStudentDialog(const Dormitory &dormitory, QWidget *parent = nullptr);

    // Valid only after exec() == QDialog::Accepted.
    QString getStudentId() const;
    QString getBlockId() const;
    int getFloorNumber() const;
    int getRoomNumber() const;

private slots:
    void onBlockChanged(int index);
    void onFloorChanged(int index);
    void onAccept();

private:
    void buildUi();
    void populateStudents();
    void populateBlocks();
    void populateFloors(const QString &blockId);
    void populateRooms(const QString &blockId, int floorNumber);

    const Dormitory &m_dormitory;

    QComboBox *m_studentCombo = nullptr;
    QComboBox *m_blockCombo   = nullptr;
    QComboBox *m_floorCombo   = nullptr;
    QComboBox *m_roomCombo    = nullptr;

    QLabel *m_noRoomsLabel = nullptr;

    QDialogButtonBox *m_buttonBox = nullptr;
};

#endif // ASSIGNSTUDENTDIALOG_H
