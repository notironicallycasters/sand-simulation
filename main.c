#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <time.h>
#include <math.h>


int main()
{
    bool running = true;

    int res = 16;
    int col = 16*res;
    int row = 9*res;
    int size = 1;


    float mX = 0;
    float mY = 0;
    float *pmX = &mX;
    float *pmY = &mY;

    int width;
    int height;
    int *pwidth = &width;
    int *pheight = &height;

    int mouseX;
    int mouseY;

    bool lmb = false;
    bool rmb = false;

    int mat = 1;
    
    int pen = 3;

    bool canFall[] = {false,true,true,true,false,true,true};
    bool canDiag[]= {false,true,true,false,false,true,false};
    bool canHor[] = {false,false,true,false,false,false,false};
    bool canTrans[] = {true,false,true,false,false,false,false};

    int amount = sizeof(canFall);

    bool paused = false;

    int checkY = 0;
    int pcheckY = 0;
    int below = 0;
    int above = 0;
    int current = 0;
    int val = 0;
    int alt = 1;

    char str8[8];

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_CreateWindowAndRenderer("Sand Engine",col*15,row*15,SDL_WINDOW_FULLSCREEN,&window,&renderer);
    
    SDL_SetRenderLogicalPresentation(renderer,col,row,SDL_LOGICAL_PRESENTATION_STRETCH);

    int eff = col*row;

    int pixels[eff];

    int tmp;
    
    srand(time(NULL));

    Uint64 start;
    Uint64 end;
    float elapsed;

    int i;
    int j;
    for (i = eff; i >= 0; i--) {
        pixels[i] = 0;
    }

    //Update loop
    while (running) {
        start = SDL_GetTicksNS();
        
        SDL_GetMouseState(pmX,pmY);
        mouseX = (int)((mX)*(res/120.0));
        mouseY = (int)((mY)*(res/120.0));
        if (mouseX < pen/2){mouseX = pen/2;}
        if (mouseY < pen/2){mouseY = pen/2;}
        if (mouseX+pen/2 >= col){mouseX = col-pen/2-1;}
        if (mouseY+pen/2 >= row){mouseY = row-pen/2-1;}

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type){
                case SDL_EVENT_QUIT:
                running = false;
                break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if (event.button.button == SDL_BUTTON_LEFT){
                        lmb = true;
                    } else if (event.button.button == SDL_BUTTON_RIGHT){
                        rmb = true;
                    }
                break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    if (event.button.button == SDL_BUTTON_LEFT){
                        lmb = false;
                    } else if (event.button.button == SDL_BUTTON_RIGHT){
                        rmb = false;
                    }
                break;
                case SDL_EVENT_MOUSE_WHEEL:
                    if (event.wheel.y == 1){
                        pen += 2;
                    } else if (event.wheel.y == -1 && pen >= 3){
                        pen -= 2;

                    }
                break;
                case SDL_EVENT_KEY_DOWN:
                    if (event.key.key == SDLK_SPACE){
                        paused = !paused;
                    } else if (event.key.key == SDLK_1){
                        mat = 1;
                    } else if (event.key.key == SDLK_2){
                        mat = 2;
                    } else if (event.key.key == SDLK_3){
                        mat = 3;
                    } else if (event.key.key == SDLK_4){
                        mat = 4;
                    }else if (event.key.key == SDLK_5){
                        mat = 5;
                    }else if (event.key.key == SDLK_6){
                        mat = 6;
                    }
                break
                ;  
                }
        }

        if (lmb){
            //pixels[mouseX+col*(mouseY)] = mat;
            for (i = -(pen/2); i < pen/2+1; i++){
               for (j = -(pen/2); j < pen/2+1; j++){
                    pixels[mouseX+i+col*(mouseY+j)] = mat;
               }
            }
                    
        } else if (rmb){
            for (i = -(pen/2); i < pen/2+1; i++){
               for (j = -(pen/2); j < pen/2+1; j++){
                    pixels[mouseX+i+col*(mouseY+j)] = 0;
               }
            }
        }

        SDL_SetRenderDrawColor(renderer, 154, 190, 255, SDL_ALPHA_OPAQUE);  /* black, full alpha */
        SDL_RenderClear(renderer);  /* start with a blank canvas. */

        checkY = 0;
        pcheckY = 0;
        //for (i = eff; i >= 0; i--) {
        for (i = 0; i < col; i++) {
            for (j = row; j >= 0; j--) {
                current = i+(col*j);
                val = pixels[current];
                if (val < 0){val=0;}
                if (val > amount){val=amount;}
                below = current+col;

                switch (pixels[current]) {
                    case 0:
                    continue;
                    break;

                    case 1:
                    SDL_SetRenderDrawColor(renderer, 245, 216, 154, SDL_ALPHA_OPAQUE);
                    break;

                    case 2:
                    SDL_SetRenderDrawColor(renderer, 0, 90, 255, SDL_ALPHA_OPAQUE);
                    break;

                    case 3:
                    SDL_SetRenderDrawColor(renderer, 192, 192, 192, SDL_ALPHA_OPAQUE);
                    break;

                    case 4:
                    SDL_SetRenderDrawColor(renderer, 48, 48, 48, SDL_ALPHA_OPAQUE);
                    break;

                    case 5:
                    SDL_SetRenderDrawColor(renderer, 48, 40, 32, SDL_ALPHA_OPAQUE);
                    break;

                    case 6:
                    SDL_SetRenderDrawColor(renderer, 54, 120, 70, SDL_ALPHA_OPAQUE);
                    break;
                }
                
                if (pixels[current]!=0){SDL_RenderPoint(renderer,i,j);}

                if (paused || pixels[current]==4){continue;}

                if (canFall[val] && below < eff && canTrans[pixels[below]] && pixels[below] != pixels[current]){
                        tmp = pixels[current];
                        pixels[current] = pixels[below];
                        pixels[below]= tmp;

                    } else if (canDiag[val] && i-alt >= 0 && below < eff && canTrans[pixels[below-alt]] && pixels[below-alt] != pixels[current]){
                        tmp = pixels[current];
                        pixels[current] = pixels[below-alt];
                        pixels[below-alt]= tmp;

                    } else if (canDiag[val] && i+alt < col && below < eff && canTrans[pixels[below+alt]] && pixels[below+alt] != pixels[current]){
                        tmp = pixels[current];
                        pixels[current] = pixels[below+alt];
                        pixels[below+alt]= tmp;

                    } else if (canHor[val] && i+alt < col && pixels[current+alt] == 0){
                        tmp = pixels[current];
                        pixels[current] = 0;
                        pixels[current+alt]= tmp;
                    } else if (canHor[val] && i-alt >= 0 && pixels[current-alt] == 0){
                            tmp = pixels[current];
                            pixels[current] = 0;
                            pixels[current-alt]= tmp;

                    }

                alt = -alt;
            
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 127);
        for (i = -(pen/2); i < pen/2+1; i++){
               for (j = -(pen/2); j < pen/2+1; j++){
                    SDL_RenderPoint(renderer,mouseX+i,mouseY+j);
               }
            }
        SDL_RenderPresent(renderer);  /* put it all on the screen! */
        end = SDL_GetTicksNS();
        elapsed = (end - start) / 1000000000.0;
        if (elapsed >= 0){SDL_Delay(floor((1000.0/60.0) - (elapsed*1000)));}
        
        
    }

    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}