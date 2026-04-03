CC     = gcc
CFLAGS = -Wall -O2 -I. $(shell pkg-config --cflags raylib)
LIBS   = $(shell pkg-config --libs raylib) -lm -ldl -lpthread

TARGET = build/app

SRCS = src/main.c \
       src/graphics/dda.c \
       src/graphics/bresenham.c \
       src/graphics/midcircle.c \
       src/screens/anim_screen.c \
       src/screens/menu_screen.c \
       src/objects/piano.c \
       src/objects/note_block.c \
       src/objects/midi.c \
       src/objects/midi_slider.c \
       src/render/render_state.c \
       src/utils/draw_utils.c \
       src/audio/audio.c \
       src/core/app.c


OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

run: all
	LIBGL_ALWAYS_SOFTWARE=1 ./$(TARGET)
