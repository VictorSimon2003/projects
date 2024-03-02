#include <stdio.h>
#include <stdbool.h> 
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include "chess_defs.h"
#include <ctype.h>
#include <wctype.h>
#include <string.h>


#ifdef _WIN32
    #include <io.h>
#endif

void InitEnvironment ()
{
    #ifdef _WIN32 // Includes both 32 bit and 64 bit
        // set the console on WINDOWS to support UNICODE
        _setmode(_fileno(stdout), 0x00020000); // _O_U16TEXT
    #else
        setlocale (LC_CTYPE, "");
	#endif

}

void flushStdin()
{
    char c; 
    do
    {
        c = getchar();
    } while (c != '\n');
}

bool ValidatePawnMove (chessPiece* piece, int destX, int destY)
{   
    if (piece == NULL) return false;

    int moveUpDown = (piece->player == COMPUTER) ? 1 : -1;
    int firstMoveUpDown = (piece->player == COMPUTER) ? 2 : -2;
    
    //first move: 2 slots forward
    if (((piece->coordinates.y == destY) && (piece->coordinates.x + firstMoveUpDown == destX) && (chessBoard[destX -1][destY -1].piece == NULL))
        && ((piece->coordinates.x == 7) || (piece->coordinates.x == 2))) 
        return true;
    //normal move: one slot forward
    if ((piece->coordinates.y == destY) && (piece->coordinates.x + moveUpDown == destX) && (chessBoard[destX - 1][destY - 1].piece == NULL)) 
        return true;
    //taking left or right
    if((destY == piece->coordinates.y + 1 || destY == piece->coordinates.y - 1) 
        && (destX == piece->coordinates.x + moveUpDown)
        && (chessBoard[destX - 1][destY - 1].piece != NULL)
        && (chessBoard[destX - 1][destY - 1].piece->player != piece->player))
        return true;

    return false;
}
int GeneratePawnMove(chessPiece* piece, bool storeComputerMove)
{
    if((storeComputerMove == true) && (piece->player == COMPUTER)) 
        wprintf(L"Pawn on %c%d\n", 'a' + piece->coordinates.y  - 1, NO_BOARD_ROWS + 1 - piece->coordinates.x);

    int finalCoordX = 0; 
    char finalCoordY = 0;
    int noMoves = 0;
    int moveUpDown = (piece->player == COMPUTER) ? 1 : -1;
    int firstMoveUpDown = (piece->player == COMPUTER) ? 2 : -2;
    // check one step forward (if not end of the range)
    if (piece->coordinates.x < NO_BOARD_ROWS)
    {
       if (ValidatePawnMove(piece, piece->coordinates.x + moveUpDown, piece->coordinates.y))
       {
            noMoves++;
            if((storeComputerMove == true) && (piece->player == COMPUTER))
            {

                finalCoordX = piece->player == COMPUTER ? NO_BOARD_ROWS -  piece->coordinates.x : NO_BOARD_ROWS -  piece->coordinates.x + 2;
                finalCoordY = 'a' + piece->coordinates.y - 1;
                wprintf(L"%c%d\n", finalCoordY, finalCoordX);

                computerMoves.moves[computerMoves.noMoves].piece = piece;
                computerMoves.moves[computerMoves.noMoves].moveToCoordinates.x = piece->coordinates.x + moveUpDown;
                computerMoves.moves[computerMoves.noMoves].moveToCoordinates.y = piece->coordinates.y;
                computerMoves.noMoves++;
            }
            //first move can make to steps ahead
            if (ValidatePawnMove(piece, piece->coordinates.x + firstMoveUpDown, piece->coordinates.y))
            {
                noMoves++;
                if((storeComputerMove == true) && (piece->player == COMPUTER))
                {
                    finalCoordX = piece->player == COMPUTER ?  NO_BOARD_ROWS -  piece->coordinates.x - 1 : NO_BOARD_ROWS -  piece->coordinates.x + 3;
                    finalCoordY = 'a' + piece->coordinates.y - 1;
                    wprintf(L"%c%d\n", finalCoordY, finalCoordX);

                    computerMoves.moves[computerMoves.noMoves].piece = piece;
                    computerMoves.moves[computerMoves.noMoves].moveToCoordinates.x = piece->coordinates.x + firstMoveUpDown;
                    computerMoves.moves[computerMoves.noMoves].moveToCoordinates.y = piece->coordinates.y;
                    computerMoves.noMoves++;                    
                }
            }  
       } 
    }

    // check left and right moves
    // LEFT MOVE
    if (piece->coordinates.y > 1 && piece->coordinates.x < NO_BOARD_ROWS
        && chessBoard[piece->coordinates.x][piece->coordinates.y - 2].piece != NULL
        && chessBoard[piece->coordinates.x][piece->coordinates.y - 2].piece->player != piece->player)
    {
        noMoves++;

        if((storeComputerMove == true) && (piece->player == COMPUTER))
        {
            finalCoordX = NO_BOARD_ROWS -  piece->coordinates.x;
            finalCoordY = 'a' + piece->coordinates.y - 2;
            wprintf(L"%c%d\n", finalCoordY, finalCoordX);
            
            computerMoves.moves[computerMoves.noMoves].piece = piece;
            computerMoves.moves[computerMoves.noMoves].moveToCoordinates.x = piece->coordinates.x + 1;
            computerMoves.moves[computerMoves.noMoves].moveToCoordinates.y = piece->coordinates.y - 1;
            computerMoves.noMoves++;
        }
        
    }

   // RIGHT MOVE
    if (piece->coordinates.y < NO_BOARD_COLUMNS && piece->coordinates.x < NO_BOARD_ROWS
        && chessBoard[piece->coordinates.x][piece->coordinates.y].piece != NULL
        && chessBoard[piece->coordinates.x][piece->coordinates.y].piece->player != piece->player)
    {
        if((storeComputerMove == true) && (piece->player == COMPUTER))
        {
            finalCoordX = NO_BOARD_ROWS -  piece->coordinates.x;
            finalCoordY = 'a' + piece->coordinates.y;
            noMoves++;
            wprintf(L"%c%d\n", finalCoordY, finalCoordX);

            computerMoves.moves[computerMoves.noMoves].piece = piece;
            computerMoves.moves[computerMoves.noMoves].moveToCoordinates.x = piece->coordinates.x + 1;
            computerMoves.moves[computerMoves.noMoves].moveToCoordinates.y = piece->coordinates.y + 1;
            computerMoves.noMoves++;
        }
    }
    return noMoves;
}

bool ValidateKnightMove (chessPiece* piece, int destX, int destY)
{
    if (piece == NULL) return false;

    if(     (((abs(piece->coordinates.x - destX) == 1) && (abs(piece->coordinates.y - destY) == 2))
        ||  ((abs(piece->coordinates.x - destX) == 2) && (abs(piece->coordinates.y - destY) == 1)))
        &&  (chessBoard[destX-1][destY-1].piece == NULL || chessBoard[destX-1][destY-1].piece->player != piece->player))
        return true;
    
    return false;
    
}
int GenerateKnightMove(chessPiece* piece, bool storeComputerMove)
{
    if((storeComputerMove == true) && (piece->player == COMPUTER))
        wprintf(L"Knight on %c%d\n", 'a' + piece->coordinates.y  - 1, NO_BOARD_ROWS + 1 - piece->coordinates.x);

    int finalCoordX = 0; 
    char finalCoordY = 0;
    int noMoves = 0;
    //boundries for both x, y min and max
    int lastXCoordinates = ((piece->coordinates.x + 2) > NO_BOARD_ROWS) ? NO_BOARD_ROWS : (piece->coordinates.x + 2);
    int lastYCoordinates = ((piece->coordinates.y + 2) > NO_BOARD_ROWS) ? NO_BOARD_COLUMNS : (piece->coordinates.y + 2);
    int xIdxCoordinates = (piece->coordinates.x - 1) <= 1 ? 0 : (piece->coordinates.x - 3);
    for( ; xIdxCoordinates < lastXCoordinates; xIdxCoordinates++)
    {
        int yIdxCoordinates = (piece->coordinates.y - 1) <= 1 ? 0 : (piece->coordinates.y - 3);
       for( ; yIdxCoordinates < lastYCoordinates; yIdxCoordinates++)
        {
            if((yIdxCoordinates != (piece->coordinates.y - 1))  && (xIdxCoordinates != (piece->coordinates.x - 1)))
            {
                if(ValidateKnightMove(piece, xIdxCoordinates + 1, yIdxCoordinates +1) == true)
                {
                    noMoves++;

                    if((storeComputerMove == true) && (piece->player == COMPUTER))
                    {
                        finalCoordX = NO_BOARD_ROWS - xIdxCoordinates;
                        finalCoordY = 'a' + yIdxCoordinates;
                        wprintf(L"%c%d\n", finalCoordY, finalCoordX);

                        computerMoves.moves[computerMoves.noMoves].piece = piece;
                        computerMoves.moves[computerMoves.noMoves].moveToCoordinates.x = xIdxCoordinates + 1;
                        computerMoves.moves[computerMoves.noMoves].moveToCoordinates.y = yIdxCoordinates + 1;
                        computerMoves.noMoves++;                    
                    }
                }
            }
        }
    } 
    return noMoves;
}

bool BishopHasObstacleOnPath(struct chessPiece_t* piece, int xCoordinates, int yCoordinates)
{
    int iteratorX, xstep = ((piece->coordinates.x < xCoordinates) ? 1 : -1), 
        iteratorY, ystep = ((piece->coordinates.y < yCoordinates) ? 1 : -1);
        iteratorX = xstep;
        iteratorY = ystep; 
    
    while((iteratorY + piece->coordinates.y) != (yCoordinates)) 
    {
        if(chessBoard[iteratorX + piece->coordinates.x - 1][iteratorY + piece->coordinates.y - 1].piece != NULL)
        {
            return true;
        }
        iteratorX += xstep;
        iteratorY += ystep;
    }

    return false;
    
}
bool ValidateBishopMove (chessPiece* piece, int destX, int destY)
{
    if (piece == NULL) return false;

    if((abs(piece->coordinates.x - destX)) == (abs(piece->coordinates.y - destY))) 
    {
        return (!BishopHasObstacleOnPath(piece, destX, destY)
                && (chessBoard[destX-1][destY-1].piece == NULL || piece->player != chessBoard[destX-1][destY-1].piece->player)); 
    }

  return false;
}
int GenerateBishopMove(chessPiece* piece, bool storeComputerMove)
{
    if((storeComputerMove == true) && (piece->player == COMPUTER))
        wprintf(L"Bishop on %c%d\n", 'a' + piece->coordinates.y  - 1, NO_BOARD_ROWS + 1 - piece->coordinates.x);

    int finalCoordX = 0;
    char finalCoordY = 0;
    int noMoves = 0;
    for(int xIdxCoordinates = 0; xIdxCoordinates < NO_BOARD_ROWS; xIdxCoordinates++)
    {
        for(int yIdxCoordinates = 0; yIdxCoordinates < NO_BOARD_COLUMNS; yIdxCoordinates++)
        {
            //not own position
            if((yIdxCoordinates != (piece->coordinates.y - 1)) && (xIdxCoordinates != (piece->coordinates.x - 1)))
            {
                if(ValidateBishopMove(piece, xIdxCoordinates + 1, yIdxCoordinates + 1)  == true)
                {
                    noMoves++;

                    if((storeComputerMove == true) && (piece->player == COMPUTER))
                    {
                        finalCoordX = NO_BOARD_ROWS -  xIdxCoordinates;
                        finalCoordY = 'a' + yIdxCoordinates;
                        wprintf(L"%c%d\n", finalCoordY, finalCoordX);

                        computerMoves.moves[computerMoves.noMoves].piece = piece;
                        computerMoves.moves[computerMoves.noMoves].moveToCoordinates.x = xIdxCoordinates + 1;
                        computerMoves.moves[computerMoves.noMoves].moveToCoordinates.y = yIdxCoordinates + 1;
                        computerMoves.noMoves++;                    
                    }

                }
                
            }
        }
    }
    return noMoves;
}

bool RookHasObstacleOnPath(struct chessPiece_t* piece, int xCoordinates, int yCoordinates)
{
    int step = 0;
    int iterator = 0;
    //case column is changing
    if(piece->coordinates.x == xCoordinates)
    {
        iterator = step = ((piece->coordinates.y < yCoordinates) ? 1 : -1);
        while(abs(iterator + piece->coordinates.y) != (yCoordinates))
        {
            if(chessBoard[piece->coordinates.x - 1][iterator + piece->coordinates.y - 1].piece != NULL)
            {
                return true;
            }
            iterator += step;
        }
    }
    //case x
    else if(piece->coordinates.y == yCoordinates)
    {
        iterator = step = ((piece->coordinates.x < xCoordinates) ? 1 : -1);
        while(abs(iterator + piece->coordinates.x) != (xCoordinates))
        {
            if(chessBoard[iterator + piece->coordinates.x - 1][piece->coordinates.y - 1].piece != NULL)
            {
                return true;
            }
            iterator += step;
        }
    }
        
    return false;
}
bool ValidateRookMove (chessPiece* piece, int destX, int destY)
{
    if (piece == NULL) return false;

    if((piece->coordinates.x == destX) || (piece->coordinates.y == destY))
    {
        return (    !RookHasObstacleOnPath(piece, destX, destY)
                &&  (chessBoard[destX-1][destY-1].piece == NULL || piece->player != chessBoard[destX-1][destY-1].piece->player));// not friendly fire;
    }
    return false;
}
int GenerateRookMove(chessPiece* piece, bool storeComputerMove)
{
    if((storeComputerMove == true) && (piece->player == COMPUTER))
        wprintf(L"Rook on %c%d\n", 'a' + piece->coordinates.y  - 1, NO_BOARD_ROWS + 1 - piece->coordinates.x);

    int finalCoordX = 0; 
    char finalCoordY = 0;
    int noMoves = 0;

    for(int xIdxCoordinates = 0; xIdxCoordinates < NO_BOARD_ROWS; xIdxCoordinates++)
    {
        if(xIdxCoordinates != (piece->coordinates.x - 1))  
        {
            if(ValidateRookMove(piece, xIdxCoordinates + 1, piece->coordinates.y) == true)
            {
                noMoves++;
                if((storeComputerMove == true) && (piece->player == COMPUTER))
                {
                    finalCoordX = NO_BOARD_ROWS - xIdxCoordinates;
                    finalCoordY ='a' + piece->coordinates.y - 1;
                    wprintf(L"%c%d\n", finalCoordY, finalCoordX);

                    computerMoves.moves[computerMoves.noMoves].piece = piece;
                    computerMoves.moves[computerMoves.noMoves].moveToCoordinates.x = xIdxCoordinates + 1;
                    computerMoves.moves[computerMoves.noMoves].moveToCoordinates.y = piece->coordinates.y;
                    computerMoves.noMoves++;
                    
                }

            }
        }
    }
    for(int yIdxCoordinates = 0; yIdxCoordinates < NO_BOARD_COLUMNS; yIdxCoordinates++)
    {
        if(yIdxCoordinates != (piece->coordinates.y - 1))  
        {
            if(ValidateRookMove(piece, piece->coordinates.x,  yIdxCoordinates + 1) == true)
            {
                noMoves++;
                if((storeComputerMove == true) && (piece->player == COMPUTER))
                {
                    finalCoordX = NO_BOARD_ROWS -  piece->coordinates.x - 1;
                    finalCoordY = 'a' + yIdxCoordinates;
                    wprintf(L"%c%d\n", finalCoordY, finalCoordX);

                    computerMoves.moves[computerMoves.noMoves].piece = piece;
                    computerMoves.moves[computerMoves.noMoves].moveToCoordinates.x = piece->coordinates.x;
                    computerMoves.moves[computerMoves.noMoves].moveToCoordinates.y = yIdxCoordinates + 1;
                    computerMoves.noMoves++;
                    
                }
            }
        }
    }
    return noMoves;
}

bool QueenHasObstacleOnPath(struct chessPiece_t* piece, int xCoordinates, int yCoordinates)
{
    if((piece->coordinates.x == xCoordinates) || (piece->coordinates.y == yCoordinates))
    {
        return RookHasObstacleOnPath(piece, xCoordinates, yCoordinates);
    }
    else
    {
        return BishopHasObstacleOnPath(piece, xCoordinates, yCoordinates);
    }
}
bool ValidateQueenMove (chessPiece* piece, int destX, int destY)
{   
    if (piece == NULL) return false;

    if((abs(piece->coordinates.x - destX)) == (abs(piece->coordinates.y - destY))
        || (piece->coordinates.x == destX) || (piece->coordinates.y == destY))
        return (!QueenHasObstacleOnPath(piece, destX, destY) && (chessBoard[destX-1][destY-1].piece == NULL || piece->player != chessBoard[destX-1][destY-1].piece->player));

    return false;
}
int GenerateQueenMove(chessPiece* piece, bool storeComputerMove)
{
    int finalCoordX = 0;
    char finalCoordY = 0;
    int noMoves = 0;
    if((storeComputerMove == true) && (piece->player == COMPUTER))
        wprintf(L"Queen on %c%d\n", 'a' + piece->coordinates.y  - 1, NO_BOARD_ROWS + 1 - piece->coordinates.x);

    for(int xIdxCoordinates = 0; xIdxCoordinates < NO_BOARD_ROWS; xIdxCoordinates++)
    {
       for(int yIdxCoordinates = 0; yIdxCoordinates < NO_BOARD_COLUMNS; yIdxCoordinates++)
        {
            if((yIdxCoordinates != (piece->coordinates.y - 1))  && (xIdxCoordinates != (piece->coordinates.x - 1)))
            {
                
                if(ValidateQueenMove(piece, xIdxCoordinates + 1, yIdxCoordinates + 1) == true)
                {
                    noMoves++;
                    if((storeComputerMove == true) && (piece->player == COMPUTER))
                    {   
                        finalCoordX = NO_BOARD_ROWS - xIdxCoordinates;
                        finalCoordY = 'a' + yIdxCoordinates;
                        wprintf(L"%c%d\n", finalCoordY, finalCoordX);

                        computerMoves.moves[computerMoves.noMoves].piece = piece;
                        computerMoves.moves[computerMoves.noMoves].moveToCoordinates.x = xIdxCoordinates + 1;
                        computerMoves.moves[computerMoves.noMoves].moveToCoordinates.y = yIdxCoordinates + 1;
                        computerMoves.noMoves++;
                    
                    }
                }
                
            }
        }
    }
    return noMoves;
}

bool KingEntersCheck(chessPiece* piece, int destX, int destY)
{
    for(int xIdxcoordinates = 0; xIdxcoordinates < NO_BOARD_ROWS; xIdxcoordinates++)
    {
        for(int yIdxcoordinates = 0; yIdxcoordinates < NO_BOARD_COLUMNS; yIdxcoordinates++)
        {
            if(chessBoard[xIdxcoordinates][yIdxcoordinates].piece != NULL && chessBoard[xIdxcoordinates][yIdxcoordinates].piece->player != piece->player)
            {
                if (chessBoard[xIdxcoordinates][yIdxcoordinates].piece->fnValidateChessPieceMove(chessBoard[xIdxcoordinates][yIdxcoordinates].piece, destX, destY) == true)
                {
                    wprintf (L"Entering check position.\n");
                    return true;
                }
            }   
        }
    }
    return false;
}
bool ValidateKingMove (chessPiece* piece, int destX, int destY)
{
    if (piece == NULL) return false;

    //check against friendly fire
    if (chessBoard[destX-1][destY-1].piece != NULL && piece->player == chessBoard[destX-1][destY-1].piece->player)
        return false;

    if(abs(piece->coordinates.x - destX) <= 1 && abs(piece->coordinates.y - destY) <= 1 && KingEntersCheck(piece, destX, destY) == false) 
        return true;
    
    return false;
}
int GenerateKingMove(chessPiece* piece, bool storeComputerMove)
{
    if((storeComputerMove == true) && (piece->player == COMPUTER))
        wprintf(L"King on %c%d\n", 'a' + piece->coordinates.y  - 1, NO_BOARD_ROWS + 1 - piece->coordinates.x);

    int finalCoordX = 0; 
    char finalCoordY = 0;
    int noMoves = 0;
    for(int xCoordinates = piece->coordinates.x-1; xCoordinates <= piece->coordinates.x+1; xCoordinates++)
    {
        if(xCoordinates == NO_BOARD_ROWS + 1) break;
        if(xCoordinates == 0) continue;
       for(int yCoordinates = piece->coordinates.y - 1; yCoordinates <= piece->coordinates.y+1; yCoordinates++)
        {
            if(yCoordinates == 0) continue;
            if(yCoordinates == NO_BOARD_COLUMNS + 1) break;

            // current position. skip it
            if((yCoordinates == piece->coordinates.y) && (xCoordinates == piece->coordinates.x))
                continue;

            if(ValidateKingMove(piece, xCoordinates, yCoordinates) == true)
            {
                noMoves++;

                if((storeComputerMove == true) && (piece->player == COMPUTER))
                {   
                    finalCoordX = NO_BOARD_ROWS -  xCoordinates + 1;
                    finalCoordY = 'a' + yCoordinates;
                    wprintf(L"%c%d\n", finalCoordY, finalCoordX);

                    computerMoves.moves[computerMoves.noMoves].piece = piece;
                    computerMoves.moves[computerMoves.noMoves].moveToCoordinates.x = xCoordinates;
                    computerMoves.moves[computerMoves.noMoves].moveToCoordinates.y = yCoordinates;
                    computerMoves.noMoves++;
                
                }
                
            }
        }
    }
    return noMoves;
}

chessPiece* CreateChessPiece (pieceType type, int positionX, int positionY, playerType player)
{
    chessPiece* piece = malloc (sizeof(chessPiece));
    piece->coordinates.x = positionX;
    piece->coordinates.y = positionY;
    piece->type = type;
    piece->player = player;

    
    switch(type)
    {
        
        case PAWN:
            player == USER ? ((piece->imageCode) = 0x265F) : ((piece->imageCode) = 0x2659);
            piece->fnValidateChessPieceMove = &ValidatePawnMove;
            piece->fnGenerateChessPieceMoves = &GeneratePawnMove; 
        break;
        case KNIGHT:
            player == USER ? ((piece->imageCode) = 0x265E) : ((piece->imageCode) = 0x2658);
            piece->fnValidateChessPieceMove = &ValidateKnightMove;
            piece->fnGenerateChessPieceMoves = &GenerateKnightMove; 
        break;
        case BISHOP:
            player == USER ? ((piece->imageCode) = 0x265D) : ((piece->imageCode) = 0x2657);
            piece->fnValidateChessPieceMove = &ValidateBishopMove;
            piece->fnGenerateChessPieceMoves = &GenerateBishopMove;
            piece->fnHasObstacleOnPath = &BishopHasObstacleOnPath; 
        break;
        case ROOK:
            player == USER ? ((piece->imageCode) = 0x265C) : ((piece->imageCode) = 0x2656);
            piece->fnValidateChessPieceMove = &ValidateRookMove;
            piece->fnGenerateChessPieceMoves = &GenerateRookMove; 
            piece->fnHasObstacleOnPath = &RookHasObstacleOnPath;
        break;
        case QUEEN:
            player == USER ? ((piece->imageCode) = 0x265B) : ((piece->imageCode) = 0x2655);
            piece->fnValidateChessPieceMove = &ValidateQueenMove;
            piece->fnGenerateChessPieceMoves = &GenerateQueenMove; 
            piece->fnHasObstacleOnPath = &QueenHasObstacleOnPath;
        break;
        case KING:
            player == USER ? ((piece->imageCode) = 0x265A) : ((piece->imageCode) = 0x2654);
            piece->fnValidateChessPieceMove = &ValidateKingMove;
            piece->fnGenerateChessPieceMoves = &GenerateKingMove; 
        break; 
    }
    
    return piece;
}

gameLoad LoadOption()
{
    
    int menuOption = 0;
    wprintf(L"Welcome to chess:\n1.New game\n2.Load previous save\n");
    scanf("%d", &menuOption);
    flushStdin();

    gameLoad game = DEFAULT_OPTION;
    switch(menuOption)
    {
        case 1:
            game = NEWGAME;
        break;

        case 2:
            game = LOADSAVE;
        break;
        default:
            wprintf(L"Invalid option.\n");
            exit(EXIT_FAILURE);
    }

    return game;    

}

gameType GameType()
{
    int menuOption = 0;
    wprintf(L"Choose the type of chess game:\n1.PvP\n2.PvC\n");
    scanf("%d", &menuOption);
    flushStdin();

    gameType game = DEFAULT;
    switch(menuOption)
    {
        case 1:
            game = PlayerVsPlayer;
        break;

        case 2:
            game = PlayerVsComputer;
        break;
        default:
            wprintf(L"Invalid option.\n");
            exit(EXIT_FAILURE);
    }

    return game;    
}

void SaveGame(gameType menuOption, playerType lastPlayerToMove)
{
    FILE *fptr;
    fptr = fopen("chess_save.csv","w");

    if(fptr == NULL)
    {
        wprintf(L"Failed to open file and save!");   
        exit(EXIT_FAILURE);             
    }

    fprintf(fptr,"%d,%d\n", menuOption, lastPlayerToMove);
    for(int xIdxcoordinates = 0; xIdxcoordinates < NO_BOARD_ROWS; xIdxcoordinates++)
    {
        for(int yIdxcoordinates = 0; yIdxcoordinates < NO_BOARD_COLUMNS; yIdxcoordinates++)
        {
            chessPiece *piece = chessBoard[xIdxcoordinates][yIdxcoordinates].piece;

            if(piece != NULL)
            {
                fprintf(fptr,"%d,%d,%d,%d\n", piece->type, piece->coordinates.x, piece->coordinates.y, piece->player);
            }
            
        }
    }
    fclose(fptr);
}

void LoadSaveGame()
{
    FILE *fptr;
    fptr = fopen("chess_save.csv","r");

    if(fptr == NULL)
    {
        wprintf(L"Failed to open file!");   
        exit(EXIT_FAILURE);             
    }

    char StringContainingChessBoardInfo[100];
    fgets(StringContainingChessBoardInfo, 100, fptr);
    printf("%s\n",StringContainingChessBoardInfo);
    
    char *token;
    token = strtok(StringContainingChessBoardInfo, ",");
    playGame.menuGameTypeOption = atoi(token);
    token = strtok(StringContainingChessBoardInfo, ",");
    playGame.lastPlayerToMove = atoi(token);

    while(feof(fptr) == 0)
    {   
        fgets(StringContainingChessBoardInfo, 100, fptr);
        token = strtok(StringContainingChessBoardInfo, ",");

        while(token != NULL)
        {
            pieceType type = atoi(token);

            token = strtok(NULL, ",");
            if(token == NULL) break;
            int positionX = atoi(token);
            
            token = strtok(NULL, ",");
            if(token == NULL) break;
            int positionY = atoi(token);

            token = strtok(NULL, ",");
            if(token == NULL) break;
            playerType lastPlayerToMove = atoi(token);
            
            chessBoard[positionX - 1][positionY - 1].piece = CreateChessPiece (type, positionX, positionY, lastPlayerToMove);

            token = strtok(NULL, ",");
        }
    }




    fclose(fptr);
}

void InitChessBoard(gameLoad menuGameLoadOption)
{
    computerMoves.noMoves = 0;

    bool isWhite = true;
    for(int i = 0; i < NO_BOARD_ROWS; i++)
    {
        (i % 2 == 0) ? (isWhite = true) : (isWhite = false);

        for(int j = 0; j < NO_BOARD_COLUMNS; j++)
        {
            (isWhite == true) ? (chessBoard[i][j].color = WHITE) : (chessBoard[i][j].color = BLACK);
            isWhite = !isWhite;
        }
    }

    if(menuGameLoadOption == NEWGAME)
    {
        //BLACK

        //top row
        chessBoard[0][0].piece = CreateChessPiece (ROOK, 1, 1, COMPUTER);
        chessBoard[0][1].piece = CreateChessPiece (KNIGHT, 1, 2, COMPUTER);
        chessBoard[0][2].piece = CreateChessPiece (BISHOP, 1, 3, COMPUTER);
        chessBoard[0][3].piece = CreateChessPiece (QUEEN, 1, 4, COMPUTER);
        chessBoard[0][4].piece = CreateChessPiece (KING, 1, 5, COMPUTER);
        chessBoard[0][5].piece = CreateChessPiece (BISHOP, 1, 6, COMPUTER);
        chessBoard[0][6].piece = CreateChessPiece (KNIGHT, 1, 7, COMPUTER);
        chessBoard[0][7].piece = CreateChessPiece (ROOK, 1, 8, COMPUTER);

        //bottom row
        chessBoard[1][0].piece = CreateChessPiece (PAWN, 2, 1, COMPUTER);
        chessBoard[1][1].piece = CreateChessPiece (PAWN, 2, 2, COMPUTER);
        chessBoard[1][2].piece = CreateChessPiece (PAWN, 2, 3, COMPUTER);
        chessBoard[1][3].piece = CreateChessPiece (PAWN, 2, 4, COMPUTER);
        chessBoard[1][4].piece = CreateChessPiece (PAWN, 2, 5, COMPUTER);
        chessBoard[1][5].piece = CreateChessPiece (PAWN, 2, 6, COMPUTER);
        chessBoard[1][6].piece = CreateChessPiece (PAWN, 2, 7, COMPUTER);
        chessBoard[1][7].piece = CreateChessPiece (PAWN, 2, 8, COMPUTER);
        
        //WHITE

        //top row
        chessBoard[6][0].piece = CreateChessPiece (PAWN, 7, 1, USER);
        chessBoard[6][1].piece = CreateChessPiece (PAWN, 7, 2, USER);
        chessBoard[6][2].piece = CreateChessPiece (PAWN, 7, 3, USER);
        chessBoard[6][3].piece = CreateChessPiece (PAWN, 7, 4, USER);
        chessBoard[6][4].piece = CreateChessPiece (PAWN, 7, 5, USER);
        chessBoard[6][5].piece = CreateChessPiece (PAWN, 7, 6, USER);
        chessBoard[6][6].piece = CreateChessPiece (PAWN, 7, 7, USER);
        chessBoard[6][7].piece = CreateChessPiece (PAWN, 7, 8, USER);

        //bottom row
        chessBoard[7][0].piece = CreateChessPiece (ROOK, 8, 1, USER);
        chessBoard[7][1].piece = CreateChessPiece (KNIGHT, 8, 2, USER);
        chessBoard[7][2].piece = CreateChessPiece (BISHOP, 8, 3, USER);
        chessBoard[7][3].piece = CreateChessPiece (QUEEN, 8, 4, USER);
        chessBoard[7][4].piece = CreateChessPiece (KING, 8, 5, USER);
        chessBoard[7][5].piece = CreateChessPiece (BISHOP, 8, 6, USER);
        chessBoard[7][6].piece = CreateChessPiece (KNIGHT, 8, 7, USER);
        chessBoard[7][7].piece = CreateChessPiece (ROOK, 8, 8, USER);
    }
    else LoadSaveGame();
}
double CalculateShanonFormulaPart_Mobility()
{
    int noComputerMoves = 0, noUserMoves = 0;
    //moving through pieces 
    for (int xCoordinates = 0; xCoordinates < NO_BOARD_ROWS; xCoordinates++)
    {
        for (int yCoordinates = 0; yCoordinates < NO_BOARD_COLUMNS; yCoordinates++)
        {   
            if (chessBoard[xCoordinates][yCoordinates].piece != NULL)
            {
                if (chessBoard[xCoordinates][yCoordinates].piece->player == COMPUTER)
                {
                    noComputerMoves += (int)chessBoard[xCoordinates][yCoordinates].piece->fnGenerateChessPieceMoves(chessBoard[xCoordinates][yCoordinates].piece, false);
                }
                else
                {
                    noUserMoves += (int)chessBoard[xCoordinates][yCoordinates].piece->fnGenerateChessPieceMoves(chessBoard[xCoordinates][yCoordinates].piece, false);
                }
            }
        }
    }
    return 0.1 * (noComputerMoves - noUserMoves);
}

int CalculateShanonFormulaPart_KQRBNP()
{
    int BRook = 0, BKing = 0, BQueen = 0, BBishop = 0, BKnight = 0,  BPawn = 0;
    int WRook = 0, WKing = 0, WQueen = 0, WBishop = 0, WKnight = 0,  WPawn = 0;
    int shanonFormula = 0;

    for(int xIdxCoordinates = 0; xIdxCoordinates < NO_BOARD_ROWS; xIdxCoordinates++)
    {
        for(int yIdxCoordinates = 0; yIdxCoordinates < NO_BOARD_COLUMNS; yIdxCoordinates++)
        {
            if (chessBoard[xIdxCoordinates][yIdxCoordinates].piece == NULL) continue;

            switch (chessBoard[xIdxCoordinates][yIdxCoordinates].piece->type)
            {
                case ROOK:
                chessBoard[xIdxCoordinates][yIdxCoordinates].piece->player == COMPUTER ? BRook++ : WRook++;
                break;

                case KING:
                chessBoard[xIdxCoordinates][yIdxCoordinates].piece->player == COMPUTER ? BKing++ : WKing++;
                break;

                case QUEEN:
                chessBoard[xIdxCoordinates][yIdxCoordinates].piece->player == COMPUTER ? BQueen++ : WQueen++;
                break;

                case BISHOP:
                chessBoard[xIdxCoordinates][yIdxCoordinates].piece->player == COMPUTER ? BBishop++ : WBishop++;
                break;

                case KNIGHT:
                chessBoard[xIdxCoordinates][yIdxCoordinates].piece->player == COMPUTER ? BKnight++ : WKnight++;
                break;

                case PAWN:
                chessBoard[xIdxCoordinates][yIdxCoordinates].piece->player == COMPUTER ? BPawn++ : WPawn++;
                break;
            }
        }
    }

    shanonFormula = 200*(BKing - WKing) 
                    + 9*(BQueen - WQueen)
                    + 5*(BRook - WRook)
                    + 3*(BBishop - WBishop + BKnight - WKnight)
                    + 1*(BPawn - WPawn);

    return shanonFormula;
}

int CalculateShanonFormulaPart_DoubledPawns()
{
    int noComputerDoubledPawns = 0, noUserDoubledPawns = 0;
    for (int xCoordinates = 0; xCoordinates < NO_BOARD_ROWS - 1; xCoordinates++)
    {
        for (int yCoordinates = 0; yCoordinates < NO_BOARD_COLUMNS; yCoordinates++)
        {   
        
            if (chessBoard[xCoordinates][yCoordinates].piece != NULL)
            {
                //doubled pawns for COMPUTER
                if(chessBoard[xCoordinates][yCoordinates].piece->player == COMPUTER && (chessBoard[xCoordinates][yCoordinates].piece == chessBoard[xCoordinates + 1][yCoordinates].piece))
                {
                    noComputerDoubledPawns ++;
                }
                //doubled pawns for USER
                if(chessBoard[xCoordinates][yCoordinates].piece->player == USER && (chessBoard[xCoordinates][yCoordinates].piece == chessBoard[xCoordinates + 1][yCoordinates].piece))
                {
                    noUserDoubledPawns ++;
                }
            }
        }
    }
    return (noComputerDoubledPawns - noUserDoubledPawns);
}

int CalculateShanonFormulaPart_BlockedPawnOrKing()
{
    int noComputerBlockedPawnOrKing = 0, noUserBlockedPawnOrKing = 0;
    for (int xCoordinates = 0; xCoordinates < NO_BOARD_ROWS - 1; xCoordinates++)
    {
        for (int yCoordinates = 0; yCoordinates < NO_BOARD_COLUMNS; yCoordinates++)
        {   
            if (chessBoard[xCoordinates][yCoordinates].piece != NULL)
            {
                if(((chessBoard[xCoordinates][yCoordinates].piece->type == KING) || (chessBoard[xCoordinates][yCoordinates].piece->type == PAWN)) 
                    &&(chessBoard[xCoordinates][yCoordinates].piece->fnGenerateChessPieceMoves(chessBoard[xCoordinates][yCoordinates].piece, false) == 0))
                {
                    (chessBoard[xCoordinates][yCoordinates].piece->player == COMPUTER) ? noComputerBlockedPawnOrKing ++ : noUserBlockedPawnOrKing ++;
                }
            }
        }
    }
    return (noComputerBlockedPawnOrKing - noUserBlockedPawnOrKing);
}

void GenerateListOfComputerMoves()
{
    //resetting the list of computer moves
    computerMoves.noMoves = 0;

    //moving through pieces 
    for (int xIdxCoordinates = 0; xIdxCoordinates < NO_BOARD_ROWS; xIdxCoordinates++)
    {
        for (int yIdxCoordinates = 0; yIdxCoordinates < NO_BOARD_COLUMNS; yIdxCoordinates++)
        {   
            if (chessBoard[xIdxCoordinates][yIdxCoordinates].piece != NULL)
            {
                //generating moves for computer
                if (chessBoard[xIdxCoordinates][yIdxCoordinates].piece->player == COMPUTER)
                {
                    chessBoard[xIdxCoordinates][yIdxCoordinates].piece->fnGenerateChessPieceMoves(chessBoard[xIdxCoordinates][yIdxCoordinates].piece, true);
                }
            }
        }
    }
}

void ComputeComputerMovesRating()
{
    int initialXCoordinates, initialYCoordinates;
    chessPiece *chesspieceToBeReplaced, *chesspieceToTemporaryMove;
    for(int i = 0; i < computerMoves.noMoves; i++)
    {

        //saving the position from where we started
        chesspieceToTemporaryMove = computerMoves.moves[i].piece;
        initialXCoordinates = chesspieceToTemporaryMove->coordinates.x;
        initialYCoordinates = chesspieceToTemporaryMove->coordinates.y;

        chesspieceToBeReplaced = chessBoard[computerMoves.moves[i].moveToCoordinates.x - 1][computerMoves.moves[i].moveToCoordinates.y - 1].piece;
        
        //replacing

        chesspieceToTemporaryMove->coordinates.x = computerMoves.moves[i].moveToCoordinates.x;
        chesspieceToTemporaryMove->coordinates.y = computerMoves.moves[i].moveToCoordinates.y;

        chessBoard[computerMoves.moves[i].moveToCoordinates.x - 1][computerMoves.moves[i].moveToCoordinates.y - 1].piece = chesspieceToTemporaryMove;

        chessBoard[initialXCoordinates - 1][initialYCoordinates - 1].piece = NULL;
        
        //calculating formula

        int shanonResult_partKQRBNP = CalculateShanonFormulaPart_KQRBNP();
        //wprintf(L"CalculateShanonFormulaPart_KQRBNP -> %d \n", shanonResult_partKQRBNP);
        int shanonResult_partMobility = CalculateShanonFormulaPart_Mobility();
        //wprintf(L"CalculateShanonFormulaPart_Mobility -> %d \n", shanonResult_partMobility);

        int shanonResult_partDoubledPawns = CalculateShanonFormulaPart_DoubledPawns();
        //wprintf(L"CalculateShanonFormulaPart_DoubledPawns -> %d \n", shanonResult_partDoubledPawns);

        int shanonResult_partBlockedPawnOrKing = CalculateShanonFormulaPart_BlockedPawnOrKing();
        //wprintf(L"CalculateShanonFormulaPart_BlockedPawnOrKing -> %d \n", shanonResult_partBlockedPawnOrKing);

        double shanonResult = shanonResult_partKQRBNP - 0.5 * (shanonResult_partDoubledPawns + shanonResult_partBlockedPawnOrKing) + 0.1 * shanonResult_partMobility; 

        computerMoves.moves[i].shanonRating = shanonResult;

        //reverting our move
        chesspieceToTemporaryMove->coordinates.x = initialXCoordinates;
        chesspieceToTemporaryMove->coordinates.y = initialYCoordinates;
        chessBoard[initialXCoordinates - 1][initialYCoordinates - 1].piece = chesspieceToTemporaryMove;

        chessBoard[computerMoves.moves[i].moveToCoordinates.x - 1][computerMoves.moves[i].moveToCoordinates.y - 1].piece = chesspieceToBeReplaced;
    }
}

void DisplayChessboard()
{
    //sidechars
    wprintf(L"  ");
    for (int l = 0; l < 8; l++)
    {
        wprintf(L"%c ", (l + 'a'));
    }  
    wprintf(L"\n");

    //printing the "ACTIVE" chessBoard
    for(int i = 0; i < NO_BOARD_ROWS; i++)
    {
        //side numbers
        wprintf(L"%lc ", ('8' - i));

        for(int j = 0; j < NO_BOARD_COLUMNS; j++)
        { 

            if(chessBoard[i][j].piece == NULL)
            {
                wprintf(L"%lc%lc", chessBoard[i][j].color, chessBoard[i][j].color);
            } 
            else
            {
                // pune piesa pe ecran
                wprintf(L"%lc ", chessBoard[i][j].piece->imageCode);
            }
        }
        wprintf(L"\n");
    }
}

bool ValidateUserInput(int initCoordX, char initCharCoordY, int finalCoordX, char finalCharCoordY)
{
    //is in range
    initCharCoordY = toupper(initCharCoordY);
    finalCharCoordY = toupper(finalCharCoordY);

    //checking validity of coordinates
    if ((   initCharCoordY >= 'A')
        && (initCharCoordY <= 'H')
        && (finalCharCoordY >= 'A')
        && (finalCharCoordY <= 'H')
        && (initCoordX >= 1) 
        && (initCoordX <= 8) 
        && (finalCoordX >= 1) 
        && (finalCoordX <= 8))
    {
        return true;
    }
    
    return false;
 
}
 

void MovePiece(chessPiece *piece, int destinationIndexX, int destinationIndexY)
{
    bool canMovePiece = true;
    if(chessBoard[destinationIndexX][destinationIndexY].piece != NULL)
    {
        if(chessBoard[destinationIndexX][destinationIndexY].piece->player 
            == chessBoard[piece->coordinates.x - 1][piece->coordinates.y - 1].piece->player)
        {
            canMovePiece = false;
            wprintf(L"Invalid move, trying to take out own piece.\n");
        }
        else
        {
            free(chessBoard[destinationIndexX][destinationIndexY].piece);
        }
        
    }
        
    if(canMovePiece == true)
    {
        chessBoard[destinationIndexX][destinationIndexY].piece = piece;
        chessBoard[piece->coordinates.x - 1][piece->coordinates.y - 1].piece = NULL;

        piece->coordinates.x = destinationIndexX + 1;
        piece->coordinates.y = destinationIndexY + 1;
    }
    
    
    
}

void ExecuteBestComputerMove()
{
    move bestComputerMove;
    bestComputerMove.shanonRating = -100000.0;
    for(int i = 0; i < computerMoves.noMoves; i++)
    {
        if(computerMoves.moves[i].shanonRating > bestComputerMove.shanonRating)
        {
            bestComputerMove = computerMoves.moves[i];
        }
    }

    MovePiece(bestComputerMove.piece, bestComputerMove.moveToCoordinates.x - 1, bestComputerMove.moveToCoordinates.y - 1);
}

void DoUserMove(gameType gameType)
{
    char initCharCoordY, finalCharCoordY; 
    int initCoordY, finalCoordY;

    int initCoordX, finalCoordX;
    bool isValidMove = false;

    while (isValidMove == false)
    {
        if(gameType == PlayerVsComputer) wprintf(L"WHITE moves\n");
        if(gameType == PlayerVsPlayer) wprintf(L"BLACK moves\n");
        
        wprintf(L"Enter coordinates for your move. Example of valid move: e4 b5\n");
        scanf("%c%d  %c%d", &initCharCoordY, &initCoordX, &finalCharCoordY, &finalCoordX);
        flushStdin();

   
        if (ValidateUserInput(initCoordX, initCharCoordY, finalCoordX, finalCharCoordY) == false)
        {
            wprintf(L"Invalid coordinates. try again.\n");
            continue;
        }


        //turn coordinates into valid matrix coordinates "x"
        initCoordY = toupper(initCharCoordY) - 'A';
        finalCoordY = toupper(finalCharCoordY) - 'A';

        initCoordX = NO_BOARD_ROWS - initCoordX;
        finalCoordX = NO_BOARD_ROWS - finalCoordX ;

        chessPiece *piece = chessBoard[initCoordX][initCoordY].piece;
        if(piece == NULL)
        {
            wprintf(L"No piece selected\n");
            continue;
        }

        //user 1 is set to USER
        if((gameType == PlayerVsComputer) && (piece->player == COMPUTER)) 
        {
            //not own piece there
            wprintf(L"Selected coordinates have no chess piece that can be moved.\n");
            continue;
        }

        //user 2 is set to COMPUTER
        if((gameType == PlayerVsPlayer) && (piece->player == USER)) 
        {
            //not own piece there
            wprintf(L"Selected coordinates have no chess piece that can be moved.\n");
            continue;
        }

        if ((piece->coordinates.x == finalCoordX + 1) && (piece->coordinates.y == finalCoordY + 1))
        {
            wprintf(L"Invalid move, try again.\n");
            continue;
        }

        if (piece->fnValidateChessPieceMove (piece, finalCoordX + 1, finalCoordY + 1) == false)
        {
            wprintf(L"Invalid move, try again.\n");
            continue;
        }


        MovePiece(piece, finalCoordX, finalCoordY);
        break;

    }
}



void DoComputerMove()
{
    GenerateListOfComputerMoves();

    ComputeComputerMovesRating();
 
    ExecuteBestComputerMove();
}

void CheckForCheckmate(int kingXCoordinates, int kingYCoordinates, playerType player)
{
   if(chessBoard[kingXCoordinates - 1][kingYCoordinates - 1].piece->fnGenerateChessPieceMoves(chessBoard[kingXCoordinates - 1][kingYCoordinates - 1].piece, false) == 0)
    {
        if(player == USER)
        {
            DisplayChessboard();
            wprintf(L"Checkmate! Black wins.");
        }
        else
        {
            DisplayChessboard();
            wprintf(L"Checkmate! White wins.");
        }
    }

    exit(EXIT_SUCCESS);
}

bool CheckForCheck(playerType userInCheck)
{
    chessPiece *userKing, *computerKing;

    //finding the kings
    for(int xIdxcoordinates = 0; xIdxcoordinates < NO_BOARD_ROWS; xIdxcoordinates++)
    {
        for(int yIdxcoordinates = 0; yIdxcoordinates < NO_BOARD_COLUMNS; yIdxcoordinates++)
        {
            if((chessBoard[xIdxcoordinates][yIdxcoordinates].piece != NULL) && (chessBoard[xIdxcoordinates][yIdxcoordinates].piece->type == KING)) 
            {
                if(chessBoard[xIdxcoordinates][yIdxcoordinates].piece->player == COMPUTER)
                {
                    computerKing = chessBoard[xIdxcoordinates][yIdxcoordinates].piece;
                }
                else
                {
                    userKing = chessBoard[xIdxcoordinates][yIdxcoordinates].piece;
                }
            }                    
        }
    }

    //checking for check
    for(int xIdxcoordinates = 0; xIdxcoordinates < NO_BOARD_ROWS; xIdxcoordinates++)
    {
        for(int yIdxcoordinates = 0; yIdxcoordinates < NO_BOARD_COLUMNS; yIdxcoordinates++)
        {
            chessPiece *piece = chessBoard[xIdxcoordinates][yIdxcoordinates].piece;
            if(chessBoard[xIdxcoordinates][yIdxcoordinates].piece != NULL)
            {
                //checking if COMPUTER is in check
                if(userInCheck == COMPUTER)
                {
                    if(chessBoard[xIdxcoordinates][yIdxcoordinates].piece->fnValidateChessPieceMove(piece, computerKing->coordinates.x, userKing->coordinates.y) == true)
                    {
                        wprintf(L"Black in check.\n");
                        CheckForCheckmate(userKing->coordinates.x, userKing->coordinates.y, COMPUTER);
                        return true;
                    }
                    
                }
                //checking if USER is in check
                else
                {
                    if(chessBoard[xIdxcoordinates][yIdxcoordinates].piece->fnValidateChessPieceMove(piece, userKing->coordinates.x, userKing->coordinates.y) == true)
                    {
                        wprintf(L"White in check.\n");
                        CheckForCheckmate(userKing->coordinates.x, userKing->coordinates.y, USER);
                        return true;
                    }    
                }
            }    
        }
    }

    return false;
}            
        


int main()
{
    InitEnvironment();
    playGame.menuGameLoadOption = LoadOption();
    InitChessBoard(playGame.menuGameLoadOption);
    if(playGame.menuGameLoadOption == NEWGAME) playGame.menuGameTypeOption = GameType();
    DisplayChessboard();
    
    while(true)
    {
        //for USER
        DoUserMove(PlayerVsComputer);
        CheckForCheck(COMPUTER);
        playGame.lastPlayerToMove = USER;
        SaveGame(playGame.menuGameTypeOption, playGame.lastPlayerToMove);

        //for COMPUTER / PLAYER_2
        if (playGame.menuGameTypeOption == PlayerVsComputer)
        {
            DoComputerMove();
        } 
        else
        {
            DisplayChessboard();
            DoUserMove(PlayerVsPlayer);
        }
        CheckForCheck(USER);
        playGame.lastPlayerToMove = COMPUTER;
        DisplayChessboard();
        SaveGame(playGame.menuGameTypeOption, playGame.lastPlayerToMove);
    }
    return 0;
}

