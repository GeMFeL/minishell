/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_and_output_fds_minner.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchakir <jchakir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 20:10:32 by jchakir           #+#    #+#             */
/*   Updated: 2022/03/31 13:01:42 by jchakir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmds_executor.h"

static bool	set_input_fd(int *infd, t_cmd_data *cmd_data)
{
	t_component	*component;

	component = cmd_data->component;
	while (component)
	{
		if (component->type == INFILE || component->type == DELIMITER)
		{
			if (*infd > 2)
				close(*infd);
			if (component->type == INFILE)
				*infd = open(component->final_content, O_RDONLY);
			if (component->type == DELIMITER)
				*infd = ft_get_here_doc(component->final_content);
			if (*infd < 0)
			{
				perror(component->final_content);
				return (false);
			}
		}
		component = component->next;
	}
	return (true);
}

static bool	set_output_fd(int *outfd, t_cmd_data *cmd_data)
{
	t_component	*component;

	component = cmd_data->component;
	while (component)
	{
		if (component->type == OUTFILE || component->type == OUTFILE_APPEND)
		{
			if (*outfd > 2)
				close(*outfd);
			if (component->type == OUTFILE)
				*outfd = open(component->final_content, O_WRONLY | O_TRUNC | O_CREAT, 0644);
			if (component->type == OUTFILE_APPEND)
				*outfd = open(component->final_content, O_WRONLY | O_APPEND | O_CREAT, 0644);
			if (*outfd < 0)
			{
				perror(component->final_content);
				return (false);
			}
		}
		component = component->next;
	}
	return (true);
}

bool	set_input_and_output_fds__minner_(int *infd, int *outfd, t_cmd_data *cmd_data)
{
	*infd = 0;
	*outfd = 1;

	if (set_input_fd(infd, cmd_data) == false)
		return (false);
	if (set_output_fd(outfd, cmd_data) == false)
	{
		if (*infd > 2)
			close(*infd);
		return (false);
	}
	return (true);
}
