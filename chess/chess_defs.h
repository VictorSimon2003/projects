#define NO_BOARD_COLUMNS 8 // NOT including spaces and letters/numbers
#define NO_BOARD_ROWS 8 // NOT including spaces and letters/numbers


enum squareColor
{
    NONE = 0,
    WHITE = 0x2588,
    BLACK = 0x2800
};
typedef enum playerType_t
{
    COMPUTER = 1,
    USER = 2
}playerType;

typedef enum pieceType_t
{
    PAWN = 1,
    KNIGHT = 2,
    BISHOP = 3,
    ROOK = 4,
    QUEEN = 9, 
    KING = 10
    
}pieceType;

typedef struct positionCoordinates_t
{
    unsigned int x;
    unsigned int y;
} positionCoordinates;

typedef struct chessPiece_t
{
    int imageCode;
    playerType player;
    pieceType type;
    positionCoordinates coordinates;
    bool (* fnValidateChessPieceMove) (struct chessPiece_t*, int, int);
    int (* fnGenerateChessPieceMoves)(struct chessPiece_t*, bool storeComputerMove);
    bool (* fnHasObstacleOnPath) (struct chessPiece_t*, int, int);
         
}chessPiece;

typedef struct square_t
{
    enum squareColor color;
    chessPiece *piece;  
    
} square;

typedef struct move_t
{
    struct chessPiece_t* piece;
    struct positionCoordinates_t moveToCoordinates;
    double shanonRating;  
} move;

typedef struct playerMoves_t
{
    struct move_t moves[1000];
    int noMoves;
} playerMoves;

typedef enum gameLoad_t
{
    DEFAULT_OPTION = 0,
    NEWGAME = 1,
    LOADSAVE = 2
}gameLoad;

typedef enum gameType_t
{
    DEFAULT = 0,
    PlayerVsPlayer = 1,
    PlayerVsComputer = 2
}gameType;

typedef struct game_t
{   
    gameLoad menuGameLoadOption;
    gameType menuGameTypeOption;
    playerType lastPlayerToMove;
}game;

square chessBoard[NO_BOARD_ROWS][NO_BOARD_COLUMNS];
playerMoves computerMoves;
game playGame;


