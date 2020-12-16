# Robottle-low-level

## low-level control

### State machine
1: control\
2: rotation\
3: setup arm\
4: pick bottle


#### 1: control
Main mode: waits commands from Jetson\
w: forward\
s: backward\
d: right\
a: left\
x: stop\

r: change to rotation mode\
o: change to setup arm mode\
p: change to arm pick bottle mode\
y: change to bottle picking mode

#### 2: rotation
Do a 360° turn, then return to control mode

#### 3: setup arm
Move arm down and up, to calibrate the pick-up and release positions of the arm

#### 4: pick bottle
Move arm to pick-up and release position and communicate if a bottle has been detected or not



### maxon - test current
for reading current. /!\/!\/!\ Need to reconfigure the drivers to output current /!\/!\/!\
