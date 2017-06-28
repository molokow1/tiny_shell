#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define READ_BUFFER_SIZE 1024
#define TOKEN_BUFFER_SIZE 64
#define TOKEN_DELIM " \t\r\n\a"

/* 
Core functions
*/
void sh_loop();

char* sh_read_line();
char** sh_parse_line(char *line);
int sh_execute(char **tokens);
int sh_execute_builtin_cmds(char **cmd);
int sh_launch_proc(char **cmd);
/* 
Built-in commands
*/
int sh_cd(char **cmd);
int sh_ls(char **cmd);
int is_builtin_cmds(char *cmd);
int sh_print_help();

/* 
Helper Functions
*/
void print_tokens(char **tokens);
void print_cwd();

char* sh_read_line(){
	char *line = NULL;
	size_t bufsize = 0;
	getline(&line, &bufsize, stdin);
	return line;

}

char** sh_parse_line(char *line){
	int bufsize = TOKEN_BUFFER_SIZE;
	int token_position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if(!tokens){
		fprintf(stderr,"tsh: allocation error\n");
		exit(EXIT_FAILURE);
	}


	token = strtok(line,TOKEN_DELIM);

	while (token != NULL){
		tokens[token_position] = token;
		token_position++;

		if(token_position >= bufsize){
			bufsize += TOKEN_BUFFER_SIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));

			if (!tokens){
				fprintf(stderr, "tsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL,TOKEN_DELIM);
	}
	tokens[token_position] = NULL;
	return tokens;
}

int sh_execute(char **tokens){
	if(tokens[0] == NULL){
		return 0;
	}


	if(strcmp(tokens[0], "cd") == 0){
		return sh_cd(tokens);
	}else if(strcmp(tokens[0], "quit") == 0){
		return 1;
	}else if(strcmp(tokens[0], "help") == 0){
		return sh_print_help();
	}else{
		return sh_launch_proc(tokens);
	}
	return 0;
}

int sh_launch_proc(char **cmd){
	pid_t pid, wpid;

	pid = fork();
	int status = 0;

	if (pid == 0){
		if (execvp(cmd[0], cmd) == -1){
			perror("tsh");
		}
		exit(EXIT_FAILURE);
	} else if (pid < 0){
		perror("tsh");
	} else {
		while(!WIFEXITED(status) && !WIFSIGNALED(status)){
			wpid = waitpid(pid, &status, WUNTRACED);
		}
	}
	return 0;
}

int sh_cd(char **cmd){
	if(cmd[0] == NULL){
		fprintf(stderr, "tsh: empty arguments for \"cd\"\n");
	} else{
		if(chdir(cmd[1]) != 0){
			perror("tsh");
		}
	}
	return 0;
}

int sh_print_help(){
	printf("Currently Built-in functions are: cd \n");
	printf("However, you can spawn other processes normally\n");
	return 0;
}


void print_tokens(char **tokens){
	int pos = 0;
	printf("Tokens: [");
	while(tokens[pos] != NULL){
		if(tokens[pos + 1] != NULL){
			printf("%s, ",tokens[pos]);
		}else{
			printf("%s",tokens[pos]);
		}
		
		pos++;
	}
	printf("]\n");
}


void print_cwd(){
	char *cwd;
	cwd = malloc(1024 * sizeof(char));
	size_t buf_size = sizeof(char) * 1024;
	if(getcwd(cwd, buf_size) != NULL){
		fprintf(stdout,"%s$ ",cwd);
	} else {
		perror("getcwd() error");
	}
	free(cwd);
}


void sh_loop(){
	int status = 0;
	char *line;
	char **tokens;
	
	while(status == 0){
		//do stuff here
		print_cwd();
		line = sh_read_line();
		// printf("line: %s",line);
		tokens = sh_parse_line(line);
		// print_tokens(tokens);
		status = sh_execute(tokens);

		free(line);
		free(tokens);
	}	
}


int main(int argc, char **argv){
	printf("Tiny Shell opened, type help for instructions\n");
	sh_loop();
	return EXIT_SUCCESS;
}