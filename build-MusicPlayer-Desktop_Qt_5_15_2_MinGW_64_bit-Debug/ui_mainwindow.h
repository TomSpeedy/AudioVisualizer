/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGroupBox *controllerGroupBox;
    QSlider *volumeSlider;
    QSlider *progressSlider;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *playButton;
    QPushButton *stopButton;
    QPushButton *nextButton;
    QLabel *currentlyPlayingLabel;
    QLabel *volumeLabel;
    QGroupBox *queueGroupBox;
    QListWidget *queueListWidget;
    QPushButton *addToQueueButton;
    QPushButton *removeFromQueueButton;
    QGraphicsView *graphicsView;
    QMenuBar *menubar;
    QMenu *menuMusic_Player;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1258, 683);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        controllerGroupBox = new QGroupBox(centralwidget);
        controllerGroupBox->setObjectName(QString::fromUtf8("controllerGroupBox"));
        controllerGroupBox->setGeometry(QRect(20, 20, 501, 161));
        volumeSlider = new QSlider(controllerGroupBox);
        volumeSlider->setObjectName(QString::fromUtf8("volumeSlider"));
        volumeSlider->setGeometry(QRect(360, 120, 121, 16));
        volumeSlider->setOrientation(Qt::Horizontal);
        progressSlider = new QSlider(controllerGroupBox);
        progressSlider->setObjectName(QString::fromUtf8("progressSlider"));
        progressSlider->setGeometry(QRect(10, 50, 451, 20));
        progressSlider->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(controllerGroupBox);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 110, 320, 25));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        playButton = new QPushButton(layoutWidget);
        playButton->setObjectName(QString::fromUtf8("playButton"));

        horizontalLayout->addWidget(playButton);

        stopButton = new QPushButton(layoutWidget);
        stopButton->setObjectName(QString::fromUtf8("stopButton"));

        horizontalLayout->addWidget(stopButton);

        nextButton = new QPushButton(layoutWidget);
        nextButton->setObjectName(QString::fromUtf8("nextButton"));

        horizontalLayout->addWidget(nextButton);

        currentlyPlayingLabel = new QLabel(controllerGroupBox);
        currentlyPlayingLabel->setObjectName(QString::fromUtf8("currentlyPlayingLabel"));
        currentlyPlayingLabel->setGeometry(QRect(20, 20, 281, 16));
        volumeLabel = new QLabel(controllerGroupBox);
        volumeLabel->setObjectName(QString::fromUtf8("volumeLabel"));
        volumeLabel->setGeometry(QRect(370, 90, 47, 13));
        queueGroupBox = new QGroupBox(centralwidget);
        queueGroupBox->setObjectName(QString::fromUtf8("queueGroupBox"));
        queueGroupBox->setGeometry(QRect(20, 190, 501, 321));
        queueListWidget = new QListWidget(queueGroupBox);
        queueListWidget->setObjectName(QString::fromUtf8("queueListWidget"));
        queueListWidget->setGeometry(QRect(20, 30, 461, 192));
        addToQueueButton = new QPushButton(queueGroupBox);
        addToQueueButton->setObjectName(QString::fromUtf8("addToQueueButton"));
        addToQueueButton->setGeometry(QRect(30, 230, 75, 23));
        removeFromQueueButton = new QPushButton(queueGroupBox);
        removeFromQueueButton->setObjectName(QString::fromUtf8("removeFromQueueButton"));
        removeFromQueueButton->setGeometry(QRect(120, 230, 75, 23));
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(550, 0, 650, 650));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1258, 21));
        menuMusic_Player = new QMenu(menubar);
        menuMusic_Player->setObjectName(QString::fromUtf8("menuMusic_Player"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuMusic_Player->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        controllerGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Controller", nullptr));
        playButton->setText(QCoreApplication::translate("MainWindow", "Play", nullptr));
        stopButton->setText(QCoreApplication::translate("MainWindow", "Stop", nullptr));
        nextButton->setText(QCoreApplication::translate("MainWindow", ">>", nullptr));
        currentlyPlayingLabel->setText(QString());
        volumeLabel->setText(QCoreApplication::translate("MainWindow", "Volume", nullptr));
        queueGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Queue", nullptr));
        addToQueueButton->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        removeFromQueueButton->setText(QCoreApplication::translate("MainWindow", "Remove", nullptr));
        menuMusic_Player->setTitle(QCoreApplication::translate("MainWindow", "Music Player", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
