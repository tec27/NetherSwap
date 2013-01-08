#ifndef GAME_MONITOR_H
#define GAME_MONITOR_H

#include <QObject>
#include <QThread>
#include <QString>
#include <Windows.h>

class QTimer;
class QLocalServer;

enum GameStatus {
  kUnknown = 0,
  kInMenu = 1,
  kInGame = 2
};
Q_DECLARE_METATYPE(GameStatus);

class GameMonitor : public QObject {
  Q_OBJECT

public:
  GameMonitor(const QString& window_title, const QString& window_class,
      QObject* parent = (QObject*)0);

public slots:
  void Start();

signals:
  void StatusChanged(GameStatus status);
  void StatusMessage(const QString& message);

private:
  void run();
  void SearchWindow();
  void FoundWindow(HWND handle);
  void RecheckWindow();
  void LostWindow();
  void NewStatus(GameStatus status);
  QString window_title_;
  QString window_class_;
  GameStatus status_;
  HWND game_window_;
  DWORD game_process_id_;
  QTimer* timer_;
  QLocalServer* ipc_server_;
  boolean started;

private slots:
  void TimerTick();
  void NewIpcConnection();
};

#endif // GAME_MONITOR_H
