Pipes
###################################################################################################

authors:
	Omar Yousef  211869284
	Hasan Yousef 322309220

about the program:
	The goal in this game is to connect all the water receivers to a pumper via pipes so it
	receives water, there game's board contains different types of pumps that can be rotated
	in different angles by clicking on each one. Diffirent levels could be solved by different
	pipes aragments.

Design:
        The game contains a sequence of levels, each level contains a board that could be solved
	in 1 waay or different ways depending on the level's difficulty, the board is made of
	blocks of pipes, there is 6 types of pipes - one of them is the goal and cannot be rotated
	and the others can be rotated so the water reaches to the receiver.
    
About the classes:
        Game:
		This is the main controller class, it contains all the game logic and
		manages all the levels.
	Level:
		Each level contains a board of pipes, this board is read from an external text
		file.
	Pipe:
		Each pipe will be a block in the board (square shaped) and it has a texture suitable
		for the pipe type, and it will show water in it if it is connected to a pumper.
	RotatablePipe:
		a Pipe inherit, but it can aslo be rotated by clicking on it by 90 degrees.
	Receiver:
		a Pipe inherit that it should be connected to a pumper so the player can win.
	Pumper:
		It is a water source that the pipes and receivers can connect to it to receive water.
	Different types of pipes:
		Each pipe class is inherited from the rotatable pipe class and it can connect with
		other pipes in one or more directions (top, right, bottom or left).
	Textures:
		There will be one object of this class and it will contain all the different textures
		for all the pipes types.

Data Structures:
        a 2d board (a vector of vectors of Pipes) for each level.

Notable Algorithms:
	Backtracking path finder - for each receiver, a path finder to a connected pumper.