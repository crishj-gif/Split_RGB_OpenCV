#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>
#include <opencv2/opencv.hpp>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpenImage_triggered()
{
    QString imagePath = QFileDialog::getOpenFileName(this, "Chọn ảnh để phân tích", "", "Images (*.png *.jpg *.jpeg *.bmp)");
    if (imagePath.isEmpty()) {
        qDebug() << "Chưa chọn ảnh nào!";
        return;
    }
    qDebug() << "Đường dẫn ảnh đã chọn:" << imagePath;
    QFileInfo fileInfo(imagePath);
    QString fileName = fileInfo.fileName();
    std::string cvFilePath=imagePath.toStdString();
    cv::Mat originalImage=cv::imread(cvFilePath);

    if(originalImage.empty()){
        qDebug()<<"Lỗi:OpenCV không thể đọc đuọc ảnh này!";
        return;
    }
    int width=originalImage.cols;
    int height =originalImage.rows;
    int channels=originalImage.channels();

    QString sizeStr=QString("%1x%2").arg(width).arg(height);
    cv::Mat rgbImage;
    cv::cvtColor(originalImage, rgbImage,cv::COLOR_BGR2RGB);

    std::vector<cv::Mat> channels_array;
    cv::split(rgbImage,channels_array);

    cv::Mat zero = cv::Mat::zeros(rgbImage.size(),CV_8UC1);
    //Red
    std::vector<cv::Mat> redChannels={channels_array[0],zero,zero};
    cv::Mat redImage;
    cv:merge(redChannels,redImage);
    //Green
    std::vector<cv::Mat> greenChannels={zero,channels_array[1],zero};
    cv::Mat greenImage;
    cv::merge(greenChannels,greenImage);
    //blue
    std::vector<cv::Mat> blueChannels={zero,zero,channels_array[2]};
    cv::Mat blueImage;
    cv::merge(blueChannels,blueImage);

    QImage qImgGoc((const uchar*)rgbImage.data, rgbImage.cols, rgbImage.rows, rgbImage.step, QImage::Format_RGB888);
    QImage qImgDo((const uchar*)redImage.data, redImage.cols, redImage.rows, redImage.step, QImage::Format_RGB888);
    QImage qImgXanhLa((const uchar*)greenImage.data, greenImage.cols, greenImage.rows, greenImage.step, QImage::Format_RGB888);
    QImage qImgXanhDuong((const uchar*)blueImage.data, blueImage.cols, blueImage.rows, blueImage.step, QImage::Format_RGB888);
    ui->lblThumbOriginal->setPixmap(QPixmap::fromImage(qImgGoc).scaled(ui->lblThumbOriginal->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->lblThumbRed->setPixmap(QPixmap::fromImage(qImgDo).scaled(
        ui->lblThumbRed->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->lblThumbGreen->setPixmap(QPixmap::fromImage(qImgXanhLa).scaled(
        ui->lblThumbGreen->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->lblThumbBlue->setPixmap(QPixmap::fromImage(qImgXanhDuong).scaled(
        ui->lblThumbBlue->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->lblMainImage->setPixmap(QPixmap::fromImage(qImgGoc).scaled(ui->lblMainImage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange = { range };
    cv::Mat r_hist, g_hist, b_hist;

    cv::calcHist(&channels_array[0], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange);
    cv::calcHist(&channels_array[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange);
    cv::calcHist(&channels_array[2], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange);

    int hist_w = 600;
    int hist_h = 400;
    int bin_w = cvRound((double) hist_w / histSize);

    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(40, 40, 40));

    cv::normalize(r_hist, r_hist, 0, histImage.rows - 20, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(g_hist, g_hist, 0, histImage.rows - 20, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(b_hist, b_hist, 0, histImage.rows - 20, cv::NORM_MINMAX, -1, cv::Mat());


    for(int i = 1; i < histSize; i++) {

        cv::line(histImage,
                 cv::Point(bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1))),
                 cv::Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
                 cv::Scalar(255, 0, 0), 2, cv::LINE_AA);


        cv::line(histImage,
                 cv::Point(bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1))),
                 cv::Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
                 cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
        cv::line(histImage,
                 cv::Point(bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1))),
                 cv::Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
                 cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
    }

    QImage qHistImg((const uchar*)histImage.data, histImage.cols, histImage.rows, histImage.step, QImage::Format_RGB888);


    ui->label_15->setPixmap(QPixmap::fromImage(qHistImg).scaled(
        ui->label_15->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    ui->lblTenTep->setText(fileName);

    ui->lblKichThuoc->setText(sizeStr);
    QString formatStr = (channels == 3) ? "RGB (24-bit)" : QString("Số kênh: %1").arg(channels);
    ui->lblDinhDang->setText(formatStr);

    }
    void MainWindow::openImage() {}
    void MainWindow::saveImage() {}
    void MainWindow::zoomIn() {}
    void MainWindow::zoomOut() {}
    void MainWindow::resetZoom() {}
    void MainWindow::onTabChanged(int index) {}
    bool MainWindow::eventFilter(QObject *watched, QEvent *event)
    {

        return false;
    }


