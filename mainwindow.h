#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QFileInfo>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QColor>
#include <QLabel>
#include <QMessageBox>

// Thư viện OpenCV xử lý ảnh chính
#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // Lắng nghe sự kiện di chuyển chuột trên ảnh chính để bắt tọa độ và màu sắc
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void openImage();             // Mở file ảnh
    void saveImage();             // Lưu ảnh đang chọn
    void zoomIn();                // Phóng to ảnh
    void zoomOut();               // Thu nhỏ ảnh
    void resetZoom();             // Khôi phục kích thước ảnh gốc
    void onTabChanged(int index); // Xử lý khi nhấn chuyển tab kênh màu

    void on_actionOpenImage_triggered();

private:
    Ui::MainWindow *ui;

    // Các ma trận OpenCV lưu trữ ảnh gốc và ảnh đang xử lý hiển thị
    cv::Mat originalMat;
    cv::Mat currentProcessedMat;

    QString loadedFileName;       // Lưu tên file ảnh đã nạp
    double zoomFactor;            // Hệ số phóng to/thu nhỏ (Mặc định 1.0)
    int currentTabChannelIndex;   // Kênh màu hiện tại (0: Gốc, 1: Đỏ, 2: Xanh Lá, 3: Xanh Dương)

    // Các hàm phụ trợ (Helpers)
    QImage matToQImage(const cv::Mat& mat);                 // Đổi OpenCV Mat sang Qt QImage
    void updateMainDisplay();                               // Cập nhật hiển thị ảnh lớn ở giữa
    void drawHistogram(const cv::Mat& src, int channelIndex); // Vẽ biểu đồ màu Histogram bằng QPainter
};
#endif // MAINWINDOW_H
