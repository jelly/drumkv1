// drumkv1_config.cpp
//
/****************************************************************************
   Copyright (C) 2012-2014, rncbc aka Rui Nuno Capela. All rights reserved.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*****************************************************************************/

#include "drumkv1_config.h"

#include <QFileInfo>


//-------------------------------------------------------------------------
// drumkv1_config - Prototype settings structure (pseudo-singleton).
//

// Singleton instance accessor (static).
drumkv1_config *drumkv1_config::g_pSettings = NULL;

drumkv1_config *drumkv1_config::getInstance (void)
{
	return g_pSettings;
}


// Constructor.
drumkv1_config::drumkv1_config (void)
	: QSettings(DRUMKV1_DOMAIN, DRUMKV1_TITLE)
{
	g_pSettings = this;

	load();
}


// Default destructor.
drumkv1_config::~drumkv1_config (void)
{
	save();

	g_pSettings = NULL;
}


// Preset utility methods.
QString drumkv1_config::presetGroup (void) const
{
	return "/Presets/";
}


QString drumkv1_config::presetFile ( const QString& sPreset )
{
	QSettings::beginGroup(presetGroup());
	const QString sPresetFile(QSettings::value(sPreset).toString());
	QSettings::endGroup();
	return sPresetFile;
}


void drumkv1_config::setPresetFile (
	const QString& sPreset, const QString& sPresetFile )
{
	QSettings::beginGroup(presetGroup());
	QSettings::setValue(sPreset, sPresetFile);
	QSettings::endGroup();
}


void drumkv1_config::removePreset ( const QString& sPreset )
{
	QSettings::beginGroup(presetGroup());
	const QString& sPresetFile = QSettings::value(sPreset).toString();
	if (QFileInfo(sPresetFile).exists())
		QFile(sPresetFile).remove();
	QSettings::remove(sPreset);
	QSettings::endGroup();
}


QStringList drumkv1_config::presetList (void)
{
	QStringList list;
	QSettings::beginGroup(presetGroup());
	QStringListIterator iter(QSettings::childKeys());
	while (iter.hasNext()) {
		const QString& sPreset = iter.next();
		if (QFileInfo(QSettings::value(sPreset).toString()).exists())
			list.append(sPreset);
	}
	QSettings::endGroup();
	return list;
}


// Explicit I/O methods.
void drumkv1_config::load (void)
{
	QSettings::beginGroup("/Default");
	sPreset = QSettings::value("/Preset").toString();
	sPresetDir = QSettings::value("/PresetDir").toString();
	sSampleDir = QSettings::value("/SampleDir").toString();
	QSettings::endGroup();

	QSettings::beginGroup("/Dialogs");
	bUseNativeDialogs = QSettings::value("/UseNativeDialogs", true).toBool();
	// Run-time special non-persistent options.
	bDontUseNativeDialogs = !bUseNativeDialogs;
	QSettings::endGroup();
}


void drumkv1_config::save (void)
{
	QSettings::beginGroup("/Program");
	QSettings::setValue("/Version", DRUMKV1_VERSION);
	QSettings::endGroup();

	QSettings::beginGroup("/Default");
	QSettings::setValue("/Preset", sPreset);
	QSettings::setValue("/PresetDir", sPresetDir);
	QSettings::setValue("/SampleDir", sSampleDir);
	QSettings::endGroup();

	QSettings::beginGroup("/Dialogs");
	QSettings::setValue("/UseNativeDialogs", bUseNativeDialogs);
	QSettings::endGroup();

	QSettings::sync();
}


// end of drumkv1_config.cpp
