FROM ubuntu
ADD . irrlicht

RUN apt update
RUN apt install -y make
RUN apt install -y g++ cmake freeglut3 freeglut3-dev 
RUN apt install -y binutils-gold libglew-dev mesa-common-dev libglm-dev  
RUN apt install -y build-essential 
RUN apt install -y libxxf86vm-dev libxcursor-dev 
RUN apt install -y libirrlicht-dev

# ENTRYPOINT sudo irrlicht/run.sh