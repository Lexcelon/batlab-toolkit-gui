#ifndef ENCODER_H
#define ENCODER_H

#include <QObject>

class Encoder {
public:
  Encoder(double data);
  uint16_t asVoltage();
  uint16_t asVcc();
  uint16_t asFreq();
  uint16_t asIoff();
  uint16_t asSetpoint();
  uint16_t asMagDiv();
  uint16_t asCurrent();
  uint16_t asChargeL();
  uint16_t asChargeH();
  uint16_t asTemperatureF(int Rdiv, int B);
  uint16_t asTemperatureC(int Rdiv, int B);

private:
  double m_data;
};

#endif // ENCODER_H
