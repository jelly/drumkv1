// drumkv1widget_lv2.cpp
//
/****************************************************************************
   Copyright (C) 2012-2019, rncbc aka Rui Nuno Capela. All rights reserved.

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

#include "drumkv1widget_lv2.h"

#include "drumkv1_lv2.h"


#include <QCloseEvent>


//-------------------------------------------------------------------------
// drumkv1widget_lv2 - impl.
//

// Constructor.
drumkv1widget_lv2::drumkv1widget_lv2 ( drumkv1_lv2 *pDrumk,
	LV2UI_Controller controller, LV2UI_Write_Function write_function )
	: drumkv1widget()
{
	m_pDrumkUi = new drumkv1_lv2ui(pDrumk, controller, write_function);

#ifdef CONFIG_LV2_UI_EXTERNAL
	m_external_host = nullptr;
#endif
#ifdef CONFIG_LV2_UI_IDLE
	m_bIdleClosed = false;
#endif

	m_iShowEvent = 0;

	// Initialise preset stuff...
	clearPreset();

	// Initial update, always...
	refreshElements();
	activateElement();

	//resetParamValues(drumkv1::NUM_PARAMS);
	resetParamKnobs(drumkv1::NUM_PARAMS);

	// May initialize the scheduler/work notifier.
	openSchedNotifier();
}


// Destructor.
drumkv1widget_lv2::~drumkv1widget_lv2 (void)
{
	delete m_pDrumkUi;
}


// Synth engine accessor.
drumkv1_ui *drumkv1widget_lv2::ui_instance (void) const
{
	return m_pDrumkUi;
}


#ifdef CONFIG_LV2_UI_EXTERNAL

void drumkv1widget_lv2::setExternalHost ( LV2_External_UI_Host *external_host )
{
	m_external_host = external_host;

	if (m_external_host && m_external_host->plugin_human_id)
		drumkv1widget::setWindowTitle(m_external_host->plugin_human_id);
}

const LV2_External_UI_Host *drumkv1widget_lv2::externalHost (void) const
{
	return m_external_host;
}

#endif	// CONFIG_LV2_UI_EXTERNAL


#ifdef CONFIG_LV2_UI_IDLE

bool drumkv1widget_lv2::isIdleClosed (void) const
{
	return m_bIdleClosed;
}

#endif	// CONFIG_LV2_UI_IDLE


// Show event handler.
void drumkv1widget_lv2::showEvent ( QShowEvent *pShowEvent )
{
	drumkv1widget::showEvent(pShowEvent);

	++m_iShowEvent;

	drumkv1widget::updateElement();
}


// Close event handler.
void drumkv1widget_lv2::closeEvent ( QCloseEvent *pCloseEvent )
{
	drumkv1widget::closeEvent(pCloseEvent);

#ifdef CONFIG_LV2_UI_IDLE
	if (pCloseEvent->isAccepted())
		m_bIdleClosed = true;
#endif
#ifdef CONFIG_LV2_UI_EXTERNAL
	if (m_external_host && m_external_host->ui_closed) {
		if (pCloseEvent->isAccepted())
			m_external_host->ui_closed(m_pDrumkUi->controller());
	}
#endif
}


// LV2 port event dispatcher.
void drumkv1widget_lv2::port_event ( uint32_t port_index,
	uint32_t buffer_size, uint32_t format, const void *buffer )
{
	if (format == 0 && buffer_size == sizeof(float)) {
		const drumkv1::ParamIndex index
			= drumkv1::ParamIndex(port_index - drumkv1_lv2::ParamBase);
		const float fValue = *(float *) buffer;
		if (index < drumkv1::NUM_ELEMENT_PARAMS && m_iShowEvent > 0) {
			drumkv1_ui *pDrumkUi = ui_instance();
			if (pDrumkUi) {
				const int iCurrentNote = pDrumkUi->currentElement();
				drumkv1_element *element = pDrumkUi->element(iCurrentNote);
				if (element)
					element->setParamValue(index, fValue);
			}
		}
		if (index >= drumkv1::NUM_ELEMENT_PARAMS || m_iShowEvent > 0)
			setParamValue(index, fValue);
	}
}


// Param port method.
void drumkv1widget_lv2::updateParam (
	drumkv1::ParamIndex index, float fValue ) const
{
	m_pDrumkUi->write_function(index, fValue);
}


// end of drumkv1widget_lv2.cpp
