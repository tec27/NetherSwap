#ifndef HOTKEY_BUTTON_H
#define HOTKEY_BUTTON_H

#include <QPushButton>
#include <QString>
#include <memory>
#include "hotkey_sequence.h"

class QKeyEvent;

class HotkeyButton: public QPushButton {
  Q_OBJECT

public:
  explicit HotkeyButton(QWidget* parent = 0);
  HotkeyButton(const QString& text, QWidget* parent = 0);
  HotkeyButton(const QIcon& icon, const QString& text, QWidget* parent = 0);

public slots:
  void RecordHotkey();
  void SetHotkey(const HotkeySequence& keys);

signals:
  void HotkeySet(const HotkeySequence& keys);

protected:
  void keyPressEvent(QKeyEvent* ke);
  void keyReleaseEvent(QKeyEvent* ke);

private:
  void Init();
  void SetTextFromKeys();
  std::unique_ptr<HotkeySequence> current_keys_;
  bool recording_;
  Qt::KeyboardModifiers recorded_modifiers_;

private slots:
  void StopRecording();
};

#endif
