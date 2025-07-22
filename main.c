#include <stdio.h>
#include <raylib.h>
#include <unistd.h>

#define ROW_SIZE 50
#define WIDTH 600
#define HEIGHT 400

typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN,
} Direction;

typedef struct {
    int x;
    int y;
    Direction d;
} Player;

//if there's a change of direction it changes the property of the player, otherwise it continues the movement as it was doing before the check
// firstly i need to check the direction
void checkDirection(Player* p){
    if(IsKeyDown(KEY_D)||IsKeyDown(KEY_RIGHT)){
        p->d=RIGHT;
    }else if(IsKeyDown(KEY_A)||IsKeyDown(KEY_LEFT)){
        p->d=LEFT;
    }
    else if (IsKeyDown(KEY_S)||IsKeyDown(KEY_DOWN)){
        p->d =DOWN;
    }
    else if(IsKeyDown(KEY_W)|| IsKeyDown(KEY_UP)){
        p->d = UP;
    }
    
}
void movePlayer(Player* p){
    switch(p->d){
        case LEFT:
            p->x -=1;
            break;
        case RIGHT:
            p->x+=1;
            break;
        case UP:
            p->y-=1;
            break;
        case DOWN:
            p->y+=1;
            break;
    }
    if(p->x > WIDTH/ROW_SIZE){
        p->x = 0;
    }
    else if(p->x < 0){
        p->x = WIDTH/ROW_SIZE;
    }
    if (p->y > HEIGHT/ROW_SIZE){
        p->y = 0; 
    }else if(p->y <0){
        p->y = HEIGHT/ROW_SIZE;
    }
}
void printGame(Player p){
    /*print a grid WIDTH-HEIGHT with squares with a border darker */
    for(int y = 0;y<HEIGHT/ROW_SIZE ;y++){
        for(int x = 0; x < WIDTH/ROW_SIZE;x++){
            DrawRectangleLines(x*ROW_SIZE,y*ROW_SIZE ,ROW_SIZE, ROW_SIZE,WHITE); 
            
        }
    }
    DrawRectangle(p.x*ROW_SIZE,p.y*ROW_SIZE,ROW_SIZE,ROW_SIZE,GREEN);
}

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Snake");
    SetTargetFPS(10);
    Player p;
    p.x = 5;
    p.y= 5;
    while (!WindowShouldClose()){
        
        BeginDrawing();
        ClearBackground(BLACK);
        printGame(p);
        checkDirection(&p);
        movePlayer(&p);
        printf("(%d)",p.d );
        EndDrawing();
        
    }
    CloseWindow();

    return 0;
}