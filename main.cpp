#include "batlab.h"
#include <QApplication>
#include "HistogramWidget.h"
#include <QObject>
#include <QWidget>
#include <QImage>
#include <chrono>
#include <random>

#include "HistogramObject.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QVector<qreal> vec;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator (seed);

        std::normal_distribution<double> distribution (0.0,20.0);

    for (int i = 0; i < 10; ++i) {
        vec.append(distribution(generator));
    }

    qSort(vec);
    HistogramObject object;
    object.onBinWidth(1);

    QImage image("C:/Users/Seikowave/Desktop/VimbaImage_0_21.bmp");
    HistogramWidget w;
    w.show();
    QObject::connect(&object, &HistogramObject::emitDisplay,
            &w, &HistogramWidget::onReceiveValues);

//    object.onRecieveImage(image,1);
    object.onRecieveValues(vec);
    w.onXRange(vec.first(),vec.last());
    w.onYRange(0,100);
    // Create the gui and show it
//    Batlab w;
//    w.show();

    return a.exec();
}
