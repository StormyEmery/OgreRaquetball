noinst_HEADERS = Assignment2.h GameObject.h OgreMotionState.h Simulator.h TutorialApplication.h Plain.h Sound.h Ball.h Paddle.h

bin_PROGRAMS = assignment2
assignment2_CPPFLAGS = -I$(top_srcdir)
assignment2_SOURCES = Assignment2.cpp GameObject.cpp OgreMotionState.cpp Simulator.cpp TutorialApplication.cpp Plain.cpp Sound.cpp Ball.cpp Paddle.cpp

assignment2_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS)
assignment2_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS)
assignment2_LDFLAGS = -lOgreOverlay -lboost_system

assignment2_CPPFLAGS += -std=c++11
assignment2_CPPFLAGS += -I/usr/include/SDL
assignment2_LDFLAGS += -lSDL -lSDL_mixer

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
