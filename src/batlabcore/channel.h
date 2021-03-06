#ifndef CHANNEL_H
#define CHANNEL_H

#include <ctime>

#include <QObject>
#include <QTimer>

#include "batlablib.h"
#include "batlabpacket.h"
#include "cellplaylist.h"
#include "encoder.h"

class Batlab;
class BatlabManager;

class Channel : public QObject {
  Q_OBJECT
public:
  explicit Channel(int slot, QObject *parent = nullptr);
  channelStatusInfo info;
  bool testInProgress() { return m_test_state != TS_IDLE; }
  ChannelMode mode() { return info.mode; }

signals:
  void resultsUpdated();

public slots:
  void startTest();
  void abortTest();
  void completeTest();
  void handleSerialResponseBundleReady(batlabPacketBundle bundle);
  void setInternalSetpoint(quint16 val) { m_current_setpoint = val; }
  void impedance();
  void handleImpedanceResponse(QVector<BatlabPacket> response);
  void setMode(int mode);
  void handleSetModeResponse(QVector<BatlabPacket> response);
  void logLvl1(QString logstr);
  void logLvl2(QString type);
  void logLvl3();
  void handleLogLvl2Response(QVector<BatlabPacket> response);

private:
  QTimer *m_channelPeriodicCheckTimer;

  TestState m_test_state;

  std::chrono::time_point<std::chrono::system_clock> m_ts;
  std::chrono::time_point<std::chrono::system_clock> m_start_time;
  std::chrono::time_point<std::chrono::system_clock> m_last_lvl1_time;
  std::chrono::time_point<std::chrono::system_clock> m_last_lvl2_time;
  std::chrono::time_point<std::chrono::system_clock> m_last_impedance_time;
  std::chrono::time_point<std::chrono::system_clock> m_rest_time;
  std::chrono::seconds m_timeout_time;

  int m_voltage_count;
  float m_voltage_avg;
  int m_voltage_error_count;

  int m_current_count;
  float m_current_avg;
  quint16 m_current_setpoint;

  float m_z_avg;
  float m_z_count;

  float m_vcc;

  float m_e;
  float m_q;

  float m_temperature0;
  float m_delta_t;

  float m_last_charge;

  int m_current_cycle;

  quint16 m_sp_raw;

  std::chrono::time_point<std::chrono::system_clock> m_pulse_discharge_on_time;
  std::chrono::time_point<std::chrono::system_clock> m_pulse_discharge_off_time;
  std::chrono::time_point<std::chrono::system_clock> m_pulse_charge_on_time;
  std::chrono::time_point<std::chrono::system_clock> m_pulse_charge_off_time;
  bool m_pulse_state;

  bool m_trickle_engaged;

  int m_set_mode;

  void periodicCheck();
  void handlePeriodicCheckResponse(QVector<BatlabPacket> response);
  void handleStartTestResponse(QVector<BatlabPacket> response);
  void stateMachine();
  void startTimer();

  void currentCompensate(quint16 op_raw, quint16 sp_raw);
  void handleCurrentCompensateResponse();

  Batlab *batlab();
  CellPlaylist playlist();
};

#endif // CHANNEL_H
