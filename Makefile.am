noinst_HEADERS = Assignment2.h OgreMotionState.h Simulator.h TutorialApplication.h Paddle.h PaddleAI.h Plain.h Sound.h Ball.h NetManager.h

bin_PROGRAMS = assignment2
assignment2_CPPFLAGS = -I$(top_srcdir)
assignment2_SOURCES = Assignment2.cpp OgreMotionState.cpp Simulator.cpp TutorialApplication.cpp Paddle.cpp PaddleAI.cpp Plain.cpp Sound.cpp Ball.cpp NetManager.cpp

assignment2_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(CEGUI_CFLAGS)
assignment2_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS)
assignment2_LDFLAGS = -lOgreOverlay -lboost_system -R/lusr/lib/cegui-0.8

assignment2_CPPFLAGS += -std=c++11
assignment2_CPPFLAGS += -I/usr/include/SDL
assignment2_LDFLAGS += -lSDL -lSDL_mixer -lSDL_net

OgreApp_LDFLAGS= -lOgreOverlay -lboost_system -R/lusr/lib/cegui-0.8


EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign