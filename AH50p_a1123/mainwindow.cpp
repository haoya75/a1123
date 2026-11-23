#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAbstractAnimation>
#include <QColor>
#include <QDir>
#include <QEasingCurve>
#include <QFileDialog>
#include <QFileInfo>
#include <QMargins>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPalette>
#include <QPen>
#include <QPushButton>
#include <QScrollBar>
#include <QSettings>
#include <QSignalBlocker>
#include <QStyle>
#include <QTimer>
#include <QWheelEvent>
#include <QtGlobal>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 監聽影像工作區尺寸變化、Ctrl + 滑鼠滾輪縮放，以及側欄預覽區互動
    ui->imageScrollArea->installEventFilter(this);
    ui->imageScrollArea->viewport()->installEventFilter(this);
    ui->navigatorPreviewLabel->installEventFilter(this);
    ui->navigatorPreviewLabel->setMouseTracking(true);

    setupFallbackIcons();
    setupSettingsCornerButton();
    setupSidebarAnimation();
    setupConnections();
    loadSavedLanguage();
    resetZoom();
    updateActionStates();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections()
{
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpenImage);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSaveImage);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::onSaveImageAs);
    connect(ui->actionReset, &QAction::triggered, this, &MainWindow::onResetImage);
    connect(ui->actionUndo, &QAction::triggered, this, &MainWindow::onUndo);
    connect(ui->actionRedo, &QAction::triggered, this, &MainWindow::onRedo);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::onOpenSettings);
    connect(ui->zoomSlider, &QSlider::valueChanged, this, &MainWindow::onZoomSliderChanged);
    connect(ui->resetZoomButton, &QPushButton::clicked, this, &MainWindow::onResetZoom);
    connect(ui->sidebarToggleButton, &QPushButton::clicked, this, &MainWindow::onToggleSidebar);

    // 捲軸移動或範圍改變時，即時同步更新右側預覽紅框
    connect(ui->imageScrollArea->horizontalScrollBar(), &QScrollBar::valueChanged, this, [this]() {
        scheduleNavigatorUpdate(!m_isDraggingNavigator);
    });
    connect(ui->imageScrollArea->verticalScrollBar(), &QScrollBar::valueChanged, this, [this]() {
        scheduleNavigatorUpdate(!m_isDraggingNavigator);
    });
    connect(ui->imageScrollArea->horizontalScrollBar(), &QScrollBar::rangeChanged, this, [this]() {
        scheduleNavigatorUpdate(true);
    });
    connect(ui->imageScrollArea->verticalScrollBar(), &QScrollBar::rangeChanged, this, [this]() {
        scheduleNavigatorUpdate(true);
    });

    // actionExit 已於 mainwindow.ui 連接至 MainWindow::close()
}

void MainWindow::setupFallbackIcons()
{
    if (ui->actionOpen->icon().isNull()) {
        ui->actionOpen->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    }
    if (ui->actionSave->icon().isNull()) {
        ui->actionSave->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    }
    if (ui->actionSaveAs->icon().isNull()) {
        ui->actionSaveAs->setIcon(style()->standardIcon(QStyle::SP_DriveFDIcon));
    }
    if (ui->actionExit->icon().isNull()) {
        ui->actionExit->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    }
    if (ui->actionUndo->icon().isNull()) {
        ui->actionUndo->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    }
    if (ui->actionRedo->icon().isNull()) {
        ui->actionRedo->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    }
    if (ui->actionReset->icon().isNull()) {
        ui->actionReset->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    }
    if (ui->actionSettings->icon().isNull()) {
        ui->actionSettings->setIcon(createGearIcon());
    }
}

QIcon MainWindow::createGearIcon() const
{
    QPixmap pixmap(32, 32);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(palette().color(QPalette::WindowText));
    painter.translate(16.0, 16.0);

    for (int i = 0; i < 8; ++i) {
        painter.drawRoundedRect(QRectF(-2.6, -13.5, 5.2, 6.0), 1.2, 1.2);
        painter.rotate(45.0);
    }

    painter.drawEllipse(QPointF(0.0, 0.0), 9.2, 9.2);
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.drawEllipse(QPointF(0.0, 0.0), 4.2, 4.2);

    return QIcon(pixmap);
}

void MainWindow::setupSettingsCornerButton()
{
    addAction(ui->actionSettings);

    m_settingsButton = new QToolButton(ui->menubar);
    m_settingsButton->setDefaultAction(ui->actionSettings);
    m_settingsButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_settingsButton->setAutoRaise(true);
    m_settingsButton->setIconSize(QSize(16, 16));
    m_settingsButton->setFixedWidth(28);

    ui->menubar->setCornerWidget(m_settingsButton, Qt::TopRightCorner);
}

void MainWindow::setupSidebarAnimation()
{
    ui->rightSidebarWidget->setFixedWidth(0);
    ui->rightSidebarWidget->setVisible(false);
    ui->sidebarToggleButton->setText(QStringLiteral("<"));

    m_sidebarAnimation = new QVariantAnimation(this);
    m_sidebarAnimation->setDuration(220);
    m_sidebarAnimation->setEasingCurve(QEasingCurve::InOutCubic);

    connect(m_sidebarAnimation, &QVariantAnimation::valueChanged, this, [this](const QVariant &value) {
        ui->rightSidebarWidget->setFixedWidth(value.toInt());
    });

    connect(m_sidebarAnimation, &QVariantAnimation::finished, this, [this]() {
        if (m_isSidebarOpen) {
            ui->sidebarToggleButton->setText(QStringLiteral(">"));
            ui->sidebarToggleButton->setToolTip(localizedText(QStringLiteral("關閉側欄"), QStringLiteral("Close Sidebar")));
        } else {
            ui->rightSidebarWidget->setVisible(false);
            ui->sidebarToggleButton->setText(QStringLiteral("<"));
            ui->sidebarToggleButton->setToolTip(localizedText(QStringLiteral("開啟側欄"), QStringLiteral("Open Sidebar")));
        }

        if (!m_currentPixmap.isNull()) {
            m_baseScale = calculateFitScale();
            updateImageDisplay(true);
        }
    });
}

void MainWindow::loadSavedLanguage()
{
    QSettings settings(QStringLiteral("AH50p"), QStringLiteral("AH50p_a1123"));
    const int saved = settings.value(QStringLiteral("language"), 0).toInt();
    const auto language = (saved == 1)
                              ? SettingsDialog::Language::English
                              : SettingsDialog::Language::TraditionalChinese;
    applyLanguage(language);
}

void MainWindow::applyLanguage(SettingsDialog::Language language)
{
    m_currentLanguage = language;

    QSettings settings(QStringLiteral("AH50p"), QStringLiteral("AH50p_a1123"));
    settings.setValue(QStringLiteral("language"), static_cast<int>(language));

    // MenuBar
    ui->menuFile->setTitle(localizedText(QStringLiteral("檔案(&F)"), QStringLiteral("&File")));
    ui->menuEdit->setTitle(localizedText(QStringLiteral("編輯(&E)"), QStringLiteral("&Edit")));
    ui->menuImage->setTitle(localizedText(QStringLiteral("影像(&I)"), QStringLiteral("&Image")));

    // Actions
    ui->actionOpen->setText(localizedText(QStringLiteral("開啟(&O)..."), QStringLiteral("&Open...")));
    ui->actionOpen->setStatusTip(localizedText(QStringLiteral("開啟影像檔案"), QStringLiteral("Open an image file")));

    ui->actionSave->setText(localizedText(QStringLiteral("儲存(&S)"), QStringLiteral("&Save")));
    ui->actionSave->setStatusTip(localizedText(QStringLiteral("儲存目前影像"), QStringLiteral("Save current image")));

    ui->actionSaveAs->setText(localizedText(QStringLiteral("另存新檔(&A)..."), QStringLiteral("Save &As...")));
    ui->actionSaveAs->setStatusTip(localizedText(QStringLiteral("另存影像檔案"), QStringLiteral("Save image as a new file")));

    ui->actionExit->setText(localizedText(QStringLiteral("結束(&X)"), QStringLiteral("E&xit")));
    ui->actionExit->setStatusTip(localizedText(QStringLiteral("關閉應用程式"), QStringLiteral("Exit the application")));

    ui->actionUndo->setText(localizedText(QStringLiteral("復原(&U)"), QStringLiteral("&Undo")));
    ui->actionUndo->setStatusTip(localizedText(QStringLiteral("復原上一步操作"), QStringLiteral("Undo last action")));

    ui->actionRedo->setText(localizedText(QStringLiteral("重做(&R)"), QStringLiteral("&Redo")));
    ui->actionRedo->setStatusTip(localizedText(QStringLiteral("重做操作"), QStringLiteral("Redo last undone action")));

    ui->actionReset->setText(localizedText(QStringLiteral("還原影像(&R)"), QStringLiteral("&Reset Image")));
    ui->actionReset->setStatusTip(localizedText(QStringLiteral("還原為原始影像"), QStringLiteral("Restore original image")));

    ui->actionSettings->setText(localizedText(QStringLiteral("設定"), QStringLiteral("Settings")));
    ui->actionSettings->setToolTip(localizedText(QStringLiteral("設定"), QStringLiteral("Settings")));
    ui->actionSettings->setStatusTip(localizedText(QStringLiteral("開啟應用程式設定視窗"), QStringLiteral("Open application settings")));

    // ToolBar
    ui->mainToolBar->setWindowTitle(localizedText(QStringLiteral("主工具列"), QStringLiteral("Main Toolbar")));

    // 縮放控制列
    ui->zoomTitleLabel->setText(localizedText(QStringLiteral("縮放："), QStringLiteral("Zoom:")));
    ui->zoomSlider->setToolTip(localizedText(
        QStringLiteral("調整影像縮放比例 (10% - 400%)"),
        QStringLiteral("Adjust image zoom level (10% - 400%)")));
    ui->resetZoomButton->setText(localizedText(QStringLiteral("重製縮放比"), QStringLiteral("Reset Zoom")));
    ui->resetZoomButton->setToolTip(localizedText(
        QStringLiteral("將縮放比例重置為 100%"),
        QStringLiteral("Reset zoom level to 100%")));

    // 側欄與按鍵提示
    ui->sidebarTitleLabel->setText(localizedText(QStringLiteral("工具側欄"), QStringLiteral("Tool Sidebar")));
    ui->sidebarPlaceholderLabel->setText(localizedText(
        QStringLiteral("此處可放置影像調整參數或 AI 功能面板。"),
        QStringLiteral("Place image adjustment controls or AI tools here.")));
    ui->sidebarToggleButton->setToolTip(
        m_isSidebarOpen
            ? localizedText(QStringLiteral("關閉側欄"), QStringLiteral("Close Sidebar"))
            : localizedText(QStringLiteral("開啟側欄"), QStringLiteral("Open Sidebar")));

    if (m_currentPixmap.isNull()) {
        ui->imageLabel->setText(localizedText(
            QStringLiteral("影像工作區（請透過 檔案 -> 開啟 開啟影像）"),
            QStringLiteral("Image Workspace (Open an image via File -> Open)")));
        ui->navigatorPreviewLabel->setPixmap(QPixmap());
        ui->navigatorPreviewLabel->setText(localizedText(
            QStringLiteral("尚無影像"),
            QStringLiteral("No Image")));
    }

    updateWindowTitle();
}

void MainWindow::updateWindowTitle()
{
    const QString appTitle = localizedText(
        QStringLiteral("AI 輔助影像工作區"),
        QStringLiteral("AI-Assisted Image Workspace"));

    if (m_currentFilePath.isEmpty()) {
        setWindowTitle(appTitle);
    } else {
        setWindowTitle(QStringLiteral("%1 - %2").arg(QFileInfo(m_currentFilePath).fileName(), appTitle));
    }
}

QString MainWindow::localizedText(const QString &zh, const QString &en) const
{
    return (m_currentLanguage == SettingsDialog::Language::TraditionalChinese) ? zh : en;
}

void MainWindow::onOpenSettings()
{
    SettingsDialog dialog(m_currentLanguage, this);
    if (dialog.exec() == QDialog::Accepted) {
        applyLanguage(dialog.selectedLanguage());
        ui->statusbar->showMessage(
            localizedText(QStringLiteral("已套用設定"), QStringLiteral("Settings applied")),
            3000);
    }
}

int MainWindow::targetSidebarWidth() const
{
    const int fifthWidth = ui->centralwidget->width() / 5;
    const int railWidth = ui->sidebarToggleRail->width();
    return qMax(0, fifthWidth - railWidth);
}

double MainWindow::calculateFitScale() const
{
    if (m_currentPixmap.isNull() || m_currentPixmap.width() <= 0 || m_currentPixmap.height() <= 0) {
        return 1.0;
    }

    const QSize areaSize = ui->imageScrollArea->contentsRect().size();
    const QMargins margins = ui->workspaceLayout->contentsMargins();

    const int availableWidth = qMax(1, areaSize.width() - margins.left() - margins.right() - 4);
    const int availableHeight = qMax(1, areaSize.height() - margins.top() - margins.bottom() - 4);

    const double scaleX = static_cast<double>(availableWidth) / m_currentPixmap.width();
    const double scaleY = static_cast<double>(availableHeight) / m_currentPixmap.height();
    return qMin(scaleX, scaleY);
}

bool MainWindow::canScrollWorkspace() const
{
    return ui->imageScrollArea->horizontalScrollBar()->maximum() > 0 ||
           ui->imageScrollArea->verticalScrollBar()->maximum() > 0;
}

void MainWindow::onToggleSidebar()
{
    m_isSidebarOpen = !m_isSidebarOpen;

    const int currentWidth = ui->rightSidebarWidget->isVisible()
                                 ? ui->rightSidebarWidget->width()
                                 : 0;
    const int endWidth = m_isSidebarOpen ? targetSidebarWidth() : 0;

    if (m_isSidebarOpen) {
        ui->rightSidebarWidget->setFixedWidth(currentWidth);
        ui->rightSidebarWidget->setVisible(true);
    }

    m_sidebarAnimation->stop();
    m_sidebarAnimation->setStartValue(currentWidth);
    m_sidebarAnimation->setEndValue(endWidth);
    m_sidebarAnimation->start();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    if (m_isSidebarOpen && m_sidebarAnimation &&
        m_sidebarAnimation->state() != QAbstractAnimation::Running) {
        ui->rightSidebarWidget->setFixedWidth(targetSidebarWidth());
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    // 1. 影像工作區尺寸變動時，動態重算 100% 最佳比例並更新預覽紅框
    if (watched == ui->imageScrollArea && event->type() == QEvent::Resize) {
        if (!m_currentPixmap.isNull()) {
            m_baseScale = calculateFitScale();
            const bool isAnimating = m_sidebarAnimation &&
                                     m_sidebarAnimation->state() == QAbstractAnimation::Running;
            updateImageDisplay(!isAnimating);
        }
        return false;
    }

    // 2. 右側預覽縮圖：尺寸變動重繪 + 滑鼠點擊／拖曳紅框同步移動主工作區捲軸
    if (watched == ui->navigatorPreviewLabel) {
        if (event->type() == QEvent::Resize) {
            if (!m_currentPixmap.isNull()) {
                const bool isAnimating = m_sidebarAnimation &&
                                         m_sidebarAnimation->state() == QAbstractAnimation::Running;
                scheduleNavigatorUpdate(!isAnimating);
            }
            return false;
        }

        if (event->type() == QEvent::Leave) {
            if (!m_isDraggingNavigator) {
                ui->navigatorPreviewLabel->unsetCursor();
            }
            return false;
        }

        if (event->type() == QEvent::MouseButtonPress ||
            event->type() == QEvent::MouseMove ||
            event->type() == QEvent::MouseButtonRelease) {
            auto *mouseEvent = static_cast<QMouseEvent *>(event);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            const QPointF localPos = mouseEvent->position();
#else
            const QPointF localPos = mouseEvent->localPos();
#endif
            const QPointF canvasPos = localPos - QPointF(ui->navigatorPreviewLabel->contentsRect().topLeft());

            if (event->type() == QEvent::MouseButtonPress && mouseEvent->button() == Qt::LeftButton) {
                if (!m_navigatorSourcePixmap.isNull() && m_navScale > 0.0 && canScrollWorkspace()) {
                    QScrollBar *hBar = ui->imageScrollArea->horizontalScrollBar();
                    QScrollBar *vBar = ui->imageScrollArea->verticalScrollBar();

                    // 若點在紅框外，先將主工作區檢視中心跳轉至點擊位置，再無縫接續拖曳
                    if (!m_navMappedViewportRect.adjusted(-4.0, -4.0, 4.0, 4.0).contains(canvasPos)) {
                        const double contentX = m_navUnionRect.left() + (canvasPos.x() - m_navOrigin.x()) / m_navScale;
                        const double contentY = m_navUnionRect.top() + (canvasPos.y() - m_navOrigin.y()) / m_navScale;

                        const int targetH = hBar->value() + qRound(contentX - m_navViewportContentRect.center().x());
                        const int targetV = vBar->value() + qRound(contentY - m_navViewportContentRect.center().y());
                        hBar->setValue(targetH);
                        vBar->setValue(targetV);
                    }

                    m_isDraggingNavigator = true;
                    m_navDragStartPos = canvasPos;
                    m_navDragStartScroll = QPoint(hBar->value(), vBar->value());
                    m_navDragScale = m_navScale;
                    ui->navigatorPreviewLabel->setCursor(Qt::ClosedHandCursor);
                    mouseEvent->accept();
                    return true;
                }
            } else if (event->type() == QEvent::MouseMove) {
                if (m_isDraggingNavigator && (mouseEvent->buttons() & Qt::LeftButton)) {
                    if (m_navDragScale > 0.0) {
                        const QPointF deltaCanvas = canvasPos - m_navDragStartPos;
                        QScrollBar *hBar = ui->imageScrollArea->horizontalScrollBar();
                        QScrollBar *vBar = ui->imageScrollArea->verticalScrollBar();

                        hBar->setValue(m_navDragStartScroll.x() + qRound(deltaCanvas.x() / m_navDragScale));
                        vBar->setValue(m_navDragStartScroll.y() + qRound(deltaCanvas.y() / m_navDragScale));
                    }
                    mouseEvent->accept();
                    return true;
                }
                updateNavigatorCursor(canvasPos);
                return false;
            } else if (event->type() == QEvent::MouseButtonRelease && mouseEvent->button() == Qt::LeftButton) {
                if (m_isDraggingNavigator) {
                    m_isDraggingNavigator = false;
                    updateNavigatorCursor(canvasPos);
                    updateNavigatorPreview(true);
                    mouseEvent->accept();
                    return true;
                }
            }
        }
    }

    // 3. Ctrl + 滑鼠滾輪縮放影像
    if (watched == ui->imageScrollArea->viewport() && event->type() == QEvent::Wheel) {
        auto *wheelEvent = static_cast<QWheelEvent *>(event);
        if (wheelEvent->modifiers().testFlag(Qt::ControlModifier)) {
            if (!m_currentPixmap.isNull()) {
                const int angleY = wheelEvent->angleDelta().y();
                if (angleY != 0) {
                    int steps = angleY / 120;
                    if (steps == 0) {
                        steps = (angleY > 0) ? 1 : -1;
                    }
                    const int newValue = ui->zoomSlider->value() + steps * ui->zoomSlider->singleStep();
                    ui->zoomSlider->setValue(newValue);
                }
            }
            wheelEvent->accept();
            return true;
        }
    }

    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::onOpenImage()
{
    const QString startDir = m_currentFilePath.isEmpty()
    ? QDir::homePath()
    : QFileInfo(m_currentFilePath).absolutePath();

    const QString filePath = QFileDialog::getOpenFileName(
        this,
        localizedText(QStringLiteral("開啟影像"), QStringLiteral("Open Image")),
        startDir,
        localizedText(
            QStringLiteral("影像檔案 (*.png *.jpg *.jpeg *.bmp *.tif *.tiff);;所有檔案 (*.*)"),
            QStringLiteral("Image Files (*.png *.jpg *.jpeg *.bmp *.tif *.tiff);;All Files (*.*)"))
        );

    if (filePath.isEmpty()) {
        return;
    }

    QImage loadedImage(filePath);
    if (loadedImage.isNull()) {
        QMessageBox::warning(
            this,
            localizedText(QStringLiteral("開啟失敗"), QStringLiteral("Open Failed")),
            localizedText(
                QStringLiteral("無法載入影像檔案：\n%1"),
                QStringLiteral("Failed to load image file:\n%1")).arg(filePath)
            );
        return;
    }

    m_originalImage = loadedImage;
    m_currentImage = loadedImage;
    m_currentPixmap = QPixmap::fromImage(m_currentImage);
    refreshNavigatorSourcePixmap();
    m_currentFilePath = filePath;
    m_undoStack.clear();
    m_redoStack.clear();

    m_baseScale = calculateFitScale();
    resetZoom();
    updateImageDisplay(true);
    updateActionStates();
    updateWindowTitle();

    const QFileInfo info(filePath);
    ui->statusbar->showMessage(
        localizedText(
            QStringLiteral("已開啟：%1 (%2 x %3)"),
            QStringLiteral("Opened: %1 (%2 x %3)"))
            .arg(info.fileName())
            .arg(m_currentImage.width())
            .arg(m_currentImage.height()),
        5000
        );
}

void MainWindow::onSaveImage()
{
    if (m_currentImage.isNull()) {
        return;
    }

    if (m_currentFilePath.isEmpty()) {
        onSaveImageAs();
        return;
    }

    saveImageToPath(m_currentFilePath);
}

void MainWindow::onSaveImageAs()
{
    if (m_currentImage.isNull()) {
        return;
    }

    const QString defaultPath = m_currentFilePath.isEmpty()
                                    ? QDir::homePath() + "/untitled.png"
                                    : m_currentFilePath;

    const QString filePath = QFileDialog::getSaveFileName(
        this,
        localizedText(QStringLiteral("另存影像"), QStringLiteral("Save Image As")),
        defaultPath,
        QStringLiteral("PNG Image (*.png);;JPEG Image (*.jpg *.jpeg);;BMP Image (*.bmp)")
        );

    if (filePath.isEmpty()) {
        return;
    }

    if (saveImageToPath(filePath)) {
        m_currentFilePath = filePath;
        updateWindowTitle();
    }
}

bool MainWindow::saveImageToPath(const QString &filePath)
{
    if (!m_currentImage.save(filePath)) {
        QMessageBox::warning(
            this,
            localizedText(QStringLiteral("儲存失敗"), QStringLiteral("Save Failed")),
            localizedText(
                QStringLiteral("無法儲存影像至：\n%1"),
                QStringLiteral("Failed to save image to:\n%1")).arg(filePath)
            );
        return false;
    }

    ui->statusbar->showMessage(
        localizedText(QStringLiteral("已儲存：%1"), QStringLiteral("Saved: %1"))
            .arg(QFileInfo(filePath).fileName()),
        5000
        );
    return true;
}

void MainWindow::onResetImage()
{
    if (m_originalImage.isNull()) {
        return;
    }

    applyProcessedImage(m_originalImage);
    m_baseScale = calculateFitScale();
    resetZoom();
    updateImageDisplay(true);
    ui->statusbar->showMessage(
        localizedText(
            QStringLiteral("已還原為原始影像與最佳顯示比例 (100%)"),
            QStringLiteral("Restored original image and fit zoom (100%)")),
        3000
        );
}

void MainWindow::onUndo()
{
    if (m_undoStack.isEmpty()) {
        return;
    }

    m_redoStack.push(m_currentImage);
    m_currentImage = m_undoStack.pop();
    m_currentPixmap = QPixmap::fromImage(m_currentImage);
    refreshNavigatorSourcePixmap();
    m_baseScale = calculateFitScale();

    updateImageDisplay(true);
    updateActionStates();
    ui->statusbar->showMessage(
        localizedText(QStringLiteral("已復原操作"), QStringLiteral("Undone")),
        3000
        );
}

void MainWindow::onRedo()
{
    if (m_redoStack.isEmpty()) {
        return;
    }

    m_undoStack.push(m_currentImage);
    m_currentImage = m_redoStack.pop();
    m_currentPixmap = QPixmap::fromImage(m_currentImage);
    refreshNavigatorSourcePixmap();
    m_baseScale = calculateFitScale();

    updateImageDisplay(true);
    updateActionStates();
    ui->statusbar->showMessage(
        localizedText(QStringLiteral("已重做操作"), QStringLiteral("Redone")),
        3000
        );
}

void MainWindow::onZoomSliderChanged(int value)
{
    m_zoomFactor = value / 100.0;
    ui->zoomPercentLabel->setText(QStringLiteral("%1%").arg(value));
    updateImageDisplay(true);
}

void MainWindow::onResetZoom()
{
    if (m_currentPixmap.isNull()) {
        return;
    }

    m_baseScale = calculateFitScale();
    resetZoom();
    updateImageDisplay(true);
    ui->statusbar->showMessage(
        localizedText(QStringLiteral("縮放比例已重置為 100%"), QStringLiteral("Zoom reset to 100%")),
        3000
        );
}

void MainWindow::applyProcessedImage(const QImage &newImage)
{
    if (newImage.isNull()) {
        return;
    }

    if (!m_currentImage.isNull()) {
        m_undoStack.push(m_currentImage);
    }
    m_redoStack.clear();
    m_currentImage = newImage;
    m_currentPixmap = QPixmap::fromImage(m_currentImage);
    refreshNavigatorSourcePixmap();
    m_baseScale = calculateFitScale();

    updateImageDisplay(true);
    updateActionStates();
}

void MainWindow::resetZoom()
{
    m_zoomFactor = 1.0;
    const QSignalBlocker blocker(ui->zoomSlider);
    ui->zoomSlider->setValue(100);
    ui->zoomPercentLabel->setText(QStringLiteral("100%"));
}

void MainWindow::updateImageDisplay(bool smooth)
{
    if (m_currentPixmap.isNull()) {
        return;
    }

    ui->imageLabel->setText(QString());

    const double effectiveScale = m_baseScale * m_zoomFactor;
    const int targetWidth = qMax(1, qRound(m_currentPixmap.width() * effectiveScale));
    const int targetHeight = qMax(1, qRound(m_currentPixmap.height() * effectiveScale));

    const QPixmap scaledPixmap = m_currentPixmap.scaled(
        targetWidth,
        targetHeight,
        Qt::KeepAspectRatio,
        smooth ? Qt::SmoothTransformation : Qt::FastTransformation
        );
    m_displayedImageSize = scaledPixmap.size();
    ui->imageLabel->setPixmap(scaledPixmap);

    scheduleNavigatorUpdate(smooth);
}

void MainWindow::refreshNavigatorSourcePixmap()
{
    if (m_currentPixmap.isNull()) {
        m_navigatorSourcePixmap = QPixmap();
        return;
    }

    if (m_currentPixmap.width() > 512 || m_currentPixmap.height() > 512) {
        m_navigatorSourcePixmap = m_currentPixmap.scaled(
            512,
            512,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            );
    } else {
        m_navigatorSourcePixmap = m_currentPixmap;
    }
}

void MainWindow::scheduleNavigatorUpdate(bool smooth)
{
    m_navigatorSmoothPending = m_navigatorSmoothPending || smooth;
    if (m_navigatorUpdatePending) {
        return;
    }

    m_navigatorUpdatePending = true;
    QTimer::singleShot(0, this, [this]() {
        m_navigatorUpdatePending = false;
        const bool useSmooth = m_navigatorSmoothPending;
        m_navigatorSmoothPending = false;
        updateNavigatorPreview(useSmooth);
    });
}

void MainWindow::updateNavigatorCursor(const QPointF &canvasPos)
{
    if (m_navigatorSourcePixmap.isNull() || m_navScale <= 0.0 || !canScrollWorkspace()) {
        ui->navigatorPreviewLabel->unsetCursor();
        return;
    }

    if (m_navMappedViewportRect.adjusted(-4.0, -4.0, 4.0, 4.0).contains(canvasPos)) {
        ui->navigatorPreviewLabel->setCursor(Qt::OpenHandCursor);
    } else {
        ui->navigatorPreviewLabel->setCursor(Qt::PointingHandCursor);
    }
}

void MainWindow::updateNavigatorPreview(bool smooth)
{
    if (m_navigatorSourcePixmap.isNull() || m_displayedImageSize.isEmpty()) {
        m_navScale = 0.0;
        m_navMappedViewportRect = QRectF();
        m_isDraggingNavigator = false;
        ui->navigatorPreviewLabel->unsetCursor();
        ui->navigatorPreviewLabel->setPixmap(QPixmap());
        ui->navigatorPreviewLabel->setText(localizedText(
            QStringLiteral("尚無影像"),
            QStringLiteral("No Image")));
        return;
    }

    if (!ui->rightSidebarWidget->isVisible()) {
        return;
    }

    const QSize canvasSize = ui->navigatorPreviewLabel->contentsRect().size();
    if (canvasSize.width() <= 16 || canvasSize.height() <= 16) {
        return;
    }

    QWidget *viewport = ui->imageScrollArea->viewport();
    QWidget *contents = ui->scrollAreaWidgetContents;
    if (!viewport || !contents || viewport->width() <= 0 || viewport->height() <= 0) {
        return;
    }

    // 統一在 scrollAreaWidgetContents 座標系計算「目前檢視範圍」與「實際顯示圖片」
    const QPoint viewportTopLeft = contents->mapFrom(viewport, QPoint(0, 0));
    const QRectF viewportRect(
        viewportTopLeft.x(),
        viewportTopLeft.y(),
        viewport->width(),
        viewport->height()
        );

    const QPoint labelTopLeft = ui->imageLabel->mapTo(contents, QPoint(0, 0));
    const double pixmapOffsetX = (ui->imageLabel->width() - m_displayedImageSize.width()) / 2.0;
    const double pixmapOffsetY = (ui->imageLabel->height() - m_displayedImageSize.height()) / 2.0;

    const QRectF imageRect(
        labelTopLeft.x() + pixmapOffsetX,
        labelTopLeft.y() + pixmapOffsetY,
        m_displayedImageSize.width(),
        m_displayedImageSize.height()
        );

    const QRectF unionRect = imageRect.united(viewportRect);
    if (unionRect.width() <= 0.0 || unionRect.height() <= 0.0) {
        return;
    }

    const qreal dpr = ui->navigatorPreviewLabel->devicePixelRatioF();
    QPixmap canvas(
        qMax(1, qRound(canvasSize.width() * dpr)),
        qMax(1, qRound(canvasSize.height() * dpr))
        );
    canvas.setDevicePixelRatio(dpr);
    canvas.fill(Qt::transparent);

    const double padding = 6.0;
    const QRectF innerRect(
        padding,
        padding,
        qMax(1.0, canvasSize.width() - padding * 2.0),
        qMax(1.0, canvasSize.height() - padding * 2.0)
        );

    const double scale = qMin(
        innerRect.width() / unionRect.width(),
        innerRect.height() / unionRect.height()
        );

    const double originX = innerRect.left() + (innerRect.width() - unionRect.width() * scale) / 2.0;
    const double originY = innerRect.top() + (innerRect.height() - unionRect.height() * scale) / 2.0;

    auto mapToCanvas = [&](const QRectF &rect) -> QRectF {
        return QRectF(
            originX + (rect.left() - unionRect.left()) * scale,
            originY + (rect.top() - unionRect.top()) * scale,
            rect.width() * scale,
            rect.height() * scale
            );
    };

    const QRectF mappedImageRect = mapToCanvas(imageRect);
    const QRectF mappedViewportRect = mapToCanvas(viewportRect);

    // 儲存映射參數，供滑鼠點擊與拖曳反算主工作區捲軸位置
    m_navUnionRect = unionRect;
    m_navViewportContentRect = viewportRect;
    m_navMappedViewportRect = mappedViewportRect;
    m_navOrigin = QPointF(originX, originY);
    m_navScale = scale;

    QPainter painter(&canvas);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, smooth);

    painter.drawPixmap(
        mappedImageRect,
        m_navigatorSourcePixmap,
        QRectF(m_navigatorSourcePixmap.rect())
        );

    QPen redPen(QColor(255, 59, 48));
    redPen.setWidthF(2.0);
    redPen.setJoinStyle(Qt::MiterJoin);
    painter.setPen(redPen);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(mappedViewportRect);

    ui->navigatorPreviewLabel->setText(QString());
    ui->navigatorPreviewLabel->setPixmap(canvas);
}

void MainWindow::updateActionStates()
{
    const bool hasImage = !m_currentImage.isNull();
    ui->actionSave->setEnabled(hasImage);
    ui->actionSaveAs->setEnabled(hasImage);
    ui->actionReset->setEnabled(hasImage);
    ui->actionUndo->setEnabled(!m_undoStack.isEmpty());
    ui->actionRedo->setEnabled(!m_redoStack.isEmpty());
    ui->zoomSlider->setEnabled(hasImage);
    ui->resetZoomButton->setEnabled(hasImage);
}