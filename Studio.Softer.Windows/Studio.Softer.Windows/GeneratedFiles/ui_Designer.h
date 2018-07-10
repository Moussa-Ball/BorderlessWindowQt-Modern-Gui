/********************************************************************************
** Form generated from reading UI file 'Designer.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DESIGNER_H
#define UI_DESIGNER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Designer
{
public:
    QWidget *centralWidget;
    QToolBar *toolBar;
    QToolBar *toolBarParams;
    QToolBar *toolBox;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Designer)
    {
        if (Designer->objectName().isEmpty())
            Designer->setObjectName(QStringLiteral("Designer"));
        Designer->resize(716, 447);
        centralWidget = new QWidget(Designer);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        Designer->setCentralWidget(centralWidget);
        toolBar = new QToolBar(Designer);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        toolBar->setMovable(false);
        toolBar->setAllowedAreas(Qt::TopToolBarArea);
        toolBar->setFloatable(false);
        Designer->addToolBar(Qt::TopToolBarArea, toolBar);
        toolBarParams = new QToolBar(Designer);
        toolBarParams->setObjectName(QStringLiteral("toolBarParams"));
        toolBarParams->setMovable(false);
        toolBarParams->setAllowedAreas(Qt::TopToolBarArea);
        toolBarParams->setFloatable(false);
        Designer->addToolBar(Qt::TopToolBarArea, toolBarParams);
        Designer->insertToolBarBreak(toolBarParams);
        toolBox = new QToolBar(Designer);
        toolBox->setObjectName(QStringLiteral("toolBox"));
        toolBox->setMovable(false);
        toolBox->setAllowedAreas(Qt::LeftToolBarArea);
        toolBox->setFloatable(false);
        Designer->addToolBar(Qt::LeftToolBarArea, toolBox);
        statusBar = new QStatusBar(Designer);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        statusBar->setSizeGripEnabled(false);
        Designer->setStatusBar(statusBar);

        retranslateUi(Designer);

        QMetaObject::connectSlotsByName(Designer);
    } // setupUi

    void retranslateUi(QMainWindow *Designer)
    {
        Designer->setWindowTitle(QApplication::translate("Designer", "Designer", Q_NULLPTR));
        toolBar->setWindowTitle(QApplication::translate("Designer", "toolBar", Q_NULLPTR));
        toolBarParams->setWindowTitle(QApplication::translate("Designer", "toolBar_2", Q_NULLPTR));
        toolBox->setWindowTitle(QApplication::translate("Designer", "toolBar_2", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Designer: public Ui_Designer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DESIGNER_H
