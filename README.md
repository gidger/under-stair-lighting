# Arduino Powered Cascading LED Stair Lights
Arduino controlled cascading LED under stair lights. Makes use of ultrasonic sensors to detect if someone is walking by the top/bottom of a staircase. Cascades the lights on and off in the appropriate direction.

Additional functionality to act as a real-time bass visualizer.

Check out the accompanying blog posts for this code for more info and examples:
[Part 1: Basic Cascade Functionality](https://gidge.dev/stair%20lights/stair-lighting/)
[Part 2: Real-Time Bass Visualization](https://gidge.dev/stair%20lights/stair-lighting-pt2/)


## Examples

![Example Up](https://github.com/gidger/under-stair-lighting/blob/97d05713b8842876cc279ee3c0c744754c52273f/examples/bottom-up.gif)

![Example Down](https://github.com/gidger/under-stair-lighting/blob/97d05713b8842876cc279ee3c0c744754c52273f/examples/bottom-down.gif)

![<img src="https://github.com/gidger/under-stair-lighting/blob/d4d0b198ada6a9371a6818912574b9392130d69f/examples/youtube-thumbnail.jpg">](https://www.youtube.com/watch?v=AO4oNaiYNIw)


## Parts to Modify for Your Own Buid
- Pin definintion: you'll need to match the number of pins to the number of stairs/lights you have.
- Pin definition number values: likewise, this'll have to be edited to match the pin numbers you use on your Arduino. Any beginner Arduino will cover this.
- lightup_time: if you're generally slow going up/down stairs, you'll want to lengthen this.
- interlight_delay: ditto for this one. Going too large/small for this value will result in an awkwardly fast/slow animation.
- cooldown_time: if you feel like the cooldown should be more generious, make this value larger.
- Distance thresholds (distance_top < x and distance_bot < y): minimum distance needed to be picked up by the sensors to start the animation. The bundled uss-calibration.ino will help you get these values.