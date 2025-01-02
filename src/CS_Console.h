void Command_CSConnect_f(void);
void Command_CSHost_f(void);
void Command_CSPing_f(void);
void Command_CSNodes_f(void);
void Command_CSPlayerlinks(void);
void Command_CSKick_f(void);
void Command_CSBan_f(void);
void Command_CSClearbans_f(void);
void Command_CSStats(void);

#ifndef BUNNIES
void Command_CSDelay_f(void);
#endif

void ClientToClient_OnChange(void);

extern consvar_t cv_clienttoclient;
extern consvar_t cv_safenodes;
extern consvar_t cv_ringmove;
extern consvar_t cv_csdelay;
extern consvar_t cv_cssync;
