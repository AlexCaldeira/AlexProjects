from search import *
from utils import *


# Problem definition ( AI aproach )
class solitaire(Problem):
    """Models a Solitaire problem as a satisfaction problem.
    A solution cannot have more than 1 peg left on the board."""
    #Doesn't work on an empty board
    def __init__(self, board):
        
        # Creates a 'goal' state.
        # The goal state has a board with one peg left and the problem's board         
        lines = len(board)
        columns = len(board[0])
        entry = [c_empty()]
        goal = []
        
        for i in range(lines):
            new = []
            for j in range(columns):
                new.append(entry)
            goal.append(new)

        self.initial = sol_state(board)        

    def actions(self, state):

        moves = []
        available_moves = board_moves(state.board)
        
        for i in range(len(available_moves)):
            if(len(available_moves[i]) > 1):
                moves.append(available_moves[i])

        return moves             
    
    def result(self, state, action):
        
        final_board = board_perform_move(state.board, action)

        new_state = sol_state(final_board)

        return new_state        
    
    
    def goal_test(self, state):

        return countPieces(state.board) == 1
        
    def path_cost(self, c, state1, action, state2):
        
        # ee chamado no custo uniforme
        # bidirectional
        # A* e A* recursivo
        return c + 1
    
    #Needed for informed search
    def h(self, node):

        #numero de ilhas
        #distancia entre ilhas
        #numero de cantos - done
        return findCorners(node.state)
            


class sol_state(object):
    # Each state has a board
    def __init__(self, b):
        self.board = b

    # less than
    def __lt__(self, state):

        return countPieces(self.board) > countPieces(state.board)

#---------------- TAI content ------------#
def c_peg ():
    return "O"
def c_empty ():
    return "_"
def c_blocked ():
    return "X"
def is_empty (e):
    return e == c_empty()
def is_peg (e):
    return e == c_peg()
def is_blocked (e):
    return e == c_blocked() 
#-----------------------------------------#


#---------------- TAI position ----------------#
# Tuplo (line, column)
def make_position (line , column):
    return (line, column)
def position_line (position):
    return position[0]
def position_column (position):
    return position[1]
def cmp_c(lista1, lista2):
    return lista1[1] == lista2[1]
def cmp_l(lista1, lista2):
    return lista1[0] == lista2[0] 
#-----------------------------------------#


#-------------- TAI move --------------#
# Lista [position_initial, position_final]
def make_move (initial, final):
    return [initial, final]
def move_initial (move):
    return move[0]
def move_final (move):
    return move[1] 
#--------------------------------------#


#------------- TAI board --------------#

#devolve uma lista com todos os movimentos possiveis no tabuleiro


def board_moves(board):
    lines = len(board) - 1
    columns = len(board[0]) - 1
    moves = []
    i = 0
    j = 0 
    
    while i <= lines:
        while j <= columns:
            if (is_blocked(board[i][j]) or is_empty(board[i][j])) : #posicao bloqueada ou vazia
                j = j + 1
        
            elif is_peg(board[i][j]): #tem berlinde
                
                #norte
                if ( (i-2 >=0) and is_empty(board[i-2][j]) and is_peg(board[i-1][j]) ):
                    moves.append([(i, j), (i-2, j)])
                        
                #sul
                if ( (i+2 <= lines) and is_empty(board[i+2][j]) and is_peg(board[i+1][j]) ):
                    moves.append([(i, j), (i+2, j)])
                
                #este
                if ( (j+2 <= columns) and is_empty(board[i][j+2]) and is_peg(board[i][j+1]) ):
                    moves.append([(i, j), (i, j+2)])
                #oeste
                if ( (j-2 >=0) and is_empty(board[i][j-2]) and is_peg(board[i][j-1]) ):
                    moves.append([(i, j), (i, j-2)])
                    
                j = j + 1 
        j = 0
        i = i + 1
                
    return moves
#[(0, 2), (0, 0)]
# move a pecca da posicao inicial para a posicao destino removendo a pecca que se encontrava entre estas duas posicoes
def board_perform_move(board, move):
    initial = move_initial(move)
    destiny = move_final(move)
    i = 0
    j = 0
    lines = len(board) 
    columns = len(board[0])    
    board1 = [[ 0 for x in range(columns)] for y in range(lines)]
    
    
    while i < lines:
        while j < columns:
            if(board[i][j] == c_peg()):
                board1[i][j] = "O"
                j = j+1
            else:
                board1[i][j] = board[i][j]
                j = j+1
        
            
        j = 0   
        i = i+1
    
    #norte 
    if(destiny[0] == (initial[0]-2)):
        #norte
        if (destiny[1] == initial[1]):
            board1[position_line(initial)][position_column(initial)] = "_"
            board1[position_line(initial)-1][position_column(initial)] = "_"
            board1[position_line(destiny)][position_column(destiny)] = "O"         
        
    #sul 
    elif(destiny[0] == (initial[0]+2)):
        #sul
        if (destiny[1] == initial[1]):
            board1[position_line(initial)][position_column(initial)] = "_"
            board1[position_line(initial)+1][position_column(initial)] = "_"
            board1[position_line(destiny)][position_column(destiny)] = "O"
    
    elif(destiny[0] == initial[0]):
        #oeste
        if(destiny[1] == (initial[1]-2)):
            board1[position_line(initial)][position_column(initial)] = "_"
            board1[position_line(initial)][position_column(initial)-1] = "_"
            board1[position_line(destiny)][position_column(destiny)] = "O"      
        #este
        elif(destiny[1] == (initial[1]+2)):
            board1[position_line(initial)][position_column(initial)] = "_"
            board1[position_line(initial)][position_column(initial)+1] = "_"
            board1[position_line(destiny)][position_column(destiny)] = "O"
        
    return board1


def countPieces(board):

    lines = len(board) 
    columns = len(board[0]) 

    c_busy = 0
    for i in range(lines):
        for j in range(columns):
            if(board[i][j] == "O" ):
                c_busy = c_busy + 1

    return c_busy

def findCorners(board):

    lines = len(board) 
    columns = len(board[0])
    contador = 0

    if (board[0][0] == "O"):
        contador = contador + 1
    if (board[0][columns - 1] == "O"):
        contador = contador + 1
    if (board[lines - 1][0] == "O"):
        contador = contador + 1
    if (board[lines - 1][columns - 1] == "O"):
        contador = contador + 1

    return contador



def findIslands(board):

    lines = len(board)
    columns = len(board[0])
    islands = []
    
    for i in range(lines):
        for j in range(columns):

            if (board[i][j] == "O" and (i-1 >=0 and j - 1 >= 0)):

                # se norte tem ilha e oest nao tem
                if (board[i-1][j] == "O" and board[i][j-1] != "O"):
                    updateIslands(islands, i-1, j)
                
                # se norte nao tem ilha e oest tem
                if (board[i-1][j] != "O" and board[i][j-1] == "O"):
                    updateIslands(islands, i, j -1)

                # se norte e oest tem ilha
                if (board[i-1][j] == "O" and board[i][j-1] == "O"):
                    mergeIslands(islands, i-1, j-1)                


           
                
    return islands


def updateIslands(islands, linha, coluna):

    for lista in islands:
        for pos in lista:
            if( cmp_l(pos, (linha, coluna)) and cmp_c(pos, (linha, coluna))):
                lista.append((linha, coluna))
                return



def mergeIslands(lista, l, c):

    temp_lst = []
    
    for islands in lista:
        for pos in islands:
            temp_lst.append(pos)
            temp_lst.append((l, c))

    return 





#----------------------------------------#