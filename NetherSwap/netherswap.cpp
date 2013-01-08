#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QPixmap>
#include <QMouseEvent>
#include <QLocalServer>
#include <QLocalSocket>
#include <QString>
#include <Windows.h>
#include "game_monitor.h"
#include "hotkey_simulator.h"
#include "resource.h"
#include "netherswap.h"

#define NETHERSWAP_VERSION_MAJOR 0
#define NETHERSWAP_VERSION_MINOR 1

NetherSwap::NetherSwap(QWidget *parent)
    : QMainWindow(parent),
      settings_(this),
      is_dragging_(false),
      is_second_instance_(false),
      tray_icon_(QIcon(":/NetherSwap/Resources/icons/netherswap-tray.ico"), this) {
  qRegisterMetaType<GameStatus>("GameStatus");
  qRegisterMetaType<HotkeySequence>("HotkeySequence");

  QLocalSocket single_instance_checker;
  single_instance_checker.connectToServer("netherswap_instance_check");
  if(single_instance_checker.waitForConnected(250)) {
    // connected means a server is running elsewhere, exit this one
    single_instance_checker.abort();
    QTimer::singleShot(0, this, SLOT(close()));
    is_second_instance_ = true;
    return;
  }
  
  single_instance_checker.abort();
  single_instance_server_ = new QLocalServer(this);
  // running a second instance should bring up the first
  connect(single_instance_server_, SIGNAL(newConnection()), this, SLOT(Unminimize()));
  single_instance_server_->listen("netherswap_instance_check");

  QThread* monitor_thread = new QThread();
  GameMonitor* monitor = new GameMonitor("DOTA 2", "Valve001");
  monitor->moveToThread(monitor_thread);
  connect(monitor_thread, SIGNAL(started()), monitor, SLOT(Start()));
  connect(monitor_thread, SIGNAL(finished()), monitor, SLOT(deleteLater()));
  connect(monitor_thread, SIGNAL(finished()), monitor_thread, SLOT(deleteLater()));
  connect(this, SIGNAL(destroyed()), monitor_thread, SLOT(quit()));

  connect(monitor, SIGNAL(StatusChanged(GameStatus)), this, SLOT(StatusChanged(GameStatus)));

  ui.setupUi(this);
  ReadSettings();
  connect(ui.in_game_hotkey_btn, SIGNAL(HotkeySet(const HotkeySequence&)), this, 
      SLOT(InGameHotkeyChanged(const HotkeySequence&)));
  connect(ui.out_of_game_hotkey_btn, SIGNAL(HotkeySet(const HotkeySequence&)), this, 
      SLOT(OutOfGameHotkeyChanged(const HotkeySequence&)));

  setWindowFlags(Qt::FramelessWindowHint);
  connect(ui.minimize_btn, SIGNAL(clicked()), this, SLOT(hide()));
  connect(ui.close_btn, SIGNAL(clicked()), this, SLOT(close()));

  tray_icon_.show();
  connect(&tray_icon_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
      SLOT(IconActivated(QSystemTrayIcon::ActivationReason)));

  ui.version_label->setText(QString("v%1.%2").arg(NETHERSWAP_VERSION_MAJOR)
      .arg(NETHERSWAP_VERSION_MINOR));

  connect(monitor, SIGNAL(StatusMessage(const QString&)), ui.status_label, 
    SLOT(setText(const QString&)));

  monitor_thread->start();
}

NetherSwap::~NetherSwap() {
}

void NetherSwap::StatusChanged(GameStatus status) {
  switch(status) {
    case kUnknown: {
      tray_icon_.setIcon(QIcon(":/NetherSwap/Resources/icons/netherswap-tray.ico"));
      if(out_of_game_hotkey_ != nullptr) {
        HotkeySimulator::PressHotkey(out_of_game_hotkey_.get());
      }
      break;
    }
    case kInMenu: {
      ui.status_label->setText("Game is in menu/in the background");
      tray_icon_.setIcon(QIcon(":/NetherSwap/Resources/icons/netherswap-out.ico"));
      if(out_of_game_hotkey_ != nullptr) {
        HotkeySimulator::PressHotkey(out_of_game_hotkey_.get());
      }
      break;
    }
    case kInGame: {
      ui.status_label->setText("Game is in the foreground");
      tray_icon_.setIcon(QIcon(":/NetherSwap/Resources/icons/netherswap-in.ico"));
      if(in_game_hotkey_ != nullptr) {
        HotkeySimulator::PressHotkey(in_game_hotkey_.get());
      }
      break;
    }
  }
}

void NetherSwap::IconActivated(QSystemTrayIcon::ActivationReason reason) {
  switch(reason) {
    case QSystemTrayIcon::DoubleClick: {
      Unminimize();
      break;
    }
    default: break;
  }
}

void NetherSwap::Unminimize() {
  raise();
  activateWindow();
  showNormal();
}

void NetherSwap::mousePressEvent(QMouseEvent* me) {
  is_dragging_ = me->button() == Qt::LeftButton;
  if(is_dragging_) {
    drag_start_pos_ = pos() - me->globalPos();
  }
}

void NetherSwap::mouseMoveEvent(QMouseEvent* me) {
  if(is_dragging_) {
    move(drag_start_pos_ + me->globalPos());
  }
}

void NetherSwap::mouseReleaseEvent(QMouseEvent* me) {
  is_dragging_ = false;
}

void NetherSwap::ReadSettings() {
  settings_.beginGroup("hotkeys");

  QString value = settings_.value("ingame", QString()).toString();
  if(!value.isEmpty()) {
    ui.in_game_hotkey_btn->SetHotkey(HotkeySequence(value));
    in_game_hotkey_.reset(new HotkeySequence(value));
  }

  value = settings_.value("outofgame", QString()).toString();
  if(!value.isEmpty()) {
    ui.out_of_game_hotkey_btn->SetHotkey(HotkeySequence(value));
    out_of_game_hotkey_.reset(new HotkeySequence(value));
  }

  settings_.endGroup();
}

void NetherSwap::InGameHotkeyChanged(const HotkeySequence& keys) {
  settings_.beginGroup("hotkeys");
  settings_.setValue("ingame", keys.serialized());
  settings_.endGroup();

  in_game_hotkey_.reset(new HotkeySequence(keys));
}

void NetherSwap::OutOfGameHotkeyChanged(const HotkeySequence& keys) {
  settings_.beginGroup("hotkeys");
  settings_.setValue("outofgame", keys.serialized());
  settings_.endGroup();

  out_of_game_hotkey_.reset(new HotkeySequence(keys));
}
