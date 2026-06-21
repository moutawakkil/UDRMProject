#ifndef TRANSFERSTUDENTDIALOG_H
#define TRANSFERSTUDENTDIALOG_H

#include <QDialog>
#include <QString>

class QComboBox;
class QLabel;
class QDialogButtonBox;
class Dormitory;


class TransferStudentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransferStudentDialog(const Dormitory &dormitory, QWidget *parent = nullptr);

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

#endif // TRANSFERSTUDENTDIALOG_H