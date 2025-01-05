#include "mainwindow.h"
#include <QLabel>
#include <QDragEnterEvent>
#include <QColor>
#include <QMessageBox>
#include <QCheckBox>
#include <QSlider>
#include <QDropEvent>
#include <QMimeData>
#include <QVBoxLayout>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget* centralWidget = new QWidget(this);

    QLabel* label = new QLabel("Drag & Drop Your Image ...");
    label->setAlignment(Qt::AlignCenter);
    QFont labelFont = label->font();
    labelFont.setPointSize(20);
    label->setFont(labelFont);

    QLabel* qualityLabel = new QLabel("JPEG Quality: 50");
    qualityLabel->setAlignment(Qt::AlignCenter);

    QSlider* qualitySlider = new QSlider(Qt::Horizontal);
    qualitySlider->setRange(0, 100);
    qualitySlider->setValue(50);
    qualitySlider->setTickPosition(QSlider::TicksBelow);
    qualitySlider->setTickInterval(10);

    connect(qualitySlider, &QSlider::valueChanged, this, [this, qualityLabel](int value) {
        qualityLabel->setText(QString("JPEG Quality: %1").arg(value));
        this->quality = value;
    });

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(label);
    layout->addWidget(qualityLabel);
    layout->addWidget(qualitySlider);
    layout->setAlignment(Qt::AlignCenter);

    centralWidget->setLayout(layout);

    this->setWindowTitle("Smol JPEG");
    this->resize(WindowWidth, WindowHeight);
    this->setCentralWidget(centralWidget);
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
    if(event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent* event) {
    const QMimeData* mimeData = event->mimeData();

    if(mimeData->hasUrls()) {
        QList<QUrl> urls = mimeData->urls();

        for(const QUrl& url : urls) {
            if(url.isLocalFile()) {
                QString filePath = url.toLocalFile();
                QFileInfo fileInfo(filePath);

                bool isImage = fileInfo.suffix().toLower() == "jpg" || fileInfo.suffix().toLower() == "jpeg";

                if(isImage) {
                    handleDroppedFile(filePath, fileInfo);
                } else {
                    QMessageBox::warning(this, "Invalid File", "Only Supports JPEG Images.");
                }
            }
        }
    }
}

void MainWindow::handleDroppedFile(const QString& filePath, const QFileInfo& fileInfo) {
    const auto savedFilePath = fileInfo.absolutePath() + "/" + fileInfo.baseName() + "_compressed." + fileInfo.suffix().toLower();
    QImage image(filePath);
    image.save(savedFilePath, nullptr, this->quality);
}
