// SPM - SPMoves
// By Jason the Echidna
// MINE! NO STEALY!

#include "../doomdef.h"
#include "../doomstat.h"

#ifdef SPMOVES

#include "SPM_Command.h"

#include "../command.h"
#include "../p_setup.h"
#include "../m_misc.h"
#include "../z_zone.h"
#include "../d_main.h" // pandf

#define NUMSPMCHARS 3
static const char *spmchars[NUMSPMCHARS];
static const char *srb2waddir;

void SPM_InitWads(const char *dir)
{
	if(!dir || !*dir)
		return;

	srb2waddir = dir;
	spmchars[0] = "amy.wad";
	spmchars[1] = "metalsonic.wad";
	//spmchars[2] = "mechaknux.wad";
	spmchars[2] = "shadow.wad";
	//spmchars[4] = "silver.wad";
}

static void SPM_AddChars(void)
{
	int i;

	// If not in a netgame, just load 'em. No problem.
	if(!netgame)
	{
		for(i = 0; i < NUMSPMCHARS; i++)
			if(FIL_ReadFileOK(va(pandf,srb2waddir,spmchars[i])))
				P_AddWadFile(va(pandf,srb2waddir,spmchars[i]), NULL);
		CONS_Printf("*All available SPMoves characters have been loaded.*\n");
		return;
	}

	// If in you're not the server or an admin, you can't do this!
	else if(Playing() && !server && adminplayer != consoleplayer)
	{
		CONS_Printf("Sorry, only the server can do this.\n");
		return;
	}

	// Now addfile everything, fancey MD5 stuffs and network code
	// is in that which I'd rather not replicate here...
	// Though this will set modifiedgame to true... Oh well.
	for(i = 0; i < NUMSPMCHARS; i++)
		if(FIL_ReadFileOK(va(pandf,srb2waddir,spmchars[i])))
			COM_BufAddText(va("addfile \"%s\"",va(pandf,srb2waddir,spmchars[i])));
}

void SPM_InitCommands(void)
{
	COM_AddCommand("spmoves", SPM_AddChars);
}

#endif
