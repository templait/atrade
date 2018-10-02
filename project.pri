CONFIG+=debug_and_release
BUILDPATH = $$PWD/build
CONFIG(debug, debug|release) {
	DESTDIR=$$BUILDPATH/binDebug
} else {
	DESTDIR=$$BUILDPATH/binRelease
}

OBJECTS_DIR	= $$DESTDIR/objects/$$TARGET
MOC_DIR		= $$BUILDPATH/moc/$$TARGET
UI_DIR		= $$BUILDPATH/ui/$$TARGET
RCC_DIR		= $$BUILDPATH/rcc/$$TARGET

QMAKE_LIBDIR += $$DESTDIR

win32::CONFIG += embed_manifest_exe embed_manifest_dll

unix::QMAKE_LFLAGS += -Wl,-rpath,.

win32::QMAKE_CXXFLAGS += /MP
