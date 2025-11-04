#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <Zxing/BarcodeFormat.h>
#include <Zxing/BitMatrix.h>
#include <Zxing/TextUtfEncoding.h>
#include <Zxing/MultiFormatWriter.h>

using namespace cv;
using namespace ZXing;

QImage MatToQImage(const Mat& mat) {
    if (mat.type() == CV_8UC1) {
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8).copy();
    }
    else if (mat.type() == CV_8UC3) {
        Mat rgb;
        cvtColor(mat, rgb, COLOR_BGR2RGB);
        return QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888).copy();
    }
    return QImage();
}

class BarcodeWidget : public QWidget {
public:
    BarcodeWidget(QWidget* parent = nullptr) : QWidget(parent) {
        auto layout = new QVBoxLayout(this);

        inputEdit = new QLineEdit(this);
        inputEdit->setPlaceholderText("Enter text to encode");
        layout->addWidget(inputEdit);

        generateButton = new QPushButton("Generate PDF417", this);
        layout->addWidget(generateButton);

        barcodeLabel = new QLabel(this);
        barcodeLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(barcodeLabel);

        connect(generateButton, &QPushButton::clicked, this, &BarcodeWidget::generateBarcode);
    }

private:
    QLineEdit* inputEdit;
    QPushButton* generateButton;
    QLabel* barcodeLabel;

    void generateBarcode() {
        std::string text = inputEdit->text().toStdString();
        if (text.empty()) return;

        MultiFormatWriter writer(BarcodeFormat::PDF417);
        auto bitMatrix = writer.encode(text, 400, 200); // 宽高可调

        int width = bitMatrix.width();
        int height = bitMatrix.height();
        Mat barcodeImage(height, width, CV_8UC1);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                barcodeImage.at<uint8_t>(y, x) = bitMatrix.get(x, y) ? 0 : 255;
            }
        }

        QImage qimg = MatToQImage(barcodeImage);
        barcodeLabel->setPixmap(QPixmap::fromImage(qimg).scaled(400, 200, Qt::KeepAspectRatio));
    }
};

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    BarcodeWidget w;
    w.setWindowTitle("PDF417 Generator");
    w.resize(500, 300);
    w.show();

    return a.exec();
}
