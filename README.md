# OpenGL on Mac M1

you need to install [XQuartz](https://www.xquartz.org/) on Mac for GUI(X11 forward)


launch Xquartz. Go to Preference -> Security, click the box Allow connections from clients

## local on M1 Mac
```bash
# you need to install homebrew
brew -v

# install freeglut to use GLUT library
brew install freeglut
xeyes
glxgears
```

## docker
```bash
docker-compose -f .devcontainer/docker-compose.yml build m1mac-opengl
# xhost local:root
xhost +localhost
xhost + local:
# defaults write org.xquartz.X11 enable_iglx -bool YES
docker-compose -f .devcontainer/docker-compose.yml run --rm m1mac-opengl /bin/bash
xeyes
glxgears

```
