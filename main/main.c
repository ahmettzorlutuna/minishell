/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:49:57 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/04 17:49:58 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void check_type(const char *path)
{
    struct stat sb;
    if(stat(path, &sb) == -1)
    {
        perror("stat");
        return;
    }
    else
    {
        if(S_ISDIR(sb.st_mode))
            printf("%s bu bir klasör\n", path);
        else if(S_ISREG(sb.st_mode))
            printf("%s bu bir dosya\n", path);
        else if(S_ISLNK(sb.st_mode))
            printf("%s bu bir link\n", path);
    }
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;

    char *rl;
    char *pwd;
    check_type("/home/parallels/Desktop/minishell/main/file.txt");
    check_type("/home/parallels/Desktop/minishell/main");
    while(1)
    {
        rl = readline("bash$ ");
        pwd = getcwd(NULL, 0);
        if (pwd == NULL)
        {
            perror("getcwd");
            free(rl);
            continue;
        }
        if (rl == NULL)
        {
            printf("exit\n");
            break;
        }
        if (*rl)
            add_history(rl);
        printf("Before chdir the path is : %s\n", pwd);
        chdir("/home/parallels/Desktop");
        pwd = getcwd(NULL, 0);
        check_type(pwd);
        printf("Input: %s\n", rl);
        free(rl);
    }
    
    return 0;
}
