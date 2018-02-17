#ifndef MAP_H
#define MAP_H

#include "globals.h"
#include "map_data.h"

#define DIRECTION_UP 0
#define DIRECTION_DOWN 1
#define DIRECTION_LEFT 2
#define DIRECTION_RIGHT 3

struct Backdrop
{
	//represent the current view coordinates
	short view_x;
	short view_y;
};

Backdrop backdrop;

void backdrop_init() {
}

unsigned char backdrop_get_block(char map_x, char map_y)
{
 int find = (map_y * 32) + map_x;
 int curr = -1;
 int ptr = 0;
 unsigned char block = 0;
 unsigned char run = 0;
 
 while (curr < find) {
	 block = pgm_read_byte_near (MAP_DATA + ptr);
  
	 if (block > 128) {
		 block -= 128;
		 ptr++;
		 run = pgm_read_byte_near (MAP_DATA + ptr);
		 curr += run;
	 } else {
	  curr++;
	 }
   
	 ptr++;
 }
 
 //unsigned char block = pgm_read_byte_near ( MAP_DATA + ((map_y * 32) + map_x));
 return block;
}

unsigned char backdrop_check_block(unsigned char x, unsigned char  y)
{
  unsigned char  map_x = x / 8;
  unsigned char  map_y = y / 8;
  if ((map_x > 31) || (map_y > 31)) return 0;
  return backdrop_get_block(map_x, map_y);
}

bool backdrop_check_move(unsigned char  x, unsigned char  y)
{
  if (backdrop_check_block(x,y) > 6) return false;
  return true;
}

bool backdrop_check_claw(unsigned char  x, unsigned char  y)
{
  unsigned char test = backdrop_check_block(x,y);
  if (test == 6 || test == 10 ) return true;
  return false;
}

unsigned char backdrop_check_move_h(unsigned char x, unsigned char y,unsigned char &tile_y)
{
   unsigned char tile_x_pixels = (x - (x % 8)); 	//calculate the x position in pixels we are checking against
   unsigned char testend = x + 8; //calculate the end of testing
   tile_y = y >> 3; //calculate the y position (map coordinates) of the tiles we want to test   
   unsigned char tile_x = tile_x_pixels >> 3; //calculate map x coordinate for first tile
 
    //loop while the start point of the test tile is inside the bounding box
    while(tile_x_pixels <= testend){
       if(backdrop_get_block(tile_x, tile_y) > 6)	//is a solid tile is found at tile_x, tile_y?
			return 1;	
			
       tile_x++;		//increase tile x map coordinate
       tile_x_pixels +=8;	//increase tile x pixel coordinate
	}

	return 0;
}

unsigned char backdrop_check_move_v(unsigned char x, unsigned char y, unsigned char &tile_x)
{
   unsigned char tile_y_pixels = (y - (y % 8)); 	//calculate the x position in pixels we are checking against
   unsigned char testend = y + 8; //calculate the end of testing
   tile_x = x >> 3; //calculate the y position (map coordinates) of the tiles we want to test   
   unsigned char tile_y = tile_y_pixels >> 3; //calculate map x coordinate for first tile
 
    //loop while the start point of the test tile is inside the bounding box
    while(tile_y_pixels <= testend){
       if(backdrop_get_block(tile_x, tile_y) > 6)	//is a solid tile is found at tile_x, tile_y?
			return 1;	
			
       tile_y++;		//increase tile x map coordinate
       tile_y_pixels +=8;	//increase tile x pixel coordinate
	}

	return 0;
}

void backdrop_move_view(unsigned char direction, unsigned char len)
{  
   switch (direction)
   {
      case DIRECTION_UP:
                if ((backdrop.view_y - len) > 0) {
                  backdrop.view_y -= len;
                } else {
                  backdrop.view_y = 0;
                }
		break;
		
      case DIRECTION_DOWN:
                if ((backdrop.view_y + len) < 192) {
                  backdrop.view_y += len;
                } else {
                  backdrop.view_y = 192;
                }
		break;
	
      case DIRECTION_LEFT:
                if ((backdrop.view_x - len) > 0 ) {
                  backdrop.view_x -= len;
                } else {
                  backdrop.view_x = 0;
                }
		break;	
		
      case DIRECTION_RIGHT:
                if ((backdrop.view_x + len) < 128) {
                  backdrop.view_x += len;
                } else {
                  backdrop.view_x = 128;
                }
		break;			
   } 
}

char backdrop_view_test(short x, short y)
{
  if ((x > backdrop.view_x) && (x < (backdrop.view_x + 128)))
  {
    if ((y > backdrop.view_y) && (y < (backdrop.view_y + 64))) return 1;
  }
  return 0;
}

void backdrop_draw()
{
  short mapx, mapy;
  short map_xoff, map_yoff;
  unsigned char tile;
  mapx = backdrop.view_x / 8;
  mapy = backdrop.view_y / 8;
  
  map_xoff = backdrop.view_x & 7;
  map_yoff = backdrop.view_y & 7;
  
  for (int i=0; i < 9; i++)
  {
    for (int j=0; j < 17; j++)
    {
      tile = backdrop_get_block((j+mapx), (i+mapy));
      if (tile > 0) sprites.drawOverwrite(j*8 - map_xoff, i*8 - map_yoff, IMG_BACKDROP, tile-1);
    }
  }
}
#endif
