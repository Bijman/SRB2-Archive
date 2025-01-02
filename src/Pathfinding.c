#include "g_game.h"
#include "r_state.h"
#include "p_setup.h" // For mapthings.

#include <stdio.h>
#include <math.h>

#include "Pathfinding.h"

#define point_distance(x1, y1, x2, y2) (sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1))))

#define bool char

Line path_mylines[5000];
int  pathlines_count = 0;

Sector mysectors [1000];
Line   mylines   [1000];

Sector* new_mysectors[1000];
int newsector_count = 0;

Node mynodes[30000];
int  node_count = 0;

int sector_count = 0;
int line_count = 0;

int border_x1 = 0, border_y1 = 0, border_x2 = 0, border_y2 = 0, border_floor1 = 80000, border_floor2 = 0;

int global_start = 0;
int global_end = 0;

void World_Nodify();

void PF_LevelInitialise(void)
{
	unsigned int i;

	// Reset the lists - the easy way.
	sector_count    = 0;
	line_count      = 0;
	newsector_count = 0;
	node_count      = 0;

	for (i = 0; i < numsectors; i ++)
	{
		mysectors[i].floor_height   = (int) FIXED_TO_FLOAT(sectors[i].floorheight);
		mysectors[i].ceiling_height = (int) FIXED_TO_FLOAT(sectors[i].ceilingheight);

		if (mysectors[i].floor_height < border_floor1)
			border_floor1 = mysectors[i].floor_height;
		if (mysectors[i].floor_height > border_floor2)
			border_floor2 = mysectors[i].floor_height;
	}

	for (i = 0; i < numlines; i ++)
	{
		int front_id = (((int) lines[i].frontsector) - ((int) sectors)) / sizeof (sector_t);
		int back_id  = (((int) lines[i].backsector ) - ((int) sectors)) / sizeof (sector_t);
		int x1 = (int) FIXED_TO_FLOAT(lines[i].v1->x);
		int y1 = (int) FIXED_TO_FLOAT(lines[i].v1->y);
		int x2 = (int) FIXED_TO_FLOAT(lines[i].v2->x);
		int y2 = (int) FIXED_TO_FLOAT(lines[i].v2->y);

		mylines[i].x1 = x1;
		mylines[i].y1 = y1;
		mylines[i].x2 = x2;
		mylines[i].y2 = y2;

		mylines[i].front_id = front_id;
		mylines[i].back_id = back_id;

		mylines[i].single_sided = 0;
		if (lines[i].sidenum[1] == 0xFFFF)
		{
			mylines[i].single_sided = 1;
			mylines[i].back_id = -1;
		}

		if (mylines[i].x1 < border_x1)
			border_x1 = mylines[i].x1;
		if (mylines[i].y1 < border_y1)
			border_y1 = mylines[i].y1;
		if (mylines[i].x2 > border_x2)
			border_x2 = mylines[i].x2;
		if (mylines[i].y2 > border_y2)
			border_y2 = mylines[i].y2;
	}

	line_count   = (int) numlines;
	sector_count = (int) numsectors;

	CONS_Printf("Copied over %i sectors and %i lines.\n", sector_count, line_count);

	World_Nodify();

	CONS_Printf("Nodified the world about it. (%i nodes were created)\n", node_count);
}

void Trim_Path(Path*);

void Create_Node_Path(int start_x, int start_y, int end_x, int end_y, Path* result)
{
	int i;
	int start_node = -1;
	int end_node   = -1;

	int open_set[10000], closed_set[10000], came_from[10000], heights[10000], flags[10000];
	int openset_length = 1, closedset_length = 0;

    for (i = 0; i < 10000; i ++)
        flags[i] = 0;

	for (i = 0; i < node_count; i ++)
	{
		if (mynodes[i].x == start_x && mynodes[i].y == start_y)
			start_node = i;
		if (mynodes[i].x == end_x && mynodes[i].y == end_y)
			end_node = i;
	}

	if (start_node == -1 || end_node == -1)
	{
		CONS_Printf("Error: Either each or both of the nodes do not exist. (mynodes found: %i, %i)\n", start_node, end_node);
		return;
	}

	if (start_node == end_node)
	{
		CONS_Printf("Hold up, you're already at the end of this path, silly!\n");
		return;
	}

	#define GRAVITY 1f
	#define MAXJUMPHEIGHT 100

    result->node_list = NULL;
    result->list_length = 0;

	open_set[0] = start_node;
	heights[start_node] = mysectors[mynodes[start_node].sector].floor_height;

	while (openset_length)
	{
		int new_openset[10000];
		int new_openset_length = 0;

		int last_openset_length = openset_length;

		for (i = 0; i < last_openset_length; i ++)
		{
			int l;
			int current_node = open_set[i];
            int max_distance   = 64;
            int max_heightdiff = MAXJUMPHEIGHT;

			closed_set[closedset_length ++] = current_node;

            // Look for nearby mapthings. If it's a spring we could be in luck.
            for (l = 0; l < (int) nummapthings; l ++)
            {
                if (point_distance(mynodes[current_node].x, mynodes[current_node].y, mapthings[l].x, mapthings[l].y) < 64)
                {
                    int realtype;
                    for (realtype = 0; realtype < NUMMOBJTYPES; realtype++)
                        if (mapthings[l].type == mobjinfo[realtype].doomednum)
                            break;

                    if (realtype == MT_YELLOWSPRING)
                    {
                        max_heightdiff = 512;
                        max_distance = 640;
                    }
                    if (realtype == MT_REDSPRING)
                    {
                        max_heightdiff = 1024;
                        max_distance = 1024;
                    }
                }
            }

			for (l = 0; l < node_count; l ++)
			{
				int z;
				bool in_closed = 0;
				bool in_open   = 0;

				if (mysectors[mynodes[l].sector].floor_height == mysectors[mynodes[l].sector].ceiling_height)
					continue;
                if (point_distance(mynodes[l].x, mynodes[l].y, mynodes[current_node].x, mynodes[current_node].y) > max_distance)
                    continue;
                if (mysectors[mynodes[l].sector].floor_height - mysectors[mynodes[current_node].sector].floor_height > max_heightdiff)
                    continue;

				for (z = 0; z < closedset_length;   z ++) {if (closed_set[z] == l)  in_closed = 1;}
				for (z = 0; z < openset_length;     z ++) {if (open_set[z] == l)    in_open   = 1;}
				for (z = 0; z < new_openset_length; z ++) {if (new_openset[z] == l) in_open   = 1;}

				if (in_closed || in_open)
					continue;

				new_openset[new_openset_length ++] = l;
				came_from[l] = current_node;

                if (max_distance > 64)
                    flags[current_node] |= 0x10000000; // Remember that the flags aren't for current_node, but for the one preceding it.

				if (l == end_node)
				{
					int current_node = l;

                    CONS_Printf("Creating path...\n");

					while (current_node != start_node)
					{
						result->node_list = (Node**)realloc(result->node_list, (result->list_length + 2) * sizeof (Node*));
						result->node_list[result->list_length] = &mynodes[current_node];

                        result->list_length ++;

						current_node = came_from[current_node];
					}

                    CONS_Printf("Flipping path...\n");

					// Flip the list around.
					Node* temp_list[10000];
					memcpy(temp_list, result->node_list, result->list_length * sizeof (Node*));
					for (z = 0; z < result->list_length; z ++)
						result->node_list[z] = temp_list[(result->list_length - 1) - z];

					CONS_Printf("Path length: %i\n", result->list_length);

                    //Trim_Path(result);

					return;
				}
			}
		}

		memcpy(open_set, new_openset, 10000 * sizeof (int));
		openset_length = new_openset_length;
	}

	CONS_Printf("Path failed.\n");
}

void Nodify_Line_Collision(int x, int y, int* line_list, int linelist_length, int* first_collision, int* second_collision)
{
	int l;
	char collisions_found = 0;

	*first_collision  = -1;
	*second_collision = -1;

	for (l = 0; l < linelist_length; l ++)
	{
		int minx, maxx;
		int miny, maxy;
		char ydir = 1;
		char xdir = 1;
		int i = line_list[l];

		if (mylines[i].x1 > mylines[i].x2)
		{
			minx = mylines[i].x2;
			maxx = mylines[i].x1;
			xdir = ! xdir;
		}
		else
		{
			minx = mylines[i].x1;
			maxx = mylines[i].x2;
			ydir = ! ydir;
		}

		if (x < minx || x > maxx)
			continue;

		if (mylines[i].y1 > mylines[i].y2)
		{
			miny = mylines[i].y2;
			maxy = mylines[i].y1;
			xdir = ! xdir;
		}
		else
		{
			miny = mylines[i].y1;
			maxy = mylines[i].y2;
			ydir = ! ydir;
		}

		if (y < miny || y > maxy)
			continue;

		#define lheight ((maxy - miny))
		#define lwidth  ((maxx - minx))
		#define relx (x - minx)
		#define rely (y - miny)

		int expected_y = (int) (relx * ((double) lheight / lwidth));
		int expected_x = (int) (rely * ((double) lwidth / lheight));

		if (! ydir)
			expected_y = lheight - expected_y;
		if (! xdir)
			expected_x = lwidth - expected_x;

		if (rely == expected_y || relx == expected_x)
		{
			collisions_found ++;

			if (collisions_found == 1)
				*first_collision = i;
			else if (collisions_found == 2)
			{
				*second_collision = i;
				break;
			}
		}
	}

    {
        #undef lwidth
        #undef lheight
        #undef relx
        #undef rely
    }
}

int Lines_Cross(int l1_x1, int l1_y1, int l1_x2, int l1_y2, int l2_x1, int l2_y1, int l2_x2, int l2_y2)
{
	int l1_minx, l1_maxx;
	int l1_miny, l1_maxy;
	int l2_minx, l2_maxx;
	int l2_miny, l2_maxy;
	char ydir = 1;
	char xdir = 1;
#if 0 // CSTODO: Unused (called out just to make cleanup/testing easier)
	char l2_xdir = 1;
	char l2_ydir = 1;
#endif
	int i;

	if (l1_x1 > l1_x2)
	{
		l1_minx = l1_x2;
		l1_maxx = l1_x1;
		xdir = ! xdir;
	}
	else
	{
		l1_minx = l1_x1;
		l1_maxx = l1_x2;
		ydir = ! ydir;
	}

	if (l1_y1 > l1_y2)
	{
		l1_miny = l1_y2;
		l1_maxy = l1_y1;
		xdir = ! xdir;
	}
	else
	{
		l1_miny = l1_y1;
		l1_maxy = l1_y2;
		ydir = ! ydir;
	}

	if (l2_x1 > l2_x2)
	{
		l2_minx = l2_x2;
		l2_maxx = l2_x1;
	}
	else
	{
		l2_minx = l2_x1;
		l2_maxx = l2_x2;
	}

	if (l2_y1 > l2_y2)
	{
		l2_miny = l2_y2;
		l2_maxy = l2_y1;
	}
	else
	{
		l2_miny = l2_y1;
		l2_maxy = l2_y2;
	}

	#define lwidth  ((l1_maxx - l1_minx) + 1)
	#define lheight ((l1_maxy - l1_miny) + 1)
	#define l2width  ((l2_maxx - l2_minx) + 1)
	#define l2height ((l2_maxy - l2_miny) + 1)
	#define relx (x - l1_minx)
	#define rely (y - l1_miny)

	int longest;

	double l2_ystep = (double) l2height / l2width;
	double l2_xstep = (double) l2width / l2height;

	if (l2height > l2width)
	{
		longest = l2height;
		l2_ystep = 1;
		if (l2_x2 < l2_x1)
            l2_xstep = -l2_xstep;
	}
	else
	{
		longest = l2width;
		l2_xstep = 1;
		if (l2_y2 < l2_y1)
            l2_ystep = -l2_ystep;
	}

	for (i = 0; i < longest; i ++)
	{
		int x = l2_x1 + (int) (l2_xstep * i),
			y = l2_y1 + (int) (l2_ystep * i);

		int expected_y = (int) (relx * ((double) lheight / lwidth));
		int expected_x = (int) (rely * ((double) lwidth / lheight));

		if (! ydir)
			expected_y = lheight - expected_y;
		if (! xdir)
			expected_x = lwidth - expected_x;

		if (rely == expected_y || relx == expected_x)
			return 1;
	}

	return 0;

    {
        #undef lwidth
        #undef lheight
        #undef l2width
        #undef l2height
        #undef relx
        #undef rely
    }
}

void Add_Node(int x, int y, int sector)
{
	mynodes[node_count].x          = x;
	mynodes[node_count].y          = y;
	mynodes[node_count].sector     = sector;
	mynodes[node_count].greenlight = 0;
	mynodes[node_count].redlight   = 0;

	node_count ++;
}

int Line_RightHandSector(int line)
{
	char add = 0;

	if (mylines[line].y1 < mylines[line].y2)
		add = 1;

	if (add)
		return mylines[line].front_id;

	if (! mylines[line].single_sided)
		return mylines[line].back_id;
	else
		return -1;
}

void World_Nodify(void)
{
	int x, y, i;
	int line_list[500];
	int linelist_length;

	for (y = border_y1; y <= border_y2; y += 64)
	{
		int currsector = -1;

		linelist_length = 0;

		// Get a list of lines that'll be in our scanning area, for great efficiency!
		for (i = 0; i < line_count; i ++)
		{
			if ((mylines[i].y1 <= y && mylines[i].y2 >= y && mylines[i].y1 <= mylines[i].y2) ||  // <= for straight horizontal lines.
				(mylines[i].y2 <= y && mylines[i].y1 >= y && mylines[i].y1 >  mylines[i].y2))
				line_list[linelist_length ++] = i;
		}

		if (! linelist_length)
			continue; // This really shouldn't happen, but meh. It does.

		for (x = border_x1; x < border_x2; x ++)
		{
			int colliding;
			int alt_colliding;

			Nodify_Line_Collision(x, y, line_list, linelist_length, &colliding, &alt_colliding);

			if (colliding != -1)
			{
				if (mylines[colliding].y1 == mylines[colliding].y2)
				{
					int new_colliding;
					printf("Reached straight horizontal line; skipping.\n");

					do
					{
						if (! (x % 64))
							Add_Node(x, y, mylines[colliding].front_id);

						x ++;
						Nodify_Line_Collision(x, y, line_list, linelist_length, &new_colliding, &alt_colliding);
					} while (new_colliding == colliding && alt_colliding == -1);

					Nodify_Line_Collision(x, y, line_list, linelist_length, &new_colliding, &alt_colliding);

					int joined_line = alt_colliding;

					if (joined_line == colliding)
					{
						joined_line = new_colliding;
						if (new_colliding == colliding)
							printf("THIS SHOULD NEVER HAPPEN!\n");
					}

					if (mylines[joined_line].y1 == mylines[joined_line].y2)
						continue; // Here we go again!

					currsector = Line_RightHandSector(joined_line);
				}

				currsector = Line_RightHandSector(colliding);
			}

			if (currsector != -1)
			{
				if (! (x % 64))
				{
				    if (colliding != -1 && ! mylines[colliding].single_sided) // It'd break if the line were single-sided!
				    {
				        // HACK. What we're doing here is making sure that, if this node is in the middle of a line, that its
				        // sector is set to the highest one next to the line. Because it's harder to jump than fall.
				        int use_sector = mylines[colliding].front_id;

                        if (mysectors[mylines[colliding].back_id].floor_height > mysectors[mylines[colliding].front_id].floor_height)
                            use_sector = mylines[colliding].back_id;

                        Add_Node(x, y, use_sector);

				        continue;
				    }

					Add_Node(x, y, currsector);
				}
			}
		}
	}

	printf("Total nodes: %i\n", node_count);
}

void Trim_Path(Path* path)
{
    int i, line;
    Node** node_list = path->node_list; // For efficienci.

    CONS_Printf("Trimming path...\n");

    for (i = 0; i < path->list_length; i ++)
    {
        int l;
        int best_node = i;

        for (l = path->list_length - 1; l > i; l --)
        {
            boolean across = 0;

            for (line = 0; line < line_count; line ++)
            {
                int line_minx = mylines[line].x1, line_maxx = mylines[line].x2;
                int line_miny = mylines[line].y1, line_maxy = mylines[line].y2;

                if (mylines[line].x1 > mylines[line].x2)
                {
                    line_minx = mylines[line].x2;
                    line_maxx = mylines[line].x1;
                }
                if (mylines[line].y1 > mylines[line].y2)
                {
                    line_miny = mylines[line].y2;
                    line_maxy = mylines[line].y1;
                }

                if ((node_list[i]->x < line_minx && node_list[l]->x < line_minx) ||
                    (node_list[i]->x > line_maxx && node_list[l]->x > line_maxx) ||
                    (node_list[i]->y < line_miny && node_list[l]->y < line_miny) ||
                    (node_list[i]->y > line_maxy && node_list[l]->y > line_maxy))
                    continue;

                if (Lines_Cross(mylines[line].x1, mylines[line].y1, mylines[line].x2, mylines[line].y2,
                                node_list[i]->x, node_list[i]->y, node_list[l]->x, node_list[l]->y))
                {
                    across = 1;
                    break;
                }
            }

            if (across)
                continue;

            best_node = l;
            break;
        }

        CONS_Printf("Removing %i nodes\n", best_node - i);

        int remove_nodes = best_node - i;
        for (l = i; l + remove_nodes < path->list_length; l ++)
        {
            path->node_list[l] = path->node_list[l + remove_nodes];
        }

        path->list_length -= remove_nodes;
    }
}
