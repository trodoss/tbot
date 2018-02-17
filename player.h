#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"
#include "elements.h"

#define MAX_WALK_VELOCITY 4
#define MAX_JUMP_VELOCITY 6
#define STEP_LENGTH 2

#define FACING_RIGHT 0
#define FACING_LEFT  7

#define PLAYER_STAND 0
#define PLAYER_MOVE 1
#define PLAYER_JUMPING 2
#define PLAYER_THROWING_CLAW 3
#define PLAYER_SWINGING 4
#define PLAYER_FALLING 6
#define PLAYER_DEAD 10

#define CLAW_RIGHT_OUT 0
#define CLAW_RIGHT_IN 1
#define CLAW_RIGHT_GRAB 2
#define CLAW_LEFT_OUT 3
#define CLAW_LEFT_IN 4
#define CLAW_LEFT_GRAB 5

#define CLAW_HIDDEN 9

struct Player
{
  unsigned char facing;
  unsigned char state;
  unsigned char step;
  unsigned char x;
  unsigned char y;  
  unsigned char x_velocity;
  unsigned char y_velocity;
  unsigned char count;
};

struct Claw {
  unsigned char state;
  unsigned char step;
};

Player player;
Claw claw;

void player_init() {
	player.x = 8;
	player.y = 0;
	player.x_velocity = 0;
	player.y_velocity = 0;
	player.facing = FACING_RIGHT;
	player.state = PLAYER_STAND;
	player.step = 0;
	player.count = 0;
  claw.state = CLAW_HIDDEN;
  claw.step = 0;
}

void player_move (unsigned char new_facing) {
     if (player.state != PLAYER_THROWING_CLAW) {
       //if we are still moving in the same direction, accelerate
       player.state = PLAYER_MOVE;
       if (new_facing == player.facing)
       {
         if (player.x_velocity <= MAX_WALK_VELOCITY) player.x_velocity++;
       } else {
         player.facing = new_facing;
  
         //start with a little faster velocity when walking
         player.x_velocity = 2;
       }
     }
}

void player_handle () {
  unsigned char tile_adjust;
  
  if (player.state == PLAYER_SWINGING) {
    player.count++;
    if (player.count > 4) {
      player.step++; 
      if (player.step > 2) {
        player.step = 0;
        if (player.facing == FACING_LEFT) {
           player.facing = FACING_RIGHT;
        } else {
           player.facing = FACING_LEFT;
        }
      } else {
        if (player.facing == FACING_RIGHT) {
          if (player.x<251) player.x += 3;
          if (player.x - backdrop.view_x > 37) backdrop_move_view(DIRECTION_RIGHT, 3);
        } else {
          if (player.x > 3) player.x -= 3;
          if ((backdrop.view_x > 0) && (player.x < 200)) backdrop_move_view(DIRECTION_LEFT, 3);
        }
      }
      player.count = 0;
    }
    
  } else { 
    if ((player.count >= MAX_WALK_VELOCITY - player.x_velocity) || (player.y_velocity > 0) || (player.state == PLAYER_JUMPING)){
  	player.step++; 
    if (player.step > 1) player.step = 0;
  	player.count = 0;
  	
  	//handle x axis
  	if (player.x_velocity > 0)
  	{
  		if (player.facing == FACING_RIGHT) {
  			//if we have hit something moving right, adjust accordingly
  			if (backdrop_check_move_v(player.x + player.x_velocity + 8, player.y, tile_adjust) > 0)
  			{
  				player.x = (tile_adjust * 8) - 9;
  				player.x_velocity = 0;
  			} else {
  				if (player.x < 255) 
  				{
  					player.x += player.x_velocity;         
  					if (player.x - backdrop.view_x > 37)
  					{
  						backdrop_move_view(DIRECTION_RIGHT, player.x_velocity);    
  					} 
  					player.x_velocity--;
  				}
  			}
  		//facing left
  		} else {
  			if (backdrop_check_move_v(player.x - player.x_velocity, player.y, tile_adjust) > 0) {
  				player.x = ((tile_adjust + 1) * 8) + 1;
                  player.x_velocity = 0;			
  			} else {
  				
  				if (player.x - player.x_velocity > 0) player.x -= player.x_velocity;
  				if ((backdrop.view_x > 0) && (player.x < 200))
  				{
  					backdrop_move_view(DIRECTION_LEFT, player.x_velocity);    
  				} 
  				player.x_velocity--;
  			}
  		}	
  	}
  	
  	//handle y axis
  	if (player.y_velocity > 0) {
  		//moving up
  	    if (player.state == PLAYER_JUMPING)
  	    {
  		   if (backdrop_check_move_h(player.x, player.y-player.y_velocity, tile_adjust) > 0)
  		   {
  		      player.y = ((tile_adjust + 1) * 8) + 1;
  		      player.y_velocity = 1;
  	         //starting to free fall
  		     player.state = PLAYER_FALLING;
  		   } else {
            if (player.y-player.y_velocity > 0) player.y -= player.y_velocity;
            if ((backdrop.view_y > 0) && (player.y < 216))
            {
                backdrop_move_view(DIRECTION_UP, player.y_velocity);    
            } 
  		      player.y_velocity--;
  		   }
  	    } else {
  		   if (backdrop_check_move_h(player.x, (player.y + player.y_velocity) + 8, tile_adjust) > 0)
  		   {
     		    player.y = (tile_adjust * 8) - 9;
  		      player.y_velocity = 0;
  		      //standing on ground
  		      player.state = PLAYER_STAND;
  		   } else {
           if ((player.y+player.y_velocity) > 247) {
  			     player.state = PLAYER_DEAD;
    			 } else {
    			      player.y += player.y_velocity;
                if (player.y - backdrop.view_y > 23)
                {
                  backdrop_move_view(DIRECTION_DOWN, player.y_velocity);    
                }                
    			     if (player.y_velocity <= MAX_JUMP_VELOCITY) player.y_velocity++;
            }
  		   }
  	   } 
  	} else {
  	    //test for gravity pulling down (if nothing is below)
  		if (backdrop_check_move_h(player.x, player.y+9, tile_adjust) == 0)
  		{
  		   player.state = PLAYER_FALLING;
  		   player.y_velocity = 1;
  		}
  	}
  	
    } else {
  	player.count++;
    }
  }
  //handle claw
  if (claw.state < CLAW_HIDDEN) {
    claw.step++;
    switch (claw.state) {
      case CLAW_RIGHT_OUT:
      if (claw.step > 0) sprites.drawSelfMasked((player.x - backdrop.view_x)+8, (player.y - backdrop.view_y)-8, IMG_CLAW, 0);
	    if (claw.step < 3) sprites.drawSelfMasked((player.x - backdrop.view_x)+16, (player.y - backdrop.view_y)-16, IMG_CLAW, 2);
      if (backdrop_check_claw(player.x+8, player.y))
      {
        //claw grabs a grabbable block
		//todo: if anything is in the path, do not continue
        player.x+=4;	
        player.y-=4;
        if (player.x - backdrop.view_x > 37) backdrop_move_view(DIRECTION_RIGHT, 4);
        if ((backdrop.view_y > 0) && (player.y < 216)) backdrop_move_view(DIRECTION_UP, 2);
        claw.state = CLAW_HIDDEN;
        player.state = PLAYER_SWINGING;
        player.step = 0;
      }
      if (claw.step > 3) {
        sprites.drawSelfMasked((player.x - backdrop.view_x)+16, (player.y - backdrop.view_y)-16, IMG_CLAW, 0);
		    sprites.drawSelfMasked((player.x - backdrop.view_x)+24, (player.y - backdrop.view_y)-24, IMG_CLAW, 2); 
        if (backdrop_check_claw(player.x + 16, player.y - 8))
        {
          //claw grabs a grabbable block
		  //todo: if anything is in the path, do not continue
          player.x+=8;
          player.y-=8;
          if (player.x - backdrop.view_x > 37) backdrop_move_view(DIRECTION_RIGHT, 8);
          if ((backdrop.view_y > 0) && (player.y < 216)) backdrop_move_view(DIRECTION_UP, 6);
          claw.state = CLAW_HIDDEN;
          player.state = PLAYER_SWINGING;
          player.step = 0;
        }
      }
      if (claw.step > 6) {
        claw.step = 0;
        claw.state = CLAW_RIGHT_IN;
      }
      break;

     case CLAW_RIGHT_IN:
      if (claw.step > 0) {
        sprites.drawSelfMasked((player.x - backdrop.view_x)+8, (player.y - backdrop.view_y)-8, IMG_CLAW, 0);
        sprites.drawSelfMasked((player.x - backdrop.view_x)+16, (player.y - backdrop.view_y)-16, IMG_CLAW, 2);
      }
      if (claw.step < 3) {
        sprites.drawSelfMasked((player.x - backdrop.view_x)+16, (player.y - backdrop.view_y)-16, IMG_CLAW, 0);
        sprites.drawSelfMasked((player.x - backdrop.view_x)+24, (player.y - backdrop.view_y)-24, IMG_CLAW, 2);   
      }
      if (claw.step > 6) {
        claw.step = 0;
        claw.state = CLAW_HIDDEN;
        player.state = PLAYER_STAND;
      }
      break;

      case CLAW_LEFT_OUT:
      if (claw.step > 0) sprites.drawSelfMasked((player.x - backdrop.view_x)-8, (player.y - backdrop.view_y)-8, IMG_CLAW, 1);
      if (claw.step < 3) sprites.drawSelfMasked((player.x - backdrop.view_x)-16, (player.y - backdrop.view_y)-16, IMG_CLAW, 3);
      if (backdrop_check_claw(player.x, player.y))
      {
       //claw grabs a grabbable block
		   //todo: if anything is in the path, do not continue
          player.x-= 4;
          player.y-=4;
		      if ((backdrop.view_x > 0) && (player.x < 200)) backdrop_move_view(DIRECTION_LEFT, 4);
          if ((backdrop.view_y > 0) && (player.y < 216)) backdrop_move_view(DIRECTION_UP, 2);
          claw.state = CLAW_HIDDEN;
          player.state = PLAYER_SWINGING;
          player.step = 0;
      }
      if (claw.step > 3) {
        sprites.drawSelfMasked((player.x - backdrop.view_x)-16, (player.y - backdrop.view_y)-16, IMG_CLAW, 1);
        sprites.drawSelfMasked((player.x - backdrop.view_x)-24, (player.y - backdrop.view_y)-24, IMG_CLAW, 3); 
        if (backdrop_check_claw(player.x  - 8, player.y - 8))
        {
          //claw grabs a grabbable block
		  //todo: if anything is in the path, do not continue
          player.x-= 8;
          player.y-=8;
          if ((backdrop.view_x > 0) && (player.x < 200)) backdrop_move_view(DIRECTION_LEFT, 8);
          if ((backdrop.view_y > 0) && (player.y < 216)) backdrop_move_view(DIRECTION_UP, 6);	  
          claw.state = CLAW_HIDDEN;
          player.state = PLAYER_SWINGING;
          player.step = 0;
        }
      }
      if (claw.step > 6) {
        claw.step = 0;
        claw.state = CLAW_LEFT_IN;
      }
      break;

       case CLAW_LEFT_IN:
       if (claw.step > 0) {
        sprites.drawSelfMasked((player.x - backdrop.view_x)-8, (player.y - backdrop.view_y)-8, IMG_CLAW, 1);
        sprites.drawSelfMasked((player.x - backdrop.view_x)-16, (player.y - backdrop.view_y)-16, IMG_CLAW, 3);
      }
      if (claw.step < 3) {
        sprites.drawSelfMasked((player.x - backdrop.view_x)-16, (player.y - backdrop.view_y)-16, IMG_CLAW, 1);
        sprites.drawSelfMasked((player.x - backdrop.view_x)-24, (player.y - backdrop.view_y)-24, IMG_CLAW, 3);   
      }
      if (claw.step > 6) {
        claw.step = 0;
        claw.state = CLAW_HIDDEN;
        player.state = PLAYER_STAND;
      }
      break;
    }

  }

  if ((player.x_velocity > 0 && player.state == PLAYER_MOVE) || player.state == PLAYER_SWINGING) {
    sprites.drawOverwrite(player.x - backdrop.view_x, player.y - backdrop.view_y, IMG_PLAYER, player.state + player.facing + player.step);
  } else {
    sprites.drawOverwrite(player.x - backdrop.view_x, player.y - backdrop.view_y, IMG_PLAYER, player.state + player.facing);
  }
}

void player_throw_claw() {
  if (player.state != PLAYER_THROWING_CLAW && player.state != PLAYER_SWINGING) {
    player.x_velocity = 0;
    player.state = PLAYER_THROWING_CLAW;
    if (player.facing == FACING_RIGHT) {
      //
      claw.state = CLAW_RIGHT_OUT;
    } else {
      //
      claw.state = CLAW_LEFT_OUT;
    }
    claw.step = 0;
  }
}

void player_drop () {
  if (player.state == PLAYER_SWINGING) {
    player.x_velocity = MAX_WALK_VELOCITY;
    player.state = PLAYER_JUMPING;
  }
}

void player_jump() {
  if (player.state == PLAYER_SWINGING) {  
    player.y -= 8;
    if ((backdrop.view_y > 0) && (player.y < 216)) backdrop_move_view(DIRECTION_UP, 8);
    player.x_velocity = MAX_WALK_VELOCITY;
    player.y_velocity = MAX_JUMP_VELOCITY;
    player.state = PLAYER_JUMPING;
  }
}
#endif
