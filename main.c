#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"

static const char	*type_name(int t)
{
	if (t == FILE_REDIR) return ("FILE_REDIR");
	if (t == PIPE) return ("PIPE");
	if (t == LOGIC) return ("LOGIC");
	if (t == ARG) return ("ARG");
	return ("UNKNOWN");
}

static void	print_tokens(t_list *lst)
{
	while (lst)
	{
		t_token *tk = (t_token *)lst->content;
		if (tk && tk->str)
			printf("[%s] (%s)\n", tk->str, type_name(tk->type));
		lst = lst->next;
	}
}

int	main(void)
{
	char	*line;
	t_list	*toks;

	while ((line = readline("lex> ")) != NULL)
	{
		if (*line)
			add_history(line);
		toks = tokenizer(line);
		if (toks)
		{
			ft_remove_quotes(toks);
			print_tokens(toks);
			ft_lstclear(&toks, free_cmd_tok);
		}
		free(line);
	}
	return (0);
}
