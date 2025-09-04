# Microsoft Developer Studio Project File - Name="Win32SDL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Win32SDL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Win32SDL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Win32SDL.mak" CFG="Win32SDL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Win32SDL - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Win32SDL - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Win32SDL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\objs\SDL\Release"
# PROP Intermediate_Dir "..\..\objs\SDL\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /GX /Zi /O2 /D "NDEBUG" /D "SDLMAIN" /D "NO_STDIO_REDIRECT" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "SDL" /D "HAVE_MIXER" /D "DIRECTFULLSCREEN" /D "HWRENDER" /D "HW3SOUND" /D "__WIN32__" /D "__MSC__" /D "USEASM" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
# SUBTRACT RSC /x
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"..\..\objs\SDL\Release\DoomLegacy.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 SDL.lib SDL_mixer.lib user32.lib advapi32.lib opengl32.lib glu32.lib wsock32.lib dsound.lib /nologo /subsystem:windows /debug /machine:I386 /out:"c:\srb2demo2\srb2sdl.exe"
# SUBTRACT LINK32 /profile /pdb:none /incremental:yes

!ELSEIF  "$(CFG)" == "Win32SDL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\objs\SDL\Debug"
# PROP Intermediate_Dir "..\..\objs\SDL\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "SDL" /D "HAVE_MIXER" /D "DIRECTFULLSCREEN" /D "HWRENDER" /D "HW3SOUND" /D "__WIN32__" /D "__MSC__" /D "USEASM" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
# SUBTRACT RSC /x
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"..\..\objs\SDL\Debug/wLegacy.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib SDL.lib SDL_mixer.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib opengl32.lib glu32.lib wsock32.lib dsound.lib /nologo /subsystem:console /debug /machine:I386 /out:"c:\srb2demo2\srb2sdldebug.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Win32SDL - Win32 Release"
# Name "Win32SDL - Win32 Debug"
# Begin Group "SDLapp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\dosstr.c
# End Source File
# Begin Source File

SOURCE=.\endtxt.c
# End Source File
# Begin Source File

SOURCE=.\endtxt.h
# End Source File
# Begin Source File

SOURCE=.\filesrch.c
# End Source File
# Begin Source File

SOURCE=.\hwsym_sdl.c
# End Source File
# Begin Source File

SOURCE=.\hwsym_sdl.h
# End Source File
# Begin Source File

SOURCE=.\i_cdmus.c
# End Source File
# Begin Source File

SOURCE=.\i_main.c
# End Source File
# Begin Source File

SOURCE=.\i_net.c
# End Source File
# Begin Source File

SOURCE=.\i_sound.c
# End Source File
# Begin Source File

SOURCE=.\i_system.c
# End Source File
# Begin Source File

SOURCE=.\i_video.c
# End Source File
# Begin Source File

SOURCE=.\IMG_xpm.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ogl_sdl.c
# End Source File
# Begin Source File

SOURCE=.\ogl_sdl.h
# End Source File
# Begin Source File

SOURCE=..\hardware\r_opengl\r_opengl.c
# End Source File
# Begin Source File

SOURCE=..\hardware\r_opengl\r_opengl.h
# End Source File
# Begin Source File

SOURCE=..\hardware\s_ds3d\s_ds3d.c
# End Source File
# Begin Source File

SOURCE=.\SDL_icon.xpm
# End Source File
# Begin Source File

SOURCE=.\SDL_main\SDL_win32_main.c

!IF  "$(CFG)" == "Win32SDL - Win32 Release"

!ELSEIF  "$(CFG)" == "Win32SDL - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\searchp.c
# End Source File
# Begin Source File

SOURCE=.\searchp.h
# End Source File
# Begin Source File

SOURCE=..\win32\win_dbg.c

!IF  "$(CFG)" == "Win32SDL - Win32 Release"

!ELSEIF  "$(CFG)" == "Win32SDL - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\win32\win_dbg.h
# End Source File
# Begin Source File

SOURCE=..\win32\wLegacy.rc
# End Source File
# End Group
# Begin Group "Hardware"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\hardware\hw3dsdrv.h
# End Source File
# Begin Source File

SOURCE=..\hardware\hw3sound.c
# End Source File
# Begin Source File

SOURCE=..\hardware\hw3sound.h
# End Source File
# Begin Source File

SOURCE=..\hardware\hw_bsp.c
# End Source File
# Begin Source File

SOURCE=..\hardware\hw_cache.c
# End Source File
# Begin Source File

SOURCE=..\hardware\hw_data.h
# End Source File
# Begin Source File

SOURCE=..\hardware\hw_defs.h
# End Source File
# Begin Source File

SOURCE=..\hardware\hw_dll.h
# End Source File
# Begin Source File

SOURCE=..\hardware\hw_draw.c
# End Source File
# Begin Source File

SOURCE=..\hardware\hw_drv.h
# End Source File
# Begin Source File

SOURCE=..\hardware\hw_glide.h
# End Source File
# Begin Source File

SOURCE=..\hardware\hw_glob.h
# End Source File
# Begin Source File

SOURCE=..\hardware\hw_light.c
# End Source File
# Begin Source File

SOURCE=..\hardware\hw_light.h
# End Source File
# Begin Source File

SOURCE=..\hardware\hw_main.c
# End Source File
# Begin Source File

SOURCE=..\hardware\hw_main.h
# End Source File
# Begin Source File

SOURCE=..\hardware\hw_md2.c
# End Source File
# Begin Source File

SOURCE=..\hardware\hw_md2.h
# End Source File
# Begin Source File

SOURCE=..\hardware\hw_trick.c
# End Source File
# Begin Source File

SOURCE=..\hardware\hws_data.h
# End Source File
# End Group
# Begin Group "D"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\d_clisrv.c
# End Source File
# Begin Source File

SOURCE=..\d_clisrv.h
# End Source File
# Begin Source File

SOURCE=..\d_englsh.h
# End Source File
# Begin Source File

SOURCE=..\d_event.h
# End Source File
# Begin Source File

SOURCE=..\d_main.c
# End Source File
# Begin Source File

SOURCE=..\d_main.h
# End Source File
# Begin Source File

SOURCE=..\d_net.c
# End Source File
# Begin Source File

SOURCE=..\d_net.h
# End Source File
# Begin Source File

SOURCE=..\d_netcmd.c
# End Source File
# Begin Source File

SOURCE=..\d_netcmd.h
# End Source File
# Begin Source File

SOURCE=..\d_netfil.c
# End Source File
# Begin Source File

SOURCE=..\d_netfil.h
# End Source File
# Begin Source File

SOURCE=..\d_player.h
# End Source File
# Begin Source File

SOURCE=..\d_think.h
# End Source File
# Begin Source File

SOURCE=..\d_ticcmd.h
# End Source File
# End Group
# Begin Group "M"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\m_argv.c
# End Source File
# Begin Source File

SOURCE=..\m_argv.h
# End Source File
# Begin Source File

SOURCE=..\m_bbox.c
# End Source File
# Begin Source File

SOURCE=..\m_bbox.h
# End Source File
# Begin Source File

SOURCE=..\m_cheat.c
# End Source File
# Begin Source File

SOURCE=..\m_cheat.h
# End Source File
# Begin Source File

SOURCE=..\m_fixed.c
# End Source File
# Begin Source File

SOURCE=..\m_fixed.h
# End Source File
# Begin Source File

SOURCE=..\m_menu.c
# End Source File
# Begin Source File

SOURCE=..\m_menu.h
# End Source File
# Begin Source File

SOURCE=..\m_misc.c
# End Source File
# Begin Source File

SOURCE=..\m_misc.h
# End Source File
# Begin Source File

SOURCE=..\m_random.c
# End Source File
# Begin Source File

SOURCE=..\m_random.h
# End Source File
# Begin Source File

SOURCE=..\m_swap.h
# End Source File
# End Group
# Begin Group "P"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\p_ceilng.c
# End Source File
# Begin Source File

SOURCE=..\p_enemy.c
# End Source File
# Begin Source File

SOURCE=..\p_fab.c
# End Source File
# Begin Source File

SOURCE=..\p_floor.c
# End Source File
# Begin Source File

SOURCE=..\p_inter.c
# End Source File
# Begin Source File

SOURCE=..\p_inter.h
# End Source File
# Begin Source File

SOURCE=..\p_lights.c
# End Source File
# Begin Source File

SOURCE=..\p_local.h
# End Source File
# Begin Source File

SOURCE=..\p_map.c
# End Source File
# Begin Source File

SOURCE=..\p_maputl.c
# End Source File
# Begin Source File

SOURCE=..\p_maputl.h
# End Source File
# Begin Source File

SOURCE=..\p_mobj.c
# End Source File
# Begin Source File

SOURCE=..\p_mobj.h
# End Source File
# Begin Source File

SOURCE=..\p_pspr.h
# End Source File
# Begin Source File

SOURCE=..\p_saveg.c
# End Source File
# Begin Source File

SOURCE=..\p_saveg.h
# End Source File
# Begin Source File

SOURCE=..\p_setup.c
# End Source File
# Begin Source File

SOURCE=..\p_setup.h
# End Source File
# Begin Source File

SOURCE=..\p_sight.c
# End Source File
# Begin Source File

SOURCE=..\p_spec.c
# End Source File
# Begin Source File

SOURCE=..\p_spec.h
# End Source File
# Begin Source File

SOURCE=..\p_telept.c
# End Source File
# Begin Source File

SOURCE=..\p_tick.c
# End Source File
# Begin Source File

SOURCE=..\p_tick.h
# End Source File
# Begin Source File

SOURCE=..\p_user.c
# End Source File
# End Group
# Begin Group "R"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\r_bsp.c
# End Source File
# Begin Source File

SOURCE=..\r_bsp.h
# End Source File
# Begin Source File

SOURCE=..\r_data.c
# End Source File
# Begin Source File

SOURCE=..\r_data.h
# End Source File
# Begin Source File

SOURCE=..\r_defs.h
# End Source File
# Begin Source File

SOURCE=..\r_draw.c
# End Source File
# Begin Source File

SOURCE=..\r_draw.h
# End Source File
# Begin Source File

SOURCE=..\r_draw16.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\r_draw8.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\r_local.h
# End Source File
# Begin Source File

SOURCE=..\r_main.c
# End Source File
# Begin Source File

SOURCE=..\r_main.h
# End Source File
# Begin Source File

SOURCE=..\r_plane.c
# End Source File
# Begin Source File

SOURCE=..\r_plane.h
# End Source File
# Begin Source File

SOURCE=..\r_segs.c
# End Source File
# Begin Source File

SOURCE=..\r_segs.h
# End Source File
# Begin Source File

SOURCE=..\r_sky.c
# End Source File
# Begin Source File

SOURCE=..\r_sky.h
# End Source File
# Begin Source File

SOURCE=..\r_splats.c
# End Source File
# Begin Source File

SOURCE=..\r_splats.h
# End Source File
# Begin Source File

SOURCE=..\r_state.h
# End Source File
# Begin Source File

SOURCE=..\r_things.c
# End Source File
# Begin Source File

SOURCE=..\r_things.h
# End Source File
# End Group
# Begin Group "All"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\am_map.c
# End Source File
# Begin Source File

SOURCE=..\am_map.h
# End Source File
# Begin Source File

SOURCE=..\basicinlines.h
# End Source File
# Begin Source File

SOURCE=..\byteptr.h
# End Source File
# Begin Source File

SOURCE=..\command.c
# End Source File
# Begin Source File

SOURCE=..\command.h
# End Source File
# Begin Source File

SOURCE=..\console.c
# End Source File
# Begin Source File

SOURCE=..\console.h
# End Source File
# Begin Source File

SOURCE=..\d_french.h
# End Source File
# Begin Source File

SOURCE=..\d_items.c
# End Source File
# Begin Source File

SOURCE=..\d_items.h
# End Source File
# Begin Source File

SOURCE=..\dehacked.c
# End Source File
# Begin Source File

SOURCE=..\dehacked.h
# End Source File
# Begin Source File

SOURCE=..\doomdata.h
# End Source File
# Begin Source File

SOURCE=..\doomdef.h
# End Source File
# Begin Source File

SOURCE=..\doomstat.h
# End Source File
# Begin Source File

SOURCE=..\doomtype.h
# End Source File
# Begin Source File

SOURCE=..\dstrings.c
# End Source File
# Begin Source File

SOURCE=..\dstrings.h
# End Source File
# Begin Source File

SOURCE=..\f_finale.c
# End Source File
# Begin Source File

SOURCE=..\f_finale.h
# End Source File
# Begin Source File

SOURCE=..\f_wipe.c
# End Source File
# Begin Source File

SOURCE=..\f_wipe.h
# End Source File
# Begin Source File

SOURCE=..\filesrch.h
# End Source File
# Begin Source File

SOURCE=..\g_game.c
# End Source File
# Begin Source File

SOURCE=..\g_game.h
# End Source File
# Begin Source File

SOURCE=..\g_input.c
# End Source File
# Begin Source File

SOURCE=..\g_input.h
# End Source File
# Begin Source File

SOURCE=..\g_state.c
# End Source File
# Begin Source File

SOURCE=..\g_state.h
# End Source File
# Begin Source File

SOURCE=..\gccinlines.h
# End Source File
# Begin Source File

SOURCE=..\hu_stuff.c
# End Source File
# Begin Source File

SOURCE=..\hu_stuff.h
# End Source File
# Begin Source File

SOURCE=..\i_joy.h
# End Source File
# Begin Source File

SOURCE=..\i_net.h
# End Source File
# Begin Source File

SOURCE=..\i_sound.h
# End Source File
# Begin Source File

SOURCE=..\i_sound_sdl.h
# End Source File
# Begin Source File

SOURCE=..\i_system.h
# End Source File
# Begin Source File

SOURCE=..\i_tcp.c
# End Source File
# Begin Source File

SOURCE=..\i_tcp.h
# End Source File
# Begin Source File

SOURCE=..\i_video.h
# End Source File
# Begin Source File

SOURCE=..\info.c
# End Source File
# Begin Source File

SOURCE=..\info.h
# End Source File
# Begin Source File

SOURCE=..\keys.h
# End Source File
# Begin Source File

SOURCE=..\m_swap.c
# End Source File
# Begin Source File

SOURCE=..\md5.c
# End Source File
# Begin Source File

SOURCE=..\md5.h
# End Source File
# Begin Source File

SOURCE=..\mscinlines.h
# End Source File
# Begin Source File

SOURCE=..\mserv.c
# End Source File
# Begin Source File

SOURCE=..\mserv.h
# End Source File
# Begin Source File

SOURCE=..\p5prof.h
# End Source File
# Begin Source File

SOURCE=..\p_doors.c
# End Source File
# Begin Source File

SOURCE=..\p_fab.h
# End Source File
# Begin Source File

SOURCE=..\p_genlin.c
# End Source File
# Begin Source File

SOURCE=..\p_henemy.c
# End Source File
# Begin Source File

SOURCE=..\p_heretic.c
# End Source File
# Begin Source File

SOURCE=..\p_heretic.h
# End Source File
# Begin Source File

SOURCE=..\p_hpspr.c
# End Source File
# Begin Source File

SOURCE=..\p_hsight.c
# End Source File
# Begin Source File

SOURCE=..\p_info.c
# End Source File
# Begin Source File

SOURCE=..\p_info.h
# End Source File
# Begin Source File

SOURCE=..\p_plats.c
# End Source File
# Begin Source File

SOURCE=..\p_pspr.c
# End Source File
# Begin Source File

SOURCE=..\p_switch.c
# End Source File
# Begin Source File

SOURCE=..\qmus2mid.c
# End Source File
# Begin Source File

SOURCE=..\qmus2mid.h
# End Source File
# Begin Source File

SOURCE=..\s_amb.c
# End Source File
# Begin Source File

SOURCE=..\s_sound.c
# End Source File
# Begin Source File

SOURCE=..\s_sound.h
# End Source File
# Begin Source File

SOURCE=..\sb_bar.c
# End Source File
# Begin Source File

SOURCE=..\screen.c
# End Source File
# Begin Source File

SOURCE=..\screen.h
# End Source File
# Begin Source File

SOURCE=..\sounds.c
# End Source File
# Begin Source File

SOURCE=..\sounds.h
# End Source File
# Begin Source File

SOURCE=..\st_lib.c
# End Source File
# Begin Source File

SOURCE=..\st_lib.h
# End Source File
# Begin Source File

SOURCE=..\st_stuff.c
# End Source File
# Begin Source File

SOURCE=..\st_stuff.h
# End Source File
# Begin Source File

SOURCE=..\t_func.c
# End Source File
# Begin Source File

SOURCE=..\t_func.h
# End Source File
# Begin Source File

SOURCE=..\t_oper.c
# End Source File
# Begin Source File

SOURCE=..\t_oper.h
# End Source File
# Begin Source File

SOURCE=..\t_parse.c
# End Source File
# Begin Source File

SOURCE=..\t_parse.h
# End Source File
# Begin Source File

SOURCE=..\t_prepro.c
# End Source File
# Begin Source File

SOURCE=..\t_prepro.h
# End Source File
# Begin Source File

SOURCE=..\t_script.c
# End Source File
# Begin Source File

SOURCE=..\t_script.h
# End Source File
# Begin Source File

SOURCE=..\t_spec.c
# End Source File
# Begin Source File

SOURCE=..\t_spec.h
# End Source File
# Begin Source File

SOURCE=..\t_vari.c
# End Source File
# Begin Source File

SOURCE=..\t_vari.h
# End Source File
# Begin Source File

SOURCE=..\tables.c
# End Source File
# Begin Source File

SOURCE=..\tables.h
# End Source File
# Begin Source File

SOURCE=..\tmap.nas

!IF  "$(CFG)" == "Win32SDL - Win32 Release"

# Begin Custom Build - Compiling with NASM...
IntDir=.\..\..\objs\SDL\Release
InputPath=..\tmap.nas
InputName=tmap

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -o $(IntDir)/$(InputName).obj -f win32 $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "Win32SDL - Win32 Debug"

# Begin Custom Build - Compiling with NASM...
IntDir=.\..\..\objs\SDL\Debug
InputPath=..\tmap.nas
InputName=tmap

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	nasmw -o $(IntDir)/$(InputName).obj -f win32 $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\v_video.c
# End Source File
# Begin Source File

SOURCE=..\v_video.h
# End Source File
# Begin Source File

SOURCE=..\w_wad.c
# End Source File
# Begin Source File

SOURCE=..\w_wad.h
# End Source File
# Begin Source File

SOURCE=..\wi_stuff.c
# End Source File
# Begin Source File

SOURCE=..\wi_stuff.h
# End Source File
# Begin Source File

SOURCE=..\z_zone.c
# End Source File
# Begin Source File

SOURCE=..\z_zone.h
# End Source File
# End Group
# Begin Group "Docs"

# PROP Default_Filter ""
# End Group
# Begin Group "Logs"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=..\win32\srb2.ico
# End Source File
# Begin Source File

SOURCE=.\Win32SDL.ico
# End Source File
# End Target
# End Project
