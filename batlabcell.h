#ifndef BATLABCELL_H
#define BATLABCELL_H

#include <QObject>
#include <QVector>

class batlabCell
{
public:
    batlabCell();
    batlabCell(uchar key);
    ~batlabCell();

private:
    uchar unit;
    uchar cell;
    QVector<int> temperature;
    QVector<int> voltage;
    QVector<int> current;
    QVector<int> currentAmplitude;
    QVector<int> voltageAmplitude;
    QVector<int> voltagePhase;
    QVector<int> charge;
    int status;

};

#endif // BATLABCELL_H
