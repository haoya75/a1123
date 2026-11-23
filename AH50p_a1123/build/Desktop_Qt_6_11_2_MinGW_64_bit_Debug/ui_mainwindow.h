/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSaveAs;
    QAction *actionExit;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionReset;
    QAction *actionSettings;
    QWidget *centralwidget;
    QHBoxLayout *centralLayout;
    QWidget *workspaceWidget;
    QVBoxLayout *workspaceContainerLayout;
    QScrollArea *imageScrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *workspaceLayout;
    QLabel *imageLabel;
    QWidget *zoomBarWidget;
    QHBoxLayout *zoomBarLayout;
    QSpacerItem *zoomHorizontalSpacer;
    QLabel *zoomTitleLabel;
    QSlider *zoomSlider;
    QLabel *zoomPercentLabel;
    QPushButton *resetZoomButton;
    QWidget *sidebarToggleRail;
    QVBoxLayout *sidebarToggleLayout;
    QPushButton *sidebarToggleButton;
    QSpacerItem *sidebarToggleSpacer;
    QFrame *rightSidebarWidget;
    QVBoxLayout *rightSidebarLayout;
    QLabel *navigatorPreviewLabel;
    QLabel *sidebarTitleLabel;
    QLabel *sidebarPlaceholderLabel;
    QSpacerItem *sidebarBottomSpacer;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuImage;
    QStatusBar *statusbar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1024, 720);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName("actionOpen");
        QIcon icon(QIcon::fromTheme(QString::fromUtf8("document-open")));
        actionOpen->setIcon(icon);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName("actionSave");
        QIcon icon1(QIcon::fromTheme(QString::fromUtf8("document-save")));
        actionSave->setIcon(icon1);
        actionSaveAs = new QAction(MainWindow);
        actionSaveAs->setObjectName("actionSaveAs");
        QIcon icon2(QIcon::fromTheme(QString::fromUtf8("document-save-as")));
        actionSaveAs->setIcon(icon2);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName("actionExit");
        QIcon icon3(QIcon::fromTheme(QString::fromUtf8("application-exit")));
        actionExit->setIcon(icon3);
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName("actionUndo");
        QIcon icon4(QIcon::fromTheme(QString::fromUtf8("edit-undo")));
        actionUndo->setIcon(icon4);
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName("actionRedo");
        QIcon icon5(QIcon::fromTheme(QString::fromUtf8("edit-redo")));
        actionRedo->setIcon(icon5);
        actionReset = new QAction(MainWindow);
        actionReset->setObjectName("actionReset");
        QIcon icon6(QIcon::fromTheme(QString::fromUtf8("view-refresh")));
        actionReset->setIcon(icon6);
        actionSettings = new QAction(MainWindow);
        actionSettings->setObjectName("actionSettings");
        QIcon icon7(QIcon::fromTheme(QString::fromUtf8("preferences-system")));
        actionSettings->setIcon(icon7);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralLayout = new QHBoxLayout(centralwidget);
        centralLayout->setSpacing(0);
        centralLayout->setObjectName("centralLayout");
        centralLayout->setContentsMargins(0, 0, 0, 0);
        workspaceWidget = new QWidget(centralwidget);
        workspaceWidget->setObjectName("workspaceWidget");
        workspaceContainerLayout = new QVBoxLayout(workspaceWidget);
        workspaceContainerLayout->setSpacing(0);
        workspaceContainerLayout->setObjectName("workspaceContainerLayout");
        workspaceContainerLayout->setContentsMargins(0, 0, 0, 0);
        imageScrollArea = new QScrollArea(workspaceWidget);
        imageScrollArea->setObjectName("imageScrollArea");
        imageScrollArea->setWidgetResizable(true);
        imageScrollArea->setAlignment(Qt::AlignCenter);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 994, 578));
        workspaceLayout = new QVBoxLayout(scrollAreaWidgetContents);
        workspaceLayout->setObjectName("workspaceLayout");
        imageLabel = new QLabel(scrollAreaWidgetContents);
        imageLabel->setObjectName("imageLabel");
        imageLabel->setStyleSheet(QString::fromUtf8("background-color: #2b2b2b; color: #aaaaaa; font-size: 15px;"));
        imageLabel->setAlignment(Qt::AlignCenter);

        workspaceLayout->addWidget(imageLabel);

        imageScrollArea->setWidget(scrollAreaWidgetContents);

        workspaceContainerLayout->addWidget(imageScrollArea);

        zoomBarWidget = new QWidget(workspaceWidget);
        zoomBarWidget->setObjectName("zoomBarWidget");
        zoomBarLayout = new QHBoxLayout(zoomBarWidget);
        zoomBarLayout->setSpacing(8);
        zoomBarLayout->setObjectName("zoomBarLayout");
        zoomBarLayout->setContentsMargins(8, 4, 12, 4);
        zoomHorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        zoomBarLayout->addItem(zoomHorizontalSpacer);

        zoomTitleLabel = new QLabel(zoomBarWidget);
        zoomTitleLabel->setObjectName("zoomTitleLabel");

        zoomBarLayout->addWidget(zoomTitleLabel);

        zoomSlider = new QSlider(zoomBarWidget);
        zoomSlider->setObjectName("zoomSlider");
        zoomSlider->setMinimumSize(QSize(150, 0));
        zoomSlider->setMaximumSize(QSize(180, 16777215));
        zoomSlider->setMinimum(10);
        zoomSlider->setMaximum(400);
        zoomSlider->setSingleStep(10);
        zoomSlider->setPageStep(25);
        zoomSlider->setValue(100);
        zoomSlider->setOrientation(Qt::Horizontal);

        zoomBarLayout->addWidget(zoomSlider);

        zoomPercentLabel = new QLabel(zoomBarWidget);
        zoomPercentLabel->setObjectName("zoomPercentLabel");
        zoomPercentLabel->setMinimumSize(QSize(48, 0));
        zoomPercentLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        zoomBarLayout->addWidget(zoomPercentLabel);

        resetZoomButton = new QPushButton(zoomBarWidget);
        resetZoomButton->setObjectName("resetZoomButton");

        zoomBarLayout->addWidget(resetZoomButton);


        workspaceContainerLayout->addWidget(zoomBarWidget);


        centralLayout->addWidget(workspaceWidget);

        sidebarToggleRail = new QWidget(centralwidget);
        sidebarToggleRail->setObjectName("sidebarToggleRail");
        sidebarToggleRail->setMinimumSize(QSize(28, 0));
        sidebarToggleRail->setMaximumSize(QSize(28, 16777215));
        sidebarToggleRail->setStyleSheet(QString::fromUtf8("background-color: #252526; border-left: 1px solid #3c3c3c;"));
        sidebarToggleLayout = new QVBoxLayout(sidebarToggleRail);
        sidebarToggleLayout->setSpacing(0);
        sidebarToggleLayout->setObjectName("sidebarToggleLayout");
        sidebarToggleLayout->setContentsMargins(2, 6, 2, 6);
        sidebarToggleButton = new QPushButton(sidebarToggleRail);
        sidebarToggleButton->setObjectName("sidebarToggleButton");
        sidebarToggleButton->setMinimumSize(QSize(24, 32));
        sidebarToggleButton->setMaximumSize(QSize(24, 32));
        sidebarToggleButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #3a3a3d;\n"
"    color: #ffffff;\n"
"    border: none;\n"
"    border-radius: 4px;\n"
"    font-weight: bold;\n"
"    font-size: 14px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #505054;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #2c2c2e;\n"
"}"));

        sidebarToggleLayout->addWidget(sidebarToggleButton);

        sidebarToggleSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        sidebarToggleLayout->addItem(sidebarToggleSpacer);


        centralLayout->addWidget(sidebarToggleRail);

        rightSidebarWidget = new QFrame(centralwidget);
        rightSidebarWidget->setObjectName("rightSidebarWidget");
        rightSidebarWidget->setMinimumSize(QSize(0, 0));
        rightSidebarWidget->setMaximumSize(QSize(0, 16777215));
        rightSidebarWidget->setStyleSheet(QString::fromUtf8("QFrame#rightSidebarWidget {\n"
"    background-color: #252526;\n"
"    border-left: 1px solid #3c3c3c;\n"
"    color: #e0e0e0;\n"
"}"));
        rightSidebarWidget->setFrameShape(QFrame::StyledPanel);
        rightSidebarLayout = new QVBoxLayout(rightSidebarWidget);
        rightSidebarLayout->setSpacing(10);
        rightSidebarLayout->setObjectName("rightSidebarLayout");
        rightSidebarLayout->setContentsMargins(10, 10, 10, 10);
        navigatorPreviewLabel = new QLabel(rightSidebarWidget);
        navigatorPreviewLabel->setObjectName("navigatorPreviewLabel");
        navigatorPreviewLabel->setMinimumSize(QSize(0, 150));
        navigatorPreviewLabel->setMaximumSize(QSize(16777215, 180));
        navigatorPreviewLabel->setStyleSheet(QString::fromUtf8("QLabel#navigatorPreviewLabel {\n"
"    background-color: #1e1e1e;\n"
"    border: 1px solid #3c3c3c;\n"
"    border-radius: 4px;\n"
"    color: #888888;\n"
"    font-size: 12px;\n"
"}"));
        navigatorPreviewLabel->setAlignment(Qt::AlignCenter);

        rightSidebarLayout->addWidget(navigatorPreviewLabel);

        sidebarTitleLabel = new QLabel(rightSidebarWidget);
        sidebarTitleLabel->setObjectName("sidebarTitleLabel");
        sidebarTitleLabel->setStyleSheet(QString::fromUtf8("font-size: 14px; font-weight: bold; color: #ffffff;"));

        rightSidebarLayout->addWidget(sidebarTitleLabel);

        sidebarPlaceholderLabel = new QLabel(rightSidebarWidget);
        sidebarPlaceholderLabel->setObjectName("sidebarPlaceholderLabel");
        sidebarPlaceholderLabel->setStyleSheet(QString::fromUtf8("color: #aaaaaa;"));
        sidebarPlaceholderLabel->setWordWrap(true);

        rightSidebarLayout->addWidget(sidebarPlaceholderLabel);

        sidebarBottomSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        rightSidebarLayout->addItem(sidebarBottomSpacer);


        centralLayout->addWidget(rightSidebarWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1024, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName("menuEdit");
        menuImage = new QMenu(menubar);
        menuImage->setObjectName("menuImage");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName("mainToolBar");
        mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuImage->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSaveAs);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuImage->addAction(actionReset);
        mainToolBar->addAction(actionOpen);
        mainToolBar->addAction(actionSave);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionUndo);
        mainToolBar->addAction(actionRedo);

        retranslateUi(MainWindow);
        QObject::connect(actionExit, &QAction::triggered, MainWindow, qOverload<>(&QMainWindow::close));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "AI-Assisted Image Workspace", nullptr));
        actionOpen->setText(QCoreApplication::translate("MainWindow", "&Open...", nullptr));
#if QT_CONFIG(statustip)
        actionOpen->setStatusTip(QCoreApplication::translate("MainWindow", "\351\226\213\345\225\237\345\275\261\345\203\217\346\252\224\346\241\210", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(shortcut)
        actionOpen->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSave->setText(QCoreApplication::translate("MainWindow", "&Save", nullptr));
#if QT_CONFIG(statustip)
        actionSave->setStatusTip(QCoreApplication::translate("MainWindow", "\345\204\262\345\255\230\347\233\256\345\211\215\345\275\261\345\203\217", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(shortcut)
        actionSave->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSaveAs->setText(QCoreApplication::translate("MainWindow", "Save &As...", nullptr));
#if QT_CONFIG(statustip)
        actionSaveAs->setStatusTip(QCoreApplication::translate("MainWindow", "\345\217\246\345\255\230\346\226\260\346\252\224", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(shortcut)
        actionSaveAs->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExit->setText(QCoreApplication::translate("MainWindow", "E&xit", nullptr));
#if QT_CONFIG(statustip)
        actionExit->setStatusTip(QCoreApplication::translate("MainWindow", "\351\227\234\351\226\211\346\207\211\347\224\250\347\250\213\345\274\217", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(shortcut)
        actionExit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionUndo->setText(QCoreApplication::translate("MainWindow", "&Undo", nullptr));
#if QT_CONFIG(statustip)
        actionUndo->setStatusTip(QCoreApplication::translate("MainWindow", "\345\276\251\345\216\237\344\270\212\344\270\200\346\255\245\346\223\215\344\275\234", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(shortcut)
        actionUndo->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Z", nullptr));
#endif // QT_CONFIG(shortcut)
        actionRedo->setText(QCoreApplication::translate("MainWindow", "&Redo", nullptr));
#if QT_CONFIG(statustip)
        actionRedo->setStatusTip(QCoreApplication::translate("MainWindow", "\351\207\215\345\201\232\346\223\215\344\275\234", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(shortcut)
        actionRedo->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Y", nullptr));
#endif // QT_CONFIG(shortcut)
        actionReset->setText(QCoreApplication::translate("MainWindow", "&Reset Image", nullptr));
#if QT_CONFIG(statustip)
        actionReset->setStatusTip(QCoreApplication::translate("MainWindow", "\351\202\204\345\216\237\347\202\272\345\216\237\345\247\213\345\275\261\345\203\217", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(shortcut)
        actionReset->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+R", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSettings->setText(QCoreApplication::translate("MainWindow", "\350\250\255\345\256\232", nullptr));
#if QT_CONFIG(tooltip)
        actionSettings->setToolTip(QCoreApplication::translate("MainWindow", "\346\207\211\347\224\250\347\250\213\345\274\217\350\250\255\345\256\232", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(statustip)
        actionSettings->setStatusTip(QCoreApplication::translate("MainWindow", "\351\226\213\345\225\237\346\207\211\347\224\250\347\250\213\345\274\217\350\250\255\345\256\232\350\246\226\347\252\227", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(shortcut)
        actionSettings->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+,", nullptr));
#endif // QT_CONFIG(shortcut)
        imageLabel->setText(QCoreApplication::translate("MainWindow", "\345\275\261\345\203\217\345\267\245\344\275\234\345\215\200\357\274\210\350\253\213\351\200\217\351\201\216 File -> Open \351\226\213\345\225\237\345\275\261\345\203\217\357\274\211", nullptr));
        zoomTitleLabel->setText(QCoreApplication::translate("MainWindow", "\347\270\256\346\224\276\357\274\232", nullptr));
#if QT_CONFIG(tooltip)
        zoomSlider->setToolTip(QCoreApplication::translate("MainWindow", "\350\252\277\346\225\264\345\275\261\345\203\217\347\270\256\346\224\276\346\257\224\344\276\213 (10% - 400%)", nullptr));
#endif // QT_CONFIG(tooltip)
        zoomPercentLabel->setText(QCoreApplication::translate("MainWindow", "100%", nullptr));
#if QT_CONFIG(tooltip)
        resetZoomButton->setToolTip(QCoreApplication::translate("MainWindow", "\345\260\207\347\270\256\346\224\276\346\257\224\344\276\213\351\207\215\347\275\256\347\202\272 100%", nullptr));
#endif // QT_CONFIG(tooltip)
        resetZoomButton->setText(QCoreApplication::translate("MainWindow", "\351\207\215\350\243\275\347\270\256\346\224\276\346\257\224", nullptr));
#if QT_CONFIG(tooltip)
        sidebarToggleButton->setToolTip(QCoreApplication::translate("MainWindow", "\351\226\213\345\225\237\345\201\264\346\254\204", nullptr));
#endif // QT_CONFIG(tooltip)
        sidebarToggleButton->setText(QCoreApplication::translate("MainWindow", "<", nullptr));
        navigatorPreviewLabel->setText(QCoreApplication::translate("MainWindow", "\345\260\232\347\204\241\345\275\261\345\203\217", nullptr));
        sidebarTitleLabel->setText(QCoreApplication::translate("MainWindow", "\345\267\245\345\205\267\345\201\264\346\254\204", nullptr));
        sidebarPlaceholderLabel->setText(QCoreApplication::translate("MainWindow", "\346\255\244\350\231\225\345\217\257\346\224\276\347\275\256\345\275\261\345\203\217\350\252\277\346\225\264\345\217\203\346\225\270\346\210\226 AI \345\212\237\350\203\275\351\235\242\346\235\277\343\200\202", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "&File", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "&Edit", nullptr));
        menuImage->setTitle(QCoreApplication::translate("MainWindow", "&Image", nullptr));
        mainToolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "Main Toolbar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
