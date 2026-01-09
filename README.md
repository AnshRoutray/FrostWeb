**FrostWeb**

 **Changing Engine Design:** 
 
 - Redesigning the move generation by using bitboards instead of 2D vectors reducing dynamic memory allocation and making move generation more cache friendly. (**Status:** Completed)
   
 - Using bitwise operations to simplify and optimize move generation as compared to array operations and queries. (**Status:** Completed)

 - Using Pre Computed lookup tables using hardware instructons like PEXT and PDEP as provided by the Intel x86 architecture to ensure O(1) access when generating sliding move for bishops, queens, rooks, and knights. (**Status:** Completed)
   
 - Added playMove and undoMove functions to ensure moves can be undone and thus making it possible to iterate through the game tree recursively using pass by reference (only one board object) instead of pass by value, greatly decreasing stack overhead. (**Status:** Completed)

 - Added proper Unit tests for effective debugging using CMake. (**Status:** Completed)

 -  Redesigning search function to accommodate the new move generation methods. (**Status:** Work in Progress)

 -  Adding SIMD (Single Instruction, Multiple Data) to move generation to ensure move generation is even faster (**Status:** To Be Done)

 -  Adding OpenMP cpu parellization to recursive search of the game tree evaluating multiple nodes at once. (**Status:** To Be Done)

**Description**: FrostWeb is an open source chess engine that plays at an intermediate level. Can beat bots rated around 1000-1200 on chess.com


