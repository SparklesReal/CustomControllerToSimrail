Allows you to drive the EP07/EP08 with a steering wheel

For windows you need to set a register due to windows otherwise limiting the program from sending a key input to the game.

[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\System]
"EnableUIPI"=dword:00000000
0 means that it disables UIPI. If you want to learn more please read more here https://en.wikipedia.org/wiki/User_Interface_Privilege_Isolation
In short:
"it prevents processes with a lower "integrity level" (IL) from sending messages to higher IL processes (except for a very specific set of UI messages)."
