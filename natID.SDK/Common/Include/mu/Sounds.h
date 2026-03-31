// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/Types.h>
#ifdef MU_WINDOWS
#include "windows.h"
#endif

namespace mu
{
	inline void beep(td::UINT4 freqHz, td::UINT4 durationInMs)
	{
#ifdef MU_WINDOWS
		Beep(freqHz, durationInMs);
#else
		//for (i = 0; i < parms.reps; i++) {                    /* start beep */
			if (ioctl(console_fd, KIOCSOUND, (int)(CLOCK_TICK_RATE / freqHz)) < 0)
			{
				printf("\a");  /* Output the only beep we can, in an effort to fall back on usefulness */
				return;
				//perror("ioctl");
			}
			/* Look ma, I'm not ansi C compatible! */
			usleep(1000 * durationInMs);                          /* wait...    */
			ioctl(console_fd, KIOCSOUND, 0);                    /* stop beep  */
			if (parms.end_delay || (i + 1 < parms.reps))
				usleep(1000 * parms.delay);                        /* wait...    */
	//}
#endif // 

	}
}


