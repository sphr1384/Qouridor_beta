

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>


ALLEGRO_EVENT ev;
#define CELLSIZE 10
#define max_len 29
int mouseButton;

ALLEGRO_SAMPLE* audioPiece;
ALLEGRO_SAMPLE* audioWall;
ALLEGRO_SAMPLE* audioClick;
ALLEGRO_SAMPLE* audioBackgrond;
ALLEGRO_BITMAP* Square;
ALLEGRO_BITMAP* Saving;
ALLEGRO_BITMAP* loading;
ALLEGRO_BITMAP* background;
ALLEGRO_FONT* customFont;
ALLEGRO_FONT* font;
ALLEGRO_FONT* fontHelp;
enum page { endOfGame, firstMenu, playMenu, loadGame, setting, startGame, winnerPage } page = firstMenu;
struct BOARD {
    int board[max_len][max_len];
    int length;
};
struct Sum {
    int sumHuman;
    int sumComuter;
};
typedef struct Sum sum;
struct Setting {
    ALLEGRO_BITMAP* length;
    ALLEGRO_BITMAP* numberWall;
    ALLEGRO_BITMAP* Equal;
    ALLEGRO_BITMAP* leftFlash1;
    ALLEGRO_BITMAP* rightFlash1;
    ALLEGRO_BITMAP* leftFlash2;
    ALLEGRO_BITMAP* rightFlash2;
    ALLEGRO_BITMAP* leftFlash3;
    ALLEGRO_BITMAP* rightFlash3;
    ALLEGRO_BITMAP* leftFlash4;
    ALLEGRO_BITMAP* rightFlash4;
};
typedef struct Setting imgSet;
struct PLAYER {
    int term;
    int countWall[4];
    int numberWall;
    int pieceCoordinate[8];
};
struct sidbar {
    ALLEGRO_BITMAP* new;
    ALLEGRO_BITMAP* load;
    ALLEGRO_BITMAP* setting;
    ALLEGRO_BITMAP* exit;
};
struct imgPlayer {
    ALLEGRO_BITMAP* Piece1;
    ALLEGRO_BITMAP* Piece2;
    ALLEGRO_BITMAP* Piece3;
    ALLEGRO_BITMAP* Piece4;
};
struct imgPossiblityPlayer {
    ALLEGRO_BITMAP* Player1;
    ALLEGRO_BITMAP* Player2;
    ALLEGRO_BITMAP* Player3;
    ALLEGRO_BITMAP* Player4;
};
struct imgWall {
    ALLEGRO_BITMAP* hoverWall;
    ALLEGRO_BITMAP* putWall;
};
struct CHARM {
    int delet_all_wall;
    int reduce_wall_2;
    int reduce_wall_3;
    int reduce_wall_5;
    int block;
};

struct PRESENT {
    int increas_wall_2;
    int increas_wall_3;
    int increas_wall_5;
    int gain_wall_from_opponent_1;
    int gain_wall_from_opponent_2;
};
struct Button {
    int posX;
    int posY;
    int width;
    int height;
    char name[max_len];
};
typedef struct Button Button;
void initializeCharm(struct CHARM* charm) {
    charm->delet_all_wall = 2;
    charm->reduce_wall_2 = 10;
    charm->reduce_wall_3 = 5;
    charm->reduce_wall_5 = 3;
    charm->block = 7;
}

void initializePresent(struct PRESENT* present) {
    present->increas_wall_2 = 10;
    present->increas_wall_3 = 5;
    present->increas_wall_5 = 3;
    present->gain_wall_from_opponent_1 = 12;
    present->gain_wall_from_opponent_1 = 6;
}

void luck_charm(struct BOARD* data, struct PLAYER* player, struct CHARM* charm) {
    srand(time(NULL));

    int chooseCharm = rand() % 5;

    if (chooseCharm == 0 && charm->delet_all_wall >= 0) { // delete all walls
        for (int i = 0; i < 2 * data->length - 1; i++) {
            for (int j = 0; j < 2 * data->length - 1; j++) {
                if (i % 2 == 1 || j % 2 == 1) {
                    data->board[i][j] = 0;
                    if (i % 2 == 1 && j % 2 == 1) {
                        data->board[i][j] = 9;
                    }
                }
            }
        }

        charm->delet_all_wall--;
        printf("");
    }
    else if (chooseCharm == 1 && charm->reduce_wall_2 >= 0) { // reduce 2 walls
        player->countWall[player->term - 1] -= 2;
        charm->reduce_wall_2--;
        printf("reduce 2 walls\n");
    }
    else if (chooseCharm == 2 && charm->reduce_wall_3 >= 0) { // reduce 3 walls
        player->countWall[player->term - 1] -= 3;
        charm->reduce_wall_3--;
        printf("reduce 3 walls\n");
    }
    else if (chooseCharm == 3 && charm->reduce_wall_5 >= 0) { // reduce 5 walls
        player->countWall[player->term - 1] -= 5;
        charm->reduce_wall_5--;
        printf("reduce 5 walls\n");
    }
    else if (chooseCharm == 4 && charm->block >= 0) { // blocking the player
        player->term++;
        charm->block--;
        printf("blocking the player\n");
    }

}

void luck_present(struct BOARD* data, struct PLAYER* player, struct PRESENT* present, int Multiplayer) {
    srand(time(NULL));
    int choosePresent = rand() % 5;

    if (choosePresent == 0 && present->increas_wall_2 >= 0) { // increas 2 walls
        player->countWall[player->term - 1] += 2;
        present->increas_wall_2--;
        printf("increas 2 walls\n");
    }
    else if (choosePresent == 1 && present->increas_wall_3) { // increas 3 walls
        player->countWall[player->term - 1] += 3;
        present->increas_wall_3--;
        printf("increas 3 walls\n");
    }
    else if (choosePresent == 2 && present->increas_wall_5 >= 0) { // increas 5 walls
        player->countWall[player->term - 1] += 5;
        present->increas_wall_5--;
        printf("increas 5 walls\n");
    }
    else if (choosePresent == 3 && present->gain_wall_from_opponent_1 >= 0) { // gain 1 wall from opponent
        if (Multiplayer == 4) {
            if (player->term == 4) {
                player->countWall[3]++;
                player->countWall[0]--;
            }
            else {
                player->countWall[player->term - 1]++;
                player->countWall[player->term]--;
            }
        }
        else if (Multiplayer == 2) {
            if (player->term == 2) {
                player->countWall[1]++;
                player->countWall[0]--;
            }
            else {
                player->countWall[0]++;
                player->countWall[1]--;
            }
        }
        present->gain_wall_from_opponent_1--;
        printf("gain 1 wall from opponent\n");

    }
    else if (choosePresent == 4 && present->gain_wall_from_opponent_2 >= 0) { // gain 2 wals from opponent
        if (Multiplayer == 4) {
            if (player->term == 4) {
                player->countWall[3] += 2;
                player->countWall[0] -= 2;
            }
            else {
                player->countWall[player->term - 1] += 2;
                player->countWall[player->term] -= 2;
            }
        }
        else if (Multiplayer == 2) {
            if (player->term == 2) {
                player->countWall[1] += 2;
                player->countWall[0] -= 2;
            }
            else {
                player->countWall[0] += 2;
                player->countWall[1] -= 2;
            }
        }
        present->gain_wall_from_opponent_1--;
        printf("gain 2 wall from opponent\n");
    }
}

void luck(struct BOARD* data, struct PLAYER* player, struct CHARM* charm, struct PRESENT* present, int Multiplayer) {
    printf("luck");
    srand(time(NULL));
    int choice = rand() % 2;

    if (choice == 0) { // charm
        luck_charm(data, player, charm);
    }
    else if (choice == 1) {
        luck_present(data, player, present, Multiplayer);
    }
}

void makePrimaryBoard(struct BOARD* data) {
    //printf("%d", data->length);
    for (int i = 0; i < data->length * 2; i++) {
        for (int j = 0; j < data->length * 2; j++) {
            if (i % 2 == 0 && j % 2 == 0) data->board[i][j] = 5; // place for pieces 
            else if (i % 2 == 1 && j % 2 == 1) data->board[i][j] = 9;
            else data->board[i][j] = 0;
        }
    }

}

void movePiecePossiblities(struct BOARD* data, struct PLAYER* player) {

    int i = player->pieceCoordinate[2 * (player->term - 1)];
    int j = player->pieceCoordinate[2 * (player->term - 1) + 1];
    if (data->board[i - 1][j] != 7 && data->board[i - 2][j] == 5) {//move up
        data->board[i - 2][j] = 6 * 20 + player->term;
    }
    if (data->board[i + 1][j] != 7 && data->board[i + 2][j] == 5) {//move down
        data->board[i + 2][j] = 6 * 20 + player->term;
    }
    if (data->board[i][j - 1] != 7 && data->board[i][j - 2] == 5) {//move left
        data->board[i][j - 2] = 6 * 20 + player->term;
    }
    if (data->board[i][j + 1] != 7 && data->board[i][j + 2] == 5) {//move right
        data->board[i][j + 2] = 6 * 20 + player->term;
    }

}
void movePiecePossiblitiesJump(struct BOARD* data, struct PLAYER* player) {
    //printf("movePiecePossiblitiesJump\n");
    int i = player->pieceCoordinate[2 * (player->term - 1)];
    int j = player->pieceCoordinate[2 * (player->term - 1) + 1];
    if (data->board[i - 1][j] != 7 && data->board[i - 2][j] >= 1 && data->board[i - 2][j] < 5 && data->board[i - 4][j] == 5 && data->board[i - 3][j] != 7) {//move up
        data->board[i - 4][j] = 6 * 20 + player->term;
    }
    if (data->board[i + 1][j] != 7 && data->board[i + 2][j] >= 1 && data->board[i + 2][j] < 5 && data->board[i + 4][j] == 5 && data->board[i + 3][j] != 7) {//move down
        data->board[i + 4][j] = 6 * 20 + player->term;
    }
    if (data->board[i][j - 1] != 7 && data->board[i][j - 2] >= 1 && data->board[i][j - 2] < 5 && data->board[i][j - 4] == 5 && data->board[i][j - 3] != 7) {//move left
        data->board[i][j - 4] = 6 * 20 + player->term;
    }
    if (data->board[i][j + 1] != 7 && data->board[i][j + 2] >= 1 && data->board[i][j + 2] < 5 && data->board[i][j + 4] == 5 && data->board[i][j + 3] != 7) {//move right
        data->board[i][j + 4] = 6 * 20 + player->term;
    }

}

void movePiecePossiblitiesParties(struct BOARD* data, struct PLAYER* player) {
    //printf("%d\n", player->term);
    int i = player->pieceCoordinate[2 * (player->term - 1)];
    int j = player->pieceCoordinate[2 * (player->term - 1) + 1];
    //printf("%d\t%d\n", i, j);
    if (data->board[i - 1][j] != 7 && data->board[i - 2][j] >= 1 && data->board[i - 2][j] < 5 && data->board[i - 3][j] == 7 && data->board[i - 2][j + 1] != 7 && data->board[i - 2][j + 2] == 5) {//move up + right
        data->board[i - 2][j + 2] = 6 * 20 + player->term;
    }
    if (data->board[i - 1][j] != 7 && data->board[i - 2][j] >= 1 && data->board[i - 2][j] < 5 && data->board[i - 3][j] == 7 && data->board[i - 2][j - 1] != 7 && data->board[i - 2][j - 2] == 5) {//move up + left
        //printf("Abolfazl");
        data->board[i - 2][j - 2] = 6 * 20 + player->term;
    }
    if (data->board[i + 1][j] != 7 && data->board[i + 2][j] >= 1 && data->board[i + 2][j] < 5 && data->board[i + 3][j] == 7 && data->board[i + 2][j + 1] != 7 && data->board[i + 2][j + 2] == 5) {//move down + right
        data->board[i + 2][j + 2] = 6;
    }if (data->board[i + 1][j] != 7 && data->board[i + 2][j] >= 1 && data->board[i + 2][j] < 5 && data->board[i + 3][j] == 7 && data->board[i + 2][j - 1] != 7 && data->board[i + 2][j - 2] == 5) {//move down + left
        data->board[i + 2][j - 2] = 6 * 20 + player->term;
    }
    if (data->board[i][j + 1] != 7 && data->board[i][j + 2] >= 1 && data->board[i][j + 2] < 5 && data->board[i][j + 3] == 7 && data->board[i - 1][j + 2] != 7 && data->board[i - 2][j + 2] == 5) {//move right + up
        data->board[i - 2][j + 2] = 6 * 20 + player->term;
    }
    if (data->board[i][j + 1] != 7 && data->board[i][j + 2] >= 1 && data->board[i][j + 2] < 5 && data->board[i][j + 3] == 7 && data->board[i + 1][j + 2] != 7 && data->board[i + 2][j + 2] == 5) {//move right + down
        data->board[i + 2][j + 2] = 6 * 20 + player->term;
    }
    if (data->board[i][j - 1] != 7 && data->board[i][j - 2] >= 1 && data->board[i][j - 2] < 5 && data->board[i][j - 3] == 7 && data->board[i - 1][j - 2] != 7 && data->board[i - 2][j - 2] == 5) {//move left + right
        data->board[i - 2][j - 2] = 6 * 20 + player->term;
    }
    if (data->board[i][j - 1] != 7 && data->board[i][j - 2] >= 1 && data->board[i][j - 2] < 5 && data->board[i][j - 3] == 7 && data->board[i - 1][j + 2] != 7 && data->board[i - 2][j + 2] == 5) {//move left + down
        data->board[i + 2][j - 2] = 6 * 20 + player->term;
    }
}
void showBoard(struct BOARD data, struct imgPlayer Piece, struct imgPossiblityPlayer Possiblity, struct imgWall Walls) {
    al_clear_to_color(al_map_rgb(144, 202, 249));
    al_draw_filled_rectangle((5 * data.length + 1) * CELLSIZE, 0, (5 * data.length + 1) * CELLSIZE + 20 * CELLSIZE, (5 * data.length + 1) * CELLSIZE, al_map_rgb(38, 50, 56));
    for (int i = 0; i < data.length * 2; i++) {
        for (int j = 0; j < data.length * 2; j++) {
            if (data.board[i][j] == 5 && i % 2 == 0 && j % 2 == 0) {
                int Xpiece = ((j / 2) * 5 + 1) * CELLSIZE;
                int Ypiece = ((i / 2) * 5 + 1) * CELLSIZE;
                int widthPiece = 4 * CELLSIZE;
                int heightPiece = 4 * CELLSIZE;
                al_draw_filled_rectangle(Xpiece, Ypiece, Xpiece + widthPiece, Ypiece + heightPiece, al_map_rgb(0, 150, 136));
                //al_draw_scaled_bitmap(Square, 0, 0, al_get_bitmap_width(Square), al_get_bitmap_height(player1), Xpiece, Ypiece, widthPiece, heightPiece, 0);

            }
            else if (j % 2 == 0 && data.board[i][j] == 7 && (data.board[i][j + 2] == 7 || data.board[i][j - 2] == 7)) { // horizontal walls
                int wallX = ((j / 2) * 5 + 1) * CELLSIZE;
                int wallY = (i / 2 + 1) * 5 * CELLSIZE;
                int widthWall = (4 * CELLSIZE);
                int heightWall = CELLSIZE;
                al_draw_scaled_bitmap(Walls.putWall, 0, 0, al_get_bitmap_width(Walls.putWall), al_get_bitmap_height(Walls.putWall), wallX, wallY, widthWall, heightWall, 0);

                //al_draw_filled_rectangle(wallX, wallY, wallX + widthWall, wallY + heightWall, al_map_rgb(255, 0, 0));
            }
            else if (i % 2 == 0 && data.board[i][j] == 7 && (data.board[i + 2][j] == 7 || data.board[i - 2][j] == 7)) { // vertical walls
                int Xwall = (j / 2 + 1) * 5 * CELLSIZE;
                int Ywall = ((i / 2) * 5 + 1) * CELLSIZE;
                int widthWall = CELLSIZE;
                int heightWall = (4 * CELLSIZE);

                al_draw_scaled_bitmap(Walls.putWall, 0, 0, al_get_bitmap_width(Walls.putWall), al_get_bitmap_height(Walls.putWall), Xwall, Ywall, widthWall, heightWall, 0);
                //al_draw_filled_rectangle(Xwall, Ywall, Xwall + widthWall, Ywall + heightWall, al_map_rgb(255, 0, 0));
            }
            else if (i % 2 == 1 && j % 2 == 1 && data.board[i][j] == 7) {
                int Xwall = ((j + 1) / 2) * 5 * CELLSIZE;
                int Ywall = ((i + 1) / 2) * 5 * CELLSIZE;
                int widthWall = CELLSIZE;
                int heightWall = CELLSIZE;


                al_draw_scaled_bitmap(Walls.putWall, 0, 0, al_get_bitmap_width(Walls.putWall), al_get_bitmap_height(Walls.putWall), Xwall, Ywall, widthWall, heightWall, 0);
                //al_draw_filled_rectangle(Xwall, Ywall, Xwall + widthWall, Ywall + heightWall, al_map_rgb(255, 0, 0));

            }
            else if (data.board[i][j] == 1) {
                int Xpiece = ((j / 2) * 5 + 1) * CELLSIZE;
                int Ypiece = ((i / 2) * 5 + 1) * CELLSIZE;
                int widthPiece = 4 * CELLSIZE;
                int heightPiece = 4 * CELLSIZE;

                al_draw_filled_rectangle(Xpiece, Ypiece, Xpiece + widthPiece, Ypiece + heightPiece, al_map_rgb(0, 150, 136));
                float centerX = ((j / 2) * 5 + 3) * CELLSIZE;
                float centerY = ((i / 2) * 5 + 3) * CELLSIZE;
                float redius = (3 * CELLSIZE) / 2;
                //al_draw_filled_circle(centerX, centerY, redius, al_map_rgb(255, 0, 0));
                al_draw_scaled_bitmap(Piece.Piece1, 0, 0, al_get_bitmap_width(Piece.Piece1), al_get_bitmap_height(Piece.Piece1), Xpiece, Ypiece, widthPiece, heightPiece, 0);
            }
            else if (data.board[i][j] == 2) {
                int Xpiece = ((j / 2) * 5 + 1) * CELLSIZE;
                int Ypiece = ((i / 2) * 5 + 1) * CELLSIZE;
                int widthPiece = 4 * CELLSIZE;
                int heightPiece = 4 * CELLSIZE;

                al_draw_filled_rectangle(Xpiece, Ypiece, Xpiece + widthPiece, Ypiece + heightPiece, al_map_rgb(0, 150, 136));

                float centerX = ((j / 2) * 5 + 3) * CELLSIZE;
                float centerY = ((i / 2) * 5 + 3) * CELLSIZE;
                float redius = (3 * CELLSIZE) / 2;

                al_draw_scaled_bitmap(Piece.Piece2, 0, 0, al_get_bitmap_width(Piece.Piece2), al_get_bitmap_height(Piece.Piece2), Xpiece, Ypiece, widthPiece, heightPiece, 0);
                //al_draw_filled_circle(centerX, centerY, redius, al_map_rgb(0, 0, 255));
            }
            else if (data.board[i][j] == 3) {
                int Xpiece = ((j / 2) * 5 + 1) * CELLSIZE;
                int Ypiece = ((i / 2) * 5 + 1) * CELLSIZE;
                int widthPiece = 4 * CELLSIZE;
                int heightPiece = 4 * CELLSIZE;

                al_draw_filled_rectangle(Xpiece, Ypiece, Xpiece + widthPiece, Ypiece + heightPiece, al_map_rgb(0, 150, 136));
                float centerX = ((j / 2) * 5 + 3) * CELLSIZE;
                float centerY = ((i / 2) * 5 + 3) * CELLSIZE;
                float redius = (3 * CELLSIZE) / 2;
                al_draw_scaled_bitmap(Piece.Piece3, 0, 0, al_get_bitmap_width(Piece.Piece3), al_get_bitmap_height(Piece.Piece3), Xpiece, Ypiece, widthPiece, heightPiece, 0);

                //al_draw_filled_circle(centerX, centerY, redius, al_map_rgb(0, 255, 0));
            }
            else if (data.board[i][j] == 4) {
                int Xpiece = ((j / 2) * 5 + 1) * CELLSIZE;
                int Ypiece = ((i / 2) * 5 + 1) * CELLSIZE;
                int widthPiece = 4 * CELLSIZE;
                int heightPiece = 4 * CELLSIZE;

                al_draw_filled_rectangle(Xpiece, Ypiece, Xpiece + widthPiece, Ypiece + heightPiece, al_map_rgb(0, 150, 136));
                float centerX = ((j / 2) * 5 + 3) * CELLSIZE;
                float centerY = ((i / 2) * 5 + 3) * CELLSIZE;
                float redius = (3 * CELLSIZE) / 2;
                al_draw_scaled_bitmap(Piece.Piece4, 0, 0, al_get_bitmap_width(Piece.Piece4), al_get_bitmap_height(Piece.Piece4), Xpiece, Ypiece, widthPiece, heightPiece, 0);
                //al_draw_filled_circle(centerX, centerY, redius, al_map_rgb(023, 45, 123));
            }
            else if (data.board[i][j] % 10 == 1 && data.board[i][j] > 120 && i % 2 == 0 && j % 2 == 0) {
                int Xpiece = ((j / 2) * 5 + 1) * CELLSIZE;
                int Ypiece = ((i / 2) * 5 + 1) * CELLSIZE;
                int widthPiece = 4 * CELLSIZE;
                int heightPiece = 4 * CELLSIZE;

                al_draw_filled_rectangle(Xpiece, Ypiece, Xpiece + widthPiece, Ypiece + heightPiece, al_map_rgb(0, 150, 136));
                int centerX = ((j / 2) * 5 + 3) * CELLSIZE;
                int centerY = ((i / 2) * 5 + 3) * CELLSIZE;
                float redius = (3 * CELLSIZE) / 2;
                al_draw_scaled_bitmap(Possiblity.Player1, 0, 0, al_get_bitmap_width(Possiblity.Player1), al_get_bitmap_height(Possiblity.Player1), Xpiece, Ypiece, widthPiece, heightPiece, 0);
                //al_draw_filled_circle(centerX, centerY, redius, al_map_rgb(224, 224, 224));
            }
            else if (data.board[i][j] % 10 == 2 && data.board[i][j] > 120 && i % 2 == 0 && j % 2 == 0) {
                int Xpiece = ((j / 2) * 5 + 1) * CELLSIZE;
                int Ypiece = ((i / 2) * 5 + 1) * CELLSIZE;
                int widthPiece = 4 * CELLSIZE;
                int heightPiece = 4 * CELLSIZE;

                al_draw_filled_rectangle(Xpiece, Ypiece, Xpiece + widthPiece, Ypiece + heightPiece, al_map_rgb(0, 150, 136));
                int centerX = ((j / 2) * 5 + 3) * CELLSIZE;
                int centerY = ((i / 2) * 5 + 3) * CELLSIZE;
                float redius = (3 * CELLSIZE) / 2;
                al_draw_scaled_bitmap(Possiblity.Player2, 0, 0, al_get_bitmap_width(Possiblity.Player2), al_get_bitmap_height(Possiblity.Player2), Xpiece, Ypiece, widthPiece, heightPiece, 0);
                //al_draw_filled_circle(centerX, centerY, redius, al_map_rgb(224, 224, 224));
            }
            else if (data.board[i][j] % 10 == 3 && data.board[i][j] > 120 && i % 2 == 0 && j % 2 == 0) {
                int Xpiece = ((j / 2) * 5 + 1) * CELLSIZE;
                int Ypiece = ((i / 2) * 5 + 1) * CELLSIZE;
                int widthPiece = 4 * CELLSIZE;
                int heightPiece = 4 * CELLSIZE;

                al_draw_filled_rectangle(Xpiece, Ypiece, Xpiece + widthPiece, Ypiece + heightPiece, al_map_rgb(0, 150, 136));
                int centerX = ((j / 2) * 5 + 3) * CELLSIZE;
                int centerY = ((i / 2) * 5 + 3) * CELLSIZE;
                float redius = (3 * CELLSIZE) / 2;
                al_draw_scaled_bitmap(Possiblity.Player3, 0, 0, al_get_bitmap_width(Possiblity.Player3), al_get_bitmap_height(Possiblity.Player3), Xpiece, Ypiece, widthPiece, heightPiece, 0);
                //al_draw_filled_circle(centerX, centerY, redius, al_map_rgb(224, 224, 224));
            }
            else if (data.board[i][j] % 10 == 4 && data.board[i][j] > 120 && i % 2 == 0 && j % 2 == 0) {
                int Xpiece = ((j / 2) * 5 + 1) * CELLSIZE;
                int Ypiece = ((i / 2) * 5 + 1) * CELLSIZE;
                int widthPiece = 4 * CELLSIZE;
                int heightPiece = 4 * CELLSIZE;

                al_draw_filled_rectangle(Xpiece, Ypiece, Xpiece + widthPiece, Ypiece + heightPiece, al_map_rgb(0, 150, 136));
                int centerX = ((j / 2) * 5 + 3) * CELLSIZE;
                int centerY = ((i / 2) * 5 + 3) * CELLSIZE;
                float redius = (3 * CELLSIZE) / 2;
                al_draw_scaled_bitmap(Possiblity.Player4, 0, 0, al_get_bitmap_width(Possiblity.Player4), al_get_bitmap_height(Possiblity.Player4), Xpiece, Ypiece, widthPiece, heightPiece, 0);
                //al_draw_filled_circle(centerX, centerY, redius, al_map_rgb(224, 224, 224));
            }
            else if (j % 2 == 0 && data.board[i][j] == 0) { // horizontal walls
                int wallX = ((j / 2) * 5 + 1) * CELLSIZE;
                int wallY = (i / 2 + 1) * 5 * CELLSIZE;
                int widthWall = (4 * CELLSIZE);
                int heightWall = CELLSIZE;
                al_draw_filled_rectangle(wallX, wallY, wallX + widthWall, wallY + heightWall, al_map_rgb(144, 202, 249));
            }
            else if (i % 2 == 0 && data.board[i][j] == 0) { // vertical walls
                int Xwall = (j / 2 + 1) * 5 * CELLSIZE;
                int Ywall = ((i / 2) * 5 + 1) * CELLSIZE;
                int widthWall = CELLSIZE;
                int heightWall = (4 * CELLSIZE);
                al_draw_filled_rectangle(Xwall, Ywall, Xwall + widthWall, Ywall + heightWall, al_map_rgb(144, 202, 249));
            }
            else if (i % 2 == 1 && j % 2 == 1 && data.board[i][j] == 0) {
                int Xwall = ((j + 1) / 2) * 5 * CELLSIZE;
                int Ywall = ((i + 1) / 2) * 5 * CELLSIZE;
                int widthWall = CELLSIZE;
                int heightWall = CELLSIZE;

                al_draw_filled_rectangle(Xwall, Ywall, Xwall + widthWall, Ywall + heightWall, al_map_rgb(144, 202, 249));

            }
        }
    }
}

int calculatePieceCoordinate(float number, int length) {
    int i = 0;
    while (25 * i + 50 <= (5 * length + 1) * 10) {
        if (number > 25 * i + 10 && number < 25 * i + 50) return i;
        if (number > 25 * i + 50 && number < 25 * i + 60) return -1;
        i += 2;
    }
    return -1;
}

int find_HoV(float X, float Y, int length) {//This function show horizontal and vertical for walls.
    //squares for pieces
    if (calculatePieceCoordinate(X, length) >= 0 && calculatePieceCoordinate(Y, length) >= 0) return 0;

    // squares between walls
    if (calculatePieceCoordinate(X, length) == -1 && calculatePieceCoordinate(Y, length) == -1) return 1;

    // vertical walls
    if (calculatePieceCoordinate(X, length) == -1 && calculatePieceCoordinate(Y, length) >= 0) return 2;

    // horizontal walls
    if (calculatePieceCoordinate(X, length) >= 0 && calculatePieceCoordinate(Y, length) == -1) return 3;
}
int calculateWallCoordinate(float number, int length, int HoV, char XoY) {
    if (HoV == 0) return -1;
    else if (HoV == 1) {
        int a;
        a = (int)number / 50;
        return (2 * a - 1);
    }
    else if (HoV == 2) {
        if (XoY == 'x') {
            int a;
            a = (int)number / 50;
            return (2 * a - 1);
        }
        else if (XoY == 'y') {
            int b;
            b = (int)number / 50;
            return (2 * b);
        }
    }
    else if (HoV == 3) {
        if (XoY == 'x') {
            int a;
            a = (int)number / 50;
            return (2 * a);
        }
        else if (XoY == 'y') {
            int b;
            b = (int)number / 50;
            return (2 * b - 1);
        }
    }
}
void movePiece(struct BOARD* data, struct PLAYER* player, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT ev, int x_coordinate, int y_coordinate, bool* checkingPut) {

    float pos_X = x_coordinate, pos_Y = y_coordinate;




    if (data->board[calculatePieceCoordinate(pos_Y, data->length)][calculatePieceCoordinate(pos_X, data->length)] == 6 * 20 + player->term && calculatePieceCoordinate(pos_Y, data->length) != -1 && calculatePieceCoordinate(pos_X, data->length) != -1) {

        int coordinate_i = player->pieceCoordinate[(player->term - 1) * 2], coordinate_j = player->pieceCoordinate[(player->term - 1) * 2 + 1];
        data->board[coordinate_i][coordinate_j] = 5;
        data->board[calculatePieceCoordinate(pos_Y, data->length)][calculatePieceCoordinate(pos_X, data->length)] = player->term;
        player->pieceCoordinate[(player->term - 1) * 2] = calculatePieceCoordinate(pos_Y, data->length);

        player->pieceCoordinate[(player->term - 1) * 2 + 1] = calculatePieceCoordinate(pos_X, data->length);
        *checkingPut = 1;
    }



    for (register int i = 0; i < 2 * data->length; i++) {
        for (register int j = 0; j < 2 * data->length; j++) {
            if (data->board[i][j] == 6 * 20 + player->term && i % 2 == 0 && j % 2 == 0) data->board[i][j] = 5;
        }
        //printf("\n");
    }


}

struct Coordinate {
    int row;
    int col;
};

typedef struct {
    struct Coordinate coord;
    int direction;  // 0: up, 1: down, 2: left, 3: right
} StackItem;

StackItem pop(StackItem stack[], int* top) {
    if (*top >= 0) {
        return stack[(*top)--];
    }
    else {
        printf("Stack underflow!\n");
        exit(EXIT_FAILURE);
    }
}

void push(StackItem stack[], int* top, StackItem item) {
    if (*top < max_len * max_len - 1) {
        stack[++(*top)] = item;
    }
    else {
        printf("Stack overflow!\n");
        exit(EXIT_FAILURE);
    }
    //pop(stack, &top);
}



bool isEmpty(int top) {
    return top == -1;
}

bool isValidMove(struct BOARD data, int row, int col) {
    return (row >= 0 && row < 2 * data.length - 1 && col >= 0 && col < 2 * data.length - 1 && (data.board[row][col] == 5 || data.board[row][col] == 0 || data.board[row][col] == 1 || data.board[row][col] == 2 || data.board[row][col] > 120 || data.board[row][col] == 3 || data.board[row][col] == 4));
}



int dfs_vertical(struct BOARD data, int term, int start_row, int start_col, int help[][max_len], int* index) {
    int validPlacementPiece1 = 0, validPlacementPiece2 = 0, validPlacementPiece3 = 0, validPlacementPiece4 = 0;
    StackItem stack[max_len * max_len];
    int top = -1;

    struct Coordinate start_coord = { start_row, start_col };
    StackItem start_item = { start_coord, 0 };

    push(stack, &top, start_item);


    //StackItem current = pop(stack, &top);
    while (!(isEmpty(top))) {

        StackItem current = pop(stack, &top);

        int row = current.coord.row;
        int col = current.coord.col;

        if (!help[row][col]) {
            help[row][col] = 1;  // Mark cell as visited
            //printf("Visited cell: (%d, %d)\n", row, col);

            // Push unvisited neighbors onto the stack
            for (int i = 3; i >= 0; i--) {
                StackItem neighbor;
                neighbor.direction = i;

                switch (i) {
                case 0:  // Up
                    neighbor.coord.row = row - 1;
                    neighbor.coord.col = col;
                    break;
                case 1:  // Down
                    neighbor.coord.row = row + 1;
                    neighbor.coord.col = col;
                    break;
                case 2:  // Left
                    neighbor.coord.row = row;
                    neighbor.coord.col = col - 1;
                    break;
                case 3:  // Right
                    neighbor.coord.row = row;
                    neighbor.coord.col = col + 1;
                    break;
                }

                if (isValidMove(data, neighbor.coord.row, neighbor.coord.col)) {

                    push(stack, &top, neighbor);

                }
            }
        }
        if (term == 1) {
            /*for (int i = 0; i < 2 * data.length - 1; i += 2) {
                if (help[2 * data.length - 2][i] == 1) {
                    //printf("Yes1\n");
                    *index = 1;
                    top = -1;
                    return;
                }
            }*/
            if (row == 2 * data.length - 2 && help[row][col] == 1) {
                *index = 1;
                top = -1;
                return;
            }
        }
        else if (term == 2) {
            /*for (int i = 0; i < 2 * data.length - 1; i += 2) {
                if (help[0][i] == 1) {
                    // printf("Yes2\n");
                    *index = 1;
                    top = -1;
                    return;
                }
            }*/
            if (row == 0 && help[row][col] == 1) {
                *index = 1;
                top = -1;
                return;
            }
        }
        else if (term == 3) {
            /*for (int i = 0; i < 2 * data.length - 1; i += 2) {
                if (help[i][2 * data.length - 2] == 1) {
                    //printf("Yes3\n");
                    *index = 1;
                    top = -1;
                    return;
                }
            }*/
            if (col == 2 * data.length - 2 && help[row][col] == 1) {
                *index = 1;
                top = -1;
                return;
            }
        }
        else if (term == 4) {
            /*for (int i = 0; i < 2 * data.length - 1; i += 2) {
                if (help[i][0] == 1) {
                    //printf("Yes4\n");
                    *index = 1;
                    top = -1;
                    return;
                }
            }*/
            if (col == 0 && help[row][col] == 1) {
                *index = 1;
                top = -1;
                return;
            }
        }

    }

    printf("No\n");
    *index = 0;
    top = -1;

}


void delete_wall(struct BOARD* data) {
    for (register int i = 0; i < 2 * data->length; i++) {
        for (int j = 0; j < 2 * data->length; j++) {
            if (data->board[i][j] == 8) data->board[i][j] = 0;
            if (data->board[i][j] == 10) data->board[i][j] = 9;
        }
    }
}
void putWalls(struct BOARD* data, struct PLAYER* player, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT ev, int* go, int Multiplayer, int x_coodinate, int y_coordinate, bool submit, bool* checkingPut, struct imgWall wall) {

    int HoV;
    float pos_X = -1, pos_Y = -1;
    int i = 0;



    if (!submit) {
        delete_wall(data);
        pos_X = x_coodinate;
        pos_Y = y_coordinate;
        HoV = find_HoV(pos_X, pos_Y, data->length);
        if (pos_X > 10 && pos_Y > 10 && pos_X <= 50 * data->length - 10 && pos_Y <= 50 * data->length - 10 && (HoV == 2 || HoV == 3)) {
            HoV = find_HoV(pos_X, pos_Y, data->length);
            int index_i = calculateWallCoordinate(pos_Y, data->length, HoV, 'y'), index_j = calculateWallCoordinate(pos_X, data->length, HoV, 'x');
            if (HoV == 2) {//vertical wall
                if (index_i == 0 && data->board[index_i][index_j] != 7 && data->board[index_i - 1][index_j] != 7 && data->board[index_i - 2][index_j] != 7 && index_i + 1 < 2 * data->length) {

                    if (data->board[index_i][index_j] != 7) {
                        data->board[index_i][index_j] = 8;
                    }

                    if (data->board[index_i + 1][index_j] != 7) {
                        data->board[index_i + 1][index_j] = 10;
                    }

                    if (data->board[index_i + 2][index_j] != 7) {
                        data->board[index_i + 2][index_j] = 8;
                    }

                }
                else if (index_i > 0 && data->board[index_i][index_j] != 7  /* && data->board[index_i - 1][index_j] != 7 */ && data->board[index_i - 2][index_j] != 7 && index_i + 1 < 2 * data->length && (data->board[index_i - 1][index_j] != 7 || data->board[index_i - 1][index_j + 1] == 7)) {
                    if (data->board[index_i][index_j] != 7) {
                        data->board[index_i][index_j] = 8;
                    }

                    if (data->board[index_i - 1][index_j] != 7) {
                        data->board[index_i - 1][index_j] = 10;
                    }

                    if (data->board[index_i - 2][index_j] != 7) {
                        data->board[index_i - 2][index_j] = 8;
                    }

                }
            }
            else if (HoV == 3) {//horizontal wall
                if (index_j == 0 && data->board[index_i][index_j] != 7 && data->board[index_i][index_j + 1] != 7 && data->board[index_i][index_j + 2] != 7 && index_j + 1 < 2 * data->length) {
                    if (data->board[index_i][index_j] != 7) {
                        data->board[index_i][index_j] = 8;
                    }

                    if (data->board[index_i][index_j + 1] != 7) {
                        data->board[index_i][index_j + 1] = 10;
                    }

                    if (data->board[index_i][index_j + 2] != 7) {
                        data->board[index_i][index_j + 2] = 8;
                    }

                }
                else if (index_j > 0 && data->board[index_i][index_j] != 7 && /*data->board[index_i][index_j - 1] != 7 &&*/ data->board[index_i][index_j - 2] != 7 && index_j + 1 < 2 * data->length && (data->board[index_i][index_j - 1] != 7 || data->board[index_i - 1][index_j - 1] == 7)) {
                    if (data->board[index_i][index_j] != 7) {
                        data->board[index_i][index_j] = 8;
                    }

                    if (data->board[index_i][index_j - 1] != 7) {
                        data->board[index_i][index_j - 1] = 10;
                    }

                    if (data->board[index_i][index_j - 2] != 7) {
                        data->board[index_i][index_j - 2] = 8;
                    }

                }
            }
            int validPlacementPiece1 = 0, validPlacementPiece2 = 0, validPlacementPiece3 = 0, validPlacementPiece4 = 0;
            int help[max_len][max_len];
            if (Multiplayer == 4) {
                validPlacementPiece1 = validPlacementPiece2 = validPlacementPiece3 = validPlacementPiece4 = 0;

                memset(help, 0, sizeof(help));
                dfs_vertical(*data, 1, player->pieceCoordinate[0], player->pieceCoordinate[1], help, &validPlacementPiece1);
                memset(help, 0, sizeof(help));
                dfs_vertical(*data, 2, player->pieceCoordinate[2], player->pieceCoordinate[3], help, &validPlacementPiece2);
                memset(help, 0, sizeof(help));
                dfs_vertical(*data, 3, player->pieceCoordinate[4], player->pieceCoordinate[5], help, &validPlacementPiece3);
                memset(help, 0, sizeof(help));
                dfs_vertical(*data, 4, player->pieceCoordinate[6], player->pieceCoordinate[7], help, &validPlacementPiece4);
            }
            else if (Multiplayer == 2) {
                validPlacementPiece1 = validPlacementPiece2 = 0;
                validPlacementPiece3 = validPlacementPiece4 = 1;

                memset(help, 0, sizeof(help));
                dfs_vertical(*data, 1, player->pieceCoordinate[0], player->pieceCoordinate[1], help, &validPlacementPiece1);
                memset(help, 0, sizeof(help));
                dfs_vertical(*data, 2, player->pieceCoordinate[2], player->pieceCoordinate[3], help, &validPlacementPiece2);
            }
            if (validPlacementPiece1 == 1 && validPlacementPiece2 == 1 && validPlacementPiece3 == 1 && validPlacementPiece4 == 1) {
                //printf("*******\n");

                if (HoV == 2) {//vertical wall
                    if (index_i == 0 && data->board[index_i][index_j] != 7 && data->board[index_i + 1][index_j] != 7 && data->board[index_i + 2][index_j] != 7 && index_i + 1 < 2 * data->length) {
                        /*if (data->board[index_i][index_j] != 7) {
                            data->board[index_i][index_j] = 8;
                        }*/

                        int Xwall = (index_j / 2 + 1) * 5 * CELLSIZE;
                        int Ywall = ((index_i / 2) * 5 + 1) * CELLSIZE;
                        int widthWall = CELLSIZE;
                        int heightWall = (4 * CELLSIZE);
                        //al_draw_scaled_bitmap(wall.hoverWall, 0, 0, al_get_bitmap_width(wall.hoverWall), al_get_bitmap_height(wall.hoverWall), Xwall, Ywall, widthWall, heightWall, 0);
                        al_draw_filled_rectangle(Xwall, Ywall, Xwall + widthWall, Ywall + heightWall, al_map_rgb(38, 50, 56));
                        /*if (data->board[index_i + 1][index_j] != 7) {
                            data->board[index_i + 1][index_j] = 8;
                        }*/
                        index_i++;
                        Xwall = ((index_j + 1) / 2) * 5 * CELLSIZE;
                        Ywall = ((index_i + 1) / 2) * 5 * CELLSIZE;
                        widthWall = CELLSIZE;
                        heightWall = CELLSIZE;
                        //al_draw_scaled_bitmap(wall.hoverWall, 0, 0, al_get_bitmap_width(wall.hoverWall), al_get_bitmap_height(wall.hoverWall), Xwall, Ywall, widthWall, heightWall, 0);
                        al_draw_filled_rectangle(Xwall, Ywall, Xwall + widthWall, Ywall + heightWall, al_map_rgb(38, 50, 56));
                        index_i--;
                        /*if (data->board[index_i + 2][index_j] != 7) {
                            data->board[index_i + 2][index_j] = 8;
                        }*/
                        index_i += 2;
                        Xwall = (index_j / 2 + 1) * 5 * CELLSIZE;
                        Ywall = ((index_i / 2) * 5 + 1) * CELLSIZE;
                        widthWall = CELLSIZE;
                        heightWall = (4 * CELLSIZE);
                        //al_draw_scaled_bitmap(wall.hoverWall, 0, 0, al_get_bitmap_width(wall.hoverWall), al_get_bitmap_height(wall.hoverWall), Xwall, Ywall, widthWall, heightWall, 0);
                        al_draw_filled_rectangle(Xwall, Ywall, Xwall + widthWall, Ywall + heightWall, al_map_rgb(38, 50, 56));
                        index_i -= 2;
                    }
                    else if (index_i > 0 && data->board[index_i][index_j] != 7  /* && data->board[index_i - 1][index_j] != 7 */ && data->board[index_i - 2][index_j] != 7 && index_i + 1 < 2 * data->length && (data->board[index_i - 1][index_j] != 7 || data->board[index_i - 1][index_j - 1] == 7)) {
                        /*if (data->board[index_i][index_j] != 7) {
                            data->board[index_i][index_j] = 8;
                        }*/
                        int Xwall = (index_j / 2 + 1) * 5 * CELLSIZE;
                        int Ywall = ((index_i / 2) * 5 + 1) * CELLSIZE;
                        int widthWall = CELLSIZE;
                        int heightWall = (4 * CELLSIZE);
                        //al_draw_scaled_bitmap(wall.hoverWall, 0, 0, al_get_bitmap_width(wall.hoverWall), al_get_bitmap_height(wall.hoverWall), Xwall, Ywall, widthWall, heightWall, 0);
                        al_draw_filled_rectangle(Xwall, Ywall, Xwall + widthWall, Ywall + heightWall, al_map_rgb(38, 50, 56));
                        /*if (data->board[index_i - 1][index_j] != 7) {
                            data->board[index_i - 1][index_j] = 8;
                        }*/
                        index_i--;
                        Xwall = ((index_j + 1) / 2) * 5 * CELLSIZE;
                        Ywall = ((index_i + 1) / 2) * 5 * CELLSIZE;
                        widthWall = CELLSIZE;
                        heightWall = CELLSIZE;
                        //al_draw_scaled_bitmap(wall.hoverWall, 0, 0, al_get_bitmap_width(wall.hoverWall), al_get_bitmap_height(wall.hoverWall), Xwall, Ywall, widthWall, heightWall, 0);
                        al_draw_filled_rectangle(Xwall, Ywall, Xwall + widthWall, Ywall + heightWall, al_map_rgb(38, 50, 56));
                        index_i++;
                        /*if (data->board[index_i - 2][index_j] != 7) {
                            data->board[index_i - 2][index_j] = 8;
                        }*/
                        index_i -= 2;
                        Xwall = (index_j / 2 + 1) * 5 * CELLSIZE;
                        Ywall = ((index_i / 2) * 5 + 1) * CELLSIZE;
                        widthWall = CELLSIZE;
                        heightWall = (4 * CELLSIZE);
                        //al_draw_scaled_bitmap(wall.hoverWall, 0, 0, al_get_bitmap_width(wall.hoverWall), al_get_bitmap_height(wall.hoverWall), Xwall, Ywall, widthWall, heightWall, 0);
                        al_draw_filled_rectangle(Xwall, Ywall, Xwall + widthWall, Ywall + heightWall, al_map_rgb(38, 50, 56));
                        index_i += 2;
                    }
                }
                else if (HoV == 3) {//horizontal wall
                    if (index_j == 0 && data->board[index_i][index_j] != 7  /* && data->board[index_i][index_j + 1] != 7 */ && data->board[index_i][index_j + 2] != 7 && index_j + 1 < 2 * data->length && (data->board[index_i][index_j + 1] != 7)) {
                        /*if (data->board[index_i][index_j] != 7) {
                            data->board[index_i][index_j] = 8;
                        }*/
                        int wallX = ((index_j / 2) * 5 + 1) * CELLSIZE;
                        int wallY = (index_i / 2 + 1) * 5 * CELLSIZE;
                        int widthWall = (4 * CELLSIZE);
                        int heightWall = CELLSIZE;
                        //al_draw_scaled_bitmap(wall.hoverWall, 0, 0, al_get_bitmap_width(wall.hoverWall), al_get_bitmap_height(wall.hoverWall), wallX, wallY, widthWall, heightWall, 0);
                        al_draw_filled_rectangle(wallX, wallY, wallX + widthWall, wallY + heightWall, al_map_rgb(38, 50, 56));
                        /*if (data->board[index_i][index_j + 1] != 7) {
                            data->board[index_i][index_j + 1] = 8;
                        }*/
                        index_j++;
                        wallX = ((index_j + 1) / 2) * 5 * CELLSIZE;
                        wallY = ((index_i + 1) / 2) * 5 * CELLSIZE;
                        widthWall = CELLSIZE;
                        heightWall = CELLSIZE;
                        //al_draw_scaled_bitmap(wall.hoverWall, 0, 0, al_get_bitmap_width(wall.hoverWall), al_get_bitmap_height(wall.hoverWall), wallX, wallY, widthWall, heightWall, 0);
                        al_draw_filled_rectangle(wallX, wallY, wallX + widthWall, wallY + heightWall, al_map_rgb(38, 50, 56));
                        index_j--;
                        /*if (data->board[index_i][index_j + 2] != 7) {
                            data->board[index_i][index_j + 2] = 8;
                        }*/
                        index_j += 2;
                        wallX = ((index_j / 2) * 5 + 1) * CELLSIZE;
                        wallY = (index_i / 2 + 1) * 5 * CELLSIZE;
                        widthWall = (4 * CELLSIZE);
                        heightWall = CELLSIZE;
                        //al_draw_scaled_bitmap(wall.hoverWall, 0, 0, al_get_bitmap_width(wall.hoverWall), al_get_bitmap_height(wall.hoverWall), wallX, wallY, widthWall, heightWall, 0);
                        al_draw_filled_rectangle(wallX, wallY, wallX + widthWall, wallY + heightWall, al_map_rgb(38, 50, 56));
                        index_j -= 2;
                    }
                    else if (index_j > 0 && data->board[index_i][index_j] != 7 && /*data->board[index_i][index_j - 1] != 7 &&*/ data->board[index_i][index_j - 2] != 7 && index_j + 1 < 2 * data->length && (data->board[index_i][index_j - 1] != 7 || data->board[index_i - 1][index_j - 1] == 7)) {
                        /*if (data->board[index_i][index_j] != 7) {
                            data->board[index_i][index_j] = 8;
                        }*/
                        int wallX = ((index_j / 2) * 5 + 1) * CELLSIZE;
                        int wallY = (index_i / 2 + 1) * 5 * CELLSIZE;
                        int widthWall = (4 * CELLSIZE);
                        int heightWall = CELLSIZE;
                        //al_draw_scaled_bitmap(wall.hoverWall, 0, 0, al_get_bitmap_width(wall.hoverWall), al_get_bitmap_height(wall.hoverWall), wallX, wallY, widthWall, heightWall, 0);
                        al_draw_filled_rectangle(wallX, wallY, wallX + widthWall, wallY + heightWall, al_map_rgb(38, 50, 56));
                        /*if (data->board[index_i][index_j - 1] != 7) {
                            data->board[index_i][index_j - 1] = 8;
                        }*/
                        index_j--;
                        wallX = ((index_j + 1) / 2) * 5 * CELLSIZE;
                        wallY = ((index_i + 1) / 2) * 5 * CELLSIZE;
                        widthWall = CELLSIZE;
                        heightWall = CELLSIZE;
                        //al_draw_scaled_bitmap(wall.hoverWall, 0, 0, al_get_bitmap_width(wall.hoverWall), al_get_bitmap_height(wall.hoverWall), wallX, wallY, widthWall, heightWall, 0);
                        al_draw_filled_rectangle(wallX, wallY, wallX + widthWall, wallY + heightWall, al_map_rgb(38, 50, 56));
                        index_j++;
                        /*if (data->board[index_i][index_j - 2] != 7) {
                            data->board[index_i][index_j - 2] = 8;
                        }*/
                        index_j -= 2;
                        wallX = ((index_j / 2) * 5 + 1) * CELLSIZE;
                        wallY = (index_i / 2 + 1) * 5 * CELLSIZE;
                        widthWall = (4 * CELLSIZE);
                        heightWall = CELLSIZE;
                        //al_draw_scaled_bitmap(wall.hoverWall, 0, 0, al_get_bitmap_width(wall.hoverWall), al_get_bitmap_height(wall.hoverWall), wallX, wallY, widthWall, heightWall, 0);
                        al_draw_filled_rectangle(wallX, wallY, wallX + widthWall, wallY + heightWall, al_map_rgb(38, 50, 56));
                        index_j += 2;
                    }
                }

            }
            else {
                for (int i = 0; i < 2 * data->length - 1; i++) {
                    for (int j = 0; j < 2 * data->length - 1; j++) {
                        if (data->board[i][j] == 8) {
                            data->board[i][j] = 0;
                        }
                        if (data->board[i][j] == 10) {
                            data->board[i][j] = 9;
                        }
                    }
                }

            }


        }
    }


    if (submit) {
        for (register int i = 0; i < 2 * data->length; i++) {
            for (register int j = 0; j < 2 * data->length; j++) {
                if (data->board[i][j] == 8) {
                    data->board[i][j] = 7;
                    *checkingPut = 1;
                }
                if (data->board[i][j] == 10) {
                    data->board[i][j] = 7;
                }
            }

        }

        //al_rest(3.0);

    }

    al_flip_display();

    for (register int i = 0; i < 2 * data->length; i++) {
        for (int j = 0; j < 2 * data->length; j++) {
            if (data->board[i][j] == 0 && i % 2 == 1 && j % 2 == 1) data->board[i][j] = 9;
        }
    }

}
void computerMove(struct BOARD* data, struct PLAYER* player) {
    srand(time(NULL));
    int count = 0;
    for (int i = 0; i < 2 * data->length - 1; i++) {
        for (int j = 0; j < 2 * data->length - 1; j++) {
            printf("%d ", data->board[i][j]);
            if (data->board[i][j] == 6 * 20 + player->term) {
                count++;
                data->board[i][j] = 6 * 10 + count;
            }
        }
        printf("\n");
    }
    int choose, varBreak = 0;
    printf("%d\n", count);
    choose = rand() % count + 1;

    for (int i = 0; i < 2 * data->length - 1; i++) {
        for (int j = 0; j < 2 * data->length - 1; j++) {
            if (data->board[i][j] > 60 && data->board[i][j] < 120) {
                if (choose == data->board[i][j] % 10) {
                    data->board[player->pieceCoordinate[(player->term - 1) * 2]][player->pieceCoordinate[(player->term - 1) * 2 + 1]] = 5;
                    data->board[i][j] = player->term;
                    player->pieceCoordinate[(player->term - 1) * 2] = i;
                    player->pieceCoordinate[(player->term - 1) * 2 + 1] = j;
                    varBreak = 1;
                    break;
                }
            }
        }
        if (varBreak == 1) break;
    }
    /*for (int i = 0; i < 2 * data->length - 1; i++) {
        for (int j = 0; j < 2 * data->length - 1; j++) {
            //printf("$");
            if (data->board[i][j] > 120 ) {
                data->board[i][j] = 5;
            }
        }
    }*/



    for (register int i = 0; i < 2 * data->length; i++) {
        for (register int j = 0; j < 2 * data->length; j++) {
            if (data->board[i][j] == 6 * 20 + player->term && i % 2 == 0 && j % 2 == 0) data->board[i][j] = 5;
        }
        //printf("\n");
    }
}

void computerWall(struct BOARD* data, struct PLAYER* player, int Multiplayer) {

    srand(time(NULL));
    int choose = rand() % 2;
    int index_i, index_j;
    if (choose % 2 == 1) {//horizontal wall

        do {
            index_i = rand() % (2 * data->length - 1);
            index_j = rand() % (2 * data->length - 1);

        } while (index_i % 2 == 0 || index_j % 2 == 1);
        

        if (index_j == 0 && data->board[index_i][index_j] != 7 && data->board[index_i][index_j + 1] != 7 && data->board[index_i][index_j + 2] != 7 && index_j + 1 < 2 * data->length) {
            if (data->board[index_i][index_j] != 7) {
                data->board[index_i][index_j] = 8;
            }

            if (data->board[index_i][index_j + 1] != 7) {
                data->board[index_i][index_j + 1] = 10;
            }

            if (data->board[index_i][index_j + 2] != 7) {
                data->board[index_i][index_j + 2] = 8;
            }

        }
        else if (index_j > 0 && data->board[index_i][index_j] != 7 && data->board[index_i][index_j - 2] != 7 && index_j + 1 < 2 * data->length && (data->board[index_i][index_j - 1] != 7 || data->board[index_i - 1][index_j - 1] == 7)) {
            if (data->board[index_i][index_j] != 7) {
                data->board[index_i][index_j] = 8;
            }

            if (data->board[index_i][index_j - 1] != 7) {
                data->board[index_i][index_j - 1] = 10;
            }

            if (data->board[index_i][index_j - 2] != 7) {
                data->board[index_i][index_j - 2] = 8;
            }

        }
    }
    else  if (choose % 2 == 0) {//vertical wall
        do {
            index_i = rand() % (2 * data->length);
            index_j = rand() % (2 * data->length);
        } while (index_i % 2 == 1 || index_j % 2 == 0);


        if (index_i == 0 && data->board[index_i][index_j] != 7 && data->board[index_i - 1][index_j] != 7 && data->board[index_i - 2][index_j] != 7 && index_i + 1 < 2 * data->length) {

            if (data->board[index_i][index_j] != 7) {
                data->board[index_i][index_j] = 8;
            }

            if (data->board[index_i + 1][index_j] != 7) {
                data->board[index_i + 1][index_j] = 10;
            }

            if (data->board[index_i + 2][index_j] != 7) {
                data->board[index_i + 2][index_j] = 8;
            }

        }
        else if (index_i > 0 && data->board[index_i][index_j] != 7  /* && data->board[index_i - 1][index_j] != 7 */ && data->board[index_i - 2][index_j] != 7 && index_i + 1 < 2 * data->length && (data->board[index_i - 1][index_j] != 7 || data->board[index_i - 1][index_j + 1] == 7)) {
            if (data->board[index_i][index_j] != 7) {
                data->board[index_i][index_j] = 8;
            }

            if (data->board[index_i - 1][index_j] != 7) {
                data->board[index_i - 1][index_j] = 10;
            }

            if (data->board[index_i - 2][index_j] != 7) {
                data->board[index_i - 2][index_j] = 8;
            }

        }
    }

    
    

    int validPlacementPiece1 = 0, validPlacementPiece2 = 0, validPlacementPiece3 = 0, validPlacementPiece4 = 0;
    int help[max_len][max_len];
    if (Multiplayer == 4) {
        validPlacementPiece1 = validPlacementPiece2 = validPlacementPiece3 = validPlacementPiece4 = 0;

        memset(help, 0, sizeof(help));
        dfs_vertical(*data, 1, player->pieceCoordinate[0], player->pieceCoordinate[1], help, &validPlacementPiece1);
        memset(help, 0, sizeof(help));
        dfs_vertical(*data, 2, player->pieceCoordinate[2], player->pieceCoordinate[3], help, &validPlacementPiece2);
        memset(help, 0, sizeof(help));
        dfs_vertical(*data, 3, player->pieceCoordinate[4], player->pieceCoordinate[5], help, &validPlacementPiece3);
        memset(help, 0, sizeof(help));
        dfs_vertical(*data, 4, player->pieceCoordinate[6], player->pieceCoordinate[7], help, &validPlacementPiece4);
    }
    else if (Multiplayer == 2) {
        validPlacementPiece1 = validPlacementPiece2 = 0;
        validPlacementPiece3 = validPlacementPiece4 = 1;

        memset(help, 0, sizeof(help));
        dfs_vertical(*data, 1, player->pieceCoordinate[0], player->pieceCoordinate[1], help, &validPlacementPiece1);
        memset(help, 0, sizeof(help));
        dfs_vertical(*data, 2, player->pieceCoordinate[2], player->pieceCoordinate[3], help, &validPlacementPiece2);
    }
    if (validPlacementPiece1 == 1 && validPlacementPiece2 == 1 && validPlacementPiece3 == 1 && validPlacementPiece4 == 1) {
        //printf("*******\n");
        for (register int i = 0; i < 2 * data->length; i++) {
            for (register int j = 0; j < 2 * data->length; j++) {
                if (data->board[i][j] == 8) {
                    data->board[i][j] = 7;

                }
                if (data->board[i][j] == 10) {
                    data->board[i][j] = 7;
                }
            }

        }
        

        for (register int i = 0; i < 2 * data->length; i++) {
            for (register int j = 0; j < 2 * data->length; j++) {
                if (j % 2 == 0 && data->board[i][j] == 7 && (data->board[i][j + 2] == 7 || data->board[i][j - 2] == 7)) { // horizontal walls
                    int wallX = ((j / 2) * 5 + 1) * CELLSIZE;
                    int wallY = (i / 2 + 1) * 5 * CELLSIZE;
                    int widthWall = (4 * CELLSIZE);
                    int heightWall = CELLSIZE;
                    al_draw_filled_rectangle(wallX, wallY, wallX + widthWall, wallY + heightWall, al_map_rgb(38, 50, 56));
                }
                else if (i % 2 == 0 && data->board[i][j] == 7 && (data->board[i + 2][j] == 7 || data->board[i - 2][j] == 7)) { // vertical walls
                    int Xwall = (j / 2 + 1) * 5 * CELLSIZE;
                    int Ywall = ((i / 2) * 5 + 1) * CELLSIZE;
                    int widthWall = CELLSIZE;
                    int heightWall = (4 * CELLSIZE);
                    al_draw_filled_rectangle(Xwall, Ywall, Xwall + widthWall, Ywall + heightWall, al_map_rgb(38, 50, 56));
                }
                else if (i % 2 == 1 && j % 2 == 1 && data->board[i][j] == 7) {
                    int Xwall = ((j + 1) / 2) * 5 * CELLSIZE;
                    int Ywall = ((i + 1) / 2) * 5 * CELLSIZE;
                    int widthWall = CELLSIZE;
                    int heightWall = CELLSIZE;

                    al_draw_filled_rectangle(Xwall, Ywall, Xwall + widthWall, Ywall + heightWall, al_map_rgb(38, 50, 56));

                }
            }
        }
    }
    else {
        for (int i = 0; i < 2 * data->length - 1; i++) {
            for (int j = 0; j < 2 * data->length - 1; j++) {
                if (data->board[i][j] == 8) {
                    data->board[i][j] = 0;
                }
                if (data->board[i][j] == 10) {
                    data->board[i][j] = 9;
                }
            }
        }

    }

   


}

void computerPlayer(struct BOARD* data, struct PLAYER* player, int Multiplayer) {
    srand(time(NULL));
    int choose = rand() % 2;
    if (choose == 0) {//move piece
        computerMove(&data, &player);

    }
    else if (choose == 1) {// computer wall
        computerWall(&data, &player, Multiplayer);
    }
}
void showButton(Button* show, int red, int grean, int blue, int length) {

    al_draw_filled_rectangle(show->posX - 20, show->posY, show->width + 20, show->height, al_map_rgb(red, grean, blue));
    al_draw_text(customFont, al_map_rgb(0, 0, 0), (show->posX + show->width) / 2, (show->posY + show->height) / 2, ALLEGRO_ALIGN_CENTER, show->name);

    //al_draw_scaled_bitmap(Saving, 0, 0, al_get_bitmap_width(Saving), al_get_bitmap_height(Saving), show->posX , show->posY , show->width, show->height , 0);

}
int checkButton(Button* show, int x_coordinate, int y_coordinate) {
    return (x_coordinate >= show->posX && x_coordinate <= show->width && y_coordinate >= show->posY && y_coordinate <= show->height);
}
int Winner(struct BOARD data, struct PLAYER player, int term) {
    if (term == 1) {
        for (int i = 0; i < 2 * data.length - 1; i++) {
            if (data.board[2 * data.length - 2][i] == 1 || (data.board[2 * data.length - 4][i] == 1 && data.board[2 * data.length - 2][i] >= 1 && data.board[2 * data.length - 2][i] <= 4)) {
                return 1;
            }
        }
    }
    else if (term == 2) {
        for (int i = 0; i < 2 * data.length - 1; i++) {
            if (data.board[0][i] == 2 || (data.board[2][i] == 2 && data.board[0][i] >= 1 && data.board[0][i] <= 4)) {
                return 2;
            }
        }
    }
    else if (term == 3) {
        for (int i = 0; i < 2 * data.length - 1; i++) {
            if (data.board[i][2 * data.length - 2] == 3 || (data.board[i][2 * data.length - 4] == 3 && data.board[i][2 * data.length - 2] >= 1 && data.board[i][2 * data.length - 2] <= 4)) {
                return 3;
            }
        }
    }
    else if (term == 0) {
        for (int i = 0; i < 2 * data.length - 1; i++) {
            if (data.board[i][0] == 4 || (data.board[i][2] == 4 && data.board[i][0] >= 1 && data.board[i][0] <= 4)) {
                return 4;
            }
        }
    }
    else
        return 0;
}
int main() {
    int count, go = 0, pointer;
    bool checkingWall = 0, checkingPiece = 0;
    struct BOARD data;
    struct PLAYER player;
    struct CHARM charm;
    struct PRESENT present;
    struct imgPlayer showPlayer;
    struct imgPossiblityPlayer showPossiblity;
    struct imgWall Walls;
    struct sidbar menu;
    imgSet icon;
    sum sumGame;
    int Multiplayer;

    //printf("enter the length of the board: ");

    ///scanf_s("%d", &data.length);
    //printf("Do you want to play for two or four players?");
    //scanf_s("%d", &Multiplayer);
    //-------------Difine length = 12----------------
    int length = 12;
    data.length = 12;
    player.numberWall = 12;
    charm.block = 2;
    charm.delet_all_wall = 2;
    // Initialize Allegro
    al_init();
    al_init_primitives_addon();
    al_init_image_addon();
    al_install_mouse();
    al_install_audio();
    al_init_acodec_addon();
    al_init_font_addon(); // Initialize the font add-on
    al_init_ttf_addon();  // Initialize the TrueType font add-on
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    showPlayer.Piece1 = al_load_bitmap("./knight (1).png");
    showPlayer.Piece2 = al_load_bitmap("./strategy (2).png");
    showPlayer.Piece3 = al_load_bitmap("./king(3).png");
    showPlayer.Piece4 = al_load_bitmap("./rook (4).png");
    showPossiblity.Player1 = al_load_bitmap("./knight_Possiblity (1).png");
    showPossiblity.Player2 = al_load_bitmap("./strategy_Possiblity (2).png");
    showPossiblity.Player3 = al_load_bitmap("./king_possiblity(3).png");
    showPossiblity.Player4 = al_load_bitmap("./rook_Possiblity (4).png");
    Walls.putWall = al_load_bitmap("./put-wall.png");
    //menu.new = al_load_bitmap("./menubar.png");
    background = al_load_bitmap("./Backgroung.jpg");
    customFont = al_load_ttf_font("./Groovetastic Free.ttf", 3 * length - 3, 0);
    fontHelp = al_load_ttf_font("./Groovetastic Free.ttf", 35, 0);
    font = al_load_ttf_font("./Lonely Night.ttf", 70, 0);
    icon.length = al_load_bitmap("./Board.png");
    icon.Equal = al_load_bitmap("./equal.png");
    icon.leftFlash1 = al_load_bitmap("./left-arrow(1).png");
    icon.rightFlash1 = al_load_bitmap("./right-arrow(1).png");
    icon.leftFlash2 = al_load_bitmap("./left-arrow(2).png");
    icon.rightFlash2 = al_load_bitmap("./right-arrow(2).png");
    icon.leftFlash3 = al_load_bitmap("./left-arrow(3).png");
    icon.rightFlash3 = al_load_bitmap("./right-arrow(3).png");
    icon.leftFlash4 = al_load_bitmap("./left-arrow(4).png");
    icon.rightFlash4 = al_load_bitmap("./right-arrow(4).png");
    if (!customFont) {
        fprintf(stderr, "Error loading font\n");
        return -1;
    }

    audioWall = al_load_sample("D:\\Game in VS\\Project1\\Project1\\wall.wav");
    audioPiece = al_load_sample("D:\\Game in VS\\Project1\\Project1\\move.wav");
    audioClick = al_load_sample("D:\\Game in VS\\Project1\\Project1\\click.wav");
    audioBackgrond = al_load_sample("D:\\Game in VS\\Project1\\Project1\\Background.wav");

    if (!al_reserve_samples(1)) {
        fprintf(stderr, "Failed to reserve samples.\n");
        return -1;
    }

    /*Walls.hoverWall = al_load_bitmap("./hover-wall (1).png");
    Square = al_load_bitmap("./Square.png");
    Saving = al_load_bitmap("./save (2).png");
    loading = al_load_bitmap("./load.png");*/





    if (!al_init_acodec_addon()) {
        fprintf(stderr, "Failed to initialize Allegro Acodec.\n");
        return -1;
    }


    //--------------------------------------$inGame&---------------------------------------------------------------//

    //ALLEGRO_DISPLAY* display = al_create_display((5 * data.length + 1) * CELLSIZE + 20 * CELLSIZE, (5 * data.length + 1) * CELLSIZE);
    int winner = 0;
    int Previous = firstMenu;//Previous variable
    int width, height;
    page = firstMenu;
    while (page) {

        if (page == firstMenu) {


            ALLEGRO_EVENT event;

            ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
            ALLEGRO_DISPLAY* display = al_create_display((5 * length + 1) * CELLSIZE + 20 * CELLSIZE, (5 * length + 1) * CELLSIZE);
            al_register_event_source(queue, al_get_mouse_event_source());

            al_register_event_source(queue, al_get_display_event_source(display));

            al_reserve_samples(1);
            al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background), al_get_bitmap_height(background), 0, 0, (5 * length + 1) * CELLSIZE + 20 * CELLSIZE, (5 * length + 1) * CELLSIZE + 20 * CELLSIZE, 0);
            al_play_sample(audioBackgrond, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            Button NewGame;
            width = (5 * length + 1) * CELLSIZE + 20 * CELLSIZE;
            height = (5 * length + 1) * CELLSIZE;
            NewGame.posX = width * 70 / 100;
            NewGame.posY = height * 10 / 100;
            NewGame.width = width * 85 / 100;
            NewGame.height = height * 20 / 100;
            strcpy_s(NewGame.name, 9, "NEW GAME");
            showButton(&NewGame, 161, 127, 136, length);
            //al_flip_display();
            Button LoadGame;
            width = (5 * length + 1) * CELLSIZE + 20 * CELLSIZE;
            height = (5 * length + 1) * CELLSIZE;
            LoadGame.posX = width * 70 / 100;
            LoadGame.posY = height * 25 / 100;
            LoadGame.width = width * 85 / 100;
            LoadGame.height = height * 35 / 100;
            strcpy_s(LoadGame.name, 10, "LOAD GAME");
            showButton(&LoadGame, 161, 127, 136, length);
            //al_flip_display();
            Button Setting;
            width = (5 * length + 1) * CELLSIZE + 20 * CELLSIZE;
            height = (5 * length + 1) * CELLSIZE;
            Setting.posX = width * 70 / 100;
            Setting.posY = height * 40 / 100;
            Setting.width = width * 85 / 100;
            Setting.height = height * 50 / 100;
            strcpy_s(Setting.name, 10, "SETTING");
            showButton(&Setting, 161, 127, 136, length);
            //al_flip_display();
            Button exit;
            width = (5 * length + 1) * CELLSIZE + 20 * CELLSIZE;
            height = (5 * length + 1) * CELLSIZE;
            exit.posX = width * 70 / 100;
            exit.posY = height * 55 / 100;
            exit.width = width * 85 / 100;
            exit.height = height * 65 / 100;
            strcpy_s(exit.name, 6, "EXIT");
            showButton(&exit, 161, 127, 136, length);
            al_flip_display();

            while (1) {
                al_wait_for_event(queue, &event);
                if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                    return 0;
                }
                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                    if (event.mouse.button == 1) {
                        if (checkButton(&NewGame, event.mouse.x, event.mouse.y)) {
                            al_destroy_sample(audioBackgrond);

                            //al_play_sample(audioClick, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            page = playMenu;
                            Previous = firstMenu;
                            //al_flip_display();
                            break;

                        }
                        if (checkButton(&LoadGame, event.mouse.x, event.mouse.y)) {
                            al_destroy_sample(audioBackgrond);
                            //al_play_sample(audioClick, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            page = loadGame;
                            //al_flip_display();
                            Previous = firstMenu;
                            break;

                        }
                        if (checkButton(&Setting, event.mouse.x, event.mouse.y)) {
                            al_destroy_sample(audioBackgrond);
                            //al_play_sample(audioClick, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            page = setting;
                            ////al_flip_display();
                            Previous = firstMenu;
                            break;

                        }
                        if (checkButton(&exit, event.mouse.x, event.mouse.y)) {
                            al_destroy_sample(audioBackgrond);

                            page = endOfGame;
                            Previous = firstMenu;
                            //al_flip_display();
                            break;
                        }
                    }
                }
            }
            //printf("%d", page);
            al_play_sample(audioClick, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            al_flip_display();
            //al_destroy_sample(audioBackgrond);
            //al_destroy_sample(audioClick);

            al_destroy_display(display);

        }
        if (page == setting) {
            //printf("%d", page);
            al_reserve_samples(1);
            ALLEGRO_DISPLAY* display = al_create_display((5 * length + 1) * CELLSIZE + 20 * CELLSIZE, (5 * length + 1) * CELLSIZE);
            ALLEGRO_EVENT event;
            ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
            al_register_event_source(queue, al_get_mouse_event_source());

            al_register_event_source(queue, al_get_display_event_source(display));
            al_reserve_samples(1);
            al_clear_to_color(al_map_rgb(120, 144, 156));
            //al_draw_filled_rectangle((5 * data.length + 1)* CELLSIZE, 0, (5 * data.length + 1)* CELLSIZE + 20 * CELLSIZE, (5 * data.length + 1)* CELLSIZE, al_map_rgb(144, 164, 174));
            width = (5 * length + 1) * CELLSIZE + 20 * CELLSIZE;
            height = (5 * length + 1) * CELLSIZE;
            al_draw_line(0, height * 5 / 100, width, height * 5 / 100, al_map_rgb(0, 0, 0), 5.0);
            al_draw_text(customFont, al_map_rgb(0, 0, 0), width / 2, height / 10 - 10, ALLEGRO_ALIGN_CENTRE, "SETTING");
            al_draw_line(0, height * 15 / 100, width, height * 15 / 100, al_map_rgb(0, 0, 0), 5.0);
            int Xpiece = width * 5 / 100;
            int Ypiece = height * 8 / 100;
            int widthPiece = width * 10 / 100;
            int heightPiece = height * 15 / 100;
            al_draw_scaled_bitmap(icon.length, 0, 0, al_get_bitmap_width(icon.length), al_get_bitmap_height(icon.length), Xpiece + 50, Ypiece + 50, widthPiece, heightPiece, 0);
            //al_draw_text(customFont, al_map_rgb(0, 0, 0), (Xpiece + widthPiece) / 2 + 120, (Ypiece + 50 + heightPiece) / 2 + 20, ALLEGRO_ALIGN_CENTRE, "=");
            Xpiece = width * 3 / 100;
            Ypiece = height * 3 / 100;
            widthPiece = width * 5 / 100;
            heightPiece = height * 5 / 100;
            al_draw_scaled_bitmap(icon.Equal, 0, 0, al_get_bitmap_width(icon.Equal), al_get_bitmap_height(icon.Equal), Xpiece + 110 + 50, Ypiece + 120, widthPiece, heightPiece, 0);
            Button leftArrow1;
            leftArrow1.posX = ((width * 3) / 100) + 105 + 90 + 50;
            leftArrow1.posY = ((height * 3) / 100) + 115;
            leftArrow1.width = ((width * 5) / 100) + leftArrow1.posX;
            leftArrow1.height = ((height * 5) / 100) + leftArrow1.posY;
            al_draw_scaled_bitmap(icon.leftFlash1, 0, 0, al_get_bitmap_width(icon.leftFlash1), al_get_bitmap_height(icon.leftFlash1), leftArrow1.posX, leftArrow1.posY, leftArrow1.width - leftArrow1.posX, leftArrow1.height - leftArrow1.posY, 0);

            Button rightArrow1;
            rightArrow1.posX = width * 3 / 100 + 105 + 90 + 180 + 50;
            rightArrow1.posY = height * 3 / 100 + 115;
            rightArrow1.width = width * 5 / 100 + rightArrow1.posX;
            rightArrow1.height = height * 5 / 100 + rightArrow1.posY;

            al_draw_scaled_bitmap(icon.rightFlash1, 0, 0, al_get_bitmap_width(icon.rightFlash1), al_get_bitmap_height(icon.rightFlash1), rightArrow1.posX, rightArrow1.posY, rightArrow1.width - rightArrow1.posX, rightArrow1.height - rightArrow1.posY, 0);
            al_draw_textf(font, al_map_rgb(0, 0, 0), (rightArrow1.posX + leftArrow1.posX) / 2 + 20, (rightArrow1.posY + leftArrow1.posY) / 2 - 20, ALLEGRO_ALIGN_CENTRE, "%d", player.numberWall);

            //-----------------------------------------------------------countWall----------------
            Xpiece = width * 5 / 100;
            Ypiece = height * 8 / 100;
            widthPiece = width * 10 / 100;
            heightPiece = height * 15 / 100;
            al_draw_scaled_bitmap(Walls.putWall, 0, 0, al_get_bitmap_width(Walls.putWall), al_get_bitmap_height(Walls.putWall), Xpiece + 50, Ypiece + 50 + 100, widthPiece, heightPiece, 0);
            //al_draw_text(customFont, al_map_rgb(0, 0, 0), (Xpiece + widthPiece) / 2 + 120, (Ypiece + 50 + heightPiece) / 2 + 20, ALLEGRO_ALIGN_CENTRE, "=");
            Xpiece = width * 3 / 100;
            Ypiece = height * 3 / 100;
            widthPiece = width * 5 / 100;
            heightPiece = height * 5 / 100;
            al_draw_scaled_bitmap(icon.Equal, 0, 0, al_get_bitmap_width(icon.Equal), al_get_bitmap_height(icon.Equal), Xpiece + 110 + 50, Ypiece + 120 + 100, widthPiece, heightPiece, 0);
            Button leftArrow2;
            leftArrow2.posX = width * 3 / 100 + 105 + 90 + 50;
            leftArrow2.posY = height * 3 / 100 + 115 + 100;
            leftArrow2.width = width * 5 / 100 + leftArrow2.posX;
            leftArrow2.height = height * 5 / 100 + leftArrow2.posY;
            al_draw_scaled_bitmap(icon.leftFlash2, 0, 0, al_get_bitmap_width(icon.leftFlash2), al_get_bitmap_height(icon.leftFlash2), leftArrow2.posX, leftArrow2.posY, leftArrow2.width - leftArrow2.posX, leftArrow2.height - leftArrow2.posY, 0);

            Button rightArrow2;
            rightArrow2.posX = width * 3 / 100 + 105 + 90 + 180 + 50;
            rightArrow2.posY = height * 3 / 100 + 115 + 100;
            rightArrow2.width = width * 5 / 100 + rightArrow2.posX;
            rightArrow2.height = height * 5 / 100 + rightArrow2.posY;
            al_draw_scaled_bitmap(icon.rightFlash2, 0, 0, al_get_bitmap_width(icon.rightFlash2), al_get_bitmap_height(icon.rightFlash2), rightArrow2.posX, rightArrow2.posY, rightArrow2.width - rightArrow2.posX, rightArrow2.height - rightArrow2.posY, 0);
            al_draw_textf(font, al_map_rgb(0, 0, 0), (rightArrow2.posX + leftArrow2.posX) / 2 + 20, (rightArrow2.posY + leftArrow2.posY) / 2 - 20, ALLEGRO_ALIGN_CENTRE, "%d", player.numberWall);
            al_draw_text(fontHelp, al_map_rgb(0, 0, 0), (rightArrow2.posX + leftArrow2.posX) / 2 - 80, (rightArrow2.posY + leftArrow2.posY) / 2 + 80, ALLEGRO_ALIGN_CENTRE, "Number of blocking rounds");
            al_draw_scaled_bitmap(icon.Equal, 0, 0, al_get_bitmap_width(icon.Equal), al_get_bitmap_height(icon.Equal), (rightArrow2.posX + leftArrow2.posX) / 2 + 120, (rightArrow2.posY + leftArrow2.posY) / 2 + 90, width * 5 / 100, height * 5 / 100, 0);
            Button leftArrow3;
            leftArrow3.posX = width * 3 / 100 + 105 + 90 + 50 + 260;
            leftArrow3.posY = height * 3 / 100 + 115 + 100 + 90;
            leftArrow3.width = ((width * 5) / 100) + leftArrow3.posX;
            leftArrow3.height = ((height * 5) / 100) + leftArrow3.posY;
            al_draw_scaled_bitmap(icon.leftFlash1, 0, 0, al_get_bitmap_width(icon.leftFlash3), al_get_bitmap_height(icon.leftFlash3), leftArrow3.posX, leftArrow3.posY, leftArrow3.width - leftArrow3.posX, leftArrow3.height - leftArrow3.posY, 0);

            Button rightArrow3;
            rightArrow3.posX = width * 3 / 100 + 105 + 90 + 50 + 260 + 180;
            rightArrow3.posY = height * 3 / 100 + 115 + 100 + 90;
            rightArrow3.width = ((width * 5) / 100) + rightArrow3.posX;
            rightArrow3.height = ((height * 5) / 100) + rightArrow3.posY;
            al_draw_scaled_bitmap(icon.rightFlash1, 0, 0, al_get_bitmap_width(icon.rightFlash3), al_get_bitmap_height(icon.rightFlash3), rightArrow3.posX, rightArrow3.posY, rightArrow3.width - rightArrow3.posX, rightArrow3.height - rightArrow3.posY, 0);
            al_draw_textf(font, al_map_rgb(0, 0, 0), (rightArrow3.posX + leftArrow3.posX) / 2 + 20, (rightArrow3.posY + leftArrow3.posY) / 2 - 20, ALLEGRO_ALIGN_CENTRE, "%d", charm.block);

            al_draw_text(fontHelp, al_map_rgb(0, 0, 0), (rightArrow2.posX + leftArrow2.posX) / 2 - 92, (rightArrow2.posY + leftArrow2.posY) / 2 + 150, ALLEGRO_ALIGN_CENTRE, "Number of deleting walls");
            al_draw_scaled_bitmap(icon.Equal, 0, 0, al_get_bitmap_width(icon.Equal), al_get_bitmap_height(icon.Equal), (rightArrow2.posX + leftArrow2.posX) / 2 + 110, (rightArrow2.posY + leftArrow2.posY) / 2 + 157, width * 5 / 100, height * 5 / 100, 0);


            Button leftArrow4;
            leftArrow4.posX = width * 3 / 100 + 105 + 90 + 50 + 260;
            leftArrow4.posY = height * 3 / 100 + 115 + 100 + 90 + 70;
            leftArrow4.width = ((width * 5) / 100) + leftArrow4.posX;
            leftArrow4.height = ((height * 5) / 100) + leftArrow4.posY;
            al_draw_scaled_bitmap(icon.leftFlash4, 0, 0, al_get_bitmap_width(icon.leftFlash4), al_get_bitmap_height(icon.leftFlash4), leftArrow4.posX, leftArrow4.posY, leftArrow4.width - leftArrow4.posX, leftArrow4.height - leftArrow4.posY, 0);

            Button rightArrow4;
            rightArrow4.posX = width * 3 / 100 + 105 + 90 + 50 + 260 + 180;
            rightArrow4.posY = height * 3 / 100 + 115 + 100 + 90 + 70;
            rightArrow4.width = ((width * 5) / 100) + rightArrow4.posX;
            rightArrow4.height = ((height * 5) / 100) + rightArrow4.posY;
            al_draw_scaled_bitmap(icon.rightFlash4, 0, 0, al_get_bitmap_width(icon.rightFlash4), al_get_bitmap_height(icon.rightFlash4), rightArrow4.posX, rightArrow4.posY, rightArrow4.width - rightArrow4.posX, rightArrow4.height - rightArrow4.posY, 0);
            al_draw_textf(font, al_map_rgb(0, 0, 0), (rightArrow4.posX + leftArrow4.posX) / 2 + 20, (rightArrow4.posY + leftArrow4.posY) / 2 - 20, ALLEGRO_ALIGN_CENTRE, "%d", charm.delet_all_wall);


            Button back;
            width = (5 * length + 1) * CELLSIZE + 20 * CELLSIZE;
            height = (5 * length + 1) * CELLSIZE;
            back.posX = width * 18 / 100;
            back.posY = height * 85 / 100;
            back.width = width * 33 / 100;
            back.height = height * 95 / 100;
            strcpy_s(back.name, 5, "BACK");
            showButton(&back, 0, 105, 92, length);

            Button reset;
            width = (5 * length + 1) * CELLSIZE + 20 * CELLSIZE;
            height = (5 * length + 1) * CELLSIZE;
            reset.posX = width * 43 / 100;
            reset.posY = height * 85 / 100;
            reset.width = width * 58 / 100;
            reset.height = height * 95 / 100;
            strcpy_s(reset.name, 6, "RESET");
            showButton(&reset, 0, 105, 92, length);

            Button apply;
            width = (5 * length + 1) * CELLSIZE + 20 * CELLSIZE;
            height = (5 * length + 1) * CELLSIZE;
            apply.posX = width * 68 / 100;
            apply.posY = height * 85 / 100;
            apply.width = width * 83 / 100;
            apply.height = height * 95 / 100;
            strcpy_s(apply.name, 6, "APPLY");
            showButton(&apply, 0, 105, 92, length);

            al_flip_display();
            while (1) {
                al_wait_for_event(queue, &event);
                if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                    return 0;
                }
                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                    if (event.mouse.button == 1) {
                        if (checkButton(&leftArrow1, event.mouse.x, event.mouse.y)) {
                            if (data.length <= 14 && data.length > 6) {
                                data.length -= 1;
                                al_draw_filled_rectangle((rightArrow1.posX + leftArrow1.posX) / 2 - 30, (rightArrow1.posY + leftArrow1.posY) / 2 - 10, (rightArrow1.posX + leftArrow1.posX) / 2 + 50, (rightArrow1.posY + leftArrow1.posY) / 2 + 30, al_map_rgb(120, 144, 156));
                                al_draw_textf(font, al_map_rgb(0, 0, 0), (rightArrow1.posX + leftArrow1.posX) / 2 + 20, (rightArrow1.posY + leftArrow1.posY) / 2 - 20, ALLEGRO_ALIGN_CENTRE, "%d", data.length);
                                al_flip_display();
                            }
                        }
                        if (checkButton(&rightArrow1, event.mouse.x, event.mouse.y)) {
                            if (data.length < 14 && data.length >= 6) {
                                data.length += 1;
                                al_draw_filled_rectangle((rightArrow1.posX + leftArrow1.posX) / 2 - 30, (rightArrow1.posY + leftArrow1.posY) / 2 - 10, (rightArrow1.posX + leftArrow1.posX) / 2 + 50, (rightArrow1.posY + leftArrow1.posY) / 2 + 30, al_map_rgb(120, 144, 156));
                                al_draw_textf(font, al_map_rgb(0, 0, 0), (rightArrow1.posX + leftArrow1.posX) / 2 + 20, (rightArrow1.posY + leftArrow1.posY) / 2 - 20, ALLEGRO_ALIGN_CENTRE, "%d", data.length);
                                al_flip_display();
                            }
                        }
                        if (checkButton(&leftArrow2, event.mouse.x, event.mouse.y)) {
                            if (player.numberWall <= 14 && player.numberWall > 6) {
                                player.numberWall -= 1;
                                al_draw_filled_rectangle((rightArrow1.posX + leftArrow1.posX) / 2 - 30, (rightArrow1.posY + leftArrow1.posY) / 2 + 90, (rightArrow1.posX + leftArrow1.posX) / 2 + 30 + 30, (rightArrow1.posY + leftArrow1.posY) / 2 + 130, al_map_rgb(120, 144, 156));
                                al_draw_textf(font, al_map_rgb(0, 0, 0), (rightArrow2.posX + leftArrow2.posX) / 2 + 20, (rightArrow2.posY + leftArrow2.posY) / 2 - 20, ALLEGRO_ALIGN_CENTRE, "%d", player.numberWall);
                                al_flip_display();
                            }
                        }
                        if (checkButton(&rightArrow2, event.mouse.x, event.mouse.y)) {
                            if (player.numberWall < 14 && player.numberWall >= 6) {
                                player.numberWall += 1;
                                al_draw_filled_rectangle((rightArrow1.posX + leftArrow1.posX) / 2 - 30, (rightArrow1.posY + leftArrow1.posY) / 2 + 90, (rightArrow1.posX + leftArrow1.posX) / 2 + 30 + 30, (rightArrow1.posY + leftArrow1.posY) / 2 + 130, al_map_rgb(120, 144, 156));
                                al_draw_textf(font, al_map_rgb(0, 0, 0), (rightArrow2.posX + leftArrow2.posX) / 2 + 20, (rightArrow2.posY + leftArrow2.posY) / 2 - 20, ALLEGRO_ALIGN_CENTRE, "%d", player.numberWall);
                                al_flip_display();
                            }
                        }
                        if (checkButton(&leftArrow3, event.mouse.x, event.mouse.y)) {
                            if (charm.block <= 6 && charm.block > 0) {
                                charm.block -= 1;
                                al_draw_filled_rectangle((rightArrow3.posX + leftArrow3.posX) / 2 - 30, (rightArrow3.posY + leftArrow3.posY) / 2 - 10, (rightArrow3.posX + leftArrow3.posX) / 2 + 35, (rightArrow3.posY + leftArrow3.posY) / 2 + 30, al_map_rgb(120, 144, 156));
                                al_draw_textf(font, al_map_rgb(0, 0, 0), (rightArrow3.posX + leftArrow3.posX) / 2 + 15, (rightArrow3.posY + leftArrow3.posY) / 2 - 20, ALLEGRO_ALIGN_CENTRE, "%d", charm.block);
                                al_flip_display();
                            }
                        }
                        if (checkButton(&rightArrow3, event.mouse.x, event.mouse.y)) {
                            if (charm.block < 6 && charm.block >= 0) {
                                charm.block += 1;
                                al_draw_filled_rectangle((rightArrow3.posX + leftArrow3.posX) / 2 - 30, (rightArrow3.posY + leftArrow3.posY) / 2 - 10, (rightArrow3.posX + leftArrow3.posX) / 2 + 35, (rightArrow3.posY + leftArrow3.posY) / 2 + 30, al_map_rgb(120, 144, 156));
                                al_draw_textf(font, al_map_rgb(0, 0, 0), (rightArrow3.posX + leftArrow3.posX) / 2 + 15, (rightArrow3.posY + leftArrow3.posY) / 2 - 20, ALLEGRO_ALIGN_CENTRE, "%d", charm.block);
                                al_flip_display();
                            }
                        }
                        if (checkButton(&leftArrow4, event.mouse.x, event.mouse.y)) {
                            if (charm.delet_all_wall <= 4 && charm.delet_all_wall > 0) {
                                charm.delet_all_wall -= 1;
                                al_draw_filled_rectangle((rightArrow4.posX + leftArrow4.posX) / 2 - 30, (rightArrow4.posY + leftArrow4.posY) / 2 - 10, (rightArrow4.posX + leftArrow4.posX) / 2 + 35, (rightArrow4.posY + leftArrow4.posY) / 2 + 30, al_map_rgb(120, 144, 156));
                                al_draw_textf(font, al_map_rgb(0, 0, 0), (rightArrow4.posX + leftArrow4.posX) / 2 + 15, (rightArrow4.posY + leftArrow4.posY) / 2 - 20, ALLEGRO_ALIGN_CENTRE, "%d", charm.delet_all_wall);
                                al_flip_display();
                            }
                        }
                        if (checkButton(&rightArrow4, event.mouse.x, event.mouse.y)) {
                            if (charm.delet_all_wall < 4 && charm.delet_all_wall >= 0) {
                                charm.delet_all_wall += 1;
                                al_draw_filled_rectangle((rightArrow4.posX + leftArrow4.posX) / 2 - 30, (rightArrow4.posY + leftArrow4.posY) / 2 - 10, (rightArrow4.posX + leftArrow4.posX) / 2 + 35, (rightArrow4.posY + leftArrow4.posY) / 2 + 30, al_map_rgb(120, 144, 156));
                                al_draw_textf(font, al_map_rgb(0, 0, 0), (rightArrow4.posX + leftArrow4.posX) / 2 + 15, (rightArrow4.posY + leftArrow4.posY) / 2 - 20, ALLEGRO_ALIGN_CENTRE, "%d", charm.delet_all_wall);
                                al_flip_display();
                            }
                        }
                        if (checkButton(&back, event.mouse.x, event.mouse.y)) {
                            al_play_sample(audioClick, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

                            page = Previous;
                            Previous = setting;
                            break;
                        }
                        if (checkButton(&reset, event.mouse.x, event.mouse.y)) {
                            al_play_sample(audioClick, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

                            data.length = 12;
                            player.numberWall = 12;
                            charm.block = 2;
                            charm.delet_all_wall = 2;
                            //al_draw_filled_rectangle((5 * data.length + 1)* CELLSIZE, 0, (5 * data.length + 1)* CELLSIZE + 20 * CELLSIZE, (5 * data.length + 1)* CELLSIZE, al_map_rgb(144, 164, 174));
                            width = (5 * length + 1) * CELLSIZE + 20 * CELLSIZE;
                            height = (5 * length + 1) * CELLSIZE;
                            al_draw_line(0, height * 5 / 100, width, height * 5 / 100, al_map_rgb(0, 0, 0), 5.0);
                            al_draw_text(customFont, al_map_rgb(0, 0, 0), width / 2, height / 10 - 10, ALLEGRO_ALIGN_CENTRE, "SETTING");
                            al_draw_line(0, height * 15 / 100, width, height * 15 / 100, al_map_rgb(0, 0, 0), 5.0);
                            int Xpiece = width * 5 / 100;
                            int Ypiece = height * 8 / 100;
                            int widthPiece = width * 10 / 100;
                            int heightPiece = height * 15 / 100;
                            al_draw_scaled_bitmap(icon.length, 0, 0, al_get_bitmap_width(icon.length), al_get_bitmap_height(icon.length), Xpiece + 50, Ypiece + 50, widthPiece, heightPiece, 0);
                            //al_draw_text(customFont, al_map_rgb(0, 0, 0), (Xpiece + widthPiece) / 2 + 120, (Ypiece + 50 + heightPiece) / 2 + 20, ALLEGRO_ALIGN_CENTRE, "=");
                            Xpiece = width * 3 / 100;
                            Ypiece = height * 3 / 100;
                            widthPiece = width * 5 / 100;
                            heightPiece = height * 5 / 100;
                            al_draw_scaled_bitmap(icon.Equal, 0, 0, al_get_bitmap_width(icon.Equal), al_get_bitmap_height(icon.Equal), Xpiece + 110 + 50, Ypiece + 120, widthPiece, heightPiece, 0);
                            al_draw_filled_rectangle((rightArrow1.posX + leftArrow1.posX) / 2 - 30, (rightArrow1.posY + leftArrow1.posY) / 2 - 10, (rightArrow1.posX + leftArrow1.posX) / 2 + 50, (rightArrow1.posY + leftArrow1.posY) / 2 + 30, al_map_rgb(120, 144, 156));

                            Button leftArrow1;
                            leftArrow1.posX = ((width * 3) / 100) + 105 + 90 + 50;
                            leftArrow1.posY = ((height * 3) / 100) + 115;
                            leftArrow1.width = ((width * 5) / 100) + leftArrow1.posX;
                            leftArrow1.height = ((height * 5) / 100) + leftArrow1.posY;
                            al_draw_scaled_bitmap(icon.leftFlash1, 0, 0, al_get_bitmap_width(icon.leftFlash1), al_get_bitmap_height(icon.leftFlash1), leftArrow1.posX, leftArrow1.posY, leftArrow1.width - leftArrow1.posX, leftArrow1.height - leftArrow1.posY, 0);
                            al_draw_filled_rectangle((rightArrow1.posX + leftArrow1.posX) / 2 - 30, (rightArrow1.posY + leftArrow1.posY) / 2 - 10, (rightArrow1.posX + leftArrow1.posX) / 2 + 50, (rightArrow1.posY + leftArrow1.posY) / 2 + 30, al_map_rgb(120, 144, 156));
                            al_draw_textf(font, al_map_rgb(0, 0, 0), (rightArrow1.posX + leftArrow1.posX) / 2 + 20, (rightArrow1.posY + leftArrow1.posY) / 2 - 20, ALLEGRO_ALIGN_CENTRE, "%d", data.length);

                            Button rightArrow1;
                            rightArrow1.posX = width * 3 / 100 + 105 + 90 + 180 + 50;
                            rightArrow1.posY = height * 3 / 100 + 115;
                            rightArrow1.width = width * 5 / 100 + rightArrow1.posX;
                            rightArrow1.height = height * 5 / 100 + rightArrow1.posY;


                            al_draw_filled_rectangle((rightArrow1.posX + leftArrow1.posX) / 2 - 30, (rightArrow1.posY + leftArrow1.posY) / 2 - 10, (rightArrow1.posX + leftArrow1.posX) / 2 + 50, (rightArrow1.posY + leftArrow1.posY) / 2 + 30, al_map_rgb(120, 144, 156));
                            al_draw_textf(font, al_map_rgb(0, 0, 0), (rightArrow1.posX + leftArrow1.posX) / 2 + 20, (rightArrow1.posY + leftArrow1.posY) / 2 - 20, ALLEGRO_ALIGN_CENTRE, "%d", data.length);

                            //al_draw_textf(font, al_map_rgb(0, 0, 0), (rightArrow1.posX + leftArrow1.posX) / 2 + 20, (rightArrow1.posY + leftArrow1.posY) / 2 - 20, ALLEGRO_ALIGN_CENTRE, "%d", player.numberWall);

                            //-----------------------------------------------------------countWall----------------
                            Xpiece = width * 5 / 100;
                            Ypiece = height * 8 / 100;
                            widthPiece = width * 10 / 100;
                            heightPiece = height * 15 / 100;
                            al_draw_scaled_bitmap(Walls.putWall, 0, 0, al_get_bitmap_width(Walls.putWall), al_get_bitmap_height(Walls.putWall), Xpiece + 50, Ypiece + 50 + 100, widthPiece, heightPiece, 0);
                            //al_draw_text(customFont, al_map_rgb(0, 0, 0), (Xpiece + widthPiece) / 2 + 120, (Ypiece + 50 + heightPiece) / 2 + 20, ALLEGRO_ALIGN_CENTRE, "=");
                            Xpiece = width * 3 / 100;
                            Ypiece = height * 3 / 100;
                            widthPiece = width * 5 / 100;
                            heightPiece = height * 5 / 100;

                            al_draw_scaled_bitmap(icon.Equal, 0, 0, al_get_bitmap_width(icon.Equal), al_get_bitmap_height(icon.Equal), Xpiece + 110 + 50, Ypiece + 120 + 100, widthPiece, heightPiece, 0);
                            al_draw_filled_rectangle((rightArrow1.posX + leftArrow1.posX) / 2 - 30, (rightArrow1.posY + leftArrow1.posY) / 2 + 90, (rightArrow1.posX + leftArrow1.posX) / 2 + 30, (rightArrow1.posY + leftArrow1.posY) / 2 + 130, al_map_rgb(120, 144, 156));

                            Button leftArrow2;
                            leftArrow2.posX = width * 3 / 100 + 105 + 90 + 50;
                            leftArrow2.posY = height * 3 / 100 + 115 + 100;
                            leftArrow2.width = width * 5 / 100 + leftArrow2.posX;
                            leftArrow2.height = height * 5 / 100 + leftArrow2.posY;
                            al_draw_scaled_bitmap(icon.leftFlash2, 0, 0, al_get_bitmap_width(icon.leftFlash2), al_get_bitmap_height(icon.leftFlash2), leftArrow2.posX, leftArrow2.posY, leftArrow2.width - leftArrow2.posX, leftArrow2.height - leftArrow2.posY, 0);
                            al_draw_filled_rectangle((rightArrow1.posX + leftArrow1.posX) / 2 - 30, (rightArrow1.posY + leftArrow1.posY) / 2 + 90, (rightArrow1.posX + leftArrow1.posX) / 2 + 30 + 30, (rightArrow1.posY + leftArrow1.posY) / 2 + 130, al_map_rgb(120, 144, 156));
                            al_draw_textf(font, al_map_rgb(0, 0, 0), (rightArrow2.posX + leftArrow2.posX) / 2 + 20, (rightArrow2.posY + leftArrow2.posY) / 2 - 20, ALLEGRO_ALIGN_CENTRE, "%d", player.numberWall);


                            Button rightArrow2;
                            rightArrow2.posX = width * 3 / 100 + 105 + 90 + 180 + 50;
                            rightArrow2.posY = height * 3 / 100 + 115 + 100;
                            rightArrow2.width = width * 5 / 100 + rightArrow2.posX;
                            rightArrow2.height = height * 5 / 100 + rightArrow2.posY;
                            al_draw_scaled_bitmap(icon.rightFlash2, 0, 0, al_get_bitmap_width(icon.rightFlash2), al_get_bitmap_height(icon.rightFlash2), rightArrow2.posX, rightArrow2.posY, rightArrow2.width - rightArrow2.posX, rightArrow2.height - rightArrow2.posY, 0);
                            al_draw_textf(font, al_map_rgb(0, 0, 0), (rightArrow2.posX + leftArrow2.posX) / 2, (rightArrow2.posY + leftArrow2.posY) / 2 - 20, ALLEGRO_ALIGN_CENTRE, "%d", player.numberWall);
                            al_draw_text(fontHelp, al_map_rgb(0, 0, 0), (rightArrow2.posX + leftArrow2.posX) / 2 - 80, (rightArrow2.posY + leftArrow2.posY) / 2 + 80, ALLEGRO_ALIGN_CENTRE, "Number of blocking rounds");
                            al_draw_scaled_bitmap(icon.Equal, 0, 0, al_get_bitmap_width(icon.Equal), al_get_bitmap_height(icon.Equal), (rightArrow2.posX + leftArrow2.posX) / 2 + 120, (rightArrow2.posY + leftArrow2.posY) / 2 + 90, width * 5 / 100, height * 5 / 100, 0);
                            al_draw_filled_rectangle((rightArrow1.posX + leftArrow1.posX) / 2 - 30, (rightArrow1.posY + leftArrow1.posY) / 2 + 90, (rightArrow1.posX + leftArrow1.posX) / 2 + 30 + 30, (rightArrow1.posY + leftArrow1.posY) / 2 + 130, al_map_rgb(120, 144, 156));

                            al_draw_textf(font, al_map_rgb(0, 0, 0), (rightArrow2.posX + leftArrow2.posX) / 2 + 20, (rightArrow2.posY + leftArrow2.posY) / 2 - 20, ALLEGRO_ALIGN_CENTRE, "%d", player.numberWall);


                            Button leftArrow3;
                            leftArrow3.posX = width * 3 / 100 + 105 + 90 + 50 + 260;
                            leftArrow3.posY = height * 3 / 100 + 115 + 100 + 90;
                            leftArrow3.width = ((width * 5) / 100) + leftArrow3.posX;
                            leftArrow3.height = ((height * 5) / 100) + leftArrow3.posY;
                            al_draw_scaled_bitmap(icon.leftFlash1, 0, 0, al_get_bitmap_width(icon.leftFlash3), al_get_bitmap_height(icon.leftFlash3), leftArrow3.posX, leftArrow3.posY, leftArrow3.width - leftArrow3.posX, leftArrow3.height - leftArrow3.posY, 0);

                            al_draw_filled_rectangle((rightArrow3.posX + leftArrow3.posX) / 2 - 30, (rightArrow3.posY + leftArrow3.posY) / 2 - 10, (rightArrow3.posX + leftArrow3.posX) / 2 + 35, (rightArrow3.posY + leftArrow3.posY) / 2 + 30, al_map_rgb(120, 144, 156));

                            Button rightArrow3;
                            rightArrow3.posX = width * 3 / 100 + 105 + 90 + 50 + 260 + 180;
                            rightArrow3.posY = height * 3 / 100 + 115 + 100 + 90;
                            rightArrow3.width = ((width * 5) / 100) + rightArrow3.posX;
                            rightArrow3.height = ((height * 5) / 100) + rightArrow3.posY;
                            al_draw_scaled_bitmap(icon.rightFlash1, 0, 0, al_get_bitmap_width(icon.rightFlash3), al_get_bitmap_height(icon.rightFlash3), rightArrow3.posX, rightArrow3.posY, rightArrow3.width - rightArrow3.posX, rightArrow3.height - rightArrow3.posY, 0);
                            al_draw_textf(font, al_map_rgb(0, 0, 0), (rightArrow3.posX + leftArrow3.posX) / 2 + 20, (rightArrow3.posY + leftArrow3.posY) / 2 - 20, ALLEGRO_ALIGN_CENTRE, "%d", charm.block);

                            al_draw_text(fontHelp, al_map_rgb(0, 0, 0), (rightArrow2.posX + leftArrow2.posX) / 2 - 92, (rightArrow2.posY + leftArrow2.posY) / 2 + 150, ALLEGRO_ALIGN_CENTRE, "Number of deleting walls");
                            al_draw_scaled_bitmap(icon.Equal, 0, 0, al_get_bitmap_width(icon.Equal), al_get_bitmap_height(icon.Equal), (rightArrow2.posX + leftArrow2.posX) / 2 + 110, (rightArrow2.posY + leftArrow2.posY) / 2 + 157, width * 5 / 100, height * 5 / 100, 0);

                            al_draw_filled_rectangle((rightArrow4.posX + leftArrow4.posX) / 2 - 30, (rightArrow4.posY + leftArrow4.posY) / 2 - 10, (rightArrow4.posX + leftArrow4.posX) / 2 + 35, (rightArrow4.posY + leftArrow4.posY) / 2 + 30, al_map_rgb(120, 144, 156));


                            Button leftArrow4;
                            leftArrow4.posX = width * 3 / 100 + 105 + 90 + 50 + 260;
                            leftArrow4.posY = height * 3 / 100 + 115 + 100 + 90 + 70;
                            leftArrow4.width = ((width * 5) / 100) + leftArrow4.posX;
                            leftArrow4.height = ((height * 5) / 100) + leftArrow4.posY;
                            al_draw_scaled_bitmap(icon.leftFlash4, 0, 0, al_get_bitmap_width(icon.leftFlash4), al_get_bitmap_height(icon.leftFlash4), leftArrow4.posX, leftArrow4.posY, leftArrow4.width - leftArrow4.posX, leftArrow4.height - leftArrow4.posY, 0);

                            al_draw_filled_rectangle((rightArrow4.posX + leftArrow4.posX) / 2 - 30, (rightArrow4.posY + leftArrow4.posY) / 2 - 10, (rightArrow4.posX + leftArrow4.posX) / 2 + 35, (rightArrow4.posY + leftArrow4.posY) / 2 + 30, al_map_rgb(120, 144, 156));


                            Button rightArrow4;
                            rightArrow4.posX = width * 3 / 100 + 105 + 90 + 50 + 260 + 180;
                            rightArrow4.posY = height * 3 / 100 + 115 + 100 + 90 + 70;
                            rightArrow4.width = ((width * 5) / 100) + rightArrow4.posX;
                            rightArrow4.height = ((height * 5) / 100) + rightArrow4.posY;
                            al_draw_scaled_bitmap(icon.rightFlash4, 0, 0, al_get_bitmap_width(icon.rightFlash4), al_get_bitmap_height(icon.rightFlash4), rightArrow4.posX, rightArrow4.posY, rightArrow4.width - rightArrow4.posX, rightArrow4.height - rightArrow4.posY, 0);
                            al_draw_textf(font, al_map_rgb(0, 0, 0), (rightArrow4.posX + leftArrow4.posX) / 2 + 20, (rightArrow4.posY + leftArrow4.posY) / 2 - 20, ALLEGRO_ALIGN_CENTRE, "%d", charm.delet_all_wall);
                            al_flip_display();

                        }
                        if (checkButton(&apply, event.mouse.x, event.mouse.y)) {
                            al_play_sample(audioClick, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

                            page = playMenu;
                            Previous = setting;
                            break;
                        }
                    }
                }

            }
            al_flip_display();
            al_destroy_display(display);
        }
        if (page == playMenu) {

            ALLEGRO_DISPLAY* display = al_create_display((5 * length + 1) * CELLSIZE + 20 * CELLSIZE, (5 * length + 1) * CELLSIZE);
            ALLEGRO_EVENT event;
            ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
            al_register_event_source(queue, al_get_mouse_event_source());

            al_register_event_source(queue, al_get_display_event_source(display));
            //al_reserve_samples(1);
            al_clear_to_color(al_map_rgb(120, 144, 156));

            //al_draw_filled_rectangle((5 * data.length + 1)* CELLSIZE, 0, (5 * data.length + 1)* CELLSIZE + 20 * CELLSIZE, (5 * data.length + 1)* CELLSIZE, al_map_rgb(144, 164, 174));
            width = (5 * length + 1) * CELLSIZE + 20 * CELLSIZE;
            height = (5 * length + 1) * CELLSIZE;
            al_draw_line(0, height * 5 / 100, width, height * 5 / 100, al_map_rgb(0, 0, 0), 5.0);
            al_draw_text(customFont, al_map_rgb(0, 0, 0), width / 2, height / 10 - 10, ALLEGRO_ALIGN_CENTRE, "PLAYER MODE");
            al_draw_line(0, height * 15 / 100, width, height * 15 / 100, al_map_rgb(0, 0, 0), 5.0);

            Button submit1;

            submit1.posX = width * 24 / 100;
            submit1.posY = height * 34 / 100;
            submit1.width = width * 44 / 100;
            submit1.height = height * 45 / 100;
            strcpy_s(submit1.name, 10, "NONE");
            showButton(&submit1, 255, 255, 255, length);

            Button submit2;

            submit2.posX = width * 50 / 100;
            submit2.posY = height * 34 / 100;
            submit2.width = width * 70 / 100;
            submit2.height = height * 45 / 100;
            strcpy_s(submit2.name, 10, "NONE");
            showButton(&submit2, 255, 255, 255, length);


            Button submit3;

            submit3.posX = width * 50 / 100;
            submit3.posY = height * 50 / 100;
            submit3.width = width * 70 / 100;
            submit3.height = height * 60 / 100;
            strcpy_s(submit3.name, 10, "NONE");
            showButton(&submit3, 255, 255, 255, length);

            Button submit4;

            submit4.posX = width * 24 / 100;
            submit4.posY = height * 50 / 100;
            submit4.width = width * 44 / 100;
            submit4.height = height * 60 / 100;
            strcpy_s(submit4.name, 10, "NONE");
            showButton(&submit4, 255, 255, 255, length);

            Button back;
            width = (5 * length + 1) * CELLSIZE + 20 * CELLSIZE;
            height = (5 * length + 1) * CELLSIZE;
            back.posX = width * 10 / 100;
            back.posY = height * 70 / 100;
            back.width = width * 31 / 100;
            back.height = height * 80 / 100;
            strcpy_s(back.name, 10, "BACK");
            showButton(&back, 30, 136, 222, length);

            Button Setting;
            width = (5 * length + 1) * CELLSIZE + 20 * CELLSIZE;
            height = (5 * length + 1) * CELLSIZE;
            Setting.posX = width * 38 / 100;
            Setting.posY = height * 70 / 100;
            Setting.width = width * 60 / 100;
            Setting.height = height * 80 / 100;
            strcpy_s(Setting.name, 10, "SETTING");
            showButton(&Setting, 30, 136, 222, length);

            Button play;
            width = (5 * length + 1) * CELLSIZE + 20 * CELLSIZE;
            height = (5 * length + 1) * CELLSIZE;
            play.posX = width * 68 / 100;
            play.posY = height * 70 / 100;
            play.width = width * 90 / 100;
            play.height = height * 80 / 100;
            strcpy_s(play.name, 10, "PLAY");
            showButton(&play, 30, 136, 222, length);

            int sumNone = 0, sumHuman = 0, sumComputer = 0;
            sumGame.sumHuman = 0;
            sumGame.sumComuter = 0;

            al_flip_display();
            while (1) {
                al_wait_for_event(queue, &event);
                if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                    return 0;
                }
                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                    if (event.mouse.button == 1) {
                        if (checkButton(&submit1, event.mouse.x, event.mouse.y)) {

                            al_play_sample(audioPiece, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            if (strcmp(submit1.name, "NONE") == 0) {
                                strcpy_s(submit1.name, 10, "HUMAN");

                            }
                            else if (!strcmp(submit1.name, "HUMAN")) {

                                strcpy_s(submit1.name, 10, "COMPUTER");

                            }
                            else if (!strcmp(submit1.name, "COMPUTER")) {

                                strcpy_s(submit1.name, 10, "NONE");

                            }

                            showButton(&submit1, 255, 255, 255, length);
                            al_flip_display();
                        }


                        if (checkButton(&submit2, event.mouse.x, event.mouse.y)) {
                            al_play_sample(audioPiece, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            if (strcmp(submit2.name, "NONE") == 0) {

                                strcpy_s(submit2.name, 10, "HUMAN");


                            }
                            else if (!strcmp(submit2.name, "HUMAN")) {

                                strcpy_s(submit2.name, 10, "COMPUTER");

                            }
                            else if (!strcmp(submit2.name, "COMPUTER")) {

                                strcpy_s(submit2.name, 10, "NONE");


                            }

                            showButton(&submit2, 255, 255, 255, length);
                            al_flip_display();
                        }


                        if (checkButton(&submit3, event.mouse.x, event.mouse.y)) {

                            al_play_sample(audioPiece, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            if (strcmp(submit3.name, "NONE") == 0) {

                                strcpy_s(submit3.name, 10, "HUMAN");


                            }
                            else if (!strcmp(submit3.name, "HUMAN")) {

                                strcpy_s(submit3.name, 10, "COMPUTER");

                            }
                            else if (!strcmp(submit3.name, "COMPUTER")) {

                                strcpy_s(submit3.name, 10, "NONE");


                            }


                            showButton(&submit3, 255, 255, 255, length);
                            al_flip_display();
                        }


                        if (checkButton(&submit4, event.mouse.x, event.mouse.y)) {

                            al_play_sample(audioPiece, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            if (strcmp(submit4.name, "NONE") == 0) {

                                strcpy_s(submit4.name, 10, "HUMAN");



                            }
                            else if (!strcmp(submit4.name, "HUMAN")) {

                                strcpy_s(submit4.name, 10, "COMPUTER");


                            }
                            else if (!strcmp(submit4.name, "COMPUTER")) {

                                strcpy_s(submit4.name, 10, "NONE");


                            }
                            showButton(&submit4, 255, 255, 255, length);
                            al_flip_display();
                        }

                        if (checkButton(&back, event.mouse.x, event.mouse.y)) {

                            al_play_sample(audioClick, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            page = Previous;
                            Previous = playMenu;
                            break;
                        }
                        if (checkButton(&Setting, event.mouse.x, event.mouse.y)) {
                            al_play_sample(audioClick, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            Previous = playMenu;
                            page = setting;
                            break;
                        }
                        if (checkButton(&play, event.mouse.x, event.mouse.y)) {
                            sumGame.sumComuter = 0, sumGame.sumHuman = 0;
                            if (!strcmp(submit1.name, "HUMAN")) sumGame.sumHuman++;
                            if (!strcmp(submit1.name, "COMPUTER")) sumGame.sumComuter++;
                            if (!strcmp(submit2.name, "HUMAN")) sumGame.sumHuman++;
                            if (!strcmp(submit2.name, "COMPUTER")) sumGame.sumComuter++;
                            if (!strcmp(submit3.name, "HUMAN")) sumGame.sumHuman++;
                            if (!strcmp(submit3.name, "COMPUTER")) sumGame.sumComuter++;
                            if (!strcmp(submit4.name, "HUMAN")) sumGame.sumHuman++;
                            if (!strcmp(submit4.name, "COMPUTER")) sumGame.sumComuter++;
                            if ((sumGame.sumComuter + sumGame.sumHuman) % 2 == 0 && (sumGame.sumComuter != 0 || sumGame.sumHuman != 0)) {
                                al_play_sample(audioClick, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                printf("%d\t%d\n", sumGame.sumComuter, sumGame.sumHuman);
                                Previous = playMenu;
                                page = startGame;
                                break;
                            }

                        }
                    }
                }
            }

            al_flip_display();
            al_destroy_display(display);
        }
        if (page == startGame) {
            ALLEGRO_DISPLAY* display = al_create_display((5 * data.length + 1) * CELLSIZE + 20 * CELLSIZE, (5 * data.length + 1) * CELLSIZE);
            ALLEGRO_EVENT event;
            ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
            al_register_event_source(queue, al_get_mouse_event_source());
            al_register_event_source(queue, al_get_display_event_source(display));
            ///---------------------------------------------------------$ 4 Players  $---------------------------------------------
            if (sumGame.sumHuman == 4 && sumGame.sumComuter == 0) {
                Button save, load;
                ALLEGRO_DISPLAY* display = al_create_display((5 * data.length + 1) * CELLSIZE + 20 * CELLSIZE, (5 * data.length + 1) * CELLSIZE);
                ALLEGRO_EVENT event;
                ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
                al_register_event_source(queue, al_get_mouse_event_source());
                al_register_event_source(queue, al_get_display_event_source(display));
                width = (5 * data.length + 1) * CELLSIZE + 20 * CELLSIZE;
                height = (5 * data.length + 1) * CELLSIZE;

                save.posX = width * 80 / 100;
                save.posY = height * 35 / 100;
                save.width = width * 90 / 100;
                save.height = height * 44 / 100;
                strcpy_s(save.name, 5, "SAVE");
                /*load.posX = width * 80 / 100;
                load.posY = height * 15 / 100;
                load.width = width * 90 / 100;
                load.height = height * 24 / 100;
                strcpy_s(load.name, 5, "LOAD");*/
                makePrimaryBoard(&data);
                showBoard(data, showPlayer, showPossiblity, Walls);
                showButton(&save, 255, 255, 255, data.length);
                showButton(&load, 255, 255, 255, data.length);
                ALLEGRO_BITMAP* oldDisplay;
                data.board[0][data.length - data.length % 2] = 1;
                data.board[data.length * 2 - 2][data.length - data.length % 2] = 2;
                data.board[(data.length - data.length % 2)][0] = 3;
                data.board[(data.length - data.length % 2)][data.length * 2 - data.length % 2 - 2] = 4;
                player.pieceCoordinate[0] = 0;
                player.pieceCoordinate[1] = data.length - data.length % 2;
                player.pieceCoordinate[2] = data.length * 2 - 2;
                player.pieceCoordinate[3] = data.length - data.length % 2;
                player.pieceCoordinate[4] = data.length - data.length % 2;
                player.pieceCoordinate[5] = 0;
                player.pieceCoordinate[6] = data.length - data.length % 2;
                player.pieceCoordinate[7] = data.length * 2 - data.length % 2 - 2;
                int m = 0;


                while (1) {
                    winner = 0;
                    checkingWall = 0;
                    checkingPiece = 0;

                    if (m % 4 == 0)player.term = 1;
                    else if (m % 4 == 1) player.term = 2;
                    else if (m % 4 == 2) player.term = 3;
                    else if (m % 4 == 3)player.term = 4;

                    movePiecePossiblities(&data, &player);
                    movePiecePossiblitiesJump(&data, &player);
                    movePiecePossiblitiesParties(&data, &player);

                    showBoard(data, showPlayer, showPossiblity, Walls);
                    showButton(&save, 255, 255, 255, data.length);
                    showButton(&load, 255, 255, 255, data.length);
                    al_flip_display();
                    oldDisplay = al_clone_bitmap(al_get_backbuffer(display));

                    while (1) {

                        al_wait_for_event(queue, &event);

                        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                            return 0;
                        }

                        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                            if (event.mouse.button == 1) {

                                if (checkButton(&save, event.mouse.x, event.mouse.y)) {
                                    al_play_sample(audioClick, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    FILE* file;
                                    fopen_s(&file, "./Save.dat", "wb");
                                    fwrite(&data, sizeof(data), 1, file);
                                    fwrite(&player, sizeof(player), 1, file);
                                    fclose(file);
                                    continue;
                                }
                                /*if (checkButton(&load, event.mouse.x, event.mouse.y)) {

                                    FILE* file;
                                    fopen_s(&file, "./Save.dat", "rb");
                                    if (!file) continue;
                                    fread(&data, sizeof(data), 1, file);
                                    fread(&player, sizeof(player), 1, file);
                                    al_unregister_event_source(queue, al_get_display_event_source(display));

                                    al_destroy_display(display);
                                    display = al_create_display((5 * data.length + 1) * CELLSIZE + 20 * CELLSIZE, (5 * data.length + 1) * CELLSIZE);
                                    al_register_event_source(queue, al_get_display_event_source(display));
                                    width = (5 * data.length + 1) * CELLSIZE + 20 * CELLSIZE;
                                    height = (5 * data.length + 1) * CELLSIZE;

                                    Button save;
                                    //Button load;
                                    save.posX = width * 80 / 100;
                                    save.posY = height * 35 / 100;
                                    save.width = width * 90 / 100;
                                    save.height = height * 44 / 100;
                                    strcpy_s(save.name, 5, "SAVE");
                                    Button load;
                                    load.posX = width * 80 / 100;
                                    load.posY = height * 15 / 100;
                                    load.width = width * 90 / 100;
                                    load.height = height * 24 / 100;
                                    strcpy_s(load.name, 5, "LOAD");

                                    showBoard(data, showPlayer, showPossiblity, Walls);
                                    showButton(&save, 255, 255, 255, data.length);
                                    showButton(&load, 255, 255, 255, data.length);
                                    al_flip_display();
                                    oldDisplay = al_clone_bitmap(al_get_backbuffer(display));


                                    al_play_sample(audioClick, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    fclose(file);
                                    continue;
                                }
                                */
                                putWalls(&data, &player, queue, ev, &go, 4, event.mouse.x, event.mouse.y, 1, &checkingWall, Walls);
                                if (checkingWall) {
                                    al_play_sample(audioWall, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                }

                                movePiece(&data, &player, queue, ev, event.mouse.x, event.mouse.y, &checkingPiece);
                                if (checkingPiece) {
                                    al_play_sample(audioPiece, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                }
                                m += (checkingPiece + checkingWall);

                                showBoard(data, showPlayer, showPossiblity, Walls);
                                showButton(&save, 255, 255, 255, data.length);
                                showButton(&load, 255, 255, 255, data.length);

                                al_flip_display();

                                break;
                            }
                        }

                        if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
                            al_draw_bitmap(oldDisplay, 0, 0, 0);
                            putWalls(&data, &player, queue, ev, &go, 4, event.mouse.x, event.mouse.y, 0, &checkingWall, Walls);
                        }
                    }
                    if (Winner(data, player, m % 4) == 1) {
                        winner = 1;
                        page = winnerPage;
                        break;
                    }
                    else if (Winner(data, player, m % 4) == 2) {
                        winner = 2;
                        page = winnerPage;
                        break;
                    }
                    else if (Winner(data, player, m % 4) == 3) {
                        winner = 3;
                        page = winnerPage;
                        break;
                    }
                    else if (Winner(data, player, m % 4) == 4) {
                        winner = 4;
                        page = winnerPage;
                        break;
                    }
                }

                al_flip_display();
                al_destroy_sample(audioPiece);
                al_destroy_sample(audioWall);
                al_destroy_display(display);
                al_uninstall_audio();
            }
            //----------------------------------------------------------$ 3 players && 1 computer $-------------------------------------------------------
            else if (sumGame.sumComuter == 1 && sumGame.sumHuman == 3) {
                Button save, load;
                ALLEGRO_DISPLAY* display = al_create_display((5 * data.length + 1) * CELLSIZE + 20 * CELLSIZE, (5 * data.length + 1) * CELLSIZE);
                ALLEGRO_EVENT event;
                ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
                al_register_event_source(queue, al_get_mouse_event_source());
                al_register_event_source(queue, al_get_display_event_source(display));
                width = (5 * data.length + 1) * CELLSIZE + 20 * CELLSIZE;
                height = (5 * data.length + 1) * CELLSIZE;

                save.posX = width * 80 / 100;
                save.posY = height * 35 / 100;
                save.width = width * 90 / 100;
                save.height = height * 44 / 100;
                strcpy_s(save.name, 5, "SAVE");
                load.posX = width * 80 / 100;
                load.posY = height * 15 / 100;
                load.width = width * 90 / 100;
                load.height = height * 24 / 100;
                strcpy_s(load.name, 5, "LOAD");
                makePrimaryBoard(&data);
                showBoard(data, showPlayer, showPossiblity, Walls);
                showButton(&save, 255, 255, 255, data.length);
                showButton(&load, 255, 255, 255, data.length);
                ALLEGRO_BITMAP* oldDisplay;
                data.board[0][data.length - data.length % 2] = 1;
                data.board[data.length * 2 - 2][data.length - data.length % 2] = 2;
                data.board[(data.length - data.length % 2)][0] = 3;
                data.board[(data.length - data.length % 2)][data.length * 2 - data.length % 2 - 2] = 4;
                player.pieceCoordinate[0] = 0;
                player.pieceCoordinate[1] = data.length - data.length % 2;
                player.pieceCoordinate[2] = data.length * 2 - 2;
                player.pieceCoordinate[3] = data.length - data.length % 2;
                player.pieceCoordinate[4] = data.length - data.length % 2;
                player.pieceCoordinate[5] = 0;
                player.pieceCoordinate[6] = data.length - data.length % 2;
                player.pieceCoordinate[7] = data.length * 2 - data.length % 2 - 2;
                int m = 0;
                while (1) {
                    winner = 0;
                    checkingWall = 0;
                    checkingPiece = 0;

                    if (m % 4 == 0)player.term = 1;//player1
                    else if (m % 4 == 1) player.term = 2;//Player2
                    else if (m % 4 == 2) player.term = 3;//Player 3
                    else if (m % 4 == 3)player.term = 4;//Computer 4

                    movePiecePossiblities(&data, &player);
                    movePiecePossiblitiesJump(&data, &player);
                    movePiecePossiblitiesParties(&data, &player);

                    showBoard(data, showPlayer, showPossiblity, Walls);
                    showButton(&save, 255, 255, 255, data.length);
                    showButton(&load, 255, 255, 255, data.length);
                    al_flip_display();
                    oldDisplay = al_clone_bitmap(al_get_backbuffer(display));
                    if (m % 4 < 3) {
                        while (1) {

                            al_wait_for_event(queue, &event);

                            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                                return 0;
                            }

                            if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                                if (event.mouse.button == 1) {

                                    if (checkButton(&save, event.mouse.x, event.mouse.y)) {
                                        al_play_sample(audioClick, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                        FILE* file;
                                        fopen_s(&file, "./Save.dat", "wb");
                                        fwrite(&data, sizeof(data), 1, file);
                                        fwrite(&player, sizeof(player), 1, file);
                                        fclose(file);
                                        continue;
                                    }
                                    if (checkButton(&load, event.mouse.x, event.mouse.y)) {

                                        FILE* file;
                                        fopen_s(&file, "./Save.dat", "rb");
                                        if (!file) continue;
                                        fread(&data, sizeof(data), 1, file);
                                        fread(&player, sizeof(player), 1, file);
                                        al_unregister_event_source(queue, al_get_display_event_source(display));

                                        al_destroy_display(display);
                                        display = al_create_display((5 * data.length + 1) * CELLSIZE + 20 * CELLSIZE, (5 * data.length + 1) * CELLSIZE);
                                        al_register_event_source(queue, al_get_display_event_source(display));
                                        width = (5 * data.length + 1) * CELLSIZE + 20 * CELLSIZE;
                                        height = (5 * data.length + 1) * CELLSIZE;

                                        Button save;
                                        //Button load;
                                        save.posX = width * 80 / 100;
                                        save.posY = height * 35 / 100;
                                        save.width = width * 90 / 100;
                                        save.height = height * 44 / 100;
                                        strcpy_s(save.name, 5, "SAVE");
                                        Button load;
                                        load.posX = width * 80 / 100;
                                        load.posY = height * 15 / 100;
                                        load.width = width * 90 / 100;
                                        load.height = height * 24 / 100;
                                        strcpy_s(load.name, 5, "LOAD");

                                        showBoard(data, showPlayer, showPossiblity, Walls);
                                        showButton(&save, 255, 255, 255, data.length);
                                        showButton(&load, 255, 255, 255, data.length);
                                        al_flip_display();
                                        oldDisplay = al_clone_bitmap(al_get_backbuffer(display));


                                        al_play_sample(audioClick, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                        fclose(file);
                                        continue;
                                    }
                                    putWalls(&data, &player, queue, ev, &go, 4, event.mouse.x, event.mouse.y, 1, &checkingWall, Walls);
                                    if (checkingWall) {
                                        al_play_sample(audioWall, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    }

                                    movePiece(&data, &player, queue, ev, event.mouse.x, event.mouse.y, &checkingPiece);
                                    if (checkingPiece) {
                                        al_play_sample(audioPiece, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    }
                                    m += (checkingPiece + checkingWall);

                                    showBoard(data, showPlayer, showPossiblity, Walls);
                                    showButton(&save, 255, 255, 255, data.length);
                                    showButton(&load, 255, 255, 255, data.length);

                                    al_flip_display();

                                    break;
                                }
                            }

                            if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
                                al_draw_bitmap(oldDisplay, 0, 0, 0);
                                putWalls(&data, &player, queue, ev, &go, 4, event.mouse.x, event.mouse.y, 0, &checkingWall, Walls);
                            }
                        }
                    }
                    else if (m % 4 == 3) {
                        srand(time(NULL));
                        int choose = rand() % 2;
                        printf("%d, \n", choose);
                        if (choose == 0) {//move piece
                            //movePiecePossiblities(&data, &player);
                            //movePiecePossiblitiesJump(&data, &player); 
                            //movePiecePossiblitiesParties(&data, &player); ************ notice **************
                            computerMove(&data, &player);


                        }
                        else if (choose == 1) {// computer wall
                            computerWall(&data, &player, 2);
                        }
                        for (int i = 0; i < 2 * data.length - 1; i++) {
                            for (int j = 0; j < 2 * data.length - 1; j++) {
                                //printf("$");
                                if (data.board[i][j] > 60) {
                                    data.board[i][j] = 5;
                                }
                            }
                        }

                        showBoard(data, showPlayer, showPossiblity, Walls);
                        al_flip_display();
                        m++;

                    }

                    if (Winner(data, player, m % 4) == 1) {
                        winner = 1;
                        page = winnerPage;
                        break;
                    }
                    else if (Winner(data, player, m % 4) == 2) {
                        winner = 2;
                        page = winnerPage;
                        break;
                    }
                    else if (Winner(data, player, m % 4) == 3) {
                        winner = 3;
                        page = winnerPage;
                        break;
                    }
                    else if (Winner(data, player, m % 4) == 4) {
                        winner = 4;
                        page = winnerPage;
                        break;
                    }
                }

                al_flip_display();
                al_destroy_sample(audioPiece);
                al_destroy_sample(audioWall);
                al_destroy_display(display);
                al_uninstall_audio();
            }



            //------------------------------------------------------------------------------------------------------------------------------------------------

            /*if (Multiplayer == 2) {
                data.board[0][data.length - data.length % 2] = 1;
                data.board[data.length * 2 - 2][data.length - data.length % 2] = 2;
                player.pieceCoordinate[0] = 0;
                player.pieceCoordinate[1] = data.length - data.length % 2;
                player.pieceCoordinate[2] = data.length * 2 - 2;
                player.pieceCoordinate[3] = data.length - data.length % 2;
                int k = 0;


                while (1) {
                    luck(&data, &player, &charm, &present, Multiplayer);

                    for (register int i = 0; i < 2 * data.length; i++) {
                        for (int j = 0; j < 2 * data.length; j++) {
                            if (data.board[i][j] == 6) data.board[i][j] = 5;
                        }
                    }
                    showBoard(data);
                    if (k % 2 == 0) {//player
                        player.term = 1;

                        showBoard(data);
                        movePiecePossiblities(&data, &player);
                        movePiecePossiblitiesJump(&data, &player);
                        movePiecePossiblitiesParties(&data, &player);


                        while (1) {
                            //showBoard(data);
                            delete_wall(&data);
                            showBoard(data);
                            putWalls(&data, &player, event_queue, ev, &go, 2);
                            showBoard(data);
                            al_flip_display();

                            if (go == 1) {

                                break;
                            }
                            showBoard(data);
                        }
                        //showBoard(data);

                        movePiece(&data, &player, event_queue, ev);


                        showBoard(data);
                    }
                    else if (k % 2 == 1) {//computer
                        player.term = 2;
                        for (register int i = 0; i < 2 * data.length; i++) {
                            for (int j = 0; j < 2 * data.length; j++) {
                                if (data.board[i][j] == 6) data.board[i][j] = 5;
                            }
                        }
                        //showBoard(data);
                        movePiecePossiblities(&data, &player);
                        movePiecePossiblitiesJump(&data, &player);
                        movePiecePossiblitiesParties(&data, &player);
                        //showBoard(data);
                        //computerPlayer(&data, &player, 2);
                        //delete_wall(&data);
                        srand(time(NULL));
                        int choose = rand() % 2;
                        printf("%d, \n", choose);
                        if (choose == 0) {//move piece

                            computerMove(&data, &player);

                        }
                        else if (choose == 1) {// computer wall
                            computerWall(&data, &player, 2);
                        }
                        //showBoard(data);
                    }

                    k++;
                    al_flip_display();



                }
                al_flip_display();

                // Wait for a key press before closing the window
                al_rest(50.0);

                // Destroy the display
                al_destroy_display(display);

                return 0;
            }*/
            //----------------------------------------$endGame$------------------------------------------------------//
        //}
       //al_destroy_font(customFont);
        }
        if (winnerPage == page) {
            ALLEGRO_DISPLAY* display = al_create_display((5 * length + 1) * CELLSIZE + 20 * CELLSIZE, (5 * length + 1) * CELLSIZE);
            ALLEGRO_EVENT event;
            ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
            al_register_event_source(queue, al_get_mouse_event_source());
            al_register_event_source(queue, al_get_display_event_source(display));
            int X, Y, W, H;
            if (winner == 1) {
                ALLEGRO_DISPLAY* display = al_create_display((5 * length + 1) * CELLSIZE + 20 * CELLSIZE, (5 * length + 1) * CELLSIZE);
                ALLEGRO_EVENT event;
                ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
                al_register_event_source(queue, al_get_mouse_event_source());
                al_register_event_source(queue, al_get_display_event_source(display));
                al_clear_to_color(al_map_rgb(120, 144, 156));
                al_flip_display();
                width = (5 * length + 1) * CELLSIZE + 20 * CELLSIZE;
                height = (5 * length + 1) * CELLSIZE;
                X = width * 35 / 100;
                Y = height * 35 / 100;
                W = width * 65 / 100;
                H = height * 65 / 100;
                al_draw_filled_rectangle(X, Y, W, H, al_map_rgb(255, 255, 255));
                al_draw_text(fontHelp, al_map_rgb(0, 0, 0), (X + W) / 2 - 20, (Y + H) / 2, "PLAYER", 0);
                al_draw_scaled_bitmap(showPlayer.Piece1, 0, 0, al_get_bitmap_width(showPlayer.Piece1), al_get_bitmap_height(showPlayer.Piece1), (X + W) / 2, (Y + H) / 2, 10, 10, 0);
                al_draw_text(fontHelp, al_map_rgb(0, 0, 0), (X + W) / 2 + 20, (Y + H) / 2, "WIN", 0);
                Button Menu;
                Menu.posX = width * 35 / 100;
                Menu.posY = height * 50 / 100;
                Menu.width = width * 65 / 100;
                Menu.height = height * 60 / 100;
                strcpy_s(Menu.name, 6, "MENU");
                showButton(&Menu, 161, 127, 136, length);

                while (1) {
                    al_wait_for_event(queue, &event);
                    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                        return 0;
                    }
                    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                        if (event.mouse.button == 1) {
                            if (checkButton(&Menu, event.mouse.x, event.mouse.y)) {
                                al_play_sample(audioClick, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                page = firstMenu;
                                Previous = winnerPage;
                                break;
                            }
                        }
                    }
                    al_flip_display();
                }

            }

        }

    }

}



