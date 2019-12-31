#include "server.h"

#include <log/log.h>

namespace server
{
	bool global_stop;
	core::core global_core;

	BOOL WINAPI console_ctrl_handler(DWORD ctrl)
	{
		if (ctrl == CTRL_C_EVENT)
		{
			global_stop = true;
		}

		return TRUE;
	}

	int execute(int argc, char* argv[])
	{
		global_stop = false;

		SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

		if (!global_core.initialize())
		{
			LOG();

			return EXIT_FAILURE;
		}

		while (!global_stop);

		if (!global_core.terminate())
		{
			LOG();

			return EXIT_FAILURE;
		}

		return EXIT_SUCCESS;
	}
}