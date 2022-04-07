/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd__export_.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchakir <jchakir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 17:46:08 by jchakir           #+#    #+#             */
/*   Updated: 2022/04/05 21:25:40 by jchakir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_commands.h"

static t_env	*get_env_by_its_key(t_env *env, char *key)
{
	while (env)
	{
		if (ft_memcmp(env->key, key, ft_strlen(key) + 1) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

static void	replace_or_add_env__value_key_(t_env **envs, char *key, char *value, int part_counts)
{
	t_env	*env;

	if (part_counts != 1)
		return ;
	if (*key == '\0')
	{
		env_add_back_to_envs(envs, new_env("", value));
		return ;
	}
	env = get_env_by_its_key(*envs, key);
	if (env)
	{
		free(env->value);
		env->value = ft_strdup(value);
	}
	else
		env_add_back_to_envs(envs, new_env(key, value));
}

static void	builtin_cmd__export__print_all_envs(t_env *env, int outfd)
{
	while (env)
	{
		write(outfd, "declare -x ", 11);
		if (*env->key == '\0')
			ft_putstr_fd(env->value, outfd);
		else
		{
			ft_putstr_fd(env->key, outfd);
			write(outfd, "=\"", 2);
			ft_putstr_fd(env->value, outfd);
			write(outfd, "\"", 1);
		}
		write(outfd, "\n", 1);
		env = env->next;
	}
}

static bool	is_this_key_valid_as_var_key__export_(char *str)
{
	if (*str == '\0')
		return (false);
	if (ft_isdigit(*str))
		return (false);
	while (*str)
	{
		if (ft_isalnum(*str) || *str == '_')
			str++;
		else
			return (false);
	}
	return (true);
}

static void	put_not_a_valid_identifier_error(char *str)
{
    ft_putstr_fd(SHELL_NAME, 2);
	ft_putstr_fd("export: `", 2);

    if (str)
    	ft_putstr_fd(str, 2);

	ft_putstr_fd("': not a valid identifier", 2);
    write(2, "\n", 1);
}


void	builtin_cmd__export_(char **args, t_env **envs, int outfd, int parts_count)
{
	char	*equal_index;
	char	*key;

	if (args[0] == NULL)
		builtin_cmd__export__print_all_envs(*envs, outfd);
	while (*args)
	{
		equal_index = ft_strchr(*args, '=');
		if (equal_index == NULL)
		{
			if (is_this_key_valid_as_var_key__export_(*args))
				replace_or_add_env__value_key_(envs, "", *args, parts_count);
			else
				put_not_a_valid_identifier_error(*args);
		}
		else
		{
			key = strdup_from_to__address_(*args, equal_index);
			if (is_this_key_valid_as_var_key__export_(key))
				replace_or_add_env__value_key_(envs, key, equal_index + 1, parts_count);
			else
				put_not_a_valid_identifier_error(*args);
			free(key);
		}
		args++;
	}
}