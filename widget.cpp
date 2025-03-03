#include "widget.h"
#include "qapplication.h"

#include <QCalendarWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLocale>
#include <QTextCharFormat>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QSpinBox>
#include <QSettings>
#include <QStringList>
#include <QTime>

#include <QTextEdit>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

#include <QStandardPaths>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    createPreviewGroupBox();
    createDatesGroupBox();
    createRightInfoBar();

    QGridLayout *layout = new QGridLayout;

    layout->addWidget(previewGroupBox, 0, 0);
    layout->addWidget(datesGroupBox, 1, 0);
    layout->addWidget(rightGroupBox, 2, 0);

    layout->setRowStretch(0, 1);
    layout->setRowStretch(1, 0);
    layout->setRowStretch(2, 0);
    layout->setColumnStretch(0, 1);

    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);

    previewLayout->setRowMinimumHeight(0, calendar->sizeHint().height());
    previewLayout->setColumnMinimumWidth(0, calendar->sizeHint().width());

    setWindowTitle(tr("Calendar Widget"));
}

inline void Widget::selectedDateChanged()
{
    currentDateEdit->setDate(calendar->selectedDate());
    calendar->setFocus();
    calendar->update();
    QApplication::processEvents();
}

inline void Widget::maximumDateChanged(QDate date)
{
    calendar->setMaximumDate(date);
}

inline void Widget::createPreviewGroupBox()
{
    previewGroupBox = new QGroupBox();

    calendar = new QCalendarWidget;
    calendar->setMinimumDate(QDate(1900, 1, 1));
    calendar->setMaximumDate(QDate(3000, 1, 1));
    calendar->setGridVisible(true);
    calendar->setNavigationBarVisible(false);
    calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);


    connect(calendar, &QCalendarWidget::selectionChanged, this, &Widget::selectedDateChanged);

    previewLayout = new QGridLayout;
    previewLayout->addWidget(calendar, 0, 0);
    previewLayout->setRowStretch(0, 1);
    previewLayout->setColumnStretch(0, 1);

    previewGroupBox->setLayout(previewLayout);

    calendar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

inline void Widget::createDatesGroupBox()
{
    datesGroupBox = new QGroupBox();

    currentDateEdit = new QDateEdit;
    currentDateEdit->setDisplayFormat("MMM d yyyy");
    currentDateEdit->setDate(calendar->selectedDate());
    currentDateEdit->setDateRange(calendar->minimumDate(),
                                  calendar->maximumDate());

    currentDateLabel = new QLabel(tr("&Current Date:"));
    currentDateLabel->setBuddy(currentDateEdit);

    maximumDateEdit = new QDateEdit;
    maximumDateEdit->setDisplayFormat("MMM d yyyy");
    maximumDateEdit->setDateRange(calendar->minimumDate(), calendar->maximumDate());
    maximumDateEdit->setDate(calendar->maximumDate());

    maximumDateLabel = new QLabel(tr("Ma&ximum Date:"));
    maximumDateLabel->setBuddy(maximumDateEdit);

    connect(currentDateEdit, &QDateEdit::dateChanged, calendar, &QCalendarWidget::setSelectedDate);

    connect(maximumDateEdit, &QDateEdit::dateChanged, this, &Widget::maximumDateChanged);

    QGridLayout *dateBoxLayout = new QGridLayout;
    dateBoxLayout->addWidget(currentDateLabel, 1, 0);
    dateBoxLayout->addWidget(currentDateEdit, 1, 1);

    datesGroupBox->setLayout(dateBoxLayout);
}

void Widget::createRightInfoBar()
{
    rightGroupBox = new QGroupBox;

    QLabel *label = new QLabel(calendar->selectedDate().toString(Qt::ISODate));
    label->setAlignment(Qt::AlignCenter);
    connect(calendar, &QCalendarWidget::selectionChanged, this, [=]() {
        label->setText("Selected Date : " + currentDateEdit->date().toString(Qt::ISODate));
    });

    QSpinBox *hourSpinBox = new QSpinBox;
    hourSpinBox->setRange(0, 23); // 小时的范围是 0 到 23
    hourSpinBox->setValue(QTime::currentTime().hour()); // 设置初始小时为当前小时
    connect(hourSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [=](int value) {
        //Q_UNUSED(value); // 避免未使用参数的警告
        qDebug() << "First Hour SpinBox value changed : " << value;
    });

    QSpinBox *minuteSpinBox = new QSpinBox;
    minuteSpinBox->setRange(0, 59); // 分钟的范围是 0 到 59
    minuteSpinBox->setValue(QTime::currentTime().minute()); // 设置初始分钟为当前分钟
    connect(minuteSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [=](int value) {
        //Q_UNUSED(value); // 避免未使用参数的警告
        qDebug() << "First Minute SpinBox value changed : " << value;
    });

    QSpinBox *hourSpinBox2 = new QSpinBox;
    hourSpinBox2->setRange(0, 23); // 小时的范围是 0 到 23
    hourSpinBox2->setValue(QTime::currentTime().hour()); // 设置初始小时为当前小时
    connect(hourSpinBox2, QOverload<int>::of(&QSpinBox::valueChanged), [=](int value) {
        //Q_UNUSED(value); // 避免未使用参数的警告
        qDebug() << "Second Hour SpinBox value changed : " << value;
    });

    QSpinBox *minuteSpinBox2 = new QSpinBox;
    minuteSpinBox2->setRange(0, 59); // 分钟的范围是 0 到 59
    minuteSpinBox2->setValue(QTime::currentTime().minute()); // 设置初始分钟为当前分钟
    connect(minuteSpinBox2, QOverload<int>::of(&QSpinBox::valueChanged), [=](int value) {
        //Q_UNUSED(value); // 避免未使用参数的警告
        qDebug() << "Second Minute SpinBox value changed : " << value;
    });

    QPlainTextEdit *logTextEdit = new QPlainTextEdit;
    logTextEdit->setPlaceholderText(tr("write here..."));
    logTextEdit->setMaximumHeight(125);

    // 在构造函数或合适的位置初始化保存路径
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(appDataPath);  // 确保目录存在


    QPushButton *saveButton = new QPushButton(tr("&Save"));
    saveButton->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));

    connect(saveButton, &QPushButton::clicked, this, [=]() {
        QString logText = logTextEdit->toPlainText();

        QSettings settings("MyCompany", "MyApp");
        QString savedDirectory = settings.value("FolderPath").toString();
        QString selectedDirectory;

        qDebug() << !savedDirectory.isEmpty() << " && " << QFile((savedDirectory + "/log.txt")).exists();
        qDebug() << savedDirectory + "/log.txt";

        if (!savedDirectory.isEmpty() && QFile((savedDirectory + "/log.txt")).exists()) {
            selectedDirectory = savedDirectory;
            qDebug() << "yes! " << savedDirectory << ' ' << savedDirectory + "/log.txt";
        } else {
            selectedDirectory = appDataPath;
            qDebug() << "Using app data path: " << appDataPath;
        }

        if (!selectedDirectory.isEmpty()) {
            filename = selectedDirectory + "/log.txt";
            qDebug() << "Selected folder: " << selectedDirectory;

            settings.setValue("FolderPath", selectedDirectory);
        } else {
            filename = appDataPath + "/log.txt";
            qDebug() << "No folder selected, using default: " << filename;
        }

        settings.setValue("Filename", filename);
        QFile file(filename);
        if (!file.exists()) {
            qDebug() << "File does not exist, creating new file: " << filename;
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                qDebug() << "Unable to create new file: " << file.errorString();
                return;
            }
            file.close();
        }

        if (!file.open(QIODevice::Append | QIODevice::Text)) {
            qDebug() << "Unable to open file for writing: " << file.errorString();
            return;
        }

        QTextStream out(&file);
        out << currentDateEdit->date().toString(Qt::ISODate) << ' '
            << hourSpinBox->value() << '-' << minuteSpinBox->value() << ' '
            << hourSpinBox2->value() << '-' << minuteSpinBox2->value() << ' '
            << " : " << '\n'
            << logText << "\n\n";
        file.close();
        qDebug() << "Log saved to file: " << filename;

        qDebug() << "Saving log: " << logText;
        logTextEdit->clear();
    });

    QPushButton *calbutton = new QPushButton(tr("%1Calculate!"));
    calbutton->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_1));
    connect(calbutton, &QPushButton::clicked, this, [=]() {
        qDebug() << filename;
        if (filename.isEmpty()) {
            QSettings settings("MyCompany", "MyApp");
            filename = settings.value("Filename").toString();
            qDebug() << "Using filename: " << filename;
        }
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Unable to open file for reading: " << file.errorString();
            return;
        }

        QTextStream in(&file);
        QString fileContent = in.readAll();
        file.close();

        qDebug() << "File content: " << fileContent;

        QStringList sl = fileContent.split("\n\n", Qt::SkipEmptyParts);

        double sum = 0;

        for (const QString &_ssl : sl) {
            QStringList parts = _ssl.split(" ", Qt::SkipEmptyParts);

            if (parts.size() <= 4)  continue;

            QString string_date = parts.value(0);
            QDate date = QDate::fromString(string_date, "yyyy-MM-dd");

            QString beginTime = parts.value(1);
            QStringList bbengintime = beginTime.split("-");
            QTime time1(bbengintime[0].toInt(), bbengintime[1].toInt());

            QString endTime = parts.value(2);
            QStringList eendintime = endTime.split("-");
            QTime time2(eendintime[0].toInt(), eendintime[1].toInt());

            double timediff = time1.secsTo(time2) / 60 / 60.;

            if (date.dayOfWeek() == Qt::Sunday || date.dayOfWeek() == Qt::Saturday) {
                sum = sum + timediff * 20.;
            } else {
                sum = sum + timediff * 17.;
            }

            qDebug() << "Date:" << string_date << "QDate:" << date.dayOfWeek() << ", Begin Time:" << beginTime << ", End Time:" << endTime;
            qDebug() << "Timediff:" << timediff;
            qDebug() << "Money:" << sum;
        }

        QFile filesum(filename);
        if (!filesum.open(QIODevice::Append | QIODevice::Text)) {
            qDebug() << "Unable to open file for writing: " << file.errorString();
            return;
        }

        QTextStream out2(&filesum);
        out2 << "sum = " << sum << "\n\n";
        filesum.close();
    });

    QPushButton *cleanButton = new QPushButton(tr("Clean"));

    connect(cleanButton, &QPushButton::clicked, this, [=]() {
        qDebug() << "Clearing file contents";
        if (filename.isEmpty()) {
            QSettings settings("MyCompany", "MyApp");
            filename = settings.value("Filename").toString();
            qDebug() << "Using filename: " << filename;
        }
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            qDebug() << "Unable to open file for clearing: " << file.errorString();
            return;
        }
        file.close();

        qDebug() << "File contents cleared: " << filename;
    });

    QPushButton *viewButton = new QPushButton(tr("View File"));
    QLabel *fileContentLabel = new QLabel(this);
    fileContentLabel->setWordWrap(true);  // 允许自动换行
    fileContentLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);  // 允许文本选择
    connect(viewButton, &QPushButton::clicked, this, [=]() {
        qDebug() << "Viewing file contents";
        if (filename.isEmpty()) {
            QSettings settings("MyCompany", "MyApp");
            filename = settings.value("Filename").toString();
            qDebug() << "Using filename: " << filename;

        }
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Unable to open file for reading: " << file.errorString();
            return;
        }

        QTextStream in(&file);
        QString fileContent = in.readAll();
        file.close();

        QPushButton *close = new QPushButton(tr("Close"));

        QWidget *viewDialog = new QWidget();

        viewDialog->setWindowTitle(tr("File Contents"));
        QVBoxLayout *layout = new QVBoxLayout(viewDialog);
        QTextEdit *textEdit = new QTextEdit(viewDialog);
        textEdit->setPlainText(fileContent);
        textEdit->setReadOnly(true);
        layout->addWidget(textEdit);
        layout->addWidget(close);
        viewDialog->setLayout(layout);
        viewDialog->resize(400, 300);
        viewDialog->show();

        viewDialog->activateWindow();
        viewDialog->raise();
        QApplication::processEvents();  // 强制处理事件循环

        connect(close, &QPushButton::clicked, this, [=]() {
            viewDialog->close();
        });
    });

    rightLayout = new QGridLayout;
    rightLayout->addWidget(label);

    QGridLayout *spinboxLayout = new QGridLayout;
    QGroupBox *spinboxGroupbox = new QGroupBox;
    spinboxLayout->addWidget(hourSpinBox, 0, 0);
    spinboxLayout->addWidget(minuteSpinBox, 0, 1);
    spinboxLayout->addWidget(hourSpinBox2, 0, 2);
    spinboxLayout->addWidget(minuteSpinBox2, 0, 3);
    spinboxGroupbox->setLayout(spinboxLayout);
    rightLayout->addWidget(spinboxGroupbox);
    rightLayout->addWidget(logTextEdit);

    QGridLayout *buttonlayout = new QGridLayout;
    buttonlayout->addWidget(saveButton, 0, 0);
    buttonlayout->addWidget(calbutton, 0, 1);
    buttonlayout->addWidget(cleanButton, 1, 0);
    buttonlayout->addWidget(viewButton, 1, 1);

    QGroupBox *buttongroupbox = new QGroupBox;
    buttongroupbox->setLayout(buttonlayout);

    rightLayout->addWidget(buttongroupbox);

    rightGroupBox->setLayout(rightLayout);
}

