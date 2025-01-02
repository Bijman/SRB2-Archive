typedef struct
{
	signed short floor_height;
	signed short ceiling_height;
} Sector;

typedef struct
{
	short x1, y1;
	short x2, y2;

	char single_sided;
	int front_id;
	int back_id;
	int new_front_id;
	int new_back_id;
} Line;

typedef struct
{
	int x;
	int y;

	int sector;
	int line; // If it's on a line, it shares two mysectors.

	char greenlight;
	char redlight;
} Node;

typedef struct
{
	Node** node_list;
	int list_length;
} Path;

typedef struct
{
    int x;
    int y;

    int node_id;

    int flags; // Flags that determine how this node will be used.
} PathNode;

extern Line path_mylines[5000];
extern int  pathlines_count;

extern Sector mysectors [1000];
extern Line   mylines   [1000];

extern Sector* new_mysectors[1000];
extern int newsector_count;

extern Node mynodes[30000];
extern int  node_count;

extern int sector_count;
extern int line_count;

extern int border_x1, border_y1,
		   border_x2, border_y2,
		   border_floor1, border_floor2;

extern int global_start;
extern int global_end;

void Create_Node_Path(int start_x, int start_y, int end_x, int end_y, Path* result);
void PF_LevelInitialise(void);
