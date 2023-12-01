/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 09:55:17 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/29 09:38:12 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <limits.h>
# include <ctype.h>
# include <errno.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}			t_list;

int		ft_isalpha(int c);
int		ft_isalnum(int c);
int		ft_isdigit(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
int		ft_is_whitespace(char c);
size_t	ft_strlen(char const *s);
size_t	ft_strlcpy(char *dst, char const *src, size_t size);
size_t	ft_strlcat(char *dst, char const *src, size_t size);
char	*ft_strchr(char const *s, int c);
char	*ft_strrchr(char const *s, int c);
int		ft_strncmp(char const *s1, char const *s2, size_t n);
char	*ft_strnstr(const char *big, const char *little, size_t len);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
int		ft_toupper(int c);
int		ft_tolower(int c);
void	ft_bzero(void *s, size_t n);
int		ft_atoi(char const *nptr);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
char	*ft_itoa(int n);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char*s, void (*f)(unsigned int, char*));
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char const *s, int fd);
void	ft_putendl_fd(char const *s, int fd);
void	ft_putnbr_fd(int n, int fd);
void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strdup(const char *s);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstadd_front(t_list **lst, t_list *new);
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstlast(t_list *lst);
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_list	*ft_lstnew(void *content);
int		ft_lstsize(t_list *lst);
long	ft_strtol(const char *str, char **endptr, int base);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_realloc(char *dest, int size);
char	*ft_strcpy(char *dest, const char *src);
char	*ft_strtok(char *str, const char *delimiters);
char	*ft_strcat(char *dest, const char *src);
char	*ft_strtok_r(char *str, const char *delim, char **saveptr);
char	*ft_strpbrk(const char *s1, const char *s2);
int		ft_putenv(char *string);
int		ft_setenv(const char *name, const char *value, int overwrite);
int		ft_unsetenv(const char *name);
void	ft_putchar(char c);
char	*ft_strstr(const char *haystack, const char *needle);
char	*ft_strncpy(char *dest, const char *src, size_t n);
int		ft_isspecialchar(char c);
char	**ft_dupenv(char **environ);
void	ft_putstr(char *str);

#endif
