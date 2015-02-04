
#ifndef __X_ANIMATE__
#define __X_ANIMATE__

#include <math.h>

#include "xTables.h"
#include "xCallbacks.h"

// IFDEF WIN
#include <windows.h>

/* xAsync API :
 *
 *	void	async_init	( void )
 *	bool	async_ready	( void )
 *
 *	bool	async_task	( xAsyncFunc, xMap* args, xAsyncCallback )
 *
 */


//================================================================
// Definitions
//================================================================

bool XASYNC_READY = false;

typedef xMap* (*xAsyncFunc)(xMap*);

typedef void (*xAsyncCallback)(xMap*);

typedef struct _xAsyncTask xAsyncTask;
struct _xAsyncTask
{
	xAsyncFunc func;
	xMap* args;
	xAsyncCallback callback;
	void* thread;
	xMap* result;
};
XTABLE_GENERATE_FUNCTIONS(xAsyncTask, xAsyncTask*);

xArray* tasks = NULL;

// IFDEF WIN
DWORD XASYNC_entry(xAsyncTask*);

void XASYNC_tick(uint32_t);


//================================================================
// Public API
//================================================================

//==================== init ====================

void async_init(void)
{
	if ( !xCallbacks_ready() )
	{
		fprintf(stderr, "async_init: xCallbacks not initialized\n");
		return;
	}
	tasks = arr_create();
	xCallbacks_add_tick(XASYNC_tick);
	XASYNC_READY = true;
}

bool async_ready(void)
{
	return XASYNC_READY;
}


//==================== start ====================

xAsyncTask* async_task(xAsyncFunc func, xMap* args, xAsyncCallback callback)
{
	xAsyncTask* task;
	DWORD tid;			// IFDEF WIN

	task = malloc(sizeof(xAsyncTask));
	if ( task == NULL ) return NULL;

	task->func = func;
	task->args = args;
	task->callback = callback;
	task->thread = NULL;
	task->result = NULL;

	// IFDEF WIN
	task->thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)XASYNC_entry, task, CREATE_SUSPENDED, &tid);
	if ( task->thread == NULL )
	{
		free(task);
		return NULL;
	}
	arr_add_xAsyncTask(tasks, task);
	ResumeThread(task->thread);
	return task;
}


//==================== free ====================

void async_free(void)
{
	int i;
	for ( i=0; i<tasks->size; i++ )
		free(arr_get_xAsyncTask(tasks, i));
	arr_free(tasks);
	tasks = NULL;
	xCallbacks_remove_tick(XASYNC_tick);
	XASYNC_READY = false;
}


//================================================================
// "Private" Functions
//================================================================

// IFDEF WIN
DWORD XASYNC_entry(xAsyncTask* task)
{
	task->result = task->func(task->args);
	ExitThread(0);
}

void XASYNC_tick(uint32_t dt)
{
	xAsyncTask* task;
	xMap* res;
	int i;
	DWORD code;		// IFDEF WIN

	for ( i=0; i<tasks->size; i++ )
	{
		task = arr_get_xAsyncTask(tasks, i);
		if ( GetExitCodeThread(task->thread, &code) )	// IFDEF WIN
		{
			if ( code == STILL_ACTIVE ) continue;		// IFDEF WIN

			arr_remove(tasks, i);
			i--;

			task->callback(task->result);

			free(task);
		}
	}
}

#endif
