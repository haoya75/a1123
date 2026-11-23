#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onOpenImage();
    void onSaveImage();
    void onUndoAction();
    void onResetImage();

private:
    void setupIcons();
    void setupConnections();

    Ui::MainWindow *ui;
    QImage m_currentImage;
    QImage m_originalImage;
};

#endif // MAINWINDOW_H