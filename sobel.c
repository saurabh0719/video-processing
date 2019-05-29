#include <stdio.h>
#include <math.h>
#include <time.h>
 
// Video resolution
#define W 1280
#define H 720
 
// Allocate a buffer to store one frame
unsigned char frame[H][W] = {0};
unsigned char grayframe[H][W]; 
void main()
{
    int x, y, count;

    int pixel_x;
    int pixel_y;

    int sobel_x[3][3] =
    { { -1, 0, 1},
      { -2, 0, 2 },
      { -1, 0, 1} };

    int sobel_y[3][3] =
    { { -1, -2, -1},
      { 0,  0,  0 },
      { 1,  2,  1 } };
clock_t clock1;     

    // Open an input pipe from ffmpeg and an output pipe to a second instance of ffmpeg
    FILE *pipein = popen("ffmpeg -i output_gray.mp4 -f image2pipe -vcodec rawvideo -pix_fmt gray -", "r");
    FILE *pipeout = popen("ffmpeg -y -f rawvideo -vcodec rawvideo -pix_fmt gray -s 1280x720 -r 25 -i - -f mp4 -q:v 5 -an -vcodec mpeg4 output_sobel.mp4", "w");
     
    // Process video frames
clock1 = clock();
    while(1)
    {
        // Read a frame from the input pipe into the buffer
        count = fread(frame, 1, H*W, pipein);
         
        // If we didn't get a frame of video, we're probably at the end
        if (count != H*W) break;
         
        // Process this frame

    for (int x=1; x < W-1; x++)
    {
        for (int y=1; y < H-1; y++)
        {

            pixel_x = (sobel_x[0][0] * frame[y-1][x-1])
                    + (sobel_x[0][1] * frame[y-1][x])
                    + (sobel_x[0][2] * frame[y-1][x+1])
                    + (sobel_x[1][0] * frame[y][x-1])
                    + (sobel_x[1][1] * frame[y][x])
                    + (sobel_x[1][2] * frame[y][x+1])
                    + (sobel_x[2][0] * frame[y+1][x-1])
                    + (sobel_x[2][1] * frame[y+1][x])
                    + (sobel_x[2][2] * frame[y+1][x+1]);

            pixel_y = (sobel_y[0][0] * frame[y-1][x-1])
                    + (sobel_y[0][1] * frame[y-1][x])
                    + (sobel_y[0][2] * frame[y-1][x+1])
                    + (sobel_y[1][0] * frame[y][x-1])
                    + (sobel_y[1][1] * frame[y][x])
                    + (sobel_y[1][2] * frame[y][x+1])
                    + (sobel_y[2][0] * frame[y+1][x-1])
                    + (sobel_y[2][1] * frame[y+1][x])
                    + (sobel_y[2][2] * frame[y+1][x+1]);
            float val = (float)sqrt((pixel_x * pixel_x) + (pixel_y * pixel_y));

            grayframe[y][x] = (unsigned char)val;
        }
    }
        // Write this frame to the output pipe
        fwrite(grayframe, 1, H*W, pipeout);
    }

clock1 = clock() - clock1;
double time_taken = ((double)clock1)/CLOCKS_PER_SEC;
printf("Time taken %f",time_taken);
     
    // Flush and close input and output pipes
    fflush(pipein);
    pclose(pipein);
    fflush(pipeout);
    pclose(pipeout);
}
