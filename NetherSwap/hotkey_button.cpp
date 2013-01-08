#include <QPushButton>
#include <QKeyEvent>
#include <QTimer>
#include "hotkey_button.h"
#include "hotkey_sequence.h"

using namespace std;

HotkeyButton::HotkeyButton(QWidget* parent)
  : QPushButton(parent) {
  Init();
}

HotkeyButton::HotkeyButton(const QString& text, QWidget* parent)
  : QPushButton(text, parent) {
  Init();
}

HotkeyButton::HotkeyButton(const QIcon& icon, const QString& text, QWidget* parent) 
  : QPushButton(icon, text, parent) {
  Init();
}

void HotkeyButton::Init() {
  connect(this, SIGNAL(clicked()), this, SLOT(RecordHotkey()));
  recording_  = false;
  SetTextFromKeys();
}

void HotkeyButton::RecordHotkey() {
  if(!recording_) {
    recording_ = true;
    setText("- Press hotkey (ESC to cancel) -");

    recorded_modifiers_ = Qt::NoModifier;
    grabKeyboard();
  }
}

void HotkeyButton::keyPressEvent(QKeyEvent* ke) {
  if(!recording_) {
    QPushButton::keyPressEvent(ke);
    return;
  }

  ke->accept();

  recorded_modifiers_ |= ke->modifiers();

  int key = ke->key();
  if(key != Qt::Key_Control && key != Qt::Key_Shift && key != Qt::Key_Alt &&
      key != Qt::Key_Meta) {
    if(recorded_modifiers_ == Qt::NoModifier && key == Qt::Key_Escape) {
      SetTextFromKeys();
    }
    else {
      SetHotkey(HotkeySequence(ke, recorded_modifiers_));
      emit HotkeySet(HotkeySequence(ke, recorded_modifiers_));
    }
    
    QTimer::singleShot(0, this, SLOT(StopRecording()));
  }
  else {
    switch(ke->key()) {
      case Qt::Key_Control: recorded_modifiers_ |= Qt::ControlModifier; break;
      case Qt::Key_Shift: recorded_modifiers_ |= Qt::ShiftModifier; break;
      case Qt::Key_Alt: recorded_modifiers_ |= Qt::AltModifier; break;
      case Qt::Key_Meta: recorded_modifiers_ |= Qt::MetaModifier; break;
    }
    setText(QString("Recording: %1").arg(QKeySequence(
        HotkeySequence::GetKeySequenceValue(0, recorded_modifiers_)).toString()));
  }
}

void HotkeyButton::keyReleaseEvent(QKeyEvent* ke) {
  if(!recording_) {
    QPushButton::keyReleaseEvent(ke);
    return;
  }

  ke->accept();
  recorded_modifiers_ &= ~ke->modifiers();
  switch(ke->key()) {
    case Qt::Key_Control: recorded_modifiers_ &= ~Qt::ControlModifier; break;
    case Qt::Key_Shift: recorded_modifiers_ &= ~Qt::ShiftModifier; break;
    case Qt::Key_Alt: recorded_modifiers_ &= ~Qt::AltModifier; break;
    case Qt::Key_Meta: recorded_modifiers_ &= ~Qt::MetaModifier; break;
  }

  if(recorded_modifiers_ == Qt::NoModifier) {
    setText("- Press hotkey (ESC to cancel) -");
  }
  else {
    setText(QString("Recording: %1").arg(QKeySequence(
        HotkeySequence::GetKeySequenceValue(0, recorded_modifiers_)).toString()));
  }
}

void HotkeyButton::StopRecording() {
  recording_ = false;
  releaseKeyboard();
}

void HotkeyButton::SetHotkey(const HotkeySequence& keys) {
  current_keys_.reset(new HotkeySequence(keys));
  SetTextFromKeys();
}

void HotkeyButton::SetTextFromKeys() {
  if(current_keys_ != NULL) {
    setText(current_keys_->ToString());
  }
  else {
    setText("Not Set");
  }
}
