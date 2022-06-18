#include "main.h"

/**
 * _getline - read input from standard input
 *
 * @line: input ptr
 * @n: size
 * @stream: File stream
 * Return: number of chars read
 */
ssize_t _getline(char **line, size_t *n, FILE *stream)
{
	return (getdelim(line, n, '\n', stream));
}

/**
 * _execpath - execute path command
 * @token: array of strings
 * @loop: loop number
 * @status: status number
 * Return: exit status
 */
int _execpath(char **token, int loop, int *status)
{
	pid_t pid;
	int wstatus;

	if (!path(token, loop))
		return (1);

	if (!echocheck(token[0]))
	{
		if (!_echo(token, status))
			return (0);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("Error:");
		return (1);
	}
	if (pid == 0)
	{
		if (execve(token[0], token, environ) == -1)
		{
			perror("Error:");
			exit(1);
		}
		exit(0);
	}
	else
	{
		wait(&wstatus);
	}
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	return (1);
}

/**
 * _execbuiltins - check if the command is a built in and execute
 * @tokens: array of strings
 * @stat: status number
 * Return: integer, status vaue
 */
int _execbuiltins(char **tokens, int *stat)
{
	int i, status = 1;
	builtin b[] = {
	    {"cd", _chdir},
	    {"env", env},
	    {"echo", _echo},
	    {"setenv", _setenv},
	    {"unsetenv", _unsetenv},
	    {NULL, NULL}};

	for (i = 0; b[i].name; i++)
	{
		if (_strcmp(b[i].name, tokens[0]) == 0)
		{
			status = (b[i].func)(tokens, stat);
			break;
		}
	}
	return (status);
}

