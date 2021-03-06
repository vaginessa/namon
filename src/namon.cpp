/** 
 *  @file       namon.cpp
 *  @brief      Network Traffic Capturing With Application Tags
 *  @author     Jozef Zuzelka <xzuzel00@stud.fit.vutbr.cz>
 *  @date
 *   - Created: 20.03.2017 16:34
 *   - Edited:  23.06.2017 12:12
 */

#include <map>              //  map

#include "netflow.hpp"      //  Netflow
#include "debug.hpp"        //  log()
#include "namon.hpp"

//http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
#if defined(__linux__)
#include "namon_linux.hpp"
int(*getId)(NAMON::Netflow *) = NAMON::getInode;

#elif defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include <sys/param.h>
#if defined(BSD)
#include "namon_bsd.hpp"
int(*getId)(NAMON::Netflow *) = NAMON::getInode;
#endif

#elif defined(__APPLE__)
#include "namon_apple.hpp"
int(*getId)(NAMON::Netflow *) = NAMON::getInode;

#elif defined(_WIN32)
#include "namon_win.hpp"
int (*getId)(NAMON::Netflow *) = NAMON::getPid;
#endif

extern std::map<string, std::vector<NAMON::Netflow *>> g_finalResults;
extern unsigned int g_notFoundSockets, g_allSockets;




namespace NAMON
{


int determineApp(Netflow *n, TEntry &e, const char mode)
{
	int id = getId(n);
	if (id == -2)
		return -1;

	// if we are updating existing cache record
	if (mode == UPDATE)
	{
		if (id == e.getInodeOrPid())
		{ // if nothing changed, update time
			e.updateTime();
			e.getNetflowPtr()->setEndTime(n->getEndTime());
			return 0;
		}
		else if (e.getAppName() != "")
		{ // save expired record to results
			Netflow *res = new Netflow;
			*res = *e.getNetflowPtr();
			g_finalResults[e.getAppName()].push_back(res);
            e.setAppName("");
		}
	}

	g_allSockets++;
	// update new pid in cache
	e.setInodeOrPid(id);
	if (id == -1)
		g_notFoundSockets++;
	else
	{
		string appName;
		if (getApp(id, appName))
			return -1;

		e.setAppName(appName);
	}


	if (mode == FIND)
	{ // if we are not updating same netflow, move if from cacheBuffer
		Netflow *newN = new Netflow;
		*newN = std::move(*n);
		e.setNetflowPtr(newN);
	}
	//! @todo packets destined for closed socket
	else
	{ // else we update expired record with a new application so just update times
		e.getNetflowPtr()->setStartTime(n->getStartTime());
		e.getNetflowPtr()->setEndTime(n->getEndTime());
	}
	return 0;
}


}	// namespace NAMON
