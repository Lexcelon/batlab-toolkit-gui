#include "encoder.h"
#include "math.h"

Encoder::Encoder(double data) { m_data = data; }

uint16_t Encoder::asVoltage() {
  return static_cast<uint16_t>(m_data * pow(2, 15) / 4.5);
}

uint16_t Encoder::asVcc() {
  return static_cast<uint16_t>((pow(2, 15) * 4.096) / m_data);
}

uint16_t Encoder::asFreq() {
  return static_cast<uint16_t>(m_data / (10000.0 / 256.0));
}

uint16_t Encoder::asIoff() { return static_cast<uint16_t>(m_data * 128.0); }

uint16_t Encoder::asSetpoint() { return static_cast<uint16_t>(m_data * 128); }

uint16_t Encoder::asMagDiv() { return static_cast<uint16_t>(1 - log2(m_data)); }

uint16_t Encoder::asCurrent() {
  return static_cast<uint16_t>(m_data * (pow(2, 15) - 1) / 4.096);
}

uint16_t Encoder::asChargeL() {
  return static_cast<uint16_t>(
      static_cast<uint32_t>((m_data * 9.765625 / 4.096 / 6) * pow(2, 15)) &
      0xFFFF);
}

uint16_t Encoder::asChargeH() {
  return static_cast<uint16_t>(
      static_cast<uint32_t>((m_data * 9.765625 / 4.096 / 6) * pow(2, 15)) >>
      16);
}

uint16_t Encoder::asTemperatureF(int Rdiv, int B) {
  double To = 25 + 273.15;
  double Ro = 10000;
  double F = (m_data - 32) / 1.8;
  F = 1 / (F + 273.15);
  double R = exp((F - (1 / To)) * B) * Ro;
  if (R > 0 && Rdiv > 0) {
    R = pow(2, 15) / (((1 / R) * Rdiv) + 1);
  } else {
    R = -100;
  }
  return static_cast<uint16_t>(R);
}

uint16_t Encoder::asTemperatureC(int Rdiv, int B) {
  double To = 25 + 273.15;
  double Ro = 10000;
  double C = 1 / (m_data + 273.15);
  double R = exp((C - (1 / To)) * B) * Ro;
  if (R > 0 && Rdiv > 0) {
    R = pow(2, 15) / (((1 / R) * Rdiv) + 1);
  } else {
    R = -100;
  }
  return static_cast<uint16_t>(R);
}
