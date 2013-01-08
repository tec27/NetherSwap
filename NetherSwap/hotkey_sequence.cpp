#include <QKeyEvent>
#include <QString>
#include <QStringList>
#include "hotkey_sequence.h"

int HotkeySequence::GetKeySequenceValue(int key, const Qt::KeyboardModifiers modifiers) {
  if(modifiers & Qt::ShiftModifier) {
    key += Qt::SHIFT;
  }
  if(modifiers & Qt::ControlModifier) {
    key += Qt::CTRL;
  }
  if(modifiers & Qt::AltModifier) {
    key += Qt::ALT;
  }
  if(modifiers & Qt::MetaModifier) {
    key += Qt::META;
  }

  return key;
}

HotkeySequence::HotkeySequence()
  : text_("Not Set") {
    // I have to include this for Qt's metatype stuff, but this should never actually
    // be used
}

HotkeySequence::HotkeySequence(const QKeyEvent* key_event, const Qt::KeyboardModifiers modifiers)
  : modifiers_(modifiers),
    scan_code_(key_event->nativeScanCode()),
    virtual_key_(key_event->nativeVirtualKey()),
    key_(key_event->key()),
    text_(QKeySequence(HotkeySequence::GetKeySequenceValue(key_, modifiers)).toString()) {

}

HotkeySequence::HotkeySequence(const QString& serialized_str) {
  QStringList parts = serialized_str.split(",");
  bool ok;
  
  if(parts.size() > 0) {
    modifiers_ = Qt::KeyboardModifiers(parts[0].toUInt(&ok));
    if(!ok) modifiers_ = Qt::NoModifier;
  }
  if(parts.size() > 1) {
    scan_code_ = parts[1].toUInt(&ok);
    if(!ok) scan_code_ = 0;
  }
  if(parts.size() > 2) {
    virtual_key_ = parts[2].toUInt(&ok);
    if(!ok) virtual_key_ = 0;
  }
  if(parts.size() > 3) {
    key_ = parts[2].toInt(&ok);
    if(!ok) key_ = 0;
  }

  text_ = QKeySequence(HotkeySequence::GetKeySequenceValue(key_, modifiers_)).toString();
}

HotkeySequence::HotkeySequence(const HotkeySequence& other) 
  : modifiers_(other.modifiers_),
    scan_code_(other.scan_code_),
    virtual_key_(other.virtual_key_),
    text_(QString(other.text_)) {

}

QString HotkeySequence::serialized() const {
  return QString("%1,%2,%3,%4")
      .arg(static_cast<int>(modifiers_))
      .arg(scan_code_)
      .arg(virtual_key_)
      .arg(key_);
}