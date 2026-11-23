#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupIcons();
    setupConnections();

    statusBar()->showMessage("就緒：請開啟影像檔案");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupIcons()
{
    // 若未在 Qt Designer 指定 .qrc 圖示，自動套用系統標準圖示
    if (ui->actionOpen->icon().isNull())
        ui->actionOpen->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    if (ui->actionSave->icon().isNull())
        ui->actionSave->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    if (ui->actionExit->icon().isNull())
        ui->actionExit->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    if (ui->actionUndo->icon().isNull())
        ui->actionUndo->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    if (ui->actionResetImage->icon().isNull())
        ui->actionResetImage->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
}

void MainWindow::setupConnections()
{
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpenImage);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSaveImage);
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
    connect(ui->actionUndo, &QAction::triggered, this, &MainWindow::onUndoAction);
    connect(ui->actionResetImage, &QAction::triggered, this, &MainWindow::onResetImage);
}

void MainWindow::onOpenImage()
{
    const QString filePath = QFileDialog::getOpenFileName(
        this,
        "開啟影像",
        QString(),
        "Image Files (*.png *.jpg *.jpeg *.bmp);;All Files (*)"
        );

    if (filePath.isEmpty())
        return;

    QImage loaded(filePath);
    if (loaded.isNull()) {
        QMessageBox::warning(this, "開啟失敗", "無法讀取所選影像檔案。");
        return;
    }

    m_originalImage = loaded;
    m_currentImage = loaded;

    ui->imageLabel->setText(QString());
    ui->imageLabel->setPixmap(QPixmap::fromImage(m_currentImage));
    ui->imageLabel->adjustSize();

    statusBar()->showMessage(
        QString("已載入：%1 (%2 x %3)").arg(filePath).arg(loaded.width()).arg(loaded.height())
        );
}

void MainWindow::onSaveImage()
{
    if (m_currentImage.isNull()) {
        QMessageBox::information(this, "提示", "目前影像工作區沒有可儲存的影像。");
        return;
    }

    const QString filePath = QFileDialog::getSaveFileName(
        this,
        "儲存影像",
        "output.png",
        "PNG Image (*.png);;JPEG Image (*.jpg *.jpeg);;BMP Image (*.bmp)"
        );

    if (filePath.isEmpty())
        return;

    if (!m_currentImage.save(filePath)) {
        QMessageBox::warning(this, "儲存失敗", "無法寫入目標路徑。");
        return;
    }

    statusBar()->showMessage(QString("已儲存：%1").arg(filePath));
}

void MainWindow::onUndoAction()
{
    statusBar()->showMessage("Edit → Undo（已預留未來影像處理復原功能）", 3000);
}

void MainWindow::onResetImage()
{
    if (m_originalImage.isNull()) {
        statusBar()->showMessage("目前尚未載入影像", 3000);
        return;
    }

    m_currentImage = m_originalImage;
    ui->imageLabel->setPixmap(QPixmap::fromImage(m_currentImage));
    statusBar()->showMessage("已重設為原始影像", 3000);
}