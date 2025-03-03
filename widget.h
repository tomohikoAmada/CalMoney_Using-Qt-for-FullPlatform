#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDateTime>

QT_BEGIN_NAMESPACE
class QCalendarWidget;
class QCheckBox;
class QComboBox;
class QDate;
class QDateEdit;
class QGridLayout;
class QGroupBox;
class QLabel;
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    //~Widget();
private slots:
    void selectedDateChanged();
    void maximumDateChanged(QDate date);

private:
    void createPreviewGroupBox();
    void createDatesGroupBox();
    void createRightInfoBar();

    QGroupBox *rightGroupBox;
    QGridLayout *rightLayout;

    QGroupBox *previewGroupBox;
    QGridLayout *previewLayout;
    QCalendarWidget *calendar;

    QGroupBox *datesGroupBox;
    QLabel *currentDateLabel;
    QLabel *maximumDateLabel;
    QDateEdit *currentDateEdit;
    QDateEdit *maximumDateEdit;

    QString filename;
};
#endif // WIDGET_H
