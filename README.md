
# Video to ASCII Player

This Project was made in C++ with OpenCV 4.7.0 in Windows.

It takes a Video (mp4,webm,avi,...) and plays it in the Terminal with ASCII chars.

## usage

```
video_to_ascii.exe C:\PATH\video.mp4 -w 256 -h 128
```
You can also click on the .exe file and it will guide you with a small dialog asking you for the path and output size.

## Dependencies

After setting up OpenCV in your Environment-Path you can compile it wiht no problems.

When you want to share your .exe file, you need to also make sure that the users have opencv_worldXXX.dll (where XXX means the OpenCV Version, mine was opencv_worl470) and the opencv_videoio_ffmpegXXX_64.dll.

