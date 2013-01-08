#ifndef HOTKEY_SEQUENCE_H
#define HOTKEY_SEQUENCE_H

#include <QString>
#include <QKeyEvent>

class HotkeySequence {
public:
  HotkeySequence();
  HotkeySequence(const QKeyEvent* key_event, const Qt::KeyboardModifiers modifiers);
  explicit HotkeySequence(const QString& serialized_str);
  explicit HotkeySequence(const HotkeySequence& other);
  Qt::KeyboardModifiers modifiers() const { return modifiers_; }
  quint32 scan_code() const { return scan_code_; }
  quint32 virtual_key() const { return virtual_key_; }
  QString ToString() const { return text_; }
  QString serialized() const;

  static int GetKeySequenceValue(int key, const Qt::KeyboardModifiers modifiers);

private:
  Qt::KeyboardModifiers modifiers_;
  quint32 scan_code_;
  quint32 virtual_key_;
  int key_;
  QString text_;
};

Q_DECLARE_METATYPE(HotkeySequence);

#endif
