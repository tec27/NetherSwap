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
  // These have to be split up and delayed in order to work properly with OBS
  // It checks the hotkeys using GetAsyncKeyState once every 30ms (ugh) and then has
  // some really weird conditions regarding modifiers and the key (it has to have detected the
  // modifiers were down BEFORE it detects the key was pressed?)

  // Note that OBS will still have issues with this sometimes if your hotkey contains modifiers
  // you pressed to put the game to the background (namely Alt and using Alt-Tab) so I'd
  // recommend you try to have as few modifiers in your OBS hotkeys as possible
  QVector<INPUT> down_modifiers;
  QVector<INPUT> up_modifiers;
  QVector<INPUT> key_press;
  down_modifiers.reserve(4);
  up_modifiers.reserve(4);
  key_press.reserve(2);

  PushKeyDownForModifiers(keys->modifiers(), down_modifiers);
  PushKeyDown(keys->virtual_key(), key_press);
  PushKeyUp(keys->virtual_key(), key_press);
  PushKeyUpForModifiers(keys->modifiers(), up_modifiers);

  if(down_modifiers.size() > 0) {
    SendInput(down_modifiers.size(), down_modifiers.data(), sizeof(INPUT));
    Sleep(70);
  }
  SendInput(key_press.size(), key_press.data(), sizeof(INPUT));
  if(up_modifiers.size() > 0) {
    Sleep(70);
    SendInput(up_modifiers.size(), up_modifiers.data(), sizeof(INPUT));
  }
}

}