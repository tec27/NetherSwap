#include <Windows.h>
#include <QVector>
#include <memory>
#include "hotkey_simulator.h"

namespace HotkeySimulator {

void InitInput(const quint32 virtual_key, bool is_key_up, INPUT* target) {
  memset(target, 0, sizeof(target));
  target->type = INPUT_KEYBOARD;
  target->ki.wVk = virtual_key;
  target->ki.dwFlags = is_key_up ? KEYEVENTF_KEYUP : 0;
}

void PushKeyDown(const quint32 virtual_key, QVector<INPUT>& target) {
  target.resize(target.size()+1);
  InitInput(virtual_key, false, &target[target.size()-1]);
}

void PushKeyUp(const quint32 virtual_key, QVector<INPUT>& target) {
  target.resize(target.size()+1);
  InitInput(virtual_key, true, &target[target.size()-1]);
}

void PushKeyDownForModifiers(const Qt::KeyboardModifiers modifiers, QVector<INPUT>& target) {
  if(modifiers & Qt::ControlModifier) {
    PushKeyDown(VK_CONTROL, target);
  }
  if(modifiers & Qt::ShiftModifier) {
    PushKeyDown(VK_SHIFT, target);
  }
  if(modifiers & Qt::AltModifier) {
    PushKeyDown(VK_MENU, target);
  }
  if(modifiers & Qt::MetaModifier) {
    PushKeyDown(VK_LWIN, target);
  }
}

void PushKeyUpForModifiers(const Qt::KeyboardModifiers modifiers, QVector<INPUT>& target) {
  if(modifiers & Qt::ControlModifier) {
    PushKeyUp(VK_CONTROL, target);
  }
  if(modifiers & Qt::ShiftModifier) {
    PushKeyUp(VK_SHIFT, target);
  }
  if(modifiers & Qt::AltModifier) {
    PushKeyUp(VK_MENU, target);
  }
  if(modifiers & Qt::MetaModifier) {
    PushKeyUp(VK_LWIN, target);
  }
}

void PressHotkey(const HotkeySequence* keys) {
  QVector<INPUT> inputs;
  inputs.reserve(10); // not really that much memory, might as well reap the performance benefits

  PushKeyDownForModifiers(keys->modifiers(), inputs);
  PushKeyDown(keys->virtual_key(), inputs);
  PushKeyUp(keys->virtual_key(), inputs);
  PushKeyUpForModifiers(keys->modifiers(), inputs);

  SendInput(inputs.size(), inputs.data(), sizeof(INPUT));
}

}