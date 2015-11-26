/* Under CC0.
 * http://creativecommons.org/publicdomain/zero/1.0/
 * Use as you please.
 */

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	if(argc <= 1) {
		fputs("Error: program to run not specified.\n", stderr);
		return 1;
	}
	for(;;)
	{
		switch(fork())
		{
			case 0:
				execvp(argv[1], argv + 1);

			case -1:
				perror("Fork failed:");
				fputs("Retrying in 3 seconds.\n", stderr);
				sleep(3);
				break;

			default:
				{
					int status;
				keep_waiting:
					if(wait(&status) == -1)
						/* Wait interrupted by signal */
						goto keep_waiting;

					if(!WIFEXITED(status) && !WIFSIGNALED(status))
						/* Process not terminated */
						goto keep_waiting;
				}
		}
	}
}
