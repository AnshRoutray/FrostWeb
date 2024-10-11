"""
Name: FrostWeb.py

Description: 
This script serves as a graphical user interface (GUI) for the FrostWeb Chess Engine, enabling users to play chess against the engine. 
It provides an interactive chessboard with legal move highlighting, sound effects for moves and captures, and handles complex chess rules 
like castling and pawn promotion. 

The GUI is built using the Tkinter library, and it uses Pygame for sound effects. 

Key Features:
- Interactive chessboard display
- Sound effects for various actions (moves, captures, loss)
- Legal move highlighting
- Handling of special moves such as castling and en passant
- Pawn promotion via a simple dialog
- Communication with an external chess engine (main.exe) for move generation

Author: Anshuman Routray

Date: October 11th, 2024

Usage:

To run this script, ensure that the necessary sound files are available in a 'Sounds' directory. Additionally, the executable 'main.exe' 
must be placed in an 'Executable' directory relative to this script. The chessboard can be interacted with by clicking on the pieces 
to move them, with the engine generating responses for the opponent's moves.

"""

import tkinter as tk
import subprocess
import pygame
import time

# Mapping for chess pieces
chess_pieces = {
    1: "♙", 2: "♘", 3: "♗", 4: "♖", 5: "♕", 6: "♔",  
    -1: "♟", -2: "♞", -3: "♝", -4: "♜", -5: "♛", -6: "♚",  
}

pygame.mixer.init()
moveSound = pygame.mixer.Sound("Sounds/move.wav")
captureSound = pygame.mixer.Sound("Sounds/capture.wav")
loserSound = pygame.mixer.Sound("Sounds/Loser.wav")
rageSound = pygame.mixer.Sound("Sounds/RageQuitter.wav")
first_click = None
board = []  # This will be the board state
legal_moves = []  # To store all legal moves for the selected piece

# Function to highlight legal squares
def highlight_legal_moves(canvas, legal_moves, square_size=80):
    for move in legal_moves:
        row, col = move
        x1 = col * square_size
        y1 = row * square_size
        x2 = x1 + square_size
        y2 = y1 + square_size
        piece = board[row][col]
        if piece == 0: 
            canvas.create_oval(
                x1 + square_size * 0.3, y1 + square_size * 0.3,
                x2 - square_size * 0.3, y2 - square_size * 0.3,
                outline="light green", width = 3, tags="highlight"
            )
        else:    
            canvas.create_oval(
                x1 + square_size * 0.1, y1 + square_size * 0.1,
                x2 - square_size * 0.1, y2 - square_size * 0.1,
                outline="light green", width = 4, tags="highlight"
            )

# Function to remove highlights
def remove_highlight(canvas):
    canvas.delete("highlight")

# Function to determine if a move is legal
def is_legal_move(row, col):
    return (row, col) in legal_moves

# Function to get all legal moves for a piece at (row, col)
def get_legal_moves(row, col, board):
    piece = board[row][col]
    moves = []

    # Depending on the piece, generate moves
    if abs(piece) == 1:  
        moves += get_pawn_moves(row, col, piece, board)
    elif abs(piece) == 2:  
        moves += get_knight_moves(row, col, piece, board)
    elif abs(piece) == 3:  
        moves += get_bishop_moves(row, col, piece, board)
    elif abs(piece) == 4: 
        moves += get_rook_moves(row, col, piece, board)
    elif abs(piece) == 5:  
        moves += get_queen_moves(row, col, piece, board)
    elif abs(piece) == 6:  
        moves += get_king_moves(row, col, piece, board)

    return moves

# Helper functions for move generation
def get_pawn_moves(row, col, piece, board):
    moves = []
    direction = -1 if piece == 1 else 1  

    # Normal move
    if board[row + direction][col] == 0:
        moves.append((row + direction, col))
        # Double move on first row
        if (piece == 1 and row == 6) or (piece == -1 and row == 1):
            if board[row + 2 * direction][col] == 0:
                moves.append((row + 2 * direction, col))
    
    # Captures
    for dc in [-1, 1]:
        if 0 <= col + dc < 8 and board[row + direction][col + dc] * piece < 0:
            moves.append((row + direction, col + dc))
        # En passant
        if (board[8][5] == 1 and  
                board[8][3] == row and  
                col + dc == board[8][4]):  
            moves.append((row + direction, col + dc))
    
    return moves

def get_knight_moves(row, col, piece, board):
    moves = []
    knight_offsets = [(-2, -1), (-1, -2), (1, -2), (2, -1), (2, 1), (1, 2), (-1, 2), (-2, 1)]
    for dr, dc in knight_offsets:
        new_row, new_col = row + dr, col + dc
        if 0 <= new_row < 8 and 0 <= new_col < 8 and board[new_row][new_col] * piece <= 0:
            moves.append((new_row, new_col))
    return moves

def get_bishop_moves(row, col, piece, board):
    return get_sliding_moves(row, col, piece, board, [(1, 1), (1, -1), (-1, 1), (-1, -1)])

def get_rook_moves(row, col, piece, board):
    return get_sliding_moves(row, col, piece, board, [(1, 0), (-1, 0), (0, 1), (0, -1)])

def get_queen_moves(row, col, piece, board):
    return get_bishop_moves(row, col, piece, board) + get_rook_moves(row, col, piece, board)

def get_king_moves(row, col, piece, board):
    moves = []
    for dr in [-1, 0, 1]:
        for dc in [-1, 0, 1]:
            if dr != 0 or dc != 0:
                new_row, new_col = row + dr, col + dc
                if 0 <= new_row < 8 and 0 <= new_col < 8 and board[new_row][new_col] * piece <= 0:
                    moves.append((new_row, new_col))

    # Castling
    if board[8][1 if piece > 0 else 0] == 0:  
        if board[row][5] == 0 and board[row][6] == 0:  
            moves.append((row, 6))
        if board[row][1] == 0 and board[row][2] == 0 and board[row][3] == 0: 
            moves.append((row, 2))
    
    return moves

def get_sliding_moves(row, col, piece, board, directions):
    moves = []
    for dr, dc in directions:
        r, c = row + dr, col + dc
        while 0 <= r < 8 and 0 <= c < 8:
            if board[r][c] == 0:
                moves.append((r, c))
            elif board[r][c] * piece < 0:
                moves.append((r, c))
                break
            else:
                break
            r += dr
            c += dc
    return moves

def on_square_click(event, root, square_size = 80):
    global first_click, board, legal_moves
    col = event.x // square_size
    row = event.y // square_size
    
    if row >= 8:
        return  # Ignore clicks on the metadata row

    if first_click is None:
        first_click = (row, col)
        legal_moves = get_legal_moves(row, col, board)
        highlight_legal_moves(canvas, legal_moves, square_size)
    else:
        if not is_legal_move(row, col):
            first_click = None
            legal_moves = []
            remove_highlight(canvas)
            if board[row][col] * board[8][2] > 0:
                first_click = (row, col)
                legal_moves = get_legal_moves(row, col, board)
                highlight_legal_moves(canvas, legal_moves, square_size)
            return
        remove_highlight(canvas)
        start_row, start_col = first_click
        end_row, end_col = row, col
        
        piece = board[start_row][start_col]
        board[start_row][start_col] = 0
        if(board[end_row][end_col] != 0):
            captureSound.play()
        else:
            moveSound.play()
        board[end_row][end_col] = piece
        # Handle castling logic
        if abs(piece) == 6 and abs(start_col - end_col) == 2:  
            if end_col == 6:  
                board[end_row][5] = board[end_row][7]
                board[end_row][7] = 0
            elif end_col == 2:  
                board[end_row][3] = board[end_row][0]
                board[end_row][0] = 0
            
            # Update castling rights
            if piece > 0:
                board[8][1] = 1  
            else:
                board[8][0] = 1  

        # Handle pawn promotion
        if abs(piece) == 1 and (end_row == 0 or end_row == 7):
            promote_pawn(end_row, end_col)
            promotion_window.wait_window()
        # Update castling state for rook movements
        if piece == 4 and start_row == 7:  
            if start_col == 0:
                board[8][1] = 3 if board[8][1] == 0 else 1 
            elif start_col == 7:
                board[8][1] = 2 if board[8][1] == 0 else 1 
        elif piece == -4 and start_row == 0:  
            if start_col == 0:
                board[8][0] = 3 if board[8][0] == 0 else 1  
            elif start_col == 7:
                board[8][0] = 2 if board[8][0] == 0 else 1  

        # Check for double pawn move
        if abs(piece) == 1 and abs(start_row - end_row) == 2:
            board[8][5] = 1
        else:
            board[8][5] = 0

        # Update the metadata 
        board[8][3] = end_row  
        board[8][4] = end_col  
        board[8][2] = -board[8][2]  
        
        drawBoard(canvas, root, board, square_size)
        prevBoard = board
        root.update()

        make_engine_move(root)
        if(prevBoard[board[8][3]][board[8][4]] == 0):
            moveSound.play()
        else:
            captureSound.play()
        
        root.update()

        input_str = "2\n"
        for row in board:
            input_str += " ".join(map(str, row)) + "\n"

        # Call the main.exe executable
        process = subprocess.Popen(
            ["Executable/main.exe"], 
                stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        # Send the board as input to the engine
        stdout_data, stderr_data = process.communicate(input=input_str.strip())
        message = None
        stdout_data = int(stdout_data)
        if stdout_data == -1:
            loserSound.play()
            message = "I WIN :)"
        elif stdout_data == 0:
            message = "DRAW"
        else:
            first_click = None
            return

        time.sleep(1.5)
        # Clear the canvas and display the message
        canvas.delete("all")
        canvas.create_text(320, 320, text=message, font=("Arial", 48), fill = "red", tags = "message")

def promote_pawn(row, col):
    # Create a simple popup window for pawn promotion
    global promotion_window
    promotion_window = tk.Toplevel()
    promotion_window.title("Promote Pawn")

    def select_piece(piece_value, square_size=80):
        board[row][col] = piece_value
        drawBoard(canvas, board, square_size)
        promotion_window.destroy()

    options = [("Queen", 5), ("Rook", 4), ("Bishop", 3), ("Knight", 2)]
    for (text, value) in options:
        btn = tk.Button(promotion_window, text=text, command = lambda v=value: select_piece(v if board[8][2] == 1 else -v))
        btn.pack()

def draw_chessboard(canvas, root, board_size = 8, square_size = 80):
    colors = ["#D2B48C", "#8B4513"]
    for row in range(board_size):
        for col in range(board_size):
            x1 = col * square_size
            y1 = row * square_size
            x2 = x1 + square_size
            y2 = y1 + square_size
            color = colors[(row + col) % 2]
            square = canvas.create_rectangle(x1, y1, x2, y2, fill=color, outline="black")
            canvas.tag_bind(square, "<Button-1>", lambda event, sq_size=square_size: on_square_click(event, root, sq_size))

def drawBoard(canvas, root, board, square_size = 80):
    canvas.delete("piece")
    for row in range(8):
        for col in range(8):
            if(board == 80):
                break
            piece = board[row][col]
            if piece != 0:
                x = col * square_size + square_size // 2
                y = row * square_size + square_size // 2
                piece_text = canvas.create_text(x, y, text=chess_pieces.get(piece, ""), font=("Arial", 32), tags="piece")
                canvas.tag_bind(piece_text, "<Button-1>", lambda event, sq_size=square_size: on_square_click(event, root, sq_size))

def make_engine_move(root):
    global board
    # Convert the current board to the input format for the engine
    input_str = "1\n"
    for row in board:
        input_str += " ".join(map(str, row)) + "\n"

    # Call the main.exe executable
    process = subprocess.Popen(
        ["Executable/main.exe"], 
            stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True
    )
    
    # Send the board as input to the engine
    stdout_data, stderr_data = process.communicate(input=input_str.strip())

    # Parse the output back into a 2D list
    lines = stdout_data.strip().splitlines()
    new_board = [list(map(int, line.split())) for line in lines]

    # Update the board and redraw it
    if len(new_board) == 1 and len(new_board[0]) == 1:
        outcome = new_board[0][0]

        if outcome == -1:
            time.sleep(1.5)
            rageSound.play()
            message = "YOU WIN >:("
        elif outcome == 0:
            message = "DRAW"
        else:
            message = "UNKNOWN OUTCOME"

        # Clear the canvas and display the message
        canvas.delete("all")
        canvas.create_text(320, 320, text=message, font=("Arial", 48), fill = "red", tags = "message")
    else:
        # Update the board and redraw it
        board = new_board
        drawBoard(canvas, root, board, square_size=80)

def main():
    global board, canvas
    root = tk.Tk()
    root.title("Frost Web")

    board_size = 8
    square_size = 80  

    canvas = tk.Canvas(root, width=board_size * square_size, height=(board_size + 1) * square_size)
    canvas.pack()

    draw_chessboard(canvas, root, board_size, square_size)

    # Define a starting chess position with a metadata row
    board = [
        [-4, -2, -3, -5, -6, -3, -2, -4],
        [-1, -1, -1, -1, -1, -1, -1, -1],
        [0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0],
        [1, 1, 1, 1, 1, 1, 1, 1],
        [4, 2, 3, 5, 6, 3, 2, 4],
        [0, 0, 1, -1, -1, 0]  # Metadata row
    ]

    drawBoard(canvas, root, board, square_size)

    root.mainloop()


if __name__ == "__main__":
    main()
