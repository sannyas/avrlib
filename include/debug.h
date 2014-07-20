/**
 * @file debug.h
 * @brief This file provide some macro for debug
 * @author Liao Maoyi
 * @date 2014/03/09
 *
 * <pre>
 * Usage:
 * 	The macro AVRLIB_DEBUG will decide the behavior of
 * 	debugging macro in the following code. Befor including 
 * 	this file. Should determine whether The macro AVRLIB_DEBUG
 * 	is defined or not. The sample code ( enable ):
 * 	@code
 * 		#define AVRLIB_DEBUG
 * 		#include <debug.h>
 * 		...
 * 		DEBUG_FUNC_ENTER();
 * 		PDEBUG("...");
 * 		DEBUG_FUNC_EXIT();
 * 	@endcode
 *
 * 	or( disable ):
 * 	@code
 * 		#undef AVRLIB_DEBUG
 * 		#include <debug.h>
 * 	@endcode
 * 	or:
 * 	@code
 * 		#undef AVRLIB_DEBUG
 * 		#ifdef xxx_DEBUG
 * 		  #define AVRLIB_DEBUG
 * 		#endif
 * 		#include <debug.h>
 * 	@endcode
 * </pre>
 */
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>

#define DEBUG_FUNC_ENTER_MSG	"---> "
#define DEBUG_FUNC_EXIT_MSG	"<--- "
#ifdef AVRLIB_DEBUG
  extern uint8_t func_call_chain_cnt;
  #ifndef DEBUG_OUTPUT
    /* If don't define PDEBUG, define a default empty PDEBUG */
    #define DEBUG_OUTPUT(fmt,args...) printf(fmt,##args)
  #endif

  #define PDEBUG_SPACE() do{ \
	  		uint8_t __deep_cnt; \
	  		for( __deep_cnt=0; \
				__deep_cnt<func_call_chain_cnt; \
				++__deep_cnt ){ \
				DEBUG_OUTPUT( "  " );} \
	 		}while( 0 )
  #define PDEBUG(fmt,args...)  do{ PDEBUG_SPACE(); DEBUG_OUTPUT(fmt,##args); }while(0)

  #define DEBUG_FUNC_ENTER() do{ PDEBUG( DEBUG_FUNC_ENTER_MSG "%s\n", __FUNCTION__ );\
	  			 ++func_call_chain_cnt; \
 			     }while(0)
  #define DEBUG_FUNC_EXIT() do{ PDEBUG( DEBUG_FUNC_EXIT_MSG "\n");\
	  			--func_call_chain_cnt; \
  			    }while(0)

#else
  #undef PDEBUG
  #define DEBUG_INIT()		uint8_t func_call_chain_cnt
  #define PDEBUG(fmt,args...)  do{}while(0)
  #undef DEBUG_OUTPUT
  #define DEBUG_OUTPUT(fmt,args...) do{}while(0)
  #define DEBUG_FUNC_ENTER()  do{}while(0)
  #define DEBUG_FUNC_EXIT() do{}while(0)
#endif

#endif /* _DEBUG_H_ */
