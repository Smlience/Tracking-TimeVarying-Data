/********************************************************************************
** Form generated from reading UI file 'featuretracking.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FEATURETRACKING_H
#define UI_FEATURETRACKING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_featureTrackingClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuOpen;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *featureTrackingClass)
    {
        if (featureTrackingClass->objectName().isEmpty())
            featureTrackingClass->setObjectName(QStringLiteral("featureTrackingClass"));
        featureTrackingClass->resize(600, 400);
        centralWidget = new QWidget(featureTrackingClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        featureTrackingClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(featureTrackingClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        menuOpen = new QMenu(menuBar);
        menuOpen->setObjectName(QStringLiteral("menuOpen"));
        featureTrackingClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(featureTrackingClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        featureTrackingClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(featureTrackingClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        featureTrackingClass->setStatusBar(statusBar);

        menuBar->addAction(menuOpen->menuAction());

        retranslateUi(featureTrackingClass);

        QMetaObject::connectSlotsByName(featureTrackingClass);
    } // setupUi

    void retranslateUi(QMainWindow *featureTrackingClass)
    {
        featureTrackingClass->setWindowTitle(QApplication::translate("featureTrackingClass", "featureTracking", 0));
        menuOpen->setTitle(QApplication::translate("featureTrackingClass", "open", 0));
    } // retranslateUi

};

namespace Ui {
    class featureTrackingClass: public Ui_featureTrackingClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FEATURETRACKING_H
