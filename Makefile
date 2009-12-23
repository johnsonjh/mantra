EXE_NAME = build/Mantra.exe

CC = gcc 
CFLAGS = -mno-cygwin -O2 -Wall -Iinclude -Isrc -DALLEGRO_STATICLINK -c

INT_LIBRARIES = -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lole32 -ldinput -lddraw -ldxguid -lwinmm -ldsound -lmoldname -lmsvcrt
OUR_LIBRARIES = ./libs/windows/libaldmb.a ./libs/windows/libdumb.a ./libs/windows/libalfont.a ./libs/windows/liballeg_s.a
LDFLAGS = -Wl,--subsystem,windows -mno-cygwin -Wall

SRCFILES = src/Dialogs.c \
           src/Enemies.c \
           src/EnemyCollision.c \
           src/EnemyUpdate.c \
           src/GameConstants.c \
           src/Input.c \
           src/LoadData.c \
           src/main.c \
           src/Map.c \
           src/OpeningScreen.c \
           src/Saric.c \
           src/Sound.c \
           src/Tests.c \
           src/Utils.c \

OBJFILES = $(SRCFILES:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

#	@(echo "Compiling $<")

all: $(EXE_NAME)

$(EXE_NAME): $(OBJFILES)
	$(CC) $(LDFLAGS) -o $(EXE_NAME) $(OBJFILES) $(OUR_LIBRARIES) $(INT_LIBRARIES)
	@(cp dats/* build)
	@(echo "All done!  Enjoy Mantra!")
