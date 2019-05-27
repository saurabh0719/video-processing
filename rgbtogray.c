//
// Video processing example using FFmpeg
// Written by Ted Burke - last updated 12-2-2017
//
 
#include <stdio.h>
 
// Video resolution
#define W 1280
#define H 720
 
// Allocate a buffer to store one frame
unsigned char frame[H][W][3] = {0};
unsigned char grayframe[H][W]; 
void main()
{
    int x, y, count;
     
    // Open an input pipe from ffmpeg and an output pipe to a second instance of ffmpeg
    FILE *pipein = popen("ffmpeg -i teapot.mp4 -f image2pipe -vcodec rawvideo -pix_fmt rgb24 -", "r");
    FILE *pipeout = popen("ffmpeg -y -f rawvideo -vcodec rawvideo -pix_fmt gray -s 1280x720 -r 25 -i - -f mp4 -q:v 5 -an -vcodec mpeg4 output_gray.mp4", "w");
     
    // Process video frames
    while(1)
    {
        // Read a frame from the input pipe into the buffer
        count = fread(frame, 1, H*W*3, pipein);
         
        // If we didn't get a frame of video, we're probably at the end
        if (count != H*W*3) break;
         
        // Process this frame
        for (y=0 ; y<H ; ++y) for (x=0 ; x<W ; ++x)
        {
            // Invert each colour component in every pixel
            grayframe[y][x] =  (frame[y][x][0] +  frame[y][x][1] + frame[y][x][2])/3; 
            
        }
         
        // Write this frame to the output pipe
        fwrite(grayframe, 1, H*W, pipeout);
    }
     
    // Flush and close input and output pipes
    fflush(pipein);
    pclose(pipein);
    fflush(pipeout);
    pclose(pipeout);
}
