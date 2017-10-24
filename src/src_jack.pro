# drumkv1_jack.pro
#
NAME = drumkv1

TARGET = $${NAME}_jack
TEMPLATE = app
LIBS += -L.

include(src_jack.pri)

HEADERS = \
	config.h \
	drumkv1.h \
	drumkv1_ui.h \
	drumkv1_config.h \
	drumkv1_param.h \
	drumkv1_programs.h \
	drumkv1_controls.h \
	drumkv1_nsm.h \
	drumkv1_jack.h \
	drumkv1widget.h \
	drumkv1widget_env.h \
	drumkv1widget_filt.h \
	drumkv1widget_sample.h \
	drumkv1widget_wave.h \
	drumkv1widget_param.h \
	drumkv1widget_preset.h \
	drumkv1widget_status.h \
	drumkv1widget_elements.h \
	drumkv1widget_programs.h \
	drumkv1widget_controls.h \
	drumkv1widget_control.h \
	drumkv1widget_config.h \
	drumkv1widget_jack.h

SOURCES = \
	drumkv1_nsm.cpp \
	drumkv1_jack.cpp \
	drumkv1widget_jack.cpp


unix {

	OBJECTS_DIR = .obj_jack
	MOC_DIR     = .moc_jack
	UI_DIR      = .ui_jack

	isEmpty(PREFIX) {
		PREFIX = /usr/local
	}

	isEmpty(BINDIR) {
		BINDIR = $${PREFIX}/bin
	}

	isEmpty(LIBDIR) {
		TARGET_ARCH = $$system(uname -m)
		contains(TARGET_ARCH, x86_64) {
			LIBDIR = $${PREFIX}/lib64
		} else {
			LIBDIR = $${PREFIX}/lib
		}
	}

	isEmpty(DATADIR) {
		DATADIR = $${PREFIX}/share
	}

	#DEFINES += DATADIR=\"$${DATADIR}\"

	INSTALLS += target desktop icon appdata \
		icon_scalable mimeinfo mimetypes mimetypes_scalable

	target.path = $${BINDIR}

	desktop.path = $${DATADIR}/applications
	desktop.files += $${NAME}.desktop

	icon.path = $${DATADIR}/icons/hicolor/32x32/apps
	icon.files += images/$${NAME}.png 

	icon_scalable.path = $${DATADIR}/icons/hicolor/scalable/apps
	icon_scalable.files += images/$${NAME}.svg

	appdata.path = $${DATADIR}/metainfo
	appdata.files += appdata/$${NAME}.appdata.xml

	mimeinfo.path = $${DATADIR}/mime/packages
	mimeinfo.files += mimetypes/$${NAME}.xml

	mimetypes.path = $${DATADIR}/icons/hicolor/32x32/mimetypes
	mimetypes.files += mimetypes/application-x-$${NAME}-preset.png

	mimetypes_scalable.path = $${DATADIR}/icons/hicolor/scalable/mimetypes
	mimetypes_scalable.files += mimetypes/application-x-$${NAME}-preset.svg

	LIBS += -l$${NAME} -l$${NAME}_ui -Wl,-rpath,$${LIBDIR}
}

QT += xml

# QT5 support
!lessThan(QT_MAJOR_VERSION, 5) {
	QT += widgets
}
