#ifndef RUN_COMMAND_H
#define RUN_COMMAND_H

enum {
	ERR_RUN_COMMAND_FORK = 10000,
	ERR_RUN_COMMAND_EXEC,
	ERR_RUN_COMMAND_PIPE,
	ERR_RUN_COMMAND_WAITPID,
	ERR_RUN_COMMAND_WAITPID_WRONG_PID,
	ERR_RUN_COMMAND_WAITPID_SIGNAL,
	ERR_RUN_COMMAND_WAITPID_NOEXIT,
};

struct child_process {
	const char **argv;
	pid_t pid;
	int in;
	int out;
	int err;
	const char *dir;
	const char *const *env;
	unsigned no_stdin:1;
	unsigned no_stdout:1;
	unsigned no_stderr:1;
	unsigned git_cmd:1; /* if this is to be git sub-command */
	unsigned stdout_to_stderr:1;
};

int start_command(struct child_process *);
int finish_command(struct child_process *);
int run_command(struct child_process *);

#define RUN_COMMAND_NO_STDIN 1
#define RUN_GIT_CMD	     2	/*If this is to be git sub-command */
#define RUN_COMMAND_STDOUT_TO_STDERR 4
int run_command_v_opt(const char **argv, int opt);
int run_command_v_opt_cd(const char **argv, int opt, const char *dir);

/*
 * env (the environment) is to be formatted like environ: "VAR=VALUE".
 * To unset an environment variable use just "VAR".
 */
int run_command_v_opt_cd_env(const char **argv, int opt, const char *dir, const char *const *env);

/*
 * The purpose of the following functions is to feed a pipe by running
 * a function asynchronously and providing output that the caller reads.
 *
 * It is expected that no synchronization and mutual exclusion between
 * the caller and the feed function is necessary so that the function
 * can run in a thread without interfering with the caller.
 */
struct async {
	/*
	 * proc writes to fd and closes it;
	 * returns 0 on success, non-zero on failure
	 */
	int (*proc)(int fd, void *data);
	void *data;
	int out;	/* caller reads from here and closes it */
	pid_t pid;
};

int start_async(struct async *async);
int finish_async(struct async *async);

#endif
