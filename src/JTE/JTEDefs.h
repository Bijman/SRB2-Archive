#ifdef SRB2JTE
// Official edits/additions
#define JOHNNYFUNCODE // WHEE!
//#define CHAOSISNOTDEADYET

// Edits (OF EXISTING THINGS)
#define JTEMISC // Misc. fun stuff. Bunches of small bug fixes and other odditys I have yet to create new groups for.
#define JTEFUNCODE // JOHNNYFUNCODE fixes and that fun object counter thingy. >.> <.<
#define JTEFREEZECODE // Time freezes while in objectplace mode.
#define JTEIGNOREWAD // Alternate version of W_VerifyNMUSlumps which allows things like sprites too.
#define THOKTARGET // MT_THOK takes the form of it's target mobj
#define SUPEREXIT // De-Super when exiting

// Additions
#define JTEBOTS // Bots! w00t!
//#define SUCKYBOTWAYPOINTS // THESE WAYPOINT TEH SUCKS.
#define JTEFLYCHAINS // Sonic Heroes-style flying chains
#define DARKWORLD // Dark World map type
#define SPMOVES // SPMoves character abilities
#define CCPAL // The Chaos Control palette change for the SPMoves ability
//#define UNIVERSE // SRB2 Universe
#define MULTIHPENEMIES // Multiple hit enemies.
#define CENTEREDMENU // M_DrawCenteredMenu for the title screen menu and
					// any other new menus which would look nicer with centering.
					// Doesn't center options or sliders, though, so don't use it on options menus.
//#define SUPERVIRUS0 // Teh original "fair" SRB2 SuperVirus!1 Overrides SUPERVIRUS1
#define SUPERVIRUS1 // Teh SRB2 SuperVirus!1
#define SUPERVIRUS2 // Teh SRB2 SuperVirus!1
#define SUPERVIRUS3 // Replaces the intro wiff my own. >:3
#define SACITY // SA City extentions
#define RUSSIA // Soviet Russia Blast 2!!
#define SPRITESHADOWS // Sprite-based object shadows in OpenGL
//#define ZELDA // TML / LOZSRB2
#define MOBJSCALE // Scale up/down objects
//#define SPRITEOFFS // Speshule sucky hardware sprite effects (Eggmobile)
#ifdef _WINDOWS
#define JTESONGSPEED // FMOD speeding up/down songs
#endif
#define BUGFIXES // Random bug fixes all over the place.
#define HOLIDAYS // Holiday changes!
#endif

#ifdef SPMOVES
	#define HOLDDIST (pickup->radius*2 + mo->radius*2)
	#define TOSSMIN (3*FRACUNIT)
	#define TOSSMAX (24*FRACUNIT)
	#define TOSSMOD (pickup->radius/4)
#endif

#ifdef UNIVERSE
	#define SULOBBYMAP 20
	#define SUHUBMAP 21
#endif

#ifdef ZELDA
	#define HEARTSEGS (2)
	#define HEARTS(x) (x*HEARTSEGS)
#endif

#ifdef SUPERVIRUS0
	#ifndef SUPERVIRUS1
		#define SUPERVIRUS1
	#endif
	#define VIRSPEED 20*FRACUNIT
#elif defined(SUPERVIRUS1)
	#define VIRSPEED 50*FRACUNIT
#endif

#if defined(SUPERVIRUS1) || defined(SUPERVIRUS3)
	// JTERAND turns num into a long between 0 and max, assuming num is a number from 0 to 255.
	#define JTERAND(num,max) ((long)(max)*(long)(num)/256)
#endif

#if defined(SUPERVIRUS1) || defined(SUPERVIRUS2)
	#define SUPERVIRUS
#endif

#ifdef MOBJSCALE
	//#define SCALEVAR // Console variable "scale"
	#define SCALESIZE // Scale the object's height and radius as well as it's sprite.
	//#define SCALEGRAVITY // Scale gravity and jumping
	//#define SCALESPEEDS // Scale mobj speeds as well
	#define SCALEMARIO // Mario mode scaling (Mushroom replaces shields)
	#define SCALEMISC // Scale other things (Shields, sparkles, etc)
	#define SCALEMACS // Scaling macros, required for scaling stuffs.
#endif

#ifdef SCALEMARIO
	#define MARIOSIZE 70
#endif

#ifdef SCALEMACS
	// Scale is an integer percent. So multiply by scale and divide by 100.
	// FLOATSCALE is because OpenGL uses real floating point rather then fixed_t.
	#define FLOATSCALE(x,y) ((y)*(x)/100.0f)
	#define FIXEDSCALE(x,y) FixedDiv(FixedMul((y)<<FRACBITS,(x)),100<<FRACBITS)
	#define FIXEDUNSCALE(x,y) ((y) == 0 ? 0 : FixedDiv(FixedMul((100<<FRACBITS),(x)),(y)<<FRACBITS))
#endif

#if defined(SUPERVIRUS2) || defined(SACITY) || defined(JTEMISC)
	#define A_TORNADOPARTICLE // Tornado particle thinker: Swirls around it's target while flying upwards and outwards.
#endif

#ifdef BUGFIXES
	// Straight fixes
	#define ANTICRASH // Anti-crash code: Trys to fix any otherwise fatal errors it encounters.
					// At this point, it's just an incomplete copy of RANGECHECK which doesn't I_Error.
	#define MOVEFIX // P_Move fixes.
	//#define EXITFIX // A billion extra checks for player->exiting in P_Spec.c so you don't die while exiting!
	#define HOMINGFIX // Fixes homing attack bugs.
	#define SPRITEFIX // Doesn't really affect SRB2 official, makes NUMSPRITES = NULL
					// rather then whatever is right after the last free sprite slot.
	#define SUPERFIX // Fixes Super Sonic coronas
	#define TEETERFIX // Teetering logic errors. :/
	#define HARDWAREFIX // Hardware rendering mode fixes.
	#define HARDWAREFIX2 // Hardware rendering mode colormap fixes...
	#define TRANSFIX // Gets translation table from mobj->skin rather then player->skin. Doesn't affect SRB2 official.
	#define MUSICFIX // When nossmusic and super, invincibility music does not play.
					// However, P_RestoreMusic, called after something like getting an extra life,
					// plays the invincibility music. Thus, I assume that the invincibility music is meant to play when
					// super in a nossmusic map, and have made it do so. :)
					// Also, drowning and joining netgames / spawning set the music to the map music directly, which
					// may not always be perfectly accurate.
	#define SKYSCROLLFIX // Updated F_SkyScroll to deal with ANY size patch and ANY size BASEVIDWIDTH. Mwahahahaha. :3
#endif
