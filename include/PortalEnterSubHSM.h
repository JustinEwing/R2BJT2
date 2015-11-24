/* 
 * File:   PortalEnterSubHSM.h
 * Author: druatta
 *
 * Created on November 21, 2015, 1:22 PM
 */

#ifndef PORTAL_ENTER_SUB_HSM_H
#define PORTAL_ENTER_SUB_HSM_H

/*******************************************************************************
 * PUBLIC #INCLUDES                                                            *
 ******************************************************************************/

#include "ES_Configure.h"   // defines ES_Event, INIT_EVENT, ENTRY_EVENT, and EXIT_EVENT

/*******************************************************************************
 * @Function InitPortalEnterSubHSM(void)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @author Daniel Ruatta, 11.21.2015 */
uint8_t InitPortalEnterSubHSM(void);

/*******************************************************************************
 * @Function RunPortalEnterSubHSM(void)
 * @param ThisEvent - the event (type and param) to be responded.
 * @return Event - return event (type and param), in general should be ES_NO_EVENT
 * @author Daniel Ruatta, 11.21.2015 */
ES_Event RunPortalEnterSubHSM(ES_Event ThisEvent);




#endif	/* PORTALENTERSUBHSM_H */

