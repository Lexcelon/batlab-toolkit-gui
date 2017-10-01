#include "testdata.h"

TestData::TestData(QObject *parent) : QObject(parent)
{
}

testPacket TestData::getTestData()
{
    return testData;
}

void TestData::setTestData(testPacket packet)
{
    testData = packet;
}

void TestData::readTestData(QString filename)
{
    QFile f(filename);


    if (f.open(QFile::ReadOnly)) {
        QTextStream data( &f );
        QString temp;
        QStringList strList;
        do {
            data >> temp;
            strList.append(temp);
        } while (!temp.isNull());
        int num = strList.takeFirst().toInt();
        testData.REG_MODE.resize(num);
        for (int j = 0; j < num; ++j) {
            testData.REG_MODE[j] = strList.takeFirst().toInt();
        }

        num = strList.takeFirst().toInt();
        testData.TIME.resize(num);
        for (int j = 0; j < num; ++j) {
            testData.TIME[j] = strList.takeFirst().toInt();
        }

        num = strList.takeFirst().toInt();
        testData.REG_VOLTAGE.resize(num);
        for (int j = 0; j < num; ++j) {
            testData.REG_VOLTAGE[j] = strList.takeFirst().toFloat();
        }

        num = strList.takeFirst().toInt();
        testData.REG_CURRENT.resize(num);
        for (int j = 0; j < num; ++j) {
            testData.REG_CURRENT[j] = strList.takeFirst().toFloat();
        }


        num = strList.takeFirst().toInt();
        testData.REG_TEMPERATURE.resize(num);
        for (int j = 0; j < num; ++j) {
            testData.REG_TEMPERATURE[j] = strList.takeFirst().toFloat();
        }


        f.close();
    }
    qDebug() << testData.REG_MODE << testData.REG_VOLTAGE;
}
