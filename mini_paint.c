/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_paint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablondel <ablondel@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 17:19:03 by ablondel          #+#    #+#             */
/*   Updated: 2022/01/09 18:38:33 by ablondel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

typedef struct	s_map
{
	int			w;
	int			h;
	char		c;
	char		map[302][302];
}				t_map;

typedef struct	s_c
{
	char		type;
	float		xpos;
	float		ypos;
	float		rad;
	char		c;
}				t_c;

int    ft_putstr(char *s)
{
    while (*s)
        write(1, s++, 1);
	return (1);
}

void	display(t_map *map)
{
	int i = 0;
	while (i < map->h)
		ft_putstr(map->map[i++]);
}

void	set_map(t_map *map)
{
	int i = -1;
	int j = -1;

	while (++i < map->h)
	{
		while (++j < map->w)
			map->map[i][j] = map->c;
		map->map[i][j] = '\n';
		map->map[i][j + 1] = '\0';
		j = -1;
	}
}

int		background_error(t_map *map)
{
	return (map->w < 1 || map->w > 300 || map->h < 1 || map->h > 300);
}

int		is_in(float x, float y, t_c *c)
{
	return (sqrtf((x - c->xpos) * (x - c->xpos) + (y - c->ypos) * (y - c->ypos)) <= c->rad);
}

int		is_border(float x, float y, t_c *c)
{
	int dist = sqrtf((x - c->xpos) * (x - c->xpos) + (y - c->ypos) * (y - c->ypos));
	if (is_in(x, y, c) && c->rad - 1.0 < dist)
		return (1);
	return (0);
}

void	draw_c(t_map *map, t_c *c)
{
	int i = -1;
	int j = -1;

	while (++i < map->h)
	{
		while (++j < map->w)
		{
			if (c->type == 'c')
			{
				if (is_border(j, i, c) == 1)
					map->map[i][j] = c->c;
			}
			else if (c->type == 'C')
			{
				if (is_in(j, i, c) == 1)
					map->map[i][j] = c->c;
			}
		}
		j = -1;
	}
}

int     main(int ac, char **av)
{
	FILE	*f = NULL;
	t_map	map;
	t_c		c;
	int		background = 0;
	int		new_c = 0;

    if (ac != 2)
		return (ft_putstr("Error: argument\n"));
	f = fopen(av[1], "r");
	if (f == NULL || feof(f))
		return (1);
	background = fscanf(f, "%d %d %c \n", &map.w, &map.h, &map.c);
	if (background != 3 || background_error(&map) == 1)
		return (ft_putstr("Error: Operation file corrupted\n"));
	set_map(&map);
	new_c = fscanf(f, "%c %f %f %f %c \n", &c.type, &c.xpos, &c.ypos, &c.rad, &c.c);
	while (new_c == 5)
	{
		printf("[%d]\n", new_c);
		printf("[%c]\n", c.type);
		printf("[%f]\n", c.xpos);
		printf("[%f]\n", c.ypos);
		printf("[%f]\n", c.rad);
		printf("[%c]\n", c.c);
		if ((c.type != 'c' && c.type != 'C') || (c.rad <= 0.0))
			return (ft_putstr("Error: Operation file corrupted\n"));
		draw_c(&map, &c);
		new_c = fscanf(f, "%c %f %f %f %c \n", &c.type, &c.xpos, &c.ypos, &c.rad, &c.c);
	}
	if (new_c != -1)
		return (ft_putstr("Error: Operation file corrupted\n"));
	display(&map);
	if (fclose(f) != 0)
		return (1);
	return (0);
}
