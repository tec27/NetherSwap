#include "game_monitor.h"
#include "hook_injector.h"
#include <QString>
#include <QDebug>
#include <QTimer>
#include <QLocalServer>
#include <QLocalSocket>
#include <Windows.h>

using namespace std;

#define SEARCH_WINDOW_INTERVAL 3000
#define RECHECK_WINDOW_INTERVAL 13000

GameMonitor::GameMonitor(const QString& window_title, const QString& window_class,
    QObject* parent)
  : QObject(parent),
    window_title_(window_title),
    window_class_(window_class), 
    status_(kUnknown),
    game_window_(NULL),
    game_process_id_(-1),
    started(false) {
}

void GameMonitor::Start() {
  if(!started) {
    started = true;
    run();
  }
  else {
    qDebug() << "GameMonitor cannot be started more than once.";
  }
}

void GameMonitor::run() {
  emit StatusMessage("Searching for game instance...");

  timer_ = new QTimer(this);
  connect(timer_, SIGNAL(timeout()), this, SLOT(TimerTick()));
  connect(this, SIGNAL(destroyed()), timer_, SLOT(stop()));
  timer_->setInterval(SEARCH_WINDOW_INTERVAL);

  ipc_server_ = new QLocalServer(this);
  connect(ipc_server_, SIGNAL(newConnection()), this, SLOT(NewIpcConnection()));

  game_window_ = NULL;
  game_process_id_ = NULL;
  SearchWindow();

  if(!ipc_server_->listen("netherswap_ipc")) {
    emit StatusMessage("ERROR: Could not open status reception server!");
    return;
  }

  timer_->start();
}

void GameMonitor::TimerTick() {
  if(game_window_ == NULL) {
    SearchWindow();
  }
  else {
    RecheckWindow();
  }
}

void GameMonitor::NewIpcConnection() {
  QLocalSocket* client = ipc_server_->nextPendingConnection();
  bool in_game = false;

  while(client->bytesAvailable() < sizeof(in_game)) {
    client->waitForReadyRead();
  }

  connect(client, SIGNAL(disconnected()), client, SLOT(deleteLater()));
 
  qint64 num_read = client->read(reinterpret_cast<char*>(&in_game), sizeof(in_game));
  if(num_read < sizeof(in_game)) {
    qDebug() << "Read less data than expected.";
    NewStatus(kUnknown);
    return;
  }

  NewStatus(in_game ? kInGame : kInMenu);
}

void GameMonitor::SearchWindow() {
  HWND handle = FindWindow(window_class_.utf16(), window_title_.utf16());

  if(handle == NULL) {
    return;
  }

  FoundWindow(handle);
}

void GameMonitor::FoundWindow(HWND handle) {
  qDebug() << "Found window!";
  emit StatusMessage("Found a game instance, injecting...");
  game_window_ = handle;
  GetWindowThreadProcessId(game_window_, &game_process_id_);
  timer_->setInterval(RECHECK_WINDOW_INTERVAL);
  
  HookInjector injector(game_process_id_);
  if(!injector.Inject()) {
    emit StatusMessage(QString("ERROR: Injection Failed: %1").arg(GetLastError()));
  }
  else {
    emit StatusMessage("Connected, awaiting game status updates...");
  }
}

void GameMonitor::RecheckWindow() {
  if(!IsWindow(game_window_)) {
    LostWindow();
    return;
  }

  // At this point we know the handle is valid, but it could be a different window than
  // we started with (according to MSDN, anyway). Double-check this by checking process ID.
  DWORD process_id;
  GetWindowThreadProcessId(game_window_, &process_id);
  if(game_process_id_ != process_id) {
    LostWindow();
    return;
  }
}

void GameMonitor::LostWindow() {
  emit StatusMessage("Game instance lost, searching for new instances...");
  game_window_ = NULL;
  game_process_id_ = -1;
  NewStatus(kUnknown);
  timer_->setInterval(SEARCH_WINDOW_INTERVAL);
  return;
}

void GameMonitor::NewStatus(GameStatus status) {
  if(status != status_) {
    status_ = status;
    emit StatusChanged(status);
  }
}
