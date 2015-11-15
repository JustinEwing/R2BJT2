/*
 * File: R2Service.h
 * @Author Daniel Ruatta, 11/13/2015 7:00pm
 */

#ifndef R2BumperService_H  // <- This should be changed to your own guard on both
#define R2BumperService_H  //    of these lines


/*******************************************************************************
 * PUBLIC #INCLUDES                                                            *
 ******************************************************************************/

#include "ES_Configure.h"   // defines ES_Event, INIT_EVENT, ENTRY_EVENT, and EXIT_EVENT
#include "IO_Ports.h"

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/


/*******************************************************************************
 * PUBLIC TYPEDEFS                                                             *
 ******************************************************************************/
typedef enum {
    WAS_BUMPED, WAS_UNBUMPED
} bumpstate_t;

/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

 
/**
 * @Function InitR2Service(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunR2Service function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @Author Daniel Ruatta, 11/13/2015 7:00pm */
uint8_t InitR2BumperService(uint8_t Priority);

/**
 * @Function PostR2Service(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be posted to queue
 * @return TRUE or FALSE
 * @brief This function is a wrapper to the queue posting function, and its name
 *        will be used inside ES_Configure to point to which queue events should
 *        be posted to. Remember to rename to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @Author Daniel Ruatta, 11/13/2015 7:00pm */
uint8_t PostR2BumperService(ES_Event ThisEvent);

/**
 * @Function RunR2Service(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be responded.
 * @return Event - return event (type and param), in general should be ES_NO_EVENT
 * @brief This function is where you implement the whole of the service,
 *        as this is called any time a new event is passed to the event queue. 
 * @note Remember to rename to something appropriate.
 *       Returns ES_NO_EVENT if the event have been "consumed." 
 * @Author Daniel Ruatta, 11/13/2015 7:00pm */
ES_Event RunR2BumperService(ES_Event ThisEvent);

/******************************************************************
 * @Function InitBumpers
 * @Param void
 * @Return SUCCESS or ERROR
 * @Author Daniel Ruatta, 11/13/2015
 * @Brief Sets the Bumper pins (Y3 and Y4) to inputs so that we
 * can read BUMP_EVENTS from them
 * @Usage To be called in main() to initialize our bumpers
 */
uint8_t InitBumpers(void);



#endif /* R2Service_H */

