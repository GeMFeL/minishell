/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_other_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchakir <jchakir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 22:44:35 by jchakir           #+#    #+#             */
/*   Updated: 2022/04/20 01:07:04 by jchakir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands_executor.h"

char	*ft_get_cmd_full_path(char *path, char *cmd)
{
	char	*path_slash;
	char	*cmd_full_path;
	char	**paths;
	char	*temp_cmd_full_path;

	if (*cmd == '\0')
	{
		put_custom_error("", COMMAND_NOT_FOUND_ERROR);
		exit (127);
	}
	if (ft_strchr(cmd, '/'))
		return (cmd);
	paths = ft_split(path, ':');
	temp_cmd_full_path = NULL;
	while (paths && *paths)
	{
		path_slash = ft_strjoin(*paths, "/");
		cmd_full_path = ft_strjoin(path_slash, cmd);
		free(path_slash);
		if (access(cmd_full_path, F_OK) == 0)
		{
			if (access(cmd_full_path, X_OK) == 0)
				return (cmd_full_path);
			else
			{
				free(temp_cmd_full_path);
				temp_cmd_full_path = cmd_full_path;
			}
		}
		else
			free(cmd_full_path);
		paths++;
	}
	if (temp_cmd_full_path)
		return (temp_cmd_full_path);
	put_custom_error(cmd, COMMAND_NOT_FOUND_ERROR);
	exit (127);
}

static void	execut_command(char *cmd_path, char **args, char **envs, int fds[2])
{
	if (dup2(fds[0], 0) < 0 || dup2(fds[1], 1) < 0)
	{
		custom_msg_then_perror(DUP2_ERROR);
		exit (1);
	}
	if (execve(cmd_path, args, envs) < 0)
	{
		custom_msg_then_perror(args[0]);
		exit(1);
	}
}

void	get_cmd_full_path_then_exec_it(t_cmd_data *cmd_data, int outfd)
{
	char	*cmd_full_path;
	char	**cmd_and_args;
	int		in_out_fds[2];

	in_out_fds[0] = cmd_data->infd;
	in_out_fds[1] = outfd;
	cmd_and_args = get_cmd_and_args__from_component_(cmd_data->component);
	if (*cmd_and_args == NULL)
		exit (0);
	cmd_full_path = ft_get_cmd_full_path(cmd_data->path_env, cmd_and_args[0]);
	execut_command(cmd_full_path, cmd_and_args, cmd_data->envs, in_out_fds);
	exit(0);
}
