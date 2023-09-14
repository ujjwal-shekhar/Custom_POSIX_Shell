main:
	g++ -fsanitize=address -fsanitize=undefined -Wall -g main.c prompt.c warpHandler/warp.c peekHandler/peek.c seekHandler/seek.c utils/pathHandler.c utils/flagHandler.c utils/errorHandler.c utils/inputHandlers/commandHandler.c utils/inputHandlers/commandArgsHandler.c utils/inputHandlers/commandExecutor.c pasteventsHandler/pastevents.c systemCommands/backgroundManager/backgroundProcHandler.c systemCommands/foregroundManager/foregroundProcHandler.c procloreHandler/proclore.c pingHandler/ping.c activitiesHandler/activities.c redirPipeHandler/pipeHandler.c redirPipeHandler/redirectionHandler.c iManHandler/iManHandler.c 