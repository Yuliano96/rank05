#ifndef TOOLS_H
# define TOOLS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

# define MAX_SIZE 100

typedef struct s_point
{
	int	x;
	int	y;
	int	flag;
	int	width;
	int	height;
} 	t_point;

t_point	*init_pen(int width, int height);
int		read_commands(char *buffer, size_t size);
void	delete_table(char ***matrix, int rows);
char	**create_table(int rows, int columns);
void	init_table(char **matrix, int rows, int columns);
void	print_table(char **matrix, int rows, int columns);
void	move_position(t_point *pen, int x, int y);
void	draw_table(char **matrix, char *buffer, int size, t_point *pen);
char	**copy_table(char **matrix, int rows, int columns);
void	modify_table(char **old_matrix, char **new_matrix,
			int y, int x, int count);
int		show(int x, int y, char **matrix, int rows, int columns);
int		count_neighbors(char **matrix, int rows, int columns, int y, int x);
int		is_alive(int y, int x, char **matrix);
void	delete_pos(t_point **pen);

#endif
