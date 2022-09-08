# 보드는 1차원 리스트로 구현한다.
game_board = [' ', ' ', ' ',
              ' ', ' ', ' ',
              ' ', ' ', ' ']

# 비어있는 칸을 찾아서 리스트로 반환한다.
def empty_cells(board):
    cells = []
    for x, cell in enumerate(board):
        if cell == ' ':
            cells.append(x)
    return cells

# 비어있는 칸에는 놓을 수 있다.
def valid_move(x):
    return x in empty_cells(game_board)

# 위치 x에 놓는다.
def move(x, player):
    if valid_move(x):
        game_board[x] = player
        return True
    return False

# 현재 게임 보드를 그린다.
def draw(board):
    for i, cell in enumerate(board):
        if i%3 == 0:
            print('\n----------------')
        print('|', cell , '|', end='')
    print('\n----------------')
    
# 보드의 상태를 평가한다.
def evaluate(board):
    if check_win(board, 'X'):
        score = 1
    elif check_win(board, 'O'):
        score = -1
    else:
        score = 0
    return score

# 1차원 리스트에서 동일한 문자가 수직선이나 수평선, 대각선으로 나타나면
# 승리한 것으로 한다.
def check_win(board, player):
    win_conf = [
        [board[0], board[1], board[2]],
        [board[3], board[4], board[5]],
        [board[6], board[7], board[8]],
        [board[0], board[3], board[6]],
        [board[1], board[4], board[7]],
        [board[2], board[5], board[8]],
        [board[0], board[4], board[8]],
        [board[2], board[4], board[6]],
    ]
    return [player, player, player] in win_conf

# X와 O 중 승리자를 결정한다.
def game_over(board):
    return check_win(board, 'X') or check_win(board, 'O')

# 미니맥스 알고리즘을 구현한다.
# 이 함수는 순환적으로 호출된다.
def minimax(board, depth, maxPlayer):
    pos = -1
    # 단말 노드이면 보드를 평가하여 위치와 평가값을 반환한다.
    if depth == 0 or len(empty_cells(board)) == 0 or game_over(board):
        return -1, evaluate(board)
    
    if maxPlayer:
        value = -10000 # 음의 무한대
        # 자식 노드를 하나씩 평가하여 최선의 수를 찾는다.
        for p in empty_cells(board):
            board[p] = 'X' # 보드의 p 위치에 'X'를 놓는다.
            # 경기자를 교체하여서 imnimax()를 순환호출한다.
            x, score = minimax(board, depth-1, False)
            board[p] = ' ' # 보드는 원 상태로 돌린다.
            if score > value:
                value = score # 최대값을 취한다.
                pos = p # 최대값의 위치를 기억한다.
    else:
        value = +10000 # 양의 무한대
        # 자식 노드를 하나씩 평가하여 최선의 수를 찾는다.
        for p in empty_cells(board):
            board[p] = 'O' # 보드의 p 위치에 'O'를 놓는다.
            
            # 경기자를 교체하여서 imnimax()를 순환호출한다.
            x, score = minimax(board, depth-1, True)
            board[p] = ' ' # 보드는 원 상태로 돌린다.
            if score < value:
                value = score # 최소값을 취한다.
                pos = p # 최소값의 위치를 기억한다.
    return pos, value # 위치와 값을 반환한다.

player='X'
# 메인 프로그램
while True:
    draw(game_board)
    if len(empty_cells(game_board)) == 0 or game_over(game_board):
        break
    i, v = minimax(game_board, 9, player=='X')
    move(i, player)
    if player=='X':
        player='O'
    else:
        player='X'
if check_win(game_board, 'X'):
    print('X 승리!')
elif check_win(game_board, 'O'):
    print('O 승리!')
else:
    print('비겼습니다!')
