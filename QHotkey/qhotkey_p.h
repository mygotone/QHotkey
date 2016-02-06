#ifndef QHOTKEY_P_H
#define QHOTKEY_P_H

#include "qhotkey.h"
#include <QAbstractNativeEventFilter>
#include <QMultiHash>
#include <QMutex>
#include <QGlobalStatic>

class QHotkeyPrivate : public QAbstractNativeEventFilter
{
public:
	QHotkeyPrivate();//singleton!!!
	~QHotkeyPrivate();

	static QHotkeyPrivate *instance();
	static inline bool testValid(QHotkey::NativeShortcut nativeShortcut);

	inline QHotkey::NativeShortcut nativeShortcut(Qt::Key keycode, Qt::KeyboardModifiers modifiers);

	bool hasShortcut(Qt::Key keycode, Qt::KeyboardModifiers modifiers);

	bool addShortcut(QHotkey *hotkey);
	bool removeShortcut(QHotkey *hotkey);

protected:
	void activateShortcut(QHotkey::NativeShortcut shortcut);

	virtual quint32 nativeKeycode(Qt::Key keycode) = 0;//platform implement
	virtual quint32 nativeModifiers(Qt::KeyboardModifiers modifiers) = 0;//platform implement

	virtual bool registerShortcut(QHotkey::NativeShortcut shortcut) = 0;//platform implement
	virtual bool unregisterShortcut(QHotkey::NativeShortcut shortcut) = 0;//platform implement

private:
	QMutex mutex;
	QMultiHash<QHotkey::NativeShortcut, QHotkey*> shortcuts;
};

#define NATIVE_INSTANCE(ClassName) \
	Q_GLOBAL_STATIC(ClassName, hotkeyPrivate) \
	\
	QHotkeyPrivate *QHotkeyPrivate::instance()\
	{\
		return hotkeyPrivate;\
	}

inline bool QHotkeyPrivate::testValid(QHotkey::NativeShortcut nativeShortcut)
{
	return (nativeShortcut.first != 0);
}

inline QHotkey::NativeShortcut QHotkeyPrivate::nativeShortcut(Qt::Key keycode, Qt::KeyboardModifiers modifiers) {
	return {this->nativeKeycode(keycode), this->nativeModifiers(modifiers)};
}

#endif // QHOTKEY_P_H
