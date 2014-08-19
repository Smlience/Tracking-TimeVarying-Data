/********************************************************************************
** Form generated from reading UI file 'LightParameterWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIGHTPARAMETERWIDGET_H
#define UI_LIGHTPARAMETERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LightParameterWidget
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QDoubleSpinBox *sb_ambient;
    QLabel *label_3;
    QDoubleSpinBox *sb_specular;
    QDoubleSpinBox *sb_diffuse;
    QLabel *label_2;
    QDoubleSpinBox *sb_shininess;
    QLabel *label_4;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_5;
    QDoubleSpinBox *lp_x;
    QDoubleSpinBox *lp_y;
    QDoubleSpinBox *lp_z;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QComboBox *cb_shader;
    QLabel *label_7;
    QLabel *label_6;
    QComboBox *cb_gradient;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *LightParameterWidget)
    {
        if (LightParameterWidget->objectName().isEmpty())
            LightParameterWidget->setObjectName(QStringLiteral("LightParameterWidget"));
        LightParameterWidget->resize(367, 186);
        LightParameterWidget->setMinimumSize(QSize(0, 0));
        LightParameterWidget->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout = new QVBoxLayout(LightParameterWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(LightParameterWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label, 0, 1, 1, 1);

        sb_ambient = new QDoubleSpinBox(groupBox);
        sb_ambient->setObjectName(QStringLiteral("sb_ambient"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sb_ambient->sizePolicy().hasHeightForWidth());
        sb_ambient->setSizePolicy(sizePolicy1);
        sb_ambient->setMaximumSize(QSize(16777215, 16777215));
        sb_ambient->setSingleStep(0.1);
        sb_ambient->setValue(0.1);

        gridLayout->addWidget(sb_ambient, 0, 2, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_3, 4, 3, 1, 1);

        sb_specular = new QDoubleSpinBox(groupBox);
        sb_specular->setObjectName(QStringLiteral("sb_specular"));
        sizePolicy1.setHeightForWidth(sb_specular->sizePolicy().hasHeightForWidth());
        sb_specular->setSizePolicy(sizePolicy1);
        sb_specular->setMaximumSize(QSize(16777215, 16777215));
        sb_specular->setSingleStep(0.1);
        sb_specular->setValue(1);

        gridLayout->addWidget(sb_specular, 4, 2, 1, 1);

        sb_diffuse = new QDoubleSpinBox(groupBox);
        sb_diffuse->setObjectName(QStringLiteral("sb_diffuse"));
        sizePolicy1.setHeightForWidth(sb_diffuse->sizePolicy().hasHeightForWidth());
        sb_diffuse->setSizePolicy(sizePolicy1);
        sb_diffuse->setMaximumSize(QSize(16777215, 16777215));
        sb_diffuse->setSingleStep(0.1);
        sb_diffuse->setValue(0.1);

        gridLayout->addWidget(sb_diffuse, 0, 4, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_2, 4, 1, 1, 1);

        sb_shininess = new QDoubleSpinBox(groupBox);
        sb_shininess->setObjectName(QStringLiteral("sb_shininess"));
        sizePolicy1.setHeightForWidth(sb_shininess->sizePolicy().hasHeightForWidth());
        sb_shininess->setSizePolicy(sizePolicy1);
        sb_shininess->setMaximumSize(QSize(16777215, 16777215));
        sb_shininess->setMaximum(128);
        sb_shininess->setSingleStep(0.1);
        sb_shininess->setValue(3);

        gridLayout->addWidget(sb_shininess, 4, 4, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_4, 0, 3, 1, 1);

        widget = new QWidget(groupBox);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(widget);
        label_5->setObjectName(QStringLiteral("label_5"));
        sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(label_5);

        lp_x = new QDoubleSpinBox(widget);
        lp_x->setObjectName(QStringLiteral("lp_x"));
        sizePolicy1.setHeightForWidth(lp_x->sizePolicy().hasHeightForWidth());
        lp_x->setSizePolicy(sizePolicy1);
        lp_x->setMaximumSize(QSize(16777215, 16777215));
        lp_x->setMinimum(-1000);
        lp_x->setMaximum(1000);
        lp_x->setSingleStep(0.1);

        horizontalLayout->addWidget(lp_x);

        lp_y = new QDoubleSpinBox(widget);
        lp_y->setObjectName(QStringLiteral("lp_y"));
        sizePolicy1.setHeightForWidth(lp_y->sizePolicy().hasHeightForWidth());
        lp_y->setSizePolicy(sizePolicy1);
        lp_y->setMaximumSize(QSize(16777215, 16777215));
        lp_y->setMinimum(-1000);
        lp_y->setMaximum(1000);
        lp_y->setSingleStep(0.1);

        horizontalLayout->addWidget(lp_y);

        lp_z = new QDoubleSpinBox(widget);
        lp_z->setObjectName(QStringLiteral("lp_z"));
        sizePolicy1.setHeightForWidth(lp_z->sizePolicy().hasHeightForWidth());
        lp_z->setSizePolicy(sizePolicy1);
        lp_z->setMaximumSize(QSize(16777215, 16777215));
        lp_z->setMinimum(-1000);
        lp_z->setMaximum(1000);
        lp_z->setSingleStep(0.1);
        lp_z->setValue(1000);

        horizontalLayout->addWidget(lp_z);


        gridLayout->addWidget(widget, 5, 1, 1, 4);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(LightParameterWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        cb_shader = new QComboBox(groupBox_2);
        cb_shader->setObjectName(QStringLiteral("cb_shader"));

        gridLayout_2->addWidget(cb_shader, 0, 1, 1, 1);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));
        sizePolicy.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(label_7, 0, 2, 1, 1);

        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        sizePolicy.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(label_6, 0, 0, 1, 1);

        cb_gradient = new QComboBox(groupBox_2);
        cb_gradient->setObjectName(QStringLiteral("cb_gradient"));

        gridLayout_2->addWidget(cb_gradient, 0, 3, 1, 1);


        verticalLayout->addWidget(groupBox_2);

        verticalSpacer = new QSpacerItem(20, 4, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(LightParameterWidget);

        cb_shader->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(LightParameterWidget);
    } // setupUi

    void retranslateUi(QWidget *LightParameterWidget)
    {
        LightParameterWidget->setWindowTitle(QApplication::translate("LightParameterWidget", "LightParameterWidget", 0));
        groupBox->setTitle(QApplication::translate("LightParameterWidget", "Light Parameter", 0));
        label->setText(QApplication::translate("LightParameterWidget", "Ambient", 0));
        label_3->setText(QApplication::translate("LightParameterWidget", "Shininess", 0));
        label_2->setText(QApplication::translate("LightParameterWidget", "Specular", 0));
        label_4->setText(QApplication::translate("LightParameterWidget", "Diffuse", 0));
        label_5->setText(QApplication::translate("LightParameterWidget", "Light Pos", 0));
        groupBox_2->setTitle(QApplication::translate("LightParameterWidget", "Rendering Parameter", 0));
        cb_shader->clear();
        cb_shader->insertItems(0, QStringList()
         << QApplication::translate("LightParameterWidget", "Original Volume", 0)
         << QApplication::translate("LightParameterWidget", "Cluster Volume", 0)
         << QApplication::translate("LightParameterWidget", "Tri-Interpolate", 0)
         << QApplication::translate("LightParameterWidget", "OrigVol Shading", 0)
         << QApplication::translate("LightParameterWidget", "GMTF Shading", 0)
        );
        label_7->setText(QApplication::translate("LightParameterWidget", "Gradient", 0));
        label_6->setText(QApplication::translate("LightParameterWidget", "Shader", 0));
        cb_gradient->clear();
        cb_gradient->insertItems(0, QStringList()
         << QApplication::translate("LightParameterWidget", "RCMG", 0)
         << QApplication::translate("LightParameterWidget", "CIELUV CD", 0)
         << QApplication::translate("LightParameterWidget", "Intensity CD", 0)
         << QApplication::translate("LightParameterWidget", "Color Distance", 0)
         << QApplication::translate("LightParameterWidget", "Sobel", 0)
        );
    } // retranslateUi

};

namespace Ui {
    class LightParameterWidget: public Ui_LightParameterWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIGHTPARAMETERWIDGET_H
