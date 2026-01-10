#include "draw.h"
#include <stdio.h>
#include <switch.h>
#include <string.h>

int main()
{
	// initialize and get graphics context
	Graphics* g = init();// 画面のセットアップ
	
	playVideo(g, 160, 0);

	// wait 10s
	sleep(10);
  
	// clean up graphics
	deinit(g);

	return 0;
}
