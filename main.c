#include <stdio.h>
#include <raylib.h>
#include <unistd.h>

#define ROW_SIZE 30
#define WIDTH 600
#define HEIGHT 600

typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NONE
} Direction;

typedef struct{
    int x;
    int y;
}Apple;

typedef struct {
    int x;
    int y;
    Direction d;
    int apples;
} Player;

//if there's a change of direction it changes the property of the player, otherwise it continues the movement as it was doing before the check
// firstly i need to check the direction
void checkDirection(Player* p){
    if(IsKeyPressed(KEY_D)||IsKeyPressed(KEY_RIGHT)){
        p->d=RIGHT;
    }else if(IsKeyPressed(KEY_A)||IsKeyPressed(KEY_LEFT)){
        p->d=LEFT;
    }
    else if (IsKeyPressed(KEY_S)||IsKeyPressed(KEY_DOWN)){
        p->d =DOWN;
    }
    else if(IsKeyPressed(KEY_W)|| IsKeyPressed(KEY_UP)){
        p->d = UP;
    }
    

}
bool isApplePositionAcceptable(Apple* a ,Player* p){
    //at the moment i only check if the head of the snake and the apple dont collide
    if(a->x == p->x && a->y == p->y )return false;
    return true;

}
void positionApple(Apple* a , Player* p){
    //before i set the apple position i should check that the new position of the apple isnt the same position of the snake, in every part of the snake
    do{
        a->x = GetRandomValue(0,WIDTH/ROW_SIZE);
        a->y = GetRandomValue(0,HEIGHT/ROW_SIZE);
    }while(!isApplePositionAcceptable(a,p));
    


}
void checkCollision(Apple* a,  Player* p){
    if(a->x == p->x && p->y == a->y){
        p->apples++;
        positionApple(a,p);
    }
}

void setUpRandomly(Player* p,Apple* a ){
    p->x = GetRandomValue(0,WIDTH/ROW_SIZE);
    p->y = GetRandomValue(0,HEIGHT/ROW_SIZE);
    printf("starting position (%d,%d)",p->x,p->y);
    p->d = NONE;
    p->apples=0;
    positionApple(a,p);
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
        default:
            printf("no direction");
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
void printGame(Player p,Apple a){
    /*print a grid WIDTH-HEIGHT with squares */
    for(int y = 0;y<HEIGHT/ROW_SIZE ;y++){
        for(int x = 0; x < WIDTH/ROW_SIZE;x++){
            DrawRectangleLines(x*ROW_SIZE,y*ROW_SIZE ,ROW_SIZE, ROW_SIZE,WHITE); 
            
        }
    }
    //draw the player
    DrawRectangle(p.x*ROW_SIZE,p.y*ROW_SIZE,ROW_SIZE,ROW_SIZE,GREEN);
    //draw the apple
    DrawRectangle(a.x*ROW_SIZE,a.y*ROW_SIZE,ROW_SIZE,ROW_SIZE,RED);
}

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "Snake");
    SetTargetFPS(20);
    Player p;
    Apple a;
    setUpRandomly(&p,&a);
    while (!WindowShouldClose()){
        
        BeginDrawing();
        ClearBackground(BLACK);
        printGame(p,a);
        checkDirection(&p);
        movePlayer(&p);
        checkCollision(&a,&p);
        printf("posizione apple (%d)(%d)",a.x,a.y);
        EndDrawing();
        
    }
    CloseWindow();

    return 0;
}