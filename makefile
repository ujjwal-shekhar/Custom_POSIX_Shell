main:
	gcc -g main.c prompt.c warpHandler/warp.c peekHandler/peek.c seekHandler/seek.c utils/pathHandler.c utils/flagHandler.c utils/errorHandler.c utils/inputHandlers/commandHandler.c utils/inputHandlers/commandArgsHandler.c utils/inputHandlers/commandExecutor.c pasteventsHandler/pastevents.c systemCommands/backgroundManager/backgroundProcHandler.c 