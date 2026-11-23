#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settingsdialog.h"

#include <QEvent>
#include <QIcon>
#include <QImage>
#include <QMainWindow>
#include <QPixmap>
#include <QPoint>
#include <QPointF>
#include <QRectF>
#include <QResizeEvent>
#include <QSize>
#include <QStack>
#include <QString>
#include <QToolButton>
#include <QVariantAnimation>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 供後續影像處理功能呼叫：套用新影像並寫入 Undo 紀錄
    void applyProcessedImage(const QImage &newImage);

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onOpenImage();
    void onSaveImage();
    void onSaveImageAs();
    void onResetImage();
    void onUndo();
    void onRedo();
    void onZoomSliderChanged(int value);
    void onResetZoom();
    void onToggleSidebar();
    void onOpenSettings();

private:
    void setupConnections();
    void setupFallbackIcons();
    void setupSettingsCornerButton();
    void setupSidebarAnimation();
    void loadSavedLanguage();
    void applyLanguage(SettingsDialog::Language language);
    void updateWindowTitle();
    QIcon createGearIcon() const;
    QString localizedText(const QString &zh, const QString &en) const;

    int targetSidebarWidth() const;
    double calculateFitScale() const;
    bool canScrollWorkspace() const;
    void resetZoom();
    void updateImageDisplay(bool smooth = true);
    void refreshNavigatorSourcePixmap();
    void scheduleNavigatorUpdate(bool smooth = true);
    void updateNavigatorPreview(bool smooth = true);
    void updateNavigatorCursor(const QPointF &canvasPos);
    void updateActionStates();
    bool saveImageToPath(const QString &filePath);

    Ui::MainWindow *ui;
    QToolButton *m_settingsButton = nullptr;
    SettingsDialog::Language m_currentLanguage = SettingsDialog::Language::TraditionalChinese;

    QImage m_originalImage;
    QImage m_currentImage;
    QPixmap m_currentPixmap;
    QPixmap m_navigatorSourcePixmap;
    QSize m_displayedImageSize;
    QString m_currentFilePath;
    QStack<QImage> m_undoStack;
    QStack<QImage> m_redoStack;
    double m_baseScale = 1.0;
    double m_zoomFactor = 1.0;

    QVariantAnimation *m_sidebarAnimation = nullptr;
    bool m_isSidebarOpen = false;
    bool m_navigatorUpdatePending = false;
    bool m_navigatorSmoothPending = false;

    // 右側預覽縮圖與紅框互動座標映射狀態
    QRectF m_navUnionRect;
    QRectF m_navViewportContentRect;
    QRectF m_navMappedViewportRect;
    QPointF m_navOrigin;
    double m_navScale = 0.0;

    bool m_isDraggingNavigator = false;
    QPointF m_navDragStartPos;
    QPoint m_navDragStartScroll;
    double m_navDragScale = 0.0;
};

#endif // MAINWINDOW_H