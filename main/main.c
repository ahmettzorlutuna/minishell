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

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;

    DIR *dir;
    struct dirent *entry;

    int slot = ttyslot();
    if (slot == -1) {
        perror("ttyslot");
    } else {
        printf("Bu terminalin utmp slot numarası: %d\n", slot);
    }
    return 0;
    if (dir == NULL)
    {
        perror("opendir");
        return 1;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        printf("type...: %d | no...: %ld | name...: %s\n", entry->d_type ,entry->d_ino, entry->d_name);
    }
    closedir(dir);
    
    char *rl;
    while(1)
    {
        rl = readline("bash$ ");
        if (rl == NULL)
        {
            printf("exit\n");
            break;
        }
        if (*rl)
            add_history(rl);
        printf("Input: %s\n", rl);
        free(rl);
    }
    
    return 0;
}
