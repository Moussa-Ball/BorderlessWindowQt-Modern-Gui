/********************************************************************************
** Form generated from reading UI file 'WinHost.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINHOST_H
#define UI_WINHOST_H

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

class Ui_WinHost
{
public:
    QWidget *centralWidget;
    QToolBar *toolBar;
    QToolBar *toolBarParams;
    QToolBar *toolBox;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *WinHost)
    {
        if (WinHost->objectName().isEmpty())
            WinHost->setObjectName(QStringLiteral("WinHost"));
        WinHost->resize(713, 441);
        centralWidget = new QWidget(WinHost);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        WinHost->setCentralWidget(centralWidget);
        toolBar = new QToolBar(WinHost);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        toolBar->setMovable(false);
        toolBar->setAllowedAreas(Qt::TopToolBarArea);
        toolBar->setFloatable(false);
        WinHost->addToolBar(Qt::TopToolBarArea, toolBar);
        toolBarParams = new QToolBar(WinHost);
        toolBarParams->setObjectName(QStringLiteral("toolBarParams"));
        toolBarParams->setMovable(false);
        toolBarParams->setAllowedAreas(Qt::TopToolBarArea);
        toolBarParams->setFloatable(false);
        WinHost->addToolBar(Qt::TopToolBarArea, toolBarParams);
        WinHost->insertToolBarBreak(toolBarParams);
        toolBox = new QToolBar(WinHost);
        toolBox->setObjectName(QStringLiteral("toolBox"));
        toolBox->setMovable(false);
        toolBox->setAllowedAreas(Qt::LeftToolBarArea);
        toolBox->setFloatable(false);
        WinHost->addToolBar(Qt::LeftToolBarArea, toolBox);
        statusBar = new QStatusBar(WinHost);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        statusBar->setSizeGripEnabled(false);
        WinHost->setStatusBar(statusBar);

        retranslateUi(WinHost);

        QMetaObject::connectSlotsByName(WinHost);
    } // setupUi

    void retranslateUi(QMainWindow *WinHost)
    {
        WinHost->setWindowTitle(QApplication::translate("WinHost", "WinHost", Q_NULLPTR));
        toolBar->setWindowTitle(QApplication::translate("WinHost", "toolBar", Q_NULLPTR));
        toolBarParams->setWindowTitle(QApplication::translate("WinHost", "toolBar_2", Q_NULLPTR));
        toolBox->setWindowTitle(QApplication::translate("WinHost", "toolBar_3", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class WinHost: public Ui_WinHost {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINHOST_H
