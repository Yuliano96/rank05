/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yuliano <yuliano@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 19:13:48 by yuliano           #+#    #+#             */
/*   Updated: 2026/07/11 19:38:11 by yuliano          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tools.h"

int main(int argc, char **argv)
{
	int width, height, size;
	t_point *p;
	char buffer[MAX_SIZE];
	char **matriz;
	char **copy_matriz;
	int iterations;
	
	if (argc != 4)
		return 1;
	else
	{
		width = atoi(argv[1]);
		height = atoi(argv[2]);
		iterations = atoi(argv[3]);
		size = read_commands(buffer,sizeof(buffer));

		p = init_pen(width,height);
		matriz = create_table(height,width);
		init_pen(width, height);
		init_table(matriz, height, width);
		draw_table(matriz, buffer, size - 1, p);
	
		if (iterations == 0)
		{
			print_table(matriz, height, width);
			delete_table(&matriz, height);
			delete_pos(&p);
			return 0;	
		}
		for(int i = 0; i < iterations; i++)
		{
			copy_matriz = copy_table(matriz, height, width);
			if(!copy_matriz)
			{
				delete_table(&matriz, height);
				delete_pos(&p);
				return 1;
			}
			int i = 0, j = 0, count;
			while(j < p->height)
			{
				count = count_neighbors(matriz, p->height, p->width, &j, &i);
				modify_table(copy_matriz,j, i, count);
				
			}
			delete_table(&matriz, height);
			matriz = copy_matriz;
		}
		delete_table(&matriz, height);
		delete_pos(&p);
		
	}
	return (0);
}