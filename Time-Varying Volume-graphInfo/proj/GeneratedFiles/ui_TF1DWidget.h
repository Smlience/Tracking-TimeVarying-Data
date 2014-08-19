/********************************************************************************
** Form generated from reading UI file 'TF1DWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TF1DWIDGET_H
#define UI_TF1DWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "src/tf/1dtf/TF1DCanvas.h"

QT_BEGIN_NAMESPACE

class Ui_TF1D
{
public:
    QHBoxLayout *horizontalLayout;
    TF1DCanvas *canvas;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QPushButton *btnSave;
    QPushButton *btnLoad;
    QPushButton *btnReset;

    void setupUi(QWidget *TF1D)
    {
        if (TF1D->objectName().isEmpty())
            TF1D->setObjectName(QStringLiteral("TF1D"));
        TF1D->resize(549, 266);
        horizontalLayout = new QHBoxLayout(TF1D);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        canvas = new TF1DCanvas(TF1D);
        canvas->setObjectName(QStringLiteral("canvas"));

        horizontalLayout->addWidget(canvas);

        widget = new QWidget(TF1D);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMaximumSize(QSize(32, 16777215));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        btnSave = new QPushButton(widget);
        btnSave->setObjectName(QStringLiteral("btnSave"));
        btnSave->setMinimumSize(QSize(32, 32));
        btnSave->setMaximumSize(QSize(32, 32));

        verticalLayout->addWidget(btnSave);

        btnLoad = new QPushButton(widget);
        btnLoad->setObjectName(QStringLiteral("btnLoad"));
        btnLoad->setMinimumSize(QSize(32, 32));
        btnLoad->setMaximumSize(QSize(32, 32));

        verticalLayout->addWidget(btnLoad);

        btnReset = new QPushButton(widget);
        btnReset->setObjectName(QStringLiteral("btnReset"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnReset->sizePolicy().hasHeightForWidth());
        btnReset->setSizePolicy(sizePolicy);
        btnReset->setMinimumSize(QSize(32, 32));
        btnReset->setMaximumSize(QSize(32, 32));

        verticalLayout->addWidget(btnReset);


        horizontalLayout->addWidget(widget);


        retranslateUi(TF1D);
        QObject::connect(btnReset, SIGNAL(clicked()), canvas, SLOT(resetTF()));

        QMetaObject::connectSlotsByName(TF1D);
    } // setupUi

    void retranslateUi(QWidget *TF1D)
    {
        TF1D->setWindowTitle(QApplication::translate("TF1D", "TF1D", 0));
        btnSave->setText(QApplication::translate("TF1D", "Save", 0));
        btnLoad->setText(QApplication::translate("TF1D", "Load", 0));
#ifndef QT_NO_TOOLTIP
        btnReset->setToolTip(QApplication::translate("TF1D", "Reset", 0));
#endif // QT_NO_TOOLTIP
        btnReset->setText(QApplication::translate("TF1D", "Reset", 0));
    } // retranslateUi

};

namespace Ui {
    class TF1D: public Ui_TF1D {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TF1DWIDGET_H
