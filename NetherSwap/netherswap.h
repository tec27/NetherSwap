#ifndef NETHERSWAP_H
#define NETHERSWAP_H

#include <QtWidgets/QMainWindow>
#include <QSystemTrayIcon>
#include <QSettings>
#include <memory>
#include "game_monitor.h"
#include "ui_netherswap.h"

class QEvent;
class QLocalServer;
class QString;
class QKeySequence;

class NetherSwap : public QMainWindow {
  Q_OBJECT

public:
  explicit NetherSwap(QWidget *parent = 0);
  ~NetherSwap();
  bool is_second_instance() { return is_second_instance_; }

protected:
  void mousePressEvent(QMouseEvent* me);
  void mouseMoveEvent(QMouseEvent* me);
  void mouseReleaseEvent(QMouseEvent* me);

private:
  void ReadSettings();
  Ui::NetherSwapClass ui;
  QSettings settings_;
  QSystemTrayIcon tray_icon_;
  QPoint drag_start_pos_;
  bool is_dragging_;
  QLocalServer* single_instance_server_;
  bool is_second_instance_;
  std::unique_ptr<HotkeySequence> in_game_hotkey_;
  std::unique_ptr<HotkeySequence> out_of_game_hotkey_;

private slots:
  void IconActivated(QSystemTrayIcon::ActivationReason reason);
  void Unminimize();
  void StatusChanged(GameStatus);
  void InGameHotkeyChanged(const HotkeySequence& keys);
  void OutOfGameHotkeyChanged(const HotkeySequence& keys);
};

#endif // NETHERSWAP_H
