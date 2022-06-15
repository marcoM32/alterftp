/*
 * A small utility to work on ftp server with Curl
 *
 * Copyright 2016 @gilmoa for the public domain original code
 * Copyright 2022 Marco Magliano for this review of the file
 *
 * This file is part of alterftp#.
 *
 * alterftp# is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * alterftp# is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with alterftp#.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "alterftp.h"

int main(int argc, char *argv[])
{

    if(argc < 2) print_usage(argv[0]), exit(EXIT_SUCCESS);

    char *arg = NULL;

    char *position = calloc(strlen("/") + 1, sizeof(char));
    sprintf(position, "%s", "/");

    char option = '\0';
    while((option = getopt_long(argc, argv, "-:m:r:s:d:g:l::ih", long_options, NULL)) != -1)
    {
        switch(option)
        {
            case 'm':
            case 'r':
            case 's':
            case 'd':
            case 'g':
            case 'l':
                arg = NULL;
                if (optarg)
                {
                    arg = optarg;
                    if(STARTS_WITH(arg, "/"))
                    {
                        arg = (strlen(arg) > 1) ? arg + 1 : NULL;
                    }
                    if(option == 's' && access(arg, F_OK) != 0)
					{
						fprintf(stdout, "ERROR the indicated file not exists\n");
						exit(EXIT_FAILURE);
					}
                }
                struct creds credentials;
                if(!get_credentials(CONF_PATH, &credentials))
                {
                    fprintf(stdout, "ERROR parsing credentials, please run %s --init\n", argv[0]);
                    exit(EXIT_FAILURE);
                }
                do_execute(option, &credentials, arg, position);
                break;
            case 'i':
                do_init(), exit(EXIT_SUCCESS);
                break;
            case 'h':
            case '?':
                print_usage(argv[0]), exit(EXIT_SUCCESS);
                break;
            case 1: // Percorso di lavoro
                if (optarg)
                {
                    if(STARTS_WITH(optarg, "/"))
                    {
                        optarg = (strlen(optarg) > 1) ? optarg + 1 : NULL;
                    }
                    if(optarg)
                    {
                        position = realloc(position, (strlen("/") * 2) + strlen(optarg) + 1);
                        sprintf(position, "/%s/", optarg);
                    }
                }
                break;
        }
    }
    if(position) free(position);
    exit(EXIT_SUCCESS);
}

void print_usage(char *name)
{
    fprintf(stdout, "%-10s%s\n", "Version:", PROGRAM_VERSION);
    fprintf(stdout, "%-14s%s\n", "Source code:", "\"https://www.github.com/marcoM32/alterftp/\"");
    fprintf(stdout, "This project is based on the main idea of @gilmoa, see the original "
            "source on \"https://github.com/gilmoa/alterftp/\"\n");
    fprintf(stdout, "%-8s./%s [work-path] <commands>\n", "Usage:", name);
    fprintf(stdout, "Commands:\n");
    fprintf(stdout, "   %-9s setup login credentials and store it in \"%s\"\n", "-i/--init", CONF_PATH);
    fprintf(stdout, "   [optional path] %-9s [file name] to ftp server\n", "-s/--send");
    fprintf(stdout, "   [optional path] %-9s [file name] from ftp server\n", "-d/--delete");
    fprintf(stdout, "   [optional path] %-9s [file name] from ftp server\n", "-g/--download");
    fprintf(stdout, "   [optional path] %-9s content of server\n", "-l/--list");
    fprintf(stdout, "   [optional path] %-9s [dir name] create directory on server\n", "-m/--mkdir");
    fprintf(stdout, "   [optional path] %-9s [dir name] delete directory on server\n", "-r/--rmdir");
    fprintf(stdout, "   %-9s show this, as any invalid command or syntax\n", "-h/--help");
    fprintf(stdout, "\nThis tool by default works in the root directory \"/\", "
            "if you want refer to other directory put the absolute path of the latter as the first argument.\n");
}

void do_init()
{
    struct creds credentials;
    char c_dir[64];
    if(getcwd(c_dir, sizeof(c_dir)) != NULL)
    {
        fprintf(stdout, "Initializing in '%s'.\n", c_dir);
    }
    else
    {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "%10s: ", "username");
    scanf("%s", credentials.user);

    fprintf(stdout, "%10s: ", "password");
    scanf("%s", credentials.pwd);

    if(!save_credentials(CONF_PATH, credentials))
        exit(EXIT_FAILURE);

    fprintf(stdout, "All set up.\n");
}

void do_execute(char option, struct creds * credentials, char *arg, char *position)
{

    char base_url_adds[] = "ftp://ftp..altervista.org";
    size_t base_path_length = strlen(base_url_adds) + strlen(credentials->user);

    char *base_url = NULL;
    if(arg) {
    	base_url = (char*) calloc((base_path_length + strlen(position) + strlen(arg) + 1), sizeof(char));
    	sprintf(
			base_url,
			"ftp://ftp.%s.altervista.org%s%s",
			credentials->user,
			position,
			arg
		);
	} else {
	    base_url = (char*) calloc((base_path_length + strlen(position) + 1), sizeof(char));
    	sprintf(base_url, "ftp://ftp.%s.altervista.org%s", credentials->user, position);
	}

    char *login = (char*) calloc((strlen(credentials->user) + strlen(credentials->pwd) + strlen(":") + 1), sizeof(char));
    sprintf(login, "%s:%s", credentials->user, credentials->pwd);

    char **commandline = NULL;
    if(option == 'm')
    {
        char *mkdir = (char*) calloc((strlen(position) + strlen(arg) + strlen("MKD ") + 1), sizeof(char));
        sprintf(mkdir, "MKD %s%s", position, arg);
        commandline = (char **) calloc(8, sizeof(char*));
        commandline[0] = "curl";
        commandline[1] = "-u";
        commandline[2] = login;
        commandline[3] = DEFAULT_CURL_FLAGS;
        commandline[4] = "-Q";
        commandline[5] = mkdir;
        commandline[6] = base_url;
        commandline[7] = (char*) NULL;
    }
    else if(option == 'r')
    {
        char *dele = (char*) calloc((strlen(position) + strlen(arg) + strlen("RMD ") + 1), sizeof(char));
        sprintf(dele, "RMD %s%s", position, arg);
        commandline = (char **) calloc(8, sizeof(char*));
        commandline[0] = "curl";
        commandline[1] = "-u";
        commandline[2] = login;
        commandline[3] = DEFAULT_CURL_FLAGS;
        commandline[4] = "-Q";
        commandline[5] = dele;
        commandline[6] = base_url;
        commandline[7] = (char*) NULL;
    }
    else if(option == 'd')
    {
        char *dele = (char*) calloc((strlen(position) + strlen(arg) + strlen("DELE ") + 1), sizeof(char));
        sprintf(dele, "DELE %s%s", position, arg);
        commandline = (char **) calloc(8, sizeof(char*));
        commandline[0] = "curl";
        commandline[1] = "-u";
        commandline[2] = login;
        commandline[3] = DEFAULT_CURL_FLAGS;
        commandline[4] = "-Q";
        commandline[5] = dele;
        commandline[6] = base_url;
        commandline[7] = (char*) NULL;
    }
    else if(option == 's')
    {
        commandline = (char **) calloc(8, sizeof(char*));
        commandline[0] = "curl";
        commandline[1] = "-u";
        commandline[2] = login;
        commandline[3] = DEFAULT_CURL_FLAGS;
        commandline[4] = "-T";
        commandline[5] = arg;
        commandline[6] = base_url;
        commandline[7] = (char*) NULL;
    }
    else if(option == 'g')
    {
        commandline = (char **) calloc(7, sizeof(char*));
        commandline[0] = "curl";
        commandline[1] = "-u";
        commandline[2] = login;
        commandline[3] = DEFAULT_CURL_FLAGS;
        commandline[4] = "-O";
        commandline[5] = base_url;
        commandline[6] = (char*) NULL;
    }
    else if(option == 'l')
    {
        commandline = (char **) calloc(7, sizeof(char*));
        commandline[0] = "curl";
        commandline[1] = "-u";
        commandline[2] = login;
        commandline[3] = DEFAULT_CURL_FLAGS;
        commandline[4] = "-l";
        commandline[5] = base_url;
        commandline[6] = (char*) NULL;
    }

#ifndef _WIN32
    if(fork() == 0)
    {
        curl_cmd(commandline);
    }
    while(wait(NULL) > 0);
    fprintf(stdout, "\033[1;32mDONE.\033[0m\n");
#else
    curl_cmd(commandline);
    printf("DONE\n");
#endif

    if(option == 'm' && option == 'r' && option == 'd')
    {
        if(commandline[5]) free(commandline[5]);
    }

    if(base_url) free(base_url);
    if(login) free(login);
    if(commandline) free(commandline);
}

int curl_cmd(char *commandline[])
{
    int r = 0;
#ifndef _WIN32
    r = execvp("curl", commandline);
#else
    int size = strlen("cmd /C") + 1;
    char *system_command = (char*) calloc(size, sizeof(char));
    strcat(system_command, "cmd /C");
    while(*commandline)
    {
        if(*commandline)
        {
            size = (strlen(system_command) + strlen(" ") + strlen(*commandline) + 1) * sizeof(char);
            realloc(system_command, size);
            strcat(system_command, " ");
            strcat(system_command, *commandline);
        }
        commandline++;
    }
    realloc(system_command, (strlen(system_command) + 1) * sizeof(char));
    system_command[strlen(system_command)] = '\0';
    printf("%s\n", system_command);
    r = system(system_command);
    free(system_command);
#endif
    return r;
}
