# Silver ball detection
#

import sensor, image, time, pyb


min_degree = 0
max_degree = 179
lines = [];
xavg = [80,80,80,80,80,80,80,80] # array to take average of readings

sensor.reset()                      # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QQVGA)   # Set frame size to QVGA (320x240)
sensor.set_vflip(True);
sensor.set_hmirror(True);
sensor.skip_frames(time = 2000)     # Wait for settings take effect.
clock = time.clock()                # Create a clock object to track the FPS.

green_led = pyb.LED(2)
green_led.on()
blue_led = pyb.LED(3)
blue_led.off()
p = pyb.Pin("P8",pyb.Pin.OUT_PP)
p.high()
prevMs = time.ticks_ms()
currentMs = time.ticks_ms()
prevX = 0
while(True):
    clock.tick()                    # Update the FPS clock.
    img = sensor.snapshot()         # Take a picture and return the image.
    #img = img.gaussian(3)          #blur
    #img = img.bilateral(3)
    #img = img.laplacian(3,sharpen=True)
    img = img.binary([(0, 100, 7, 127, -128, 127)])
    #erode
    #dialate

    # `merge_distance` controls the merging of nearby lines. At 0 (the default), no
    # merging is done. At 1, any line 1 pixel away from another is merged... and so
    # on as you increase this value. You may wish to merge lines as line segment
    # detection produces a lot of line segment results.

    # `max_theta_diff` controls the maximum amount of rotation difference between
    # any two lines about to be merged. The default setting allows for 15 degrees.

    for l in img.find_line_segments(merge_distance = 10, max_theta_diff = 15):
       img.draw_line(l.line(), color = (255, 0, 255))
       lines.append(l.line())
       #print(lines)
        # print(l)


    if(len(lines) > 1):
       currentMs = time.ticks_ms()

        #read right x of leftmost line
       leftx = lines[0][2]
       #read left x of rightmost value
       rightx = lines[len(lines)-1][0]

       #print(int((rightx+leftx)/2));

       xavg.pop(0)
       xavg.append(int((rightx+leftx)/2))
       xfinal = int(sum(xavg) / len(xavg))
       img.draw_circle(xfinal,60,20)
       print(xfinal);
       print(time.ticks_ms())
       #change of 40
       if time.ticks_diff(currentMs,prevMs) < 30 and (xfinal - prevX) < 10:
          p.low()
          blue_led.on()
       else:
            p.high()
            blue_led.off()
       prevMs = time.ticks_ms();
       prevX = xfinal


    lines = []

    #print(clock.fps())              # Note: OpenMV Cam runs about half as fast when connected
                                    # to the IDE. The FPS should increase once disconnected.
