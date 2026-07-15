#ifndef UTIL_H
# define UTIL_H

# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>

typedef struct s_matriz
{
	char	**matriz;
	int		rows;
	int		columns;
}	t_matriz;

typedef struct s_square
{
	int	x;
	int	y;
	int	area;
}	t_square;

void		free_t_matriz(t_matriz **m);
void		free_matrizChar(char ***matriz, int index);
void		free_matrizInt(int ***m, int index);
t_matriz	*init_matriz(int rows, int columns);
int			read_file(char *filename, t_matriz *matriz);
void		print(t_matriz *matriz);
int			init_square(t_matriz *matriz, char empty, int ***m);
void		print_int(int **m, int rows, int columns);

/*
** Antes large_square estaba declarada como int, pero no devolvía nada.
** Como solo modifica la matriz recibida, debe ser void.
*/
void		large_square(int **m, int rows, int columns);

int			ft_isprint(int c);
int			map_error(char **buffer, FILE *fp);
int			is_valid_map(int *rows, int *columns, char *empty,
				char *obstacle, char *full, char *filename);
void		win_square(int **m, int rows, int columns, t_square *c);
void		draw_square(t_matriz *m, t_square *c, char full);
int	read_stream(FILE *fp, t_matriz **matrix,
		char *empty, char *obstacle, char *full);

#endif