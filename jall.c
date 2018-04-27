#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<unistd.h>

struct builtin
{
	char *name;
	void (*func)(char **args);
};
void helpme(char **args)
{
	char *help = 
		"Welcome to [J]ust [A]nother She[ll].\n"
		"Commands you can use are: \n"
		"	pwd	: Show current working directory from root.\n"
		"	ls	: List of all directories and files.\n"
		"	echo: You can echo anything.\n"
		"	cd 	: Change directory.\n"
		"	exit : Exit the shell.\n"
		"	clear : clear screen\n"
		"	help : Help text\n"
		"	AND MANY MORE\n";
	printf("%s", help);
}

void exitthis(char **args)
{
	exit(0);
}

void jallcd(char **args)
{
	if(args[1] == NULL)
		fprintf(stderr, "cd: ARGUMENT MISSING\n");
	else
	{
		if(chdir(args[1]) !=0)
			perror("cd");
	}
}
struct builtin builtins[] = {
	{"cd", jallcd},
	{"help", helpme},
	{"exit", exitthis},
}; 
int numberbuiltins()
{
	return sizeof(builtins)/sizeof(struct builtin);
}

void executethis(char **args)
{
	for(int i=0;i<numberbuiltins(); i++)
	{
		if(strcmp(args[0], builtins[i].name)==0)
		{
			builtins[i].func(args);
			return;
		}
	}
	pid_t child_pid = fork();
	if(child_pid == 0)
	{
		execvp(args[0], args);
		perror("Just Another Shell\n");
		exit(1);
	}
	else if(child_pid > 0)
	{
		int status;
		do{
			waitpid(child_pid, &status, WUNTRACED);
		}
		while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	else
	{
		perror("Just Another Shell\n");
	}
}

char **splitthis(char *line)
{
	int len = 0;
	int capa = 16;
	char **tokens = malloc(capa*sizeof(char*));
	char *deli = " \t\r\n";
	char *token = strtok(line, deli);
	while(token !=NULL)
	{
		tokens[len] = token;
		++len;
		if(len>=capa)
		{
			capa = (int)(capa*1.5);
			tokens = realloc(tokens, capa * sizeof(char*));
		}
		token = strtok(NULL, deli);
	} 
	tokens[len] = NULL;
	return tokens;
}
int main()
{
	while(1)
	{
		char *line=NULL;
		char **tokens;
		printf("$> ");
		size_t buflen = 0;
		getline(&line,&buflen,stdin);
		tokens = splitthis(line);
		if(tokens[0] != NULL)
		{
			executethis(tokens);
		}
		free(tokens);
		free(line);
	}
}