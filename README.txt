//Brandon Briseno|SID: 1932124|Assignment2: Game of Life
//CPSC350-01(German)
//Program description: this program runs the automation invented by
//		       John Conway called the Game of Life. the Game 
//		       follows 3-4 simple rules, but it is able to
//		       produce complex patterns/shapes. This code
//		       approaches the game by creating 3 arrays:
//                     gameTable(original grid), 
//                     currentArray (current grid),
//                     and nextArray (next grid).
//		       the first array is copied to the second.
//		       then the second and third arrays are copied,
//		       updated, then printed to a console/file.
//Issues: in the case of an infinite automation,
//         this program will infinitely write to a file.
//         this should not cause the CPU to crash. 
//         i couldn't figure out how to robustly detect for 
//         infinite configurations (as this issue is refered
//         to as the halting problem). just press Ctrl+C to 
//         terminate the program.
//Instructions: type "make" to compile the main.cpp and
//              ConwaysGame.cpp and "./Driver.exe" to run!
